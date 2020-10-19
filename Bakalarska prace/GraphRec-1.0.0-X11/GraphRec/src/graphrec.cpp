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

#include <QtCore/QCoreApplication>
#include <QtCore/QEventLoop>
#include <QtCore/QSettings>
#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtCore/QIODevice>
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QList>
#include <QtCore/QSignalMapper>
#include <QtCore/QRegExp>
#include <QtCore/QPoint>
#include <QtCore/QSize>
#include <QtGui/QApplication>
#include <QtGui/QMainWindow>
#include <QtGui/QFileDialog>
#include <QtGui/QInputDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QWidget>
#include <QtGui/QFrame>
#include <QtGui/QTabWidget>
#include <QtGui/QSplitter>
#include <QtGui/QMessageBox>
#include <QtGui/QCloseEvent>
#include <QtGui/QLabel>
#include <QtGui/QAction>

#include "graphrec.h"
#include "ui_graphrec.h"
#include "graphview.h"
#include "dialogs/opendialog.h"
#include "dialogs/helpdialog.h"
#include "factory.h"
#include "savers/saver.h"

GraphRec::GraphRec(QWidget* parent) :
        QMainWindow(parent),
        m_ui(new Ui::GraphRec)
{
    m_ui->setupUi(this);
    QCoreApplication::setOrganizationName("koupy.net");
    QCoreApplication::setOrganizationDomain("koupy.net");
    QCoreApplication::setApplicationName("GraphRec");

    m_inputDialog = new QInputDialog(this, Qt::WindowTitleHint);
    m_inputDialog->setWindowTitle(tr("Seek"));
    m_inputDialog->setInputMode(QInputDialog::IntInput);
    m_inputDialog->setIntMinimum(0);
    m_inputDialog->setIntMaximum(INT_MAX);
    m_inputDialog->setIntStep(1);
    m_inputDialog->setIntValue(0);
    m_inputDialog->setLabelText(tr("Timestep:"));

    m_signalMapperValidators = new QSignalMapper(this);
    QStringList validators = Factory::GetServantNames(Factory::sValidator);
    foreach (QString validator, validators)
    {
        QAction* action = new QAction(validator, this);
        action->setCheckable(true);
        action->setChecked(false);
        m_ui->menuValidator->addAction(action);
        connect(action, SIGNAL(triggered()), action, SLOT(toggle()));
        connect(action, SIGNAL(triggered()), m_signalMapperValidators, SLOT(map()));
        m_signalMapperValidators->setMapping(action, action->text());
    }

    m_signalMapperLayouters = new QSignalMapper(this);
    QStringList layouters = Factory::GetServantNames(Factory::sLayouter);
    foreach (QString layouter, layouters)
    {
        QAction* action = new QAction(layouter, this);
        action->setCheckable(true);
        action->setChecked(false);
        m_ui->menuLayouter->addAction(action);
        connect(action, SIGNAL(triggered()), action, SLOT(toggle()));
        connect(action, SIGNAL(triggered()), m_signalMapperLayouters, SLOT(map()));
        m_signalMapperLayouters->setMapping(action, action->text());
    }

    m_labelStatus = new QLabel();
    m_labelValidator = new QLabel();
    m_labelLayouter = new QLabel();
    m_ui->statusBar->addWidget(m_labelStatus);
    // m_ui->statusBar->addPermanentWidget(m_labelLayouter);
    m_ui->statusBar->addPermanentWidget(m_labelValidator);

    connect(m_ui->actionToolbar, SIGNAL(toggled(bool)), m_ui->toolBarMain, SLOT(setVisible(bool)));

    connect(m_ui->actionPlay, SIGNAL(triggered()), this , SLOT(RemoveCurrentFromSynchronized()));
    connect(m_ui->actionStep, SIGNAL(triggered()), this , SLOT(RemoveCurrentFromSynchronized()));
    connect(m_ui->actionStop, SIGNAL(triggered()), this , SLOT(RemoveCurrentFromSynchronized()));
    connect(m_ui->actionSeek, SIGNAL(triggered()), this , SLOT(RemoveCurrentFromSynchronized()));
    connect(m_ui->actionReset, SIGNAL(triggered()), this , SLOT(RemoveCurrentFromSynchronized()));

    setWindowTitle(tr("GraphRec"));

    QSettings settings;
    QPoint windowPosition = settings.value("MainWindow/Position", pos()).toPoint();
    QSize windowSize = settings.value("MainWindow/Size", size()).toSize();
    resize(windowSize);
    move(windowPosition);
    restoreState(settings.value("MainWindow/ToolbarState").toByteArray());
    bool toolBarVisible = settings.value("MainWindow/ToolbarIsVisible", true).toBool();
    m_ui->toolBarMain->setVisible(toolBarVisible);
    m_ui->actionToolbar->setChecked(toolBarVisible);
    m_ui->actionSynchronizeAll->setChecked(settings.value("MainWindow/SynchronizeAll", false).toBool());

    m_dialogHelp = new HelpDialog();
    m_dialogHelp->setWindowIcon(QIcon(":/icons/images/graphrec.png"));
    windowPosition = settings.value("HelpDialog/Position", m_dialogHelp->pos()).toPoint();
    windowSize = settings.value("HelpDialog/Size", m_dialogHelp->size()).toSize();
    m_dialogHelp->resize(windowSize);
    m_dialogHelp->move(windowPosition);

    ResetControls();
}

GraphRec::~GraphRec()
{
    delete m_ui;
    delete m_dialogHelp;
    delete m_inputDialog;
    foreach (QTabWidget* tabWidget, m_tabWidgets)
    {
        if (tabWidget)
        {
            for (int i = 0; i < tabWidget->count(); i++)
                delete tabWidget->widget(i);
        }
        delete tabWidget;
    }
    delete m_signalMapperValidators;
    delete m_signalMapperLayouters;
    delete m_labelStatus;
    delete m_labelValidator;
    delete m_labelLayouter;
}

void GraphRec::on_actionOpen_triggered()
{
    OpenDialog* openDialog = new OpenDialog(this);
    QSettings settings;
    QPoint windowPosition = settings.value("OpenDialog/Position", openDialog->pos()).toPoint();
    QSize windowSize = settings.value("OpenDialog/Size", openDialog->size()).toSize();
    openDialog->resize(windowSize);
    openDialog->move(windowPosition);
    openDialog->exec();
    if (openDialog->result())
    {
        foreach (OpenDialog::SolutionInfo info, openDialog->acceptedSolutions)
        {
            if (info.fileName.isEmpty())
                return;
            else
            {
                m_ui->statusBar->showMessage(tr("Loading: %1").arg(info.fileName));
                QFile file;
                file.setFileName(info.fileName);
                if (!file.open(QIODevice::ReadOnly))
                {
                    QMessageBox::warning(this, tr("Warning"), file.errorString());
                    return;
                }
                if (m_currentGraphView)
                {
                    QTabWidget* parent = qobject_cast<QTabWidget*>(m_currentGraphView->parentWidget()->parentWidget());
                    if (!parent)
                        break;
                    GraphView* graphView = new GraphView(file, info.filePosition, info.parser, info.validator, info.layouter, parent);
                    connect(graphView, SIGNAL(HasFocus(GraphView*)), this, SLOT(FocusedGraphViewChanged(GraphView*)));
                    connect(graphView, SIGNAL(Stepped(GraphView*, bool)), this, SLOT(SynchronizeAll(GraphView*, bool)));
                    parent->addTab(graphView, graphView->GetGraphName());
                    parent->setTabToolTip(parent->indexOf(graphView), info.fileName);
                    parent->setCurrentWidget(graphView);
                    graphView->setFocus();
                    qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
                }
                else
                {
                    QTabWidget* parent = new QTabWidget(m_ui->centralWidget);
                    parent->setElideMode(Qt::ElideRight);
                    parent->setUsesScrollButtons(true);
                    parent->setDocumentMode(true);
                    parent->setTabsClosable(true);
                    parent->setMovable(true);
                    m_tabWidgets.append(parent);
                    connect(parent, SIGNAL(currentChanged(int)), this, SLOT(TabChanged(int)));
                    connect(parent, SIGNAL(tabCloseRequested(int)), this, SLOT(TabCloseRequested(int)));
                    m_ui->gridLayout->addWidget(parent);
                    GraphView* graphView = new GraphView(file, info.filePosition, info.parser, info.validator, info.layouter, parent);
                    connect(graphView, SIGNAL(HasFocus(GraphView*)), this, SLOT(FocusedGraphViewChanged(GraphView*)));
                    connect(graphView, SIGNAL(Stepped(GraphView*, bool)), this, SLOT(SynchronizeAll(GraphView*, bool)));
                    parent->addTab(graphView, graphView->GetGraphName());
                    parent->setTabToolTip(parent->indexOf(graphView), info.fileName);
                    m_currentGraphView = graphView;
                    parent->setCurrentWidget(graphView);
                    graphView->setFocus();
                    qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
                }
                file.close();
            }
        }
    }
    m_ui->statusBar->clearMessage();
    settings.setValue("OpenDialog/Position", openDialog->pos());
    settings.setValue("OpenDialog/Size", openDialog->size());
    delete openDialog;
    ResetControls();
    if (m_currentGraphView)
    {
        m_currentGraphView->setFocus();
        m_currentGraphView->UpdateConnections();
    }
}

void GraphRec::on_actionSave_triggered()
{
    if (!m_currentGraphView)
        return;

    QSettings settings;
    QStringList savers = Factory::GetServantNames(Factory::sSaver);
    savers.append(tr("All Files (*)"));
    QFileDialog saveFileDialog(this, tr("Save file"), "", savers.join(";;"));
    saveFileDialog.setAcceptMode(QFileDialog::AcceptSave);
    saveFileDialog.setFileMode(QFileDialog::AnyFile);
    saveFileDialog.setViewMode(QFileDialog::Detail);
    QString defaultFilter = settings.value("SaveDialog/Saver", saveFileDialog.selectedFilter()).toString();
    saveFileDialog.selectFilter(defaultFilter);
    if (saveFileDialog.exec())
    {
        QStringList fileNames = saveFileDialog.selectedFiles();
        QString saverName = saveFileDialog.selectedNameFilter();
        QString fileName;
        if (!fileNames.empty())
            fileName = fileNames[0];
        if (!fileName.isEmpty())
        {
            m_ui->statusBar->showMessage(tr("Saving: %1").arg(fileName));
            QRegExp suffix(".*\\(\\*\\.(.*)\\)", Qt::CaseInsensitive); // *.txt
            if (suffix.exactMatch(saverName))
                if (!fileName.endsWith(suffix.cap(1)))
                    fileName.append("." + suffix.cap(1));
            QFile file;
            file.setFileName(fileName);
            if (!file.open(QIODevice::WriteOnly))
            {
                QMessageBox::warning(this, tr("Warning"), file.errorString());
                return;
            }
            Saver* saver = qobject_cast<Saver*>(Factory::CreateServant(Factory::sSaver, saverName));
            if (saver)
            {
                saver->Open(file);
                m_currentGraphView->SaveContext(saver);
                saver->Close();
            }
            delete saver;
            file.close();
        }
    }
    m_ui->statusBar->clearMessage();
    settings.setValue("SaveDialog/Saver", saveFileDialog.selectedFilter());
}

void GraphRec::on_actionSaveAll_triggered()
{
    if (m_tabWidgets.empty())
        return;

    QSettings settings;
    QStringList savers = Factory::GetServantNames(Factory::sSaver);
    savers.append(tr("All Files (*)"));
    QFileDialog saveFileDialog(this, tr("Save files"), "", savers.join(";;"));
    saveFileDialog.setAcceptMode(QFileDialog::AcceptSave);
    saveFileDialog.setFileMode(QFileDialog::AnyFile);
    saveFileDialog.setViewMode(QFileDialog::Detail);
    QString defaultFilter = settings.value("SaveDialog/Saver", saveFileDialog.selectedFilter()).toString();
    saveFileDialog.selectFilter(defaultFilter);
    if (saveFileDialog.exec())
    {
        QStringList fileNames = saveFileDialog.selectedFiles();
        QString saverName = saveFileDialog.selectedNameFilter();
        QString fileName;
        if (!fileNames.empty())
            fileName = fileNames[0];
        if (!fileName.isEmpty())
        {
            m_ui->statusBar->showMessage(tr("Saving: %1").arg(fileName));
            QRegExp suffix(".*\\(\\*\\.(.*)\\)", Qt::CaseInsensitive); // *.txt
            if (suffix.exactMatch(saverName))
                if (!fileName.endsWith(suffix.cap(1)))
                    fileName.append("." + suffix.cap(1));
            QFile file;
            file.setFileName(fileName);
            if (!file.open(QIODevice::WriteOnly))
            {
                QMessageBox::warning(this, tr("Warning"), file.errorString());
                return;
            }
            Saver* saver = qobject_cast<Saver*>(Factory::CreateServant(Factory::sSaver, saverName));
            if (saver)
            {
                saver->Open(file);
                foreach (QTabWidget* tabWidget, m_tabWidgets)
                {
                    for (int i = 0; i < tabWidget->count(); i++)
                    {
                        if (tabWidget)
                        {
                            GraphView* graphView = qobject_cast<GraphView*>(tabWidget->widget(i));
                            if (graphView)
                                graphView->SaveContext(saver);
                        }
                    }
                }
                saver->Close();
            }
            delete saver;
            file.close();
        }
    }
    m_ui->statusBar->clearMessage();
    settings.setValue("SaveDialog/Saver", saveFileDialog.selectedFilter());
}

void GraphRec::on_actionExit_triggered()
{
    this->close();
}

void GraphRec::closeEvent(QCloseEvent* event)
{
    QSettings settings;
    if (m_dialogHelp)
    {
        settings.setValue("HelpDialog/Position", m_dialogHelp->pos());
        settings.setValue("HelpDialog/Size", m_dialogHelp->size());
        m_dialogHelp->close();
    }
    settings.setValue("MainWindow/SynchronizeAll", m_ui->actionSynchronizeAll->isChecked());
    settings.setValue("MainWindow/Position", pos());
    settings.setValue("MainWindow/Size", size());
    settings.setValue("MainWindow/ToolbarState", saveState());
    settings.setValue("MainWindow/ToolbarIsVisible", m_ui->toolBarMain->isVisible());
    event->accept();
}

void GraphRec::on_actionManualContinuous_triggered()
{
    emit DiscreteDisplacementEnabled(false);
    emit LayoutingEnabled(false);
}

void GraphRec::on_actionManualDiscrete_triggered()
{
    emit LayoutingEnabled(false);
    emit DiscreteDisplacementEnabled(true);
}

void GraphRec::on_actionAutomaticContinuous_triggered()
{
    emit DiscreteDisplacementEnabled(false);
    emit LayoutingEnabled(true);
}

void GraphRec::on_actionRecalculate_triggered()
{
    emit LayoutRandomize();
    emit LayoutStart();
}

void GraphRec::on_actionSeek_triggered()
{
    m_inputDialog->exec();
}

void GraphRec::on_actionPlayAll_triggered()
{
    if (m_ui->actionSynchronizeAll->isChecked())
    {
        m_synchronizedGraphViews.clear();
        foreach (QTabWidget* tabWidget, m_tabWidgets)
        {
            GraphView* graphView = qobject_cast<GraphView*>(tabWidget->currentWidget());
            if (graphView)
                m_synchronizedGraphViews.append(graphView);
        }
    }
    foreach (QTabWidget* tabWidget, m_tabWidgets)
    {
        GraphView* graphView = qobject_cast<GraphView*>(tabWidget->currentWidget());
        if (graphView)
        {
            if (m_ui->actionSynchronizeAll->isChecked())
                graphView->AnimationStep();
            else
                graphView->AnimationStart();
        }
    }
}

void GraphRec::on_actionStepAll_triggered()
{
    if (m_ui->actionSynchronizeAll->isChecked())
        m_synchronizedGraphViews.clear();
    foreach (QTabWidget* tabWidget, m_tabWidgets)
    {
        GraphView* graphView = qobject_cast<GraphView*>(tabWidget->currentWidget());
        if (graphView)
            graphView->AnimationStep();
    }
}

void GraphRec::on_actionStopAll_triggered()
{
    if (m_ui->actionSynchronizeAll->isChecked())
        m_synchronizedGraphViews.clear();
    foreach (QTabWidget* tabWidget, m_tabWidgets)
    {
        GraphView* graphView = qobject_cast<GraphView*>(tabWidget->currentWidget());
        if (graphView)
            graphView->AnimationStop();
    }
}

void GraphRec::on_actionSeekAll_triggered()
{
    m_inputDialog->exec();
    int timestep = m_inputDialog->intValue();
    if (m_ui->actionSynchronizeAll->isChecked())
        m_synchronizedGraphViews.clear();
    foreach (QTabWidget* tabWidget, m_tabWidgets)
    {
        GraphView* graphView = qobject_cast<GraphView*>(tabWidget->currentWidget());
        if (graphView)
            graphView->SetTimeStep(timestep);
    }
}

void GraphRec::on_actionResetAll_triggered()
{
    if (m_ui->actionSynchronizeAll->isChecked())
        m_synchronizedGraphViews.clear();
    foreach (QTabWidget* tabWidget, m_tabWidgets)
    {
        GraphView* graphView = qobject_cast<GraphView*>(tabWidget->currentWidget());
        if (graphView)
            graphView->AnimationReset();
    }
}

void GraphRec::on_actionSplitHorizontally_triggered()
{
    Split(Qt::Horizontal);
}

void GraphRec::on_actionSplitVertically_triggered()
{
    Split(Qt::Vertical);
}

void GraphRec::on_actionUnsplit_triggered()
{ 
    QSplitter* splitter = qobject_cast<QSplitter*>(m_currentGraphView->parentWidget()->parentWidget()->parentWidget());
    if (!splitter)
        return;

    QSplitter* splitterParent = qobject_cast<QSplitter*>(splitter->parent());
    int index = 0;
    if (splitterParent)
        index = splitterParent->indexOf(splitter);

    QTabWidget* tabWidget = Unsplit(splitter);
    splitter->setParent(NULL);
    delete splitter;
    if (!tabWidget)
        return;

    if (splitterParent)
    {
        tabWidget->setParent(splitterParent);
        splitterParent->insertWidget(index, tabWidget);
    }
    else
    {
        tabWidget->setParent(m_ui->centralWidget);
        m_ui->gridLayout->addWidget(tabWidget);
    }
}

void GraphRec::on_actionUnsplitAll_triggered()
{
    QSplitter* root = NULL;
    foreach (QObject* widget, m_ui->centralWidget->children())
    {
        QSplitter* splitter = qobject_cast<QSplitter*>(widget);
        if (splitter)
            root = splitter;
    }
    QTabWidget* tabWidget = NULL;
    if (root)
    {
        tabWidget = Unsplit(root);
        root->setParent(NULL);
        delete root;
    }
    if (tabWidget)
    {
        tabWidget->setParent(m_ui->centralWidget);
        m_ui->gridLayout->addWidget(tabWidget);
    }
}

void GraphRec::on_actionDocumentation_triggered()
{
    if (m_dialogHelp)
        m_dialogHelp->show();
}

void GraphRec::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About GraphRec"),
        tr("<h2>GraphRec</h2>"
        "<p>version 1.0.0 built on Apr 30 2010"
        "<p>Copyright &copy; 2010 Petr Koup&yacute; &lt;<a href=\"mailto:petr.koupy@gmail.com\">petr.koupy@gmail.com</a>&gt;"
        "<p>See <a href=\"http://www.koupy.net/graphrec.php\">http://www.koupy.net/graphrec.php</a> for more information."
        "<p>This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version."
        "<p>This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details."
        "<p>You should have received a copy of the GNU General Public License along with this program.  If not, see &lt;<a href=\"http://www.gnu.org/licenses/\">http://www.gnu.org/licenses/</a>&gt;."
        ));
}

void GraphRec::on_actionAboutQt_triggered()
{
    QWidget widget;
    widget.setWindowIcon(QIcon(":/icons/images/qt.png"));
    QMessageBox::aboutQt(&widget, tr("About Qt"));
}

void GraphRec::on_actionAboutFFmpeg_triggered()
{
    QWidget widget;
    widget.setWindowIcon(QIcon(":/icons/images/ffmpeg.png"));
    QMessageBox::about(&widget, tr("About FFmpeg"),
        tr("<h3>About FFmpeg</h3>"
        "<p>This program uses FFmpeg version 0.5."
        "<p>FFmpeg is a complete, cross-platform solution to record, convert and stream audio and video. It includes libavcodec - the leading audio/video codec library."
        "<p>FFmpeg is licensed under the GNU Lesser General Public License (LGPL) version 2.1 or later. However, FFmpeg incorporates several optional parts and optimizations that are covered by the GNU General Public License (GPL) version 2 or later. If those parts get used the GPL applies to all of FFmpeg."
        "<p>Binaries were built (among others) with --enable-gpl parameter."
        "<p>Please see <a href=\"http://ffmpeg.org/legal.html\">http://ffmpeg.org/legal.html</a> for an overview of FFmpeg licensing."
        "<p>Copyright &copy; 2009 FFmpeg team"
        "<p>FFmpeg is a trademark of Fabrice Bellard, originator of the FFmpeg project."
        "<p>See <a href=\"http://ffmpeg.org/\">http://ffmpeg.org/</a> for more information."
        ));
}

void GraphRec::SetValidatorName(const QString& name)
{
    foreach (QAction* action, m_ui->menuValidator->actions())
    {
        if (action->text() == name)
            action->setChecked(true);
        else
            action->setChecked(false);
    }
}

void GraphRec::SetLayouterName(const QString& name)
{
    foreach (QAction* action, m_ui->menuLayouter->actions())
    {
        if (action->text() == name)
            action->setChecked(true);
        else
            action->setChecked(false);
    }
}

void GraphRec::SetDiscreteDisplacementEnabled(bool enable)
{
    if (enable)
    {
        m_ui->actionManualContinuous->setChecked(false);
        m_ui->actionManualDiscrete->setChecked(true);
        m_ui->actionAutomaticContinuous->setChecked(false);
        m_ui->actionSnapToGrid->setEnabled(true);
    }
    else
    {
        m_ui->actionManualDiscrete->setChecked(false);
        if (!m_ui->actionAutomaticContinuous->isChecked())
            m_ui->actionManualContinuous->setChecked(true);
        m_ui->actionSnapToGrid->setEnabled(false);
    }
}

void GraphRec::SetLayoutingEnabled(bool enable)
{
    if (enable)
    {
        m_ui->actionManualContinuous->setChecked(false);
        m_ui->actionManualDiscrete->setChecked(false);
        m_ui->actionAutomaticContinuous->setChecked(true);
        m_ui->actionRecalculate->setEnabled(true);
    }
    else
    {
        m_ui->actionAutomaticContinuous->setChecked(false);
        if (!m_ui->actionManualDiscrete->isChecked())
            m_ui->actionManualContinuous->setChecked(true);
        m_ui->actionRecalculate->setEnabled(false);
    }
}

void GraphRec::SetLayoutingInProgress(bool inProgress)
{
    if (inProgress)
    {
        m_labelStatus->setText(tr("Layouting..."));
        m_ui->statusBar->clearMessage();
    }
    else
        m_labelStatus->setText(tr("Idle"));
}

void GraphRec::SetAnimationInProgress(bool inProgress, bool isCapturing)
{
    if (inProgress)
    {
        if (isCapturing)
            m_labelStatus->setText(tr("Capturing..."));
        else
            m_labelStatus->setText(tr("Animating..."));
        m_ui->statusBar->clearMessage();
        m_ui->menuValidator->setEnabled(false);
        m_ui->actionColors->setEnabled(false);
        m_ui->actionOptions->setEnabled(false);
    }
    else
    {
        m_labelStatus->setText(tr("Idle"));
        m_ui->menuValidator->setEnabled(true);
        m_ui->actionColors->setEnabled(true);
        m_ui->actionOptions->setEnabled(true);
    }
}

void GraphRec::FocusedGraphViewChanged(GraphView* who)
{
    GraphView* oldView = m_currentGraphView;
    m_currentGraphView = who;
    GraphView* newView = who;

    if (oldView)
    {
        disconnect(m_signalMapperValidators, SIGNAL(mapped(const QString&)), oldView, SLOT(SetValidator(const QString&)));
        disconnect(oldView, SIGNAL(ValidatorNameChanged(const QString&)), this, SLOT(SetValidatorName(const QString&)));
        disconnect(oldView, SIGNAL(ValidatorDescriptionChanged(const QString&)), m_labelValidator, SLOT(setText(const QString&)));
        disconnect(m_ui->actionLog, SIGNAL(triggered()), oldView, SLOT(ShowErrorDialog()));
        disconnect(oldView, SIGNAL(Message(const QString&, int)), m_ui->statusBar, SLOT(showMessage(const QString&, int)));
        disconnect(this, SIGNAL(LayoutingEnabled(bool)), oldView, SLOT(SetLayoutingEnabled(bool)));
        disconnect(this, SIGNAL(DiscreteDisplacementEnabled(bool)), oldView, SLOT(SetDiscreteDisplacementEnabled(bool)));
        disconnect(oldView, SIGNAL(LayoutingEnabled(bool)), this, SLOT(SetLayoutingEnabled(bool)));
        disconnect(oldView, SIGNAL(DiscreteDisplacementEnabled(bool)), this, SLOT(SetDiscreteDisplacementEnabled(bool)));
        disconnect(m_signalMapperLayouters, SIGNAL(mapped(const QString&)), oldView, SLOT(SetLayouter(const QString&)));
        disconnect(oldView, SIGNAL(LayouterNameChanged(const QString&)), this, SLOT(SetLayouterName(const QString&)));
        disconnect(oldView, SIGNAL(LayouterDescriptionChanged(const QString&)), m_labelLayouter, SLOT(setText(const QString&)));
        disconnect(this, SIGNAL(LayoutRandomize()), oldView, SLOT(LayoutRandomize()));
        disconnect(this, SIGNAL(LayoutStart()), oldView, SLOT(LayoutStart()));
        disconnect(this, SIGNAL(LayoutStop()), oldView, SLOT(LayoutStop()));
        disconnect(m_ui->actionSnapToGrid, SIGNAL(triggered()), oldView, SLOT(LayoutDiscrete()));
        disconnect(oldView, SIGNAL(LayoutingInProgress(bool)), this, SLOT(SetLayoutingInProgress(bool)));
        disconnect(m_ui->actionPlay, SIGNAL(triggered()), oldView, SLOT(AnimationStart()));
        disconnect(m_ui->actionStep, SIGNAL(triggered()), oldView, SLOT(AnimationStep()));
        disconnect(m_ui->actionStop, SIGNAL(triggered()), oldView, SLOT(AnimationStop()));
        disconnect(m_inputDialog, SIGNAL(intValueSelected(int)), oldView, SLOT(SetTimeStep(int)));
        disconnect(m_ui->actionReset, SIGNAL(triggered()), oldView, SLOT(AnimationReset()));
        disconnect(oldView, SIGNAL(AnimationInProgress(bool, bool)), this, SLOT(SetAnimationInProgress(bool, bool)));
        disconnect(m_ui->actionSnapshot, SIGNAL(triggered()), oldView, SLOT(Snapshot()));
        disconnect(m_ui->actionSequence, SIGNAL(triggered()), oldView, SLOT(Sequence()));
        disconnect(m_ui->actionColors, SIGNAL(triggered()), oldView, SLOT(ShowColorDialog()));
        disconnect(m_ui->actionOptions, SIGNAL(triggered()), oldView, SLOT(ShowSetupDialog()));
        disconnect(m_ui->actionControls, SIGNAL(toggled(bool)), oldView, SLOT(SetControlsVisible(bool)));
        disconnect(oldView, SIGNAL(ControlsVisibilityChanged(bool)), m_ui->actionControls, SLOT(setChecked(bool)));
        oldView->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    }

    if (newView)
    {
        connect(m_signalMapperValidators, SIGNAL(mapped(const QString&)), newView, SLOT(SetValidator(const QString&)));
        connect(newView, SIGNAL(ValidatorNameChanged(const QString&)), this, SLOT(SetValidatorName(const QString&)));
        connect(newView, SIGNAL(ValidatorDescriptionChanged(const QString&)), m_labelValidator, SLOT(setText(const QString&)));
        connect(m_ui->actionLog, SIGNAL(triggered()),newView , SLOT(ShowErrorDialog()));
        connect(newView, SIGNAL(Message(const QString&, int)), m_ui->statusBar, SLOT(showMessage(const QString&, int)));
        connect(this, SIGNAL(LayoutingEnabled(bool)), newView, SLOT(SetLayoutingEnabled(bool)));
        connect(this, SIGNAL(DiscreteDisplacementEnabled(bool)), newView, SLOT(SetDiscreteDisplacementEnabled(bool)));
        connect(newView, SIGNAL(LayoutingEnabled(bool)), this, SLOT(SetLayoutingEnabled(bool)));
        connect(newView, SIGNAL(DiscreteDisplacementEnabled(bool)), this, SLOT(SetDiscreteDisplacementEnabled(bool)));
        connect(m_signalMapperLayouters, SIGNAL(mapped(const QString&)), newView, SLOT(SetLayouter(const QString&)));
        connect(newView, SIGNAL(LayouterNameChanged(const QString&)), this, SLOT(SetLayouterName(const QString&)));
        connect(newView, SIGNAL(LayouterDescriptionChanged(const QString&)), m_labelLayouter, SLOT(setText(const QString&)));
        connect(this, SIGNAL(LayoutRandomize()), newView, SLOT(LayoutRandomize()));
        connect(this, SIGNAL(LayoutStart()), newView, SLOT(LayoutStart()));
        connect(this, SIGNAL(LayoutStop()), newView, SLOT(LayoutStop()));
        connect(m_ui->actionSnapToGrid, SIGNAL(triggered()), newView, SLOT(LayoutDiscrete()));
        connect(newView, SIGNAL(LayoutingInProgress(bool)), this, SLOT(SetLayoutingInProgress(bool)));
        connect(m_ui->actionPlay, SIGNAL(triggered()), newView, SLOT(AnimationStart()));
        connect(m_ui->actionStep, SIGNAL(triggered()), newView, SLOT(AnimationStep()));
        connect(m_ui->actionStop, SIGNAL(triggered()), newView, SLOT(AnimationStop()));
        connect(m_inputDialog, SIGNAL(intValueSelected(int)), newView, SLOT(SetTimeStep(int)));
        connect(m_ui->actionReset, SIGNAL(triggered()), newView, SLOT(AnimationReset()));
        connect(newView, SIGNAL(AnimationInProgress(bool, bool)), this, SLOT(SetAnimationInProgress(bool, bool)));
        connect(m_ui->actionSnapshot, SIGNAL(triggered()), newView, SLOT(Snapshot()));
        connect(m_ui->actionSequence, SIGNAL(triggered()), newView, SLOT(Sequence()));
        connect(m_ui->actionColors, SIGNAL(triggered()), newView, SLOT(ShowColorDialog()));
        connect(m_ui->actionOptions, SIGNAL(triggered()), newView, SLOT(ShowSetupDialog()));
        connect(m_ui->actionControls, SIGNAL(toggled(bool)), newView, SLOT(SetControlsVisible(bool)));
        connect(newView, SIGNAL(ControlsVisibilityChanged(bool)), m_ui->actionControls, SLOT(setChecked(bool)));
        newView->setFrameStyle(QFrame::Panel | QFrame::Plain);
    }

    newView->UpdateConnections();
}

void GraphRec::SynchronizeAll(GraphView* who, bool nextStep)
{
    if (m_ui->actionSynchronizeAll->isChecked())
    {
        if (!nextStep)
            m_synchronizedGraphViews.removeAt(m_synchronizedGraphViews.indexOf(who));
        bool startAgain = true;
        foreach (GraphView* graphView, m_synchronizedGraphViews)
        {
            if (graphView)
            {
                if (graphView->IsAnimationRunning())
                    startAgain = false;
            }
        }
        if (startAgain)
        {
            foreach (GraphView* graphView, m_synchronizedGraphViews)
            {
                if (graphView)
                    graphView->AnimationStep();
            }
        }
    }
}

void GraphRec::ResetControls()
{
    bool notEmpty = !m_tabWidgets.empty();
    m_ui->actionSave->setEnabled(notEmpty);
    m_ui->actionSaveAll->setEnabled(notEmpty);
    m_ui->menuValidator->setEnabled(notEmpty);
    m_ui->actionLog->setEnabled(notEmpty);
    m_ui->menuMode->setEnabled(notEmpty);
    m_ui->actionManualContinuous->setEnabled(notEmpty);
    m_ui->actionManualDiscrete->setEnabled(notEmpty);
    m_ui->actionAutomaticContinuous->setEnabled(notEmpty);
    m_ui->menuLayouter->setEnabled(notEmpty);
    m_ui->actionRecalculate->setEnabled(notEmpty);
    m_ui->actionSnapToGrid->setEnabled(notEmpty);
    m_ui->actionPlay->setEnabled(notEmpty);
    m_ui->actionStep->setEnabled(notEmpty);
    m_ui->actionStop->setEnabled(notEmpty);
    m_ui->actionSeek->setEnabled(notEmpty);
    m_ui->actionReset->setEnabled(notEmpty);
    m_ui->actionPlayAll->setEnabled(notEmpty);
    m_ui->actionStepAll->setEnabled(notEmpty);
    m_ui->actionStopAll->setEnabled(notEmpty);
    m_ui->actionSeekAll->setEnabled(notEmpty);
    m_ui->actionResetAll->setEnabled(notEmpty);
    m_ui->actionSynchronizeAll->setEnabled(notEmpty);
    m_ui->actionSnapshot->setEnabled(notEmpty);
    m_ui->actionSequence->setEnabled(notEmpty);
    m_ui->actionColors->setEnabled(notEmpty);
    m_ui->actionOptions->setEnabled(notEmpty);
    m_ui->actionSplitHorizontally->setEnabled(notEmpty);
    m_ui->actionSplitVertically->setEnabled(notEmpty);
    m_ui->actionUnsplit->setEnabled(notEmpty);
    m_ui->actionUnsplitAll->setEnabled(notEmpty);
    m_ui->actionControls->setEnabled(notEmpty);
    if (!notEmpty)
    {
        m_labelStatus->setText("");
        m_labelLayouter->setText("");
        m_labelValidator->setText("");
        m_ui->statusBar->clearMessage();
    }
}

void GraphRec::Split(Qt::Orientation orientation)
{
    GraphView* currentGraphView = m_currentGraphView;
    if (!currentGraphView)
        return;
    QTabWidget* tabWidgetOld = qobject_cast<QTabWidget*>(currentGraphView->parentWidget()->parentWidget());
    if (!tabWidgetOld)
        return;
    int tabCount = tabWidgetOld->count();
    int tabPosition = tabWidgetOld->indexOf(currentGraphView);
    if (tabCount <= 1 || tabPosition <= 0)
        return;

    QTabWidget* tabWidgetNew = new QTabWidget();
    tabWidgetNew->setElideMode(Qt::ElideRight);
    tabWidgetNew->setUsesScrollButtons(true);
    tabWidgetNew->setDocumentMode(true);
    tabWidgetNew->setTabsClosable(true);
    tabWidgetNew->setMovable(true);
    m_tabWidgets.append(tabWidgetNew);
    connect(tabWidgetNew, SIGNAL(currentChanged(int)), this, SLOT(TabChanged(int)));
    connect(tabWidgetNew, SIGNAL(tabCloseRequested(int)), this, SLOT(TabCloseRequested(int)));
    QList<GraphView*> views;
    for (int i = tabCount - 1; i >= tabPosition; i--)
    {
        GraphView* graphView = qobject_cast<GraphView*>(tabWidgetOld->widget(i));
        tabWidgetOld->removeTab(i);
        views.append(graphView);
    }
    for (int i = views.size() - 1; i >= 0; i--)
    {
        GraphView* graphView = views[i];
        if (graphView)
        {
            tabWidgetNew->addTab(graphView, graphView->GetGraphName());
            tabWidgetNew->setTabToolTip(tabWidgetNew->indexOf(graphView), graphView->GetFileName());
        }
    }

    QWidget* grandParent = m_currentGraphView->parentWidget()->parentWidget()->parentWidget();
    QSplitter* grandSplitter = qobject_cast<QSplitter*>(grandParent);
    int index = 0;
    if (grandSplitter)
        index = grandSplitter->indexOf(tabWidgetOld);

    QSplitter* splitter = new QSplitter(orientation);
    tabWidgetOld->setParent(splitter);
    tabWidgetNew->setParent(splitter);
    splitter->addWidget(tabWidgetOld);
    splitter->addWidget(tabWidgetNew);

    splitter->setParent(grandParent);
    if (grandSplitter)
        grandSplitter->insertWidget(index, splitter);
    else
        m_ui->gridLayout->addWidget(splitter);
}

QTabWidget* GraphRec::Unsplit(QSplitter* splitter)
{
    if (!splitter)
        return NULL;

    QTabWidget* tabWidget = new QTabWidget();
    tabWidget->setElideMode(Qt::ElideRight);
    tabWidget->setUsesScrollButtons(true);
    tabWidget->setDocumentMode(true);
    tabWidget->setTabsClosable(true);
    tabWidget->setMovable(true);
    m_tabWidgets.append(tabWidget);
    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(TabChanged(int)));
    connect(tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(TabCloseRequested(int)));
    QList<GraphView*> viewsBuffer;
    foreach (QObject* widget, splitter->children())
    {
        QTabWidget* childTabWidget = qobject_cast<QTabWidget*>(widget);
        if (!childTabWidget)
        {
            QSplitter* childSplitter = qobject_cast<QSplitter*>(widget);
            if (childSplitter)
                childTabWidget = Unsplit(childSplitter);
            else
                continue;
        }
        if (!childTabWidget)
            continue;
        QList<GraphView*> viewsSubBuffer;
        for (int i = childTabWidget->count() - 1; i >= 0; i--)
        {
            GraphView* graphView = qobject_cast<GraphView*>(childTabWidget->widget(i));
            childTabWidget->removeTab(i);
            graphView->setParent(NULL);
            viewsSubBuffer.append(graphView);
        }
        m_tabWidgets.removeOne(childTabWidget);
        for (int i = 0; i < viewsSubBuffer.size(); i++)
        {
            viewsBuffer.append(viewsSubBuffer[i]);
        }
        if (qobject_cast<QSplitter*>(widget))
        {
            childTabWidget->setParent(NULL);
            delete childTabWidget;
        }
    }
    for (int i = viewsBuffer.size() - 1; i >= 0; i--)
    {
        GraphView* graphView = viewsBuffer[i];
        if (graphView)
        {
            tabWidget->addTab(graphView, graphView->GetGraphName());
            tabWidget->setTabToolTip(tabWidget->indexOf(graphView), graphView->GetFileName());
        }
    }
    return tabWidget;
}

void GraphRec::TabChanged(int index)
{
    QTabWidget* tabWidget = qobject_cast<QTabWidget*>(sender());
    if (tabWidget)
    {
        GraphView* graphView = qobject_cast<GraphView*>(tabWidget->widget(index));
        if (graphView)
            graphView->setFocus();
    }
}

void GraphRec::TabCloseRequested(int index)
{
    QTabWidget* tabWidget = qobject_cast<QTabWidget*>(sender());
    if (tabWidget)
    {
        GraphView* graphView = qobject_cast<GraphView*>(tabWidget->widget(index));
        tabWidget->removeTab(index);
        delete graphView;
    }

    if (tabWidget->count() == 0)
    {
        QSplitter* parent = qobject_cast<QSplitter*>(tabWidget->parentWidget());
        m_tabWidgets.removeOne(tabWidget);
        tabWidget->setParent(NULL);
        delete tabWidget;
        tabWidget = NULL;
        if (parent)
        {
            QObject* otherChild = NULL;
            foreach (QObject* child, parent->children())
            {
                if (qobject_cast<QSplitter*>(child) || qobject_cast<QTabWidget*>(child))
                    otherChild = child;
            }
            if (!otherChild)
                return;
            QSplitter* grandParent = qobject_cast<QSplitter*>(parent->parentWidget());
            QWidget* otherChildWidget = qobject_cast<QWidget*>(otherChild);
            if (!otherChildWidget)
                return;
            if (grandParent)
            {
                int i = grandParent->indexOf(parent);
                parent->setParent(NULL);
                otherChildWidget->setParent(grandParent);
                delete parent;
                grandParent->insertWidget(i, otherChildWidget);
            }
            else
            {
                parent->setParent(NULL);
                otherChildWidget->setParent(m_ui->centralWidget);
                delete parent;
                m_ui->gridLayout->addWidget(otherChildWidget);
            }
        }
    }
    ResetControls();
}

void GraphRec::RemoveCurrentFromSynchronized()
{
    m_synchronizedGraphViews.removeAt(m_synchronizedGraphViews.indexOf(m_currentGraphView));
}
