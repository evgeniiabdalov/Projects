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

#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QtGui/QDialog>

class QWidget;
class QEvent;

namespace Ui
{
    class HelpDialog;
}

class HelpDialog : public QDialog
{
    Q_OBJECT

public:
    HelpDialog(QWidget* parent = 0);
    ~HelpDialog();

public slots:
    void on_buttonHome_clicked();
    void on_buttonBack_clicked();
    void on_buttonClose_clicked();

protected:
    void changeEvent(QEvent* e);

private:
    Ui::HelpDialog* m_ui;
};

#endif
