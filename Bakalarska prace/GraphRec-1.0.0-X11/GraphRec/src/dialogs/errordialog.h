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

#ifndef ERRORDIALOG_H
#define ERRORDIALOG_H

#include <QtCore/QRegExp>
#include <QtGui/QDialog>

class QWidget;
class QEvent;
class QCloseEvent;
class QString;
class QListWidgetItem;

namespace Ui
{
    class ErrorDialog;
}

class ErrorDialog : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(ErrorDialog)

signals:
    void ErrorSelected(int timestep, int source, int destination, bool select);

public:
    explicit ErrorDialog(QWidget* parent = 0);
    virtual ~ErrorDialog();
    void Clear();
    bool ErrorsDetected() const;

public slots:
    void Log(const QString& message);
    void on_buttonClose_clicked();
    void on_buttonClear_clicked();
    void on_buttonSave_clicked();
    void on_listWidgetErrorLog_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
    void on_listWidgetErrorLog_itemActivated(QListWidgetItem* item);

protected:
    void closeEvent(QCloseEvent* e);
    virtual void changeEvent(QEvent* e);

private:
    Ui::ErrorDialog* m_ui;
    bool m_errorsDetected;
    QRegExp m_errorRegExp;
};

#endif
