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

#ifndef GRAPHREC_H
#define GRAPHREC_H

#include <QtCore/QPointer>
#include <QtCore/QList>
#include <QtGui/QMainWindow>

class QWidget;
class QLabel;
class QSignalMapper;
class QTabWidget;
class QSplitter;
class QInputDialog;
class QCloseEvent;
class GraphView;
class HelpDialog;

namespace Ui
{
    class GraphRec;
}

class GraphRec : public QMainWindow
{
    Q_OBJECT

signals:
    void LayoutingEnabled(bool isEnabled);
    void DiscreteDisplacementEnabled(bool isEnabled);
    void LayoutRandomize();
    void LayoutStart();
    void LayoutStop();

public:
    GraphRec(QWidget* parent = 0);
    ~GraphRec();

public slots:
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSaveAll_triggered();
    void on_actionExit_triggered();
    void on_actionManualContinuous_triggered();
    void on_actionManualDiscrete_triggered();
    void on_actionAutomaticContinuous_triggered();
    void on_actionRecalculate_triggered();
    void on_actionSeek_triggered();
    void on_actionPlayAll_triggered();
    void on_actionStepAll_triggered();
    void on_actionStopAll_triggered();
    void on_actionSeekAll_triggered();
    void on_actionResetAll_triggered();
    void on_actionSplitHorizontally_triggered();
    void on_actionSplitVertically_triggered();
    void on_actionUnsplit_triggered();
    void on_actionUnsplitAll_triggered();
    void on_actionDocumentation_triggered();
    void on_actionAbout_triggered();
    void on_actionAboutQt_triggered();
    void on_actionAboutFFmpeg_triggered();

    void SetValidatorName(const QString& name);
    void SetLayouterName(const QString& name);
    void SetDiscreteDisplacementEnabled(bool enable);
    void SetLayoutingEnabled(bool enable);
    void SetLayoutingInProgress(bool inProgress);
    void SetAnimationInProgress(bool inProgress, bool isCapturing);

    void FocusedGraphViewChanged(GraphView* who);
    void SynchronizeAll(GraphView* who, bool nextStep);

protected:
    void ResetControls();
    void Split(Qt::Orientation orientation);
    QTabWidget* Unsplit(QSplitter* splitter);
    void closeEvent(QCloseEvent* event);

protected slots:
    void TabChanged(int index);
    void TabCloseRequested(int index);
    void RemoveCurrentFromSynchronized();

private:
    Ui::GraphRec* m_ui;
    QPointer<QInputDialog> m_inputDialog;

    QPointer<GraphView> m_currentGraphView;
    QList<QTabWidget*> m_tabWidgets;
    QList<QPointer<GraphView> > m_synchronizedGraphViews;

    QPointer<QSignalMapper> m_signalMapperValidators;
    QPointer<QSignalMapper> m_signalMapperLayouters;

    QPointer<QLabel> m_labelStatus;
    QPointer<QLabel> m_labelValidator;
    QPointer<QLabel> m_labelLayouter;

    QPointer<HelpDialog> m_dialogHelp;
};

#endif
