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
#include <QtCore/QIODevice>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QStringList>
#include <QtCore/QString>
#include <QtCore/QRegExp>
#include <QtGui/QWidget>
#include <QtGui/QListWidget>
#include <QtGui/QListWidgetItem>
#include <QtGui/QDialog>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

#include "dialogs/errordialog.h"
#include "ui_errordialog.h"

ErrorDialog::ErrorDialog(QWidget* parent) :
        QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint),
        m_ui(new Ui::ErrorDialog),
        m_errorsDetected(false)
{
    m_ui->setupUi(this);
    m_errorRegExp = QRegExp("^Timestep\\s([0-9]+):\\sEvent\\s([-0-9]+)\\([-0-9]+\\)\\s--->\\s([-0-9]+)\\([-0-9]+\\)\\..*$", Qt::CaseInsensitive); // Timestep 0: Event 19(16) ---> 18(4).
}

ErrorDialog::~ErrorDialog()
{
    delete m_ui;
}

void ErrorDialog::Clear()
{
    on_buttonClear_clicked();
}

bool ErrorDialog::ErrorsDetected() const
{
    return m_errorsDetected;
}

void ErrorDialog::Log(const QString& message)
{
    m_ui->listWidgetErrorLog->addItem(message);
    m_errorsDetected = true;
}

void ErrorDialog::on_buttonClose_clicked()
{
    this->close();
}

void ErrorDialog::on_buttonClear_clicked()
{
    m_ui->listWidgetErrorLog->clear();
    m_errorsDetected = false;
}

void ErrorDialog::on_buttonSave_clicked()
{
    QFileDialog saveFileDialog(this, tr("Save log"), "", tr("Plain text (*.txt);;All Files (*)"));
    saveFileDialog.setAcceptMode(QFileDialog::AcceptSave);
    saveFileDialog.setFileMode(QFileDialog::AnyFile);
    saveFileDialog.setViewMode(QFileDialog::Detail);
    if (saveFileDialog.exec())
    {
        QStringList fileNames = saveFileDialog.selectedFiles();
        QString type = saveFileDialog.selectedNameFilter();
        QString fileName;
        if (!fileNames.empty())
            fileName = fileNames[0];
        if (!fileName.isEmpty())
        {
            QRegExp suffix(".*\\(\\*\\.(.*)\\)", Qt::CaseInsensitive); // *.txt
            if (suffix.exactMatch(type))
                if (!fileName.endsWith(suffix.cap(1)))
                    fileName.append("." + suffix.cap(1));
            QFile file;
            file.setFileName(fileName);
            if (!file.open(QIODevice::WriteOnly))
            {
                QMessageBox::warning(this, tr("Warning"), file.errorString());
                return;
            }
            QTextStream stream(&file);
            for (int i = 0; i < m_ui->listWidgetErrorLog->count(); i++)
            {
                stream << m_ui->listWidgetErrorLog->item(i)->text() << endl;
            }
            file.close();
        }
    }
}

void ErrorDialog::on_listWidgetErrorLog_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous)
{
    if (previous && m_errorRegExp.exactMatch(previous->text()))
        emit ErrorSelected(m_errorRegExp.cap(1).toInt(), m_errorRegExp.cap(2).toInt(), m_errorRegExp.cap(3).toInt(), false);
    if (current && m_errorRegExp.exactMatch(current->text()))
        emit ErrorSelected(m_errorRegExp.cap(1).toInt(), m_errorRegExp.cap(2).toInt(), m_errorRegExp.cap(3).toInt(), true);
}

void ErrorDialog::on_listWidgetErrorLog_itemActivated(QListWidgetItem* item)
{
    on_listWidgetErrorLog_currentItemChanged(item, NULL);
}

void ErrorDialog::closeEvent(QCloseEvent* e)
{
    Q_UNUSED(e)
    QListWidgetItem* current = m_ui->listWidgetErrorLog->currentItem();
    if (current && m_errorRegExp.exactMatch(current->text()))
        emit ErrorSelected(m_errorRegExp.cap(1).toInt(), m_errorRegExp.cap(2).toInt(), m_errorRegExp.cap(3).toInt(), false);
}

void ErrorDialog::changeEvent(QEvent* e)
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
