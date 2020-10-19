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
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtCore/QDir>
#include <QtGui/QApplication>
#include <QtGui/QWidget>
#include <QtGui/QDialog>

#include "dialogs/helpdialog.h"
#include "ui_helpdialog.h"

HelpDialog::HelpDialog(QWidget* parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint),
    m_ui(new Ui::HelpDialog)
{
    m_ui->setupUi(this);

    QDir directory(QApplication::applicationDirPath() + "/../doc");
    if (directory.exists("index.html"))
    {
        m_ui->textBrowser->setSearchPaths(QStringList() << directory.canonicalPath());
        m_ui->textBrowser->setSource(QUrl("index.html"));
    }
    else
        m_ui->textBrowser->setText(tr("<h4>Help file not found."));
}

HelpDialog::~HelpDialog()
{
    delete m_ui;
}

void HelpDialog::on_buttonHome_clicked()
{
    m_ui->textBrowser->home();
}

void HelpDialog::on_buttonBack_clicked()
{
    m_ui->textBrowser->backward();
}

void HelpDialog::on_buttonClose_clicked()
{
    this->close();
}

void HelpDialog::changeEvent(QEvent* e)
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
