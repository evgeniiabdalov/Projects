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

#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <QtCore/QPointer>
#include <QtCore/QPair>
#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtGui/QGraphicsView>

#include "primitives/node.h"
#include "dialogs/capturedialog.h"

class QFile;
class QString;
class QTimer;
class QKeyEvent;
class QWheelEvent;
class QMouseEvent;
class QFocusEvent;
class QTimeLine;
class QGraphicsItemAnimation;
class QSpinBox;
class QSlider;
class QColor;
class QWidget;
class QProgressDialog;
class Context;
class Validator;
class Layouter;
class Saver;
class Recorder;
class ErrorDialog;
class ColorDialog;

class GraphView : public QGraphicsView
{
    Q_OBJECT

signals:
    void HasFocus(GraphView* who);
    void ValidatorNameChanged(const QString& name);
    void LayouterNameChanged(const QString& name);
    void ValidatorDescriptionChanged(const QString& description);
    void LayouterDescriptionChanged(const QString& description);
    void Message(const QString& message, int timeout);
    void TimeStepChanged(int timestep);
    void DiscreteDisplacementEnabled(bool isEnabled);
    void DiscreteDisplacementOffsetChanged(int offset);
    void DisplacementChanged(int displacement);
    void DurationChanged(int duration);
    void NodeLabelsChanged(Node::NodeDescription type);
    void ControlsVisibilityChanged(bool areVisible);
    void LayoutingEnabled(bool isEnabled);
    void LayoutingInProgress(bool inProgress);
    void AnimationInProgress(bool inProgress, bool isCapturing);
    void AnimationStepDone();
    void Stepped(GraphView* who, bool nextStep);

public:
    enum RenderMode
    {
        rmDirectSave = 0,
        rmBufferSave
    };

    GraphView(QFile& file, qint64 filePosition, QString& parserName, QString& validatorName, QString& layouterName, QWidget* parent = 0);
    ~GraphView();

    QString GetGraphName() const;
    QString GetFileName() const;
    void UpdateConnections();
    void SaveContext(Saver* saver);
    bool IsAnimationRunning();

public slots:
    void SetControlsVisible(bool visible = true);
    void SetLayoutingEnabled(bool enable = true);
    void SetTimeStep(int timestep);
    void SetDuration(int duration);
    void SetDisplacement(int displacement);
    void SetDiscreteDisplacementEnabled(bool enable = true);
    void SetDiscreteDisplacementOffset(int offset);
    void SetValidator(const QString& name);
    void SetLayouter(const QString& name);
    void SetBackgroundColor(const QColor& color);
    void SetNodeLabels(Node::NodeDescription type);
    void SelectEvent(int timestep, int source, int destination, bool select);
    void LayoutRandomize();
    void LayoutDiscrete();
    void LayoutStart();
    void LayoutStop();
    void AnimationStart();
    void AnimationStep();
    void AnimationStop();
    void AnimationReset();
    void ShowSetupDialog();
    void ShowColorDialog();
    void ShowErrorDialog();
    void Snapshot();
    void Sequence();

protected:
    void SnapshotHandler();
    void SequenceHandler();
    void ShowCaptureDialog(CaptureDialog::Mode mode);
    void Render();
    void ScaleView(double factor);
    void RotateGraph(double angle);
    void ScrollGraph(double dx, double dy);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
    void focusInEvent(QFocusEvent* event);
    void drawBackground(QPainter* painter, const QRectF& rect);

protected slots:
    void CaptureDialogHandler();
    void AnimateTimeStep();
    void NodeMoved();
    void LayoutLock(bool enable = true);
    void on_timerLayout_timeout();

private:
    QPointer<Context> m_context;
    QPointer<Validator> m_validator;
    QPointer<Layouter> m_layouter;
    QPointer<Recorder> m_recorder;

    QPointer<QTimer> m_timerLayout;
    bool m_discreteDisplacementEnabled;
    int m_discreteDisplacementOffset;

    int m_calendarPosition;
    int m_timestep;
    bool m_animationIsRunning;
    bool m_animationStopRequest;
    QPointer<QTimeLine> m_animationTimeLine;

    // Animation-specific temporary storage
    bool m_layoutingWasEnabled;
    bool m_layoutingWasRunning;
    bool m_timeoutZero;
    int m_captureIntervalTemp;
    typedef QPair<QPointer<Node>, QPointer<Node> > Animation; // QPair<animatedNode, destinationNode>
    QList<Animation> m_nodeBuffer;
    QList<QPointer<Edge> > m_edgeBuffer;
    QList<QGraphicsItemAnimation*> m_animationBuffer;
    // ------------------------------------

    // Recording-specific variables
    bool m_recordingEnabled;
    bool m_recordingInteractive;
    int m_bufferPosition;
    int m_fps;
    GraphView::RenderMode m_renderMode;
    int m_captureWidth;
    int m_captureHeight;
    int m_captureTimeStepBegin;
    int m_captureTimeStepEnd;
    int m_captureInterval;
    QString m_captureFileName;
    QString m_captureFilePath;
    // ------------------------------------

    QPointer<QSpinBox> m_spinBoxTimeStep;
    QPointer<QSlider> m_sliderTimeStep;
    QPointer<QSlider> m_sliderDuration;
    QPointer<QSlider> m_sliderDisplacement;
    QPointer<ErrorDialog> m_dialogError;
    QPointer<ColorDialog> m_dialogColor;
    QPointer<CaptureDialog> m_dialogCapture;
    QPointer<QProgressDialog> m_dialogProgress;
};

#endif
