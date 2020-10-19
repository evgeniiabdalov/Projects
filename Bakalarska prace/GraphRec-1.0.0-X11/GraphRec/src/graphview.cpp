/* GraphRec
 * Copyright (c) 2009 Petr Koupy <petr.koupy@gmail.com>
 *
 * GNU General Public License Usage
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtCore/QSettings>
#include <QtCore/QPointer>
#include <QtCore/QSemaphore>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QString>
#include <QtCore/QTimer>
#include <QtCore/QTimeLine>
#include <QtCore/QPair>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QVector>
#include <QtCore/QLine>
#include <QtCore/QRect>
#include <QtCore/QRectF>
#include <QtCore/QPointF>
#include <QtCore/QDir>
#include <QtGui/QWidget>
#include <QtGui/QFrame>
#include <QtGui/QIcon>
#include <QtGui/QCursor>
#include <QtGui/QColor>
#include <QtGui/QPen>
#include <QtGui/QBrush>
#include <QtGui/QApplication>
#include <QtGui/QDialog>
#include <QtGui/QProgressDialog>
#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsItem>
#include <QtGui/QGraphicsItemGroup>
#include <QtGui/QGraphicsItemAnimation>
#include <QtGui/QTransform>
#include <QtGui/QPainter>
#include <QtGui/QImage>
#include <QtGui/QPaintDevice>
#include <QtGui/QSpinBox>
#include <QtGui/QSlider>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QKeyEvent>
#include <QtGui/QWheelEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QFocusEvent>
#include <cmath>

#include "graphview.h"
#include "context.h"
#include "factory.h"
#include "servant.h"
#include "parsers/parser.h"
#include "validators/validator.h"
#include "layouters/layouter.h"
#include "recorders/recorder.h"
#include "recorders/imagerecorder.h"
#include "recorders/videorecorder.h"
#include "savers/saver.h"
#include "primitives/node.h"
#include "primitives/entity.h"
#include "primitives/edge.h"
#include "dialogs/colordialog.h"
#include "dialogs/setupdialog.h"
#include "dialogs/errordialog.h"
#include "dialogs/capturedialog.h"

extern int G_GRVideoBufferSize;
extern QImage G_GRVideoBuffer[];
extern QSemaphore G_GRVideoFree;
extern QSemaphore G_GRVideoUsed;

GraphView::GraphView(QFile& file, qint64 filePosition, QString& parserName, QString& validatorName, QString& layouterName, QWidget* parent) :
        QGraphicsView(parent),
        m_discreteDisplacementEnabled(false),
        m_discreteDisplacementOffset(14),
        m_calendarPosition(0),
        m_timestep(0),
        m_animationIsRunning(false),
        m_animationStopRequest(false),
        m_layoutingWasEnabled(false),
        m_layoutingWasRunning(false),
        m_timeoutZero(false),
        m_captureIntervalTemp(0),
        m_recordingEnabled(false),
        m_recordingInteractive(false),
        m_bufferPosition(0),
        m_renderMode(GraphView::rmDirectSave),
        m_captureWidth(0),
        m_captureHeight(0),
        m_captureTimeStepBegin(0),
        m_captureTimeStepEnd(0),
        m_captureInterval(0)
{
    QSettings settings;

    QGraphicsScene* scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(scene);
    setCacheMode(QGraphicsView::CacheNone);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setFrameStyle(QFrame::Panel | QFrame::Sunken);
    viewport()->setCursor(Qt::ArrowCursor);

    QFileInfo fileInfo(file);
    m_context = new Context(fileInfo, filePosition);

    m_dialogError = new ErrorDialog(this);
    connect(m_dialogError, SIGNAL(ErrorSelected(int, int, int, bool)), this, SLOT(SelectEvent(int, int, int, bool)));

    Parser* parser = qobject_cast<Parser*>(Factory::CreateServant(Factory::sParser, parserName));
    connect(parser, SIGNAL(Error(const QString&)), m_dialogError, SLOT(Log(const QString&)));
    parser->ParseGraph(file, *m_context);
    delete parser;

    m_dialogError->setWindowTitle(m_dialogError->windowTitle() + tr(" (%1)").arg(m_context->graphName));
    QPoint windowPosition = settings.value("ErrorDialog/Position", m_dialogError->pos()).toPoint();
    QSize windowSize = settings.value("ErrorDialog/Size", m_dialogError->size()).toSize();
    m_dialogError->resize(windowSize);
    m_dialogError->move(windowPosition);

    setBackgroundBrush(QBrush(m_context->sceneBackground));
    setMatrix(m_context->sceneMatrix);
    centerOn(m_context->sceneViewCenter);

    SetValidator(validatorName);

    foreach (Node* node, m_context->nodes)
    {
        this->scene()->addItem(node);
    }

    // Ensure backwards compatibility with rotation from alpha version .
    QTransform t(m_context->sceneMatrix);
    if (t.isRotating())
    {
        // Rotate scene backward with old rotation.
        rotate(m_context->sceneAngle);
        QGraphicsItemGroup* group = this->scene()->createItemGroup(this->scene()->items());
        if (group)
        {
            QPointF c = m_context->sceneViewCenter;
            // Rotate graph forward with new rotation.
            group->setTransform(QTransform().translate(c.x(), c.y()).rotate(-m_context->sceneAngle).translate(-c.x(), -c.y()));
        }
        this->scene()->destroyItemGroup(group);
    }

    foreach (Edge* edge, m_context->edges)
    {
        this->scene()->addItem(edge);
        edge->Adapt();
    }

    m_timerLayout = new QTimer(this);
    connect(m_timerLayout, SIGNAL(timeout()), this, SLOT(on_timerLayout_timeout()));
    double halfSide = m_context->layoutDisplacement * m_context->nodes.size() * 3;
    double fullSide = halfSide * 2;
    setSceneRect(-halfSide, -halfSide, fullSide, fullSide);
    if (m_context->enabledLayouting)
        LayoutRandomize();
    SetLayouter(layouterName);

    foreach (Node* node, m_context->nodes)
    {
        if (node)
        {
            connect(node, SIGNAL(NodePositionChanged()), this, SLOT(NodeMoved()));
            connect(this, SIGNAL(TimeStepChanged(int)), node, SLOT(SetTimeStep(int)));
            connect(this, SIGNAL(NodeLabelsChanged(Node::NodeDescription)), node, SLOT(SetDescription(Node::NodeDescription)));
            connect(this, SIGNAL(DiscreteDisplacementEnabled(bool)), node, SLOT(SetDiscreteDisplacementEnabled(bool)));
            connect(this, SIGNAL(DiscreteDisplacementOffsetChanged(int)), node, SLOT(SetDiscreteDisplacementOffset(int)));
        }
    }
    emit TimeStepChanged(m_timestep);
    emit DiscreteDisplacementEnabled(m_discreteDisplacementEnabled);
    emit DiscreteDisplacementOffsetChanged(m_discreteDisplacementOffset);

    m_animationTimeLine = new QTimeLine(700, this);
    m_animationTimeLine->setCurveShape(QTimeLine::EaseInOutCurve);
    m_animationTimeLine->setUpdateInterval(40);
    connect(m_animationTimeLine, SIGNAL(finished()), this, SLOT(AnimateTimeStep()));
    connect(this, SIGNAL(AnimationStepDone()), this, SLOT(AnimateTimeStep()));

    m_spinBoxTimeStep = new QSpinBox();
    m_sliderTimeStep = new QSlider(Qt::Horizontal);
    m_sliderDuration = new QSlider(Qt::Vertical);
    m_sliderDisplacement = new QSlider(Qt::Vertical);

    m_spinBoxTimeStep->setMaximum(m_context->frames.count() - 1);
    m_spinBoxTimeStep->setMinimum(0);
    m_spinBoxTimeStep->setSingleStep(1);
    m_spinBoxTimeStep->setValue(0);
    m_spinBoxTimeStep->setToolTip(tr("Timestep"));
    connect(m_spinBoxTimeStep, SIGNAL(editingFinished()), this, SLOT(setFocus()));
    connect(m_spinBoxTimeStep, SIGNAL(valueChanged(int)), this, SLOT(SetTimeStep(int)));
    connect(m_spinBoxTimeStep, SIGNAL(valueChanged(int)), m_sliderTimeStep, SLOT(setValue(int)));

    m_sliderTimeStep->setMaximum(m_context->frames.count() - 1);
    m_sliderTimeStep->setMinimum(0);
    m_sliderTimeStep->setSingleStep(1);
    m_sliderTimeStep->setPageStep(10);
    m_sliderTimeStep->setValue(0);
    m_sliderTimeStep->setToolTip(tr("Timestep"));
    connect(m_sliderTimeStep, SIGNAL(sliderPressed()), this, SLOT(setFocus()));
    connect(m_sliderTimeStep, SIGNAL(valueChanged(int)), this, SLOT(SetTimeStep(int)));
    connect(m_sliderTimeStep, SIGNAL(valueChanged(int)), m_spinBoxTimeStep, SLOT(setValue(int)));

    m_sliderDuration->setMaximum(4000);
    m_sliderDuration->setMinimum(0);
    m_sliderDuration->setSingleStep(10);
    m_sliderDuration->setPageStep(100);
    m_sliderDuration->setValue(700);
    m_sliderDuration->setToolTip(tr("Speed"));
    m_sliderDuration->setInvertedAppearance(true);
    connect(m_sliderDuration, SIGNAL(sliderPressed()), this, SLOT(setFocus()));
    connect(m_sliderDuration, SIGNAL(valueChanged(int)), this, SLOT(SetDuration(int)));

    m_sliderDisplacement->setMaximum(90);
    m_sliderDisplacement->setMinimum(20);
    m_sliderDisplacement->setSingleStep(1);
    m_sliderDisplacement->setPageStep(5);
    m_sliderDisplacement->setValue(40);
    m_sliderDisplacement->setToolTip(tr("Displacement"));
    m_sliderDisplacement->setInvertedAppearance(true);
    connect(m_sliderDisplacement, SIGNAL(sliderPressed()), this, SLOT(setFocus()));
    connect(m_sliderDisplacement, SIGNAL(valueChanged(int)), this, SLOT(SetDisplacement(int)));

    connect(this, SIGNAL(TimeStepChanged(int)), m_spinBoxTimeStep, SLOT(setValue(int)));
    connect(this, SIGNAL(TimeStepChanged(int)), m_sliderTimeStep, SLOT(setValue(int)));
    connect(this, SIGNAL(DurationChanged(int)), m_sliderDuration, SLOT(setValue(int)));
    connect(this, SIGNAL(DisplacementChanged(int)), m_sliderDisplacement, SLOT(setValue(int)));

    QVBoxLayout* verticalLayoutRight = new QVBoxLayout();;
    QHBoxLayout* horizontalLayout = new QHBoxLayout();
    horizontalLayout->addWidget(m_spinBoxTimeStep);
    horizontalLayout->addWidget(m_sliderTimeStep);
    horizontalLayout->addSpacing(20);
    verticalLayoutRight->addLayout(horizontalLayout);
    verticalLayoutRight->addStretch();

    QVBoxLayout* verticalLayoutLeft = new QVBoxLayout();
    verticalLayoutLeft->addWidget(m_sliderDuration);
    verticalLayoutLeft->addWidget(m_sliderDisplacement);
    verticalLayoutLeft->addSpacing(20);

    QGridLayout* mainLayout = new QGridLayout();
    mainLayout->addLayout(verticalLayoutLeft, 0, 0, Qt::AlignLeft);
    mainLayout->addLayout(verticalLayoutRight, 0, 1, Qt::AlignRight);

    setLayout(mainLayout);

    m_dialogColor = new ColorDialog(m_context, this);
    windowPosition = settings.value("ColorDialog/Position", m_dialogColor->pos()).toPoint();
    windowSize = settings.value("ColorDialog/Size", m_dialogColor->size()).toSize();
    m_dialogColor->resize(windowSize);
    m_dialogColor->move(windowPosition);
    connect(m_dialogColor, SIGNAL(BackgroundColorChanged(const QColor&)), this, SLOT(SetBackgroundColor(const QColor&)));
    m_dialogColor->setWindowTitle(m_dialogColor->windowTitle() + tr(" (%1)").arg(m_context->graphName));

    // Load default settings
    m_animationTimeLine->setDuration(settings.value("GraphView/AnimationLength", m_animationTimeLine->duration()).toInt());
    m_sliderDuration->setValue(m_animationTimeLine->duration());
    QTimeLine::CurveShape curveShape = static_cast<QTimeLine::CurveShape>(settings.value("GraphView/AnimationStyle", m_animationTimeLine->curveShape()).toInt());
    m_animationTimeLine->setCurveShape(curveShape);
    SetLayoutingEnabled(settings.value("GraphView/LayoutingEnabled", m_context->enabledLayouting).toBool());
    SetDiscreteDisplacementEnabled(settings.value("GraphView/DiscreteDisplacementEnabled", m_discreteDisplacementEnabled).toBool());
    m_context->layoutDisplacement = settings.value("GraphView/NodeDisplacement", static_cast<int>(floor(m_context->layoutDisplacement))).toInt();
    if (m_context->layoutDisplacement < 20)
        m_context->layoutDisplacement = 20;
    if (m_context->layoutDisplacement > 90)
        m_context->layoutDisplacement = 90;
    m_sliderDisplacement->setValue(static_cast<int>(floor(m_context->layoutDisplacement)));
    m_discreteDisplacementOffset = settings.value("GraphView/DiscreteDisplacementOffset", m_discreteDisplacementOffset).toInt();
    if (m_discreteDisplacementOffset < 1)
        m_discreteDisplacementOffset = 1;
    Node::NodeDescription description = static_cast<Node::NodeDescription>(settings.value("GraphView/NodeDescription", Node::EntityID).toInt());
    SetNodeLabels(description);
    SetControlsVisible(settings.value("GraphView/Controls", true).toBool());
    m_recordingInteractive = settings.value("GraphView/CapturingInteractivity", false).toBool();
}

GraphView::~GraphView()
{
    QSettings settings;
    if (m_dialogError)
        m_dialogError->close();
    if (m_dialogColor)
        m_dialogColor->close();
    if (m_dialogCapture)
        m_dialogCapture->close();
    delete m_context;
    delete m_validator;
    delete m_layouter;
    delete m_recorder;
    delete m_timerLayout;
    foreach (QGraphicsItemAnimation* animation, m_animationBuffer)
    {
        delete animation;
    }
    delete m_animationTimeLine;
    delete m_spinBoxTimeStep;
    delete m_sliderTimeStep;
    delete m_sliderDuration;
    delete m_sliderDisplacement;
    settings.setValue("ErrorDialog/Position", m_dialogError->pos());
    settings.setValue("ErrorDialog/Size", m_dialogError->size());
    delete m_dialogError;
    settings.setValue("ColorDialog/Position", m_dialogColor->pos());
    settings.setValue("ColorDialog/Size", m_dialogColor->size());
    delete m_dialogColor;
    delete m_dialogProgress;
    delete m_dialogCapture;
}

QString GraphView::GetGraphName() const
{
    if (m_context)
        return m_context->graphName;
    else
        return QString();
}

QString GraphView::GetFileName() const
{
    if (m_context)
        return m_context->fileCompleteName;
    else
        return QString();
}

void GraphView::UpdateConnections()
{
    if (m_validator)
    {
        emit ValidatorNameChanged(m_validator->Name());
        emit ValidatorDescriptionChanged(m_validator->Description());
    }
    if (m_layouter)
    {
        emit LayouterNameChanged(m_layouter->Name());
        emit LayouterDescriptionChanged(m_layouter->Description());
    }
    if (m_dialogError && m_dialogError->ErrorsDetected())
        emit Message(tr("Errors found, see \"Validation - Log\" for more information."), 0);
    else
        emit Message(NULL, 0);
    emit DiscreteDisplacementEnabled(m_discreteDisplacementEnabled);
    if (m_context)
    {
        if (!m_context->nodes.empty())
        {
            Node* node = m_context->nodes.values().at(0);
            if (node)
                emit NodeLabelsChanged(node->GetDescription());
        }
        emit LayoutingEnabled(m_context->enabledLayouting);
    }
    emit AnimationInProgress(m_animationIsRunning, m_recordingEnabled);
    if (m_timerLayout && m_timerLayout->isActive())
        emit LayoutingInProgress(m_timerLayout->isActive());
    if (this->isVisible())
        emit ControlsVisibilityChanged(m_sliderTimeStep->isVisible());
    else
    {
        QSettings settings;
        emit ControlsVisibilityChanged(settings.value("GraphView/Controls", true).toBool());
    }
}

void GraphView::SaveContext(Saver* saver)
{
    if (m_animationIsRunning)
        return;

    m_context->sceneMatrix = matrix();
    m_context->sceneViewCenter = mapToScene(QPoint(width() / 2, height() / 2));
    saver->Save(*m_context);
}

bool GraphView::IsAnimationRunning()
{
    return m_animationIsRunning;
}

void GraphView::SetControlsVisible(bool visible)
{
    m_spinBoxTimeStep->setVisible(visible);
    m_sliderTimeStep->setVisible(visible);
    m_sliderDuration->setVisible(visible);
    m_sliderDisplacement->setVisible(visible);
}

void GraphView::SetLayoutingEnabled(bool enable)
{
    if (m_context)
        m_context->enabledLayouting = enable;
    if (enable)
    {
        SetDiscreteDisplacementEnabled(false);
        LayoutStart();
    }
    else
        LayoutStop();
    emit LayoutingEnabled(enable);
    if (m_context)
        SetBackgroundColor(m_context->sceneBackground);
}

void GraphView::SetTimeStep(int timestep)
{
    if (!m_animationIsRunning && m_context)
    {
        if (timestep >= m_context->frames.count() && timestep > 0)
            m_timestep = m_context->frames.count() - 1;
        else
            m_timestep = timestep;
        m_calendarPosition = m_context->timesteps[m_timestep];
        foreach (Node* node, m_context->nodes)
        {
            Entity* entity = NULL;
            if (m_context->frames[m_timestep])
                entity = m_context->frames[m_timestep]->value(node);
            if (node)
            {
                node->SetEntity(entity);
                node->update(node->boundingRect());
            }
        }
        emit TimeStepChanged(m_timestep);
    }
}

void GraphView::SetDuration(int duration)
{
    if (m_animationTimeLine && m_sliderDuration)
    {
        if (duration > m_sliderDuration->minimum() && duration < m_sliderDuration->maximum())
        {
            m_timeoutZero = false;
            m_animationTimeLine->setDuration(duration);
        }
    }
}

void GraphView::SetDisplacement(int displacement)
{
    if (m_context)
    {
        m_context->layoutDisplacement = displacement;
        emit DisplacementChanged(displacement);
        LayoutStart();
    }
}

void GraphView::SetDiscreteDisplacementEnabled(bool enable)
{
    m_discreteDisplacementEnabled = enable;
    if (enable)
        SetLayoutingEnabled(false);
    emit DiscreteDisplacementEnabled(enable);
    if (m_context)
        SetBackgroundColor(m_context->sceneBackground);
}

void GraphView::SetDiscreteDisplacementOffset(int offset)
{
    m_discreteDisplacementOffset = offset;
    emit DiscreteDisplacementOffsetChanged(offset);
    if (m_context)
        SetBackgroundColor(m_context->sceneBackground);
}

void GraphView::SetValidator(const QString& name)
{
    if (m_animationIsRunning)
        return;

    if (m_validator && m_validator->Name() == name)
        return;
    Validator* newValidator = qobject_cast<Validator*>(Factory::CreateServant(Factory::sValidator, name));
    if (newValidator && m_context && m_dialogError)
    {
        delete m_validator;
        m_validator = newValidator;
        m_dialogError->Clear();
        connect(m_validator, SIGNAL(Error(const QString&)), m_dialogError, SLOT(Log(const QString&)));
        int timestep = m_timestep;
        if (timestep > 0)
            SetTimeStep(0);
        emit Message(tr("Validating..."), 0);
        m_validator->Validate(*m_context);
        emit Message(NULL, 0);
        SetTimeStep(timestep);
        if (m_dialogError->ErrorsDetected())
            emit Message(tr("Errors found, see \"Validation - Log\" for more information."), 0);
        emit ValidatorNameChanged(m_validator->Name());
        emit ValidatorDescriptionChanged(m_validator->Description());
        if (m_context->enabledColoring)
        {
            SetBackgroundColor(m_validator->GetColor(Validator::clBackground));
            foreach (Node* node, m_context->nodes)
            {
                if (node)
                {
                    node->SetColor(Node::clBackground, m_validator->GetColor(Validator::clNodeBackground));
                    node->SetColor(Node::clForeground, m_validator->GetColor(Validator::clNodeForeground));
                    node->SetColor(Node::clBoundary, m_validator->GetColor(Validator::clBoundary));
                    node->update(node->boundingRect());
                }
            }
            foreach (Entity* entity, m_context->entities)
            {
                if (entity)
                {
                    entity->SetColor(Entity::clBackground, m_validator->GetColor(Validator::clEntityBackground));
                    entity->SetColor(Entity::clForeground, m_validator->GetColor(Validator::clEntityForeground));
                    entity->SetColor(Entity::clBackgroundFinal, m_validator->GetColor(Validator::clEntityFinalBackground));
                    entity->SetColor(Entity::clForegroundFinal, m_validator->GetColor(Validator::clEntityFinalForeground));
                }
            }
            foreach (Edge* edge, m_context->edges)
            {
                if (edge)
                {
                    edge->SetColor(Edge::clLine, m_validator->GetColor(Validator::clBoundary));
                    edge->SetColor(Edge::clHighlight, m_validator->GetColor(Validator::clHighlight));
                    edge->update(edge->boundingRect());
                }
            }
            update();
        }
    }
}

void GraphView::SetLayouter(const QString& name)
{
    if (m_layouter && m_layouter->Name() == name)
        return;
    LayoutStop();
    Layouter* newLayouter = qobject_cast<Layouter*>(Factory::CreateServant(Factory::sLayouter, name));
    if (newLayouter && m_context)
    {
        delete m_layouter;
        m_layouter = newLayouter;
        emit LayouterNameChanged(m_layouter->Name());
        emit LayouterDescriptionChanged(m_layouter->Description());
        LayoutStart();
    }
}

void GraphView::SetBackgroundColor(const QColor& color)
{
    if (m_context)
        m_context->sceneBackground = color;
    setBackgroundBrush(QBrush(color));
    update();
}

void GraphView::SetNodeLabels(Node::NodeDescription type)
{
    emit NodeLabelsChanged(type);
}

void GraphView::SelectEvent(int timestep, int source, int destination, bool select)
{
    if (select)
        SetTimeStep(timestep);
    if (m_context)
    {
        if (Node* node = m_context->nodes[source])
        {
            node->setFlag(QGraphicsItem::ItemIsSelectable, select);
            node->setSelected(select);
        }
        if (Node* node = m_context->nodes[destination])
        {
            node->setFlag(QGraphicsItem::ItemIsSelectable, select);
            node->setSelected(destination);
        }
    }
}

void GraphView::LayoutRandomize()
{
    if (!m_animationIsRunning && m_context)
    {
        int halfSide = qRound(m_context->layoutDisplacement * m_context->nodes.size());
        int fullSide = halfSide * 2;
        foreach (Node* node, m_context->nodes)
        {
            if (node)
                node->setPos(-halfSide + qrand() % fullSide, -halfSide + qrand() % fullSide);
        }
        foreach (Edge* edge, m_context->edges)
        {
            if (edge)
                edge->Adapt();
        }
    }
}

void GraphView::LayoutDiscrete()
{
    if (!m_animationIsRunning && m_context)
    {
        foreach (Node* node, m_context->nodes)
        {
            if (node)
                node->AlignPosition();
        }
    }
}

void GraphView::LayoutStart()
{
    if (m_context && m_timerLayout && m_context->enabledLayouting && !m_timerLayout->isActive() && !m_animationIsRunning)
    {
        m_timerLayout->start(10);
        emit LayoutingInProgress(true);
    }
}

void GraphView::LayoutStop()
{
    if (m_timerLayout)
    {
        m_timerLayout->stop();
        emit LayoutingInProgress(false);
    }
}

void GraphView::AnimationStart()
{
    if (m_animationIsRunning)
        return;
    if (m_dialogColor)
        m_dialogColor->close();
    if (m_timerLayout && m_timerLayout->isActive())
    {
        LayoutStop();
        m_layoutingWasRunning = true;
    }
    else
        m_layoutingWasRunning = false;
    if (m_context)
        m_layoutingWasEnabled = m_context->enabledLayouting;
    LayoutLock(true);
    m_animationIsRunning = true;
    m_animationStopRequest = false;
    emit AnimationInProgress(true, m_recordingEnabled);
    AnimateTimeStep();
}

void GraphView::AnimationStep()
{
    AnimationStart();
    AnimationStop();
}

void GraphView::AnimationStop()
{
    m_animationStopRequest = true;
    if (m_recordingEnabled && m_renderMode == GraphView::rmBufferSave)
    {
        if (VideoRecorder* vr = qobject_cast<VideoRecorder*>(m_recorder))
            vr->Stop();
    }
    if (m_dialogProgress)
        m_dialogProgress->setValue(m_dialogProgress->maximum());
}

void GraphView::AnimationReset()
{
    SetTimeStep(0);
}

void GraphView::ShowSetupDialog()
{
    QSettings settings;
    if (m_animationIsRunning || !m_context)
        return;

    SetupDialog* setupDialog = new SetupDialog(this);
    QPoint windowPosition = settings.value("SetupDialog/Position", setupDialog->pos()).toPoint();
    setupDialog->move(windowPosition);
    setupDialog->setWindowTitle(setupDialog->windowTitle() + tr(" (%1)").arg(m_context->graphName));

    if (m_animationTimeLine)
    {
        if (m_timeoutZero)
            setupDialog->SetAnimationLength(0);
        else
            setupDialog->SetAnimationLength(m_animationTimeLine->duration());
        setupDialog->SetAnimationStyle(m_animationTimeLine->curveShape());
    }

    if (m_context->enabledLayouting)
        setupDialog->SetLayoutingMode(SetupDialog::lmAutomaticContinuous);
    else if (m_discreteDisplacementEnabled)
        setupDialog->SetLayoutingMode(SetupDialog::lmManualDiscrete);
    else
        setupDialog->SetLayoutingMode(SetupDialog::lmManualContinuous);
    if (m_layouter)
        setupDialog->SetLayouterName(m_layouter->Name());
    setupDialog->SetNodeDisplacement(qRound(m_context->layoutDisplacement));
    setupDialog->SetGridOffset(m_discreteDisplacementOffset);

    if (m_validator)
        setupDialog->SetValidatorName(m_validator->Name());

    Node* node = NULL;
    if (!m_context->nodes.empty())
        node = m_context->nodes.value(m_context->nodes.keys().at(0));
    if (node)
        setupDialog->SetNodeDescription(node->GetDescription());
    if (m_sliderTimeStep)
        setupDialog->SetControlsVisibility(m_sliderTimeStep->isVisible());
    setupDialog->SetInteractiveCapturing(m_recordingInteractive);

    setupDialog->exec();
    if (setupDialog->result() == QDialog::Accepted)
    {
        if (m_animationTimeLine)
        {
            if (setupDialog->GetAnimationLength() == 0)
                m_timeoutZero = true;
            else
            {
                m_timeoutZero = false;
                m_animationTimeLine->setDuration(setupDialog->GetAnimationLength());
            }
            emit DurationChanged(setupDialog->GetAnimationLength());
            m_animationTimeLine->setCurveShape(setupDialog->GetAnimationStyle());
        }

        SetLayouter(setupDialog->GetLayouterName());
        SetDisplacement(setupDialog->GetNodeDisplacement());
        SetDiscreteDisplacementOffset(setupDialog->GetGridOffset());
        switch (setupDialog->GetLayoutingMode())
        {
            case SetupDialog::lmManualContinuous:
                SetDiscreteDisplacementEnabled(false);
                SetLayoutingEnabled(false);
                break;
            case SetupDialog::lmManualDiscrete:
                SetLayoutingEnabled(false);
                SetDiscreteDisplacementEnabled(true);
                break;
            case SetupDialog::lmAutomaticContinuous:
                SetDiscreteDisplacementEnabled(false);
                SetLayoutingEnabled(true);
                break;
            default:
                SetDiscreteDisplacementEnabled(false);
                SetLayoutingEnabled(false);
                break;
        }

        SetValidator(setupDialog->GetValidatorName());

        SetNodeLabels(setupDialog->GetNodeDescription());
        SetControlsVisible(setupDialog->GetControlsVisibility());
        m_recordingInteractive = setupDialog->GetInteractiveCapturing();
    }
    settings.setValue("SetupDialog/Position", setupDialog->pos());
    delete setupDialog;
}

void GraphView::ShowColorDialog()
{
    if (!m_animationIsRunning)
        m_dialogColor->show();
}

void GraphView::ShowErrorDialog()
{
    if (m_dialogError)
        m_dialogError->show();
}

void GraphView::Snapshot()
{
    ShowCaptureDialog(CaptureDialog::mSnapshot);
}

void GraphView::Sequence()
{
    ShowCaptureDialog(CaptureDialog::mSequence);
}

void GraphView::SnapshotHandler()
{
    m_renderMode = GraphView::rmDirectSave;
    Render();
    emit Message(tr("Image Saved"), 1500);
}

void GraphView::SequenceHandler()
{
    LayoutStop();
    if (qobject_cast<ImageRecorder*>(m_recorder))
    {
        m_renderMode = GraphView::rmDirectSave;
        if (!m_recordingInteractive)
        {
            delete m_dialogProgress;
            m_dialogProgress = new QProgressDialog(tr("Saving images"), tr("Stop"), m_captureTimeStepBegin, m_captureTimeStepEnd);
            m_dialogProgress->setWindowTitle(tr("Capturing..."));
            m_dialogProgress->setWindowFlags(Qt::WindowTitleHint);
            m_dialogProgress->setWindowModality(Qt::ApplicationModal);
            m_dialogProgress->setWindowIcon(QIcon(":/icons/images/graphrec.png"));
            m_dialogProgress->setMinimumDuration(0);

            this->setVisible(false);
            for (int i = m_captureTimeStepBegin; i <= m_captureTimeStepEnd; i++)
            {
                if (m_captureIntervalTemp == 0)
                {
                    SetTimeStep(i);
                    m_captureIntervalTemp = m_captureInterval;
                    QString nameTemp = m_captureFileName;
                    m_captureFileName += QString::number(i);
                    qApp->processEvents();
                    Render();
                    m_captureFileName = nameTemp;
                    m_dialogProgress->setValue(i);
                    if (m_dialogProgress->wasCanceled())
                        break;
                }
                else
                    m_captureIntervalTemp--;
            }
            this->setVisible(true);
        }
        else
        {
            SetTimeStep(m_captureTimeStepBegin);
            m_recordingEnabled = true;
            AnimationStart();
        }
    }
    else if (VideoRecorder* vr = qobject_cast<VideoRecorder*>(m_recorder))
    {
        SetTimeStep(m_captureTimeStepBegin);
        m_fps = vr->GetFPS();
        m_renderMode = GraphView::rmBufferSave;
        m_bufferPosition = 0;
        if (vr->Start(m_captureFilePath, m_captureFileName, m_captureWidth, m_captureHeight))
        {
            delete m_dialogProgress;
            if (!m_recordingInteractive)
            {
                m_dialogProgress = new QProgressDialog(tr("Encoding video"), tr("Stop"), 0, m_captureTimeStepEnd - m_captureTimeStepBegin);
                m_dialogProgress->setWindowTitle(tr("Capturing..."));
                m_dialogProgress->setWindowFlags(Qt::WindowTitleHint);
                m_dialogProgress->setWindowModality(Qt::ApplicationModal);
                m_dialogProgress->setWindowIcon(QIcon(":/icons/images/graphrec.png"));
                m_dialogProgress->setMinimumDuration(0);
                this->setVisible(false);
            }
            m_recordingEnabled = true;
            AnimationStart();
        }
        else
            m_recordingEnabled = false;
    }
}

void GraphView::ShowCaptureDialog(CaptureDialog::Mode mode)
{
    if (m_dialogCapture)
        m_dialogCapture->close();
    delete m_dialogCapture;
    if ((m_animationIsRunning && mode == CaptureDialog::mSequence) || !m_context)
        return;
    QSettings settings;

    m_dialogCapture = new CaptureDialog(mode, this);
    QPoint windowPosition = settings.value("CaptureDialog/Position", m_dialogCapture->pos()).toPoint();
    m_dialogCapture->move(windowPosition);
    m_dialogCapture->setWindowTitle(m_dialogCapture->windowTitle() + tr(" (%1)").arg(m_context->graphName));

    if (mode == CaptureDialog::mSnapshot)
        m_dialogCapture->SetRecorderIndex(settings.value("CaptureDialog/SnapshotRecorderIndex", 0).toInt());
    else if (mode == CaptureDialog::mSequence)
        m_dialogCapture->SetRecorderIndex(settings.value("CaptureDialog/SequenceRecorderIndex", 0).toInt());
    m_dialogCapture->SetWidth(settings.value("CaptureDialog/Width", 720).toInt());
    m_dialogCapture->SetHeight(settings.value("CaptureDialog/Height", 576).toInt());
    m_dialogCapture->SetMaxTimestep(m_context->frames.count() - 1);
    m_dialogCapture->SetStop(m_context->frames.count() - 1);
    m_dialogCapture->SetStart(0);
    m_dialogCapture->SetInterval(0);
    m_dialogCapture->SetName(settings.value("CaptureDialog/Name", tr("captured")).toString());
    m_dialogCapture->SetPath(settings.value("CaptureDialog/Path", QDir::homePath()).toString());

    connect(m_dialogCapture, SIGNAL(accepted()), this, SLOT(CaptureDialogHandler()));
    connect(m_dialogCapture, SIGNAL(rejected()), this, SLOT(CaptureDialogHandler()));
    if (mode == CaptureDialog::mSnapshot)
    {
        m_dialogCapture->setModal(false);
        m_dialogCapture->setWindowModality(Qt::NonModal);
        m_dialogCapture->show();
    }
    else if (mode == CaptureDialog::mSequence)
    {
        m_dialogCapture->setModal(true);
        m_dialogCapture->setWindowModality(Qt::WindowModal);
        m_dialogCapture->exec();
    }
    else
        CaptureDialogHandler();
}

void GraphView::Render()
{
    QPainter painter;
    QPaintDevice* device = NULL;
    QImage image(m_captureWidth, m_captureHeight, QImage::Format_RGB32);

    if (m_renderMode == GraphView::rmDirectSave)
    {
        if (m_recorder)
        {
            ImageRecorder* rec = qobject_cast<ImageRecorder*>(m_recorder);
            if (rec)
            {
                device = rec->GetPaintDevice(m_captureFilePath, m_captureFileName, m_captureWidth, m_captureHeight);
                painter.begin(device);
            }
            else
                return;
        }
    }
    else if (m_renderMode == GraphView::rmBufferSave)
    {
        painter.begin(&image);
    }

    painter.setRenderHint(QPainter::Antialiasing, true);

    QRect rect = viewport()->rect();
    double w = static_cast<double>(rect.width());
    double h = static_cast<double>(rect.height());
    double ratioInner = w / h;
    double ratioOuter = static_cast<double>(m_captureWidth) / static_cast<double>(m_captureHeight);
    if (ratioInner < ratioOuter)
        rect.setWidth(static_cast<int>(floor(ratioOuter * h)));
    else
        rect.setHeight(static_cast<int>(floor(w / ratioOuter)));

    render(&painter, QRectF(0, 0, m_captureWidth, m_captureHeight), rect, Qt::KeepAspectRatioByExpanding);

    painter.end();

    if (m_renderMode == GraphView::rmDirectSave)
    {
        if (m_recorder)
        {
            ImageRecorder* rec = qobject_cast<ImageRecorder*>(m_recorder);
            if (rec)
                rec->SaveImage(m_captureFilePath, m_captureFileName, device);
            delete device;
        }
    }
    else if (m_renderMode == GraphView::rmBufferSave)
    {
        while (!G_GRVideoFree.tryAcquire(1, 500))
            qApp->processEvents();
        G_GRVideoBuffer[m_bufferPosition] = image;
        G_GRVideoUsed.release();

        m_bufferPosition++;
        m_bufferPosition %= G_GRVideoBufferSize;
    }
}

void GraphView::ScaleView(double factor)
{
    double desiredScale = matrix().scale(factor, factor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (desiredScale < 0.01 || desiredScale > 50)
        return;

    scale(factor, factor);
    if (m_context)
    {
        m_context->sceneMatrix = matrix();
        m_context->sceneViewCenter = mapToScene(QPoint(width() / 2, height() / 2));
    }
}

void GraphView::RotateGraph(double angle)
{
    if (!m_animationIsRunning && m_context && !m_context->enabledLayouting)
    {
        m_context->sceneAngle -= angle;

        if (m_discreteDisplacementEnabled)
        {
            foreach (Node* node, m_context->nodes)
            {
                if (node)
                    node->SetDiscreteDisplacementEnabled(false);
            }
        }
        QList<QGraphicsItem*> nodeList;
        foreach (Node* node, m_context->nodes)
        {
            nodeList.append(node);
        }
        QGraphicsItemGroup* group = scene()->createItemGroup(nodeList);
        if (group)
        {
            group->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
            QPointF c = mapToScene(mapFromGlobal(QCursor::pos()));
            group->setTransform(QTransform().translate(c.x(), c.y()).rotate(angle).translate(-c.x(), -c.y()));
        }
        scene()->destroyItemGroup(group);
        foreach (Node* node, m_context->nodes)
        {
            if (node)
                node->rotate(-angle);
        }
        foreach (Edge* edge, m_context->edges)
        {
            if (edge)
                edge->Adapt();
        }
        if (m_discreteDisplacementEnabled)
        {
            foreach (Node* node, m_context->nodes)
            {
                if (node)
                    node->SetDiscreteDisplacementEnabled(true);
            }
        }
    }
}

void GraphView::ScrollGraph(double dx, double dy)
{
    if (!m_animationIsRunning && m_context && !m_context->enabledLayouting)
    {
        foreach (QGraphicsItem* item, scene()->items())
        {
            if (Node* node = qgraphicsitem_cast<Node*>(item))
                node->moveBy(dx, dy);
        }
        foreach (QGraphicsItem* item, scene()->items())
        {
            if (Edge* edge = qgraphicsitem_cast<Edge*>(item))
                edge->Adapt();
        }
    }
}

void GraphView::keyPressEvent(QKeyEvent* event)
{
    double scrollStep = 15.0;
    if (m_discreteDisplacementEnabled)
        scrollStep = m_discreteDisplacementOffset;
    double scaleStep = 1.2;
    switch (event->key())
    {
        case Qt::Key_I:
            ScrollGraph(0, -scrollStep);
            break;
        case Qt::Key_K:
            ScrollGraph(0, scrollStep);
            break;
        case Qt::Key_J:
            ScrollGraph(-scrollStep, 0);
            break;
        case Qt::Key_L:
            ScrollGraph(scrollStep, 0);
            break;
        case Qt::Key_W:
        case Qt::Key_Plus:
            ScaleView(scaleStep);
            break;
        case Qt::Key_S:
        case Qt::Key_Minus:
            ScaleView(1 / scaleStep);
            break;
        case Qt::Key_A:
            RotateGraph(-1);
            break;
        case Qt::Key_D:
            RotateGraph(1);
            break;
        case Qt::Key_R:
            LayoutRandomize();
            break;
        case Qt::Key_Control:
            if (!m_animationIsRunning && m_context)
            {
                setDragMode(QGraphicsView::RubberBandDrag);
                foreach (Node* node, m_context->nodes)
                {
                    node->setFlag(QGraphicsItem::ItemIsSelectable);
                }
            }
            break;
        default:
            QGraphicsView::keyPressEvent(event);
    }
}

void GraphView::keyReleaseEvent(QKeyEvent* event)
{
    switch (event->key())
    {
        case Qt::Key_Control:
            if (!m_animationIsRunning)
            {
                if (m_dialogColor->isVisible())
                {
                    QList<Node*> selectedNodes;
                    foreach (QGraphicsItem* item, this->scene()->selectedItems())
                    {
                        if (Node* node = qgraphicsitem_cast<Node*>(item))
                            selectedNodes.append(node);
                    }
                    m_dialogColor->SelectItems(selectedNodes);
                }

                setDragMode(QGraphicsView::ScrollHandDrag);
                foreach (Node* node, m_context->nodes)
                {
                    node->setFlag(QGraphicsItem::ItemIsSelectable, false);
                }
                viewport()->setCursor(Qt::ArrowCursor);
            }
            break;
        default:
            QGraphicsView::keyPressEvent(event);
    }
}

void GraphView::mouseMoveEvent(QMouseEvent* event)
{
    QGraphicsView::mouseMoveEvent(event);
    if (viewport()->cursor().shape() != Qt::ArrowCursor)
        viewport()->setCursor(Qt::ArrowCursor);
}

void GraphView::mousePressEvent(QMouseEvent* event)
{
    QGraphicsView::mousePressEvent(event);
    if (viewport()->cursor().shape() != Qt::ArrowCursor)
        viewport()->setCursor(Qt::ArrowCursor);
    if (!hasFocus())
        setFocus();
}

void GraphView::mouseReleaseEvent(QMouseEvent* event)
{
    QGraphicsView::mouseReleaseEvent(event);
    if (viewport()->cursor().shape() != Qt::ArrowCursor)
        viewport()->setCursor(Qt::ArrowCursor);
}

void GraphView::wheelEvent(QWheelEvent* event)
{
    int degrees = event->delta() / 8;
    double steps = degrees / 60.0;
    ScaleView(pow(2.0, steps));
    if (!hasFocus())
        setFocus();
}

void GraphView::focusInEvent(QFocusEvent* event)
{
    emit HasFocus(this);
    QGraphicsView::focusInEvent(event);
}

void GraphView::drawBackground(QPainter* painter, const QRectF& rect)
{
    QGraphicsView::drawBackground(painter, rect);
    if (m_discreteDisplacementEnabled)
    {
        int offset = m_discreteDisplacementOffset;

        int left = static_cast<int>(floor(rect.left()));
        int right = static_cast<int>(ceil(rect.right()));
        int top = static_cast<int>(floor(rect.top()));
        int bottom = static_cast<int>(ceil(rect.bottom()));
        int leftAligned = left - (left % offset);
        int topAligned = top - (top % offset);

        QVector<QLine> lines;
        for (int x = leftAligned; x <= right; x += offset)
            lines.append(QLine(x, top, x, bottom));
        for (int y = topAligned; y <= bottom; y += offset)
            lines.append(QLine(left, y, right, y));

        QColor inverseBackground;
        int r = 0; int g = 0; int b = 0;
        backgroundBrush().color().getRgb(&r, &g, &b);
        inverseBackground.setRgb(255 - r, 255 - g, 255 - b);
        painter->setPen(QPen(inverseBackground.lighter(250), 1));
        painter->drawLines(lines.data(), lines.size());
    }
}

void GraphView::CaptureDialogHandler()
{
    QSettings settings;
    settings.setValue("CaptureDialog/Position", m_dialogCapture->pos());
    if (m_dialogCapture->result() == QDialog::Accepted)
    {
        if (m_recorder.data() != m_dialogCapture->GetRecorder())
        {
            delete m_recorder;
            m_recorder = m_dialogCapture->GetRecorder();
        }
        if (m_dialogCapture->GetMode() == CaptureDialog::mSnapshot)
            settings.setValue("CaptureDialog/SnapshotRecorderIndex", m_dialogCapture->GetRecorderIndex());
        else if (m_dialogCapture->GetMode() == CaptureDialog::mSequence)
            settings.setValue("CaptureDialog/SequenceRecorderIndex", m_dialogCapture->GetRecorderIndex());
        m_captureWidth = m_dialogCapture->GetWidth();
        settings.setValue("CaptureDialog/Width", m_captureWidth);
        m_captureHeight = m_dialogCapture->GetHeight();
        settings.setValue("CaptureDialog/Height", m_captureHeight);
        m_captureTimeStepBegin = m_dialogCapture->GetStart();
        m_captureTimeStepEnd = m_dialogCapture->GetStop();
        m_captureInterval = m_dialogCapture->GetInterval();
        m_captureFileName = m_dialogCapture->GetName();
        settings.setValue("CaptureDialog/Name", m_captureFileName);
        m_captureFilePath = m_dialogCapture->GetPath();
        settings.setValue("CaptureDialog/Path", m_captureFilePath);

        if (m_dialogCapture->GetMode() == CaptureDialog::mSnapshot)
            SnapshotHandler();
        else if (m_dialogCapture->GetMode() == CaptureDialog::mSequence)
        {
            SequenceHandler();
            delete m_dialogCapture;
        }
    }
}

void GraphView::AnimateTimeStep()
{
    if (!m_context)
        return;

    for (int i = 0; i < m_nodeBuffer.size(); ++i)
    {
        Node* animatedNode = m_nodeBuffer[i].first;
        Node* destinationNode = m_nodeBuffer[i].second;
        if (animatedNode && destinationNode)
        {
            Entity* entity = animatedNode->GetEntity();
            destinationNode->SetEntity(entity);
            destinationNode->update(destinationNode->boundingRect());
            scene()->removeItem(animatedNode);
            delete animatedNode;
        }
    }
    m_nodeBuffer.clear();

    for (int i = 0; i < m_edgeBuffer.size(); ++i)
    {
        Edge* edge = m_edgeBuffer[i];
        scene()->removeItem(edge);
        delete edge;
    }
    m_edgeBuffer.clear();

    for (int i = 0; i < m_animationBuffer.size(); ++i)
        delete m_animationBuffer[i];
    m_animationBuffer.clear();

    if (m_recordingEnabled)
    {
        if (m_timestep >= m_captureTimeStepEnd)
            AnimationStop();
    }
    if (m_recordingEnabled && m_renderMode == GraphView::rmDirectSave)
    {
        if (m_captureIntervalTemp == 0)
        {
            m_captureIntervalTemp = m_captureInterval;
            QString nameTemp = m_captureFileName;
            m_captureFileName += QString::number(m_timestep);
            Render();
            m_captureFileName = nameTemp;
        }
        else
            m_captureIntervalTemp--;
    }

    if (m_animationStopRequest)
    {
        this->setVisible(true);
        m_recordingEnabled = false;
        m_animationIsRunning = false;
        m_animationStopRequest = false;
        LayoutLock(false);
        if ((m_context->enabledLayouting && m_layoutingWasEnabled && m_layoutingWasRunning)
            || (m_context->enabledLayouting && !m_layoutingWasEnabled))
            LayoutStart();
        emit AnimationInProgress(false, m_recordingEnabled);
        if (m_calendarPosition < m_context->calendar.size())
            emit Stepped(this, true);
        else
            emit Stepped(this, false);
        return;
    }

    while (m_calendarPosition < m_context->calendar.size() && m_context->calendar[m_calendarPosition].timestep == m_timestep)
    {
        Context::CalendarEvent event = m_context->calendar[m_calendarPosition];
        Context::Move move = event.move;
        Node* sourceNode = m_context->nodes[move.first];
        Node* destinationNode = m_context->nodes[move.second];
        if (sourceNode && destinationNode && event.valid)
        {
            if (event.reverse)
            {
                Node* temp = destinationNode;
                destinationNode = sourceNode;
                sourceNode = temp;
            }
            Node* animatedNode = sourceNode->CloneShallow();
            scene()->addItem(animatedNode);
            sourceNode->SetEntity(NULL);
            sourceNode->update(sourceNode->boundingRect());
            Context::Ends lr(sourceNode->GetID(), destinationNode->GetID());
            Context::Ends rl(destinationNode->GetID(), sourceNode->GetID());
            Edge* edge = m_context->edges[lr];
            if (!edge)
                edge = m_context->edges[rl];
            if (edge)
            {
                Edge* highlightedEdge = edge->CloneShallow();
                highlightedEdge->SetHighlight(true);
                m_edgeBuffer.append(highlightedEdge);
                scene()->addItem(highlightedEdge);
                highlightedEdge->update(highlightedEdge->boundingRect());
            }

            m_nodeBuffer.append(GraphView::Animation(animatedNode, destinationNode));

            QGraphicsItemAnimation* nodeAnimation = new QGraphicsItemAnimation();
            nodeAnimation->setItem(animatedNode);
            nodeAnimation->setTimeLine(m_animationTimeLine);
            nodeAnimation->setPosAt(0, sourceNode->pos());
            nodeAnimation->setPosAt(1, destinationNode->pos());

            m_animationBuffer.append(nodeAnimation);
        }
        m_calendarPosition++;
    }
    m_timestep++;
    emit TimeStepChanged(m_timestep);

    if (m_calendarPosition < m_context->calendar.size() || !m_animationBuffer.empty())
    {
        if (m_animationBuffer.empty())
            emit AnimationStepDone();
        else if (m_recordingEnabled && m_renderMode == GraphView::rmBufferSave)
        {
            int interval = 1000 / m_fps;
            int time = 0;
            while (time < m_animationTimeLine->duration())
            {
                foreach (QGraphicsItemAnimation* animation, m_animationBuffer)
                {
                    animation->setStep(m_animationTimeLine->valueForTime(time));
                }
                Render();
                qApp->processEvents();
                time += interval;
            }
            if (m_dialogProgress)
            {
                m_dialogProgress->setValue(m_dialogProgress->value() + 1);
                if (m_dialogProgress->wasCanceled())
                    AnimationStop();
            }
            emit AnimationStepDone();
        }
        else if (m_timeoutZero)
            emit AnimationStepDone();
        else
            m_animationTimeLine->start();
    }
    else
    {
        AnimationStop();
        emit AnimationStepDone();
    }
}

void GraphView::NodeMoved()
{
    LayoutStart();
}

void GraphView::LayoutLock(bool enable)
{
    if (!m_context)
        return;
    if (enable)
    {
        foreach (Node* node, m_context->nodes)
        {
            node->setFlag(QGraphicsItem::ItemIsMovable, false);
        }
    }
    else
    {
        foreach (Node* node, m_context->nodes)
        {
            node->setFlag(QGraphicsItem::ItemIsMovable, true);
        }
    }
}

void GraphView::on_timerLayout_timeout()
{
    if (!m_layouter || !m_context)
    {
        LayoutStop();
        return;
    }
    if (m_layouter->Layout(*m_context))
        LayoutStop();
}
