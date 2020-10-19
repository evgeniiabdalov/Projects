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

#include <QtCore/QEvent>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtGui/QWidget>
#include <QtGui/QFileDialog>

#include "dialogs/capturedialog.h"
#include "ui_capturedialog.h"
#include "recorders/recorder.h"
#include "recorders/imagerecorder.h"
#include "recorders/videorecorder.h"
#include "factory.h"

CaptureDialog::CaptureDialog(CaptureDialog::Mode mode, QWidget* parent) :
        QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint),
        m_ui(new Ui::CaptureDialog),
        m_mode(mode)
{
    m_ui->setupUi(this);
    QStringList recorders = Factory::GetServantNames(Factory::sRecorder);
    m_widgetRecorder = NULL;
    if (mode == CaptureDialog::mSnapshot)
    {
        foreach (QString name, recorders)
        {
            Recorder* test = qobject_cast<Recorder*>(Factory::CreateServant(Factory::sRecorder, name));
            if (test && qobject_cast<ImageRecorder*>(test))
                m_ui->comboBoxRecorder->addItem(name);
            delete test;
        }
    }
    else
    {
        m_ui->comboBoxRecorder->addItems(recorders);
    }   
    if (m_ui->comboBoxRecorder->count() > 0)
    {
        // on_comboBoxRecorder_currentIndexChanged() has already added widget.
        if (m_widgetRecorder)
        {
            m_ui->horizontalLayout->removeWidget(m_widgetRecorder);
            delete m_widgetRecorder;
        }

        m_recorder = qobject_cast<Recorder*>(Factory::CreateServant(Factory::sRecorder, m_ui->comboBoxRecorder->itemText(0)));
        if (m_recorder)
            m_widgetRecorder = m_recorder->GetSettingsWidget();
        if (m_widgetRecorder)
            m_ui->horizontalLayout->addWidget(m_widgetRecorder);
        if (qobject_cast<VideoRecorder*>(m_recorder))
            m_ui->spinBoxInterval->setEnabled(false);
        else
            m_ui->spinBoxInterval->setEnabled(true);
    }
    if (mode == CaptureDialog::mSnapshot)
    {
        m_ui->buttonCancel->setText(tr("&Close"));
        m_ui->spinBoxStart->setEnabled(false);
        m_ui->spinBoxStop->setEnabled(false);
        m_ui->spinBoxInterval->setEnabled(false);
    }
}

CaptureDialog::~CaptureDialog()
{
    delete m_widgetRecorder;
    delete m_ui;
}

CaptureDialog::Mode CaptureDialog::GetMode() const
{
    return m_mode;
}

int CaptureDialog::GetRecorderIndex() const
{
    return m_ui->comboBoxRecorder->currentIndex();
}

void CaptureDialog::SetRecorderIndex(int index)
{
    m_ui->comboBoxRecorder->setCurrentIndex(index);
}

Recorder* CaptureDialog::GetRecorder() const
{
    return m_recorder;
}

int CaptureDialog::GetWidth() const
{
    return m_ui->spinBoxWidth->value();
}

void CaptureDialog::SetWidth(int pixels)
{
    m_ui->spinBoxWidth->setValue(pixels);
}

int CaptureDialog::GetHeight() const
{
    return m_ui->spinBoxHeight->value();
}

void CaptureDialog::SetHeight(int pixels)
{
    m_ui->spinBoxHeight->setValue(pixels);
}

int CaptureDialog::GetStart() const
{
    return m_ui->spinBoxStart->value();
}

void CaptureDialog::SetStart(int timestep)
{
    m_ui->spinBoxStart->setValue(timestep);
}

int CaptureDialog::GetStop() const
{
    return m_ui->spinBoxStop->value();
}

void CaptureDialog::SetStop(int timestep)
{
    m_ui->spinBoxStop->setValue(timestep);
}

int CaptureDialog::GetInterval() const
{
    return m_ui->spinBoxInterval->value();
}

void CaptureDialog::SetInterval(int interval)
{
    m_ui->spinBoxInterval->setValue(interval);
}

void CaptureDialog::SetMaxTimestep(int timestep)
{
    m_ui->spinBoxStart->setMaximum(timestep);
    m_ui->spinBoxStop->setMaximum(timestep);
    m_ui->spinBoxInterval->setMaximum(timestep);
}

QString CaptureDialog::GetName() const
{
    return m_ui->lineEditName->text();
}

void CaptureDialog::SetName(const QString& name)
{
    m_ui->lineEditName->setText(name);
}

QString CaptureDialog::GetPath() const
{
    return m_ui->lineEditPath->text();
}

void CaptureDialog::SetPath(const QString& path)
{
    m_ui->lineEditPath->setText(path);
}

void CaptureDialog::on_comboBoxRecorder_currentIndexChanged(const QString& recorder)
{
    if (m_widgetRecorder)
    {
        m_ui->horizontalLayout->removeWidget(m_widgetRecorder);
        delete m_widgetRecorder;
    }
    Recorder* temp = qobject_cast<Recorder*>(Factory::CreateServant(Factory::sRecorder, recorder));
    delete m_recorder;
    m_recorder = temp; // New pointer must have different address than old one.
    temp = NULL;
    if (m_recorder)
        m_widgetRecorder = m_recorder->GetSettingsWidget();
    if (m_widgetRecorder)
        m_ui->horizontalLayout->addWidget(m_widgetRecorder);
    if (qobject_cast<VideoRecorder*>(m_recorder) || m_mode == CaptureDialog::mSnapshot)
        m_ui->spinBoxInterval->setEnabled(false);
    else
        m_ui->spinBoxInterval->setEnabled(true);
}

void CaptureDialog::on_spinBoxStart_valueChanged(int value)
{
    if (value > m_ui->spinBoxStop->value())
        m_ui->spinBoxStart->setValue(m_ui->spinBoxStop->value());
}

void CaptureDialog::on_spinBoxStop_valueChanged(int value)
{
    if (value < m_ui->spinBoxStart->value())
        m_ui->spinBoxStop->setValue(m_ui->spinBoxStart->value());
}

void CaptureDialog::on_lineEditName_textChanged(const QString& text)
{
    if (text.isEmpty())
        m_ui->lineEditName->setText("_");
}

void CaptureDialog::on_buttonPath_clicked()
{
    QFileDialog openFileDialog(this, tr("Choose directory"), "", "");
    openFileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    openFileDialog.setFileMode(QFileDialog::Directory);
    openFileDialog.setOption(QFileDialog::ShowDirsOnly, true);
    openFileDialog.setViewMode(QFileDialog::Detail);
    QStringList paths;
    if (openFileDialog.exec())
    {
        paths = openFileDialog.selectedFiles();
        if (!paths.isEmpty())
            m_ui->lineEditPath->setText(paths[0]);
    }
}

void CaptureDialog::on_buttonCapture_clicked()
{
    if (m_mode == CaptureDialog::mSnapshot)
    {
        setResult(QDialog::Accepted);
        emit accepted();
    }
    else
        this->accept();
}

void CaptureDialog::on_buttonCancel_clicked()
{
    this->reject();
}

void CaptureDialog::changeEvent(QEvent* e)
{
    QDialog::changeEvent(e);
    switch (e->type())
    {
        case QEvent::LanguageChange:
            m_ui->retranslateUi(this);
            break;
        default:
            break;
    }
}
