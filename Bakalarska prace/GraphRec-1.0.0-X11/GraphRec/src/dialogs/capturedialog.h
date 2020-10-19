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

#ifndef CAPTUREDIALOG_H
#define CAPTUREDIALOG_H

#include <QtCore/QString>
#include <QtCore/QPointer>
#include <QtGui/QDialog>

#include "recorders/recorder.h"

class QWidget;
class QEvent;

namespace Ui
{
    class CaptureDialog;
}

class CaptureDialog : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(CaptureDialog)

public:
    enum Mode
    {
        mSnapshot = 0,
        mSequence
    };

    CaptureDialog(CaptureDialog::Mode mode, QWidget* parent = 0);
    ~CaptureDialog();

    CaptureDialog::Mode GetMode() const;
    int GetRecorderIndex() const;
    void SetRecorderIndex(int index);
    Recorder* GetRecorder() const;
    int GetWidth() const;
    void SetWidth(int pixels);
    int GetHeight() const;
    void SetHeight(int pixels);
    int GetStart() const;
    void SetStart(int timestep);
    int GetStop() const;
    void SetStop(int timestep);
    int GetInterval() const;
    void SetInterval(int interval);
    void SetMaxTimestep(int timestep);
    QString GetName() const;
    void SetName(const QString& name);
    QString GetPath() const;
    void SetPath(const QString& path);

public slots:
    void on_comboBoxRecorder_currentIndexChanged(const QString& recorder);
    void on_spinBoxStart_valueChanged(int value);
    void on_spinBoxStop_valueChanged(int value);
    void on_lineEditName_textChanged(const QString& text);
    void on_buttonPath_clicked();
    void on_buttonCapture_clicked();
    void on_buttonCancel_clicked();

protected:
    void changeEvent(QEvent* e);

private:
    Ui::CaptureDialog* m_ui;
    CaptureDialog::Mode m_mode;
    QWidget* m_widgetRecorder;
    QPointer<Recorder> m_recorder;
};

#endif
