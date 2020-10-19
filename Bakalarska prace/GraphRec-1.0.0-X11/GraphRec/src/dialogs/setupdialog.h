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

#ifndef SETUPDIALOG_H
#define SETUPDIALOG_H

#include <QtCore/QTimeLine>
#include <QtGui/QDialog>

#include "primitives/node.h"

class QWidget;
class QEvent;

namespace Ui
{
    class SetupDialog;
}

class SetupDialog : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(SetupDialog)

signals:
     void Changed(bool isChanged);

public:
    enum LayoutingMode
    {
        lmManualContinuous = 0,
        lmManualDiscrete,
        lmAutomaticContinuous
    };

    explicit SetupDialog(QWidget* parent = 0);
    virtual ~SetupDialog();

    int GetAnimationLength() const;
    void SetAnimationLength(int length);
    QTimeLine::CurveShape GetAnimationStyle() const;
    void SetAnimationStyle(QTimeLine::CurveShape style);

    SetupDialog::LayoutingMode GetLayoutingMode() const;
    void SetLayoutingMode(SetupDialog::LayoutingMode mode);
    QString GetLayouterName() const;
    void SetLayouterName(const QString& name);
    int GetNodeDisplacement() const;
    void SetNodeDisplacement(int displacement);
    int GetGridOffset() const;
    void SetGridOffset(int offset);

    QString GetValidatorName() const;
    void SetValidatorName(const QString& name);

    Node::NodeDescription GetNodeDescription() const;
    void SetNodeDescription(Node::NodeDescription type);
    bool GetControlsVisibility() const;
    void SetControlsVisibility(bool areVisible);
    bool GetInteractiveCapturing() const;
    void SetInteractiveCapturing(bool isInteractive);

public slots:
    void on_comboBoxLayoutingMode_currentIndexChanged(int index);

    void on_buttonDefault_clicked();
    void on_buttonOK_clicked();
    void on_buttonCancel_clicked();

    void ChangeTrigger(int index = 0);

protected:
    virtual void changeEvent(QEvent* e);

private:
    Ui::SetupDialog* m_ui;
};

#endif
