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

#include <QtCore/QObject>
#include <QtCore/QSettings>
#include <QtCore/QEvent>
#include <QtCore/QTimeLine>
#include <QtGui/QDialog>
#include <QtGui/QWidget>

#include "dialogs/setupdialog.h"
#include "ui_setupdialog.h"
#include "primitives/node.h"
#include "factory.h"

SetupDialog::SetupDialog(QWidget* parent) :
        QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint),
        m_ui(new Ui::SetupDialog)
{
    m_ui->setupUi(this);

    m_ui->comboBoxAnimationStyle->addItem(tr("Linear"));
    m_ui->comboBoxAnimationStyle->addItem(tr("EaseInOut"));

    for (int i = 0; i < 3; i++)
    {
        if (i == SetupDialog::lmManualContinuous)
            m_ui->comboBoxLayoutingMode->addItem(tr("Manual (continuous)"));
        else if (i == SetupDialog::lmManualDiscrete)
            m_ui->comboBoxLayoutingMode->addItem(tr("Manual (discrete)"));
        else if (i == SetupDialog::lmAutomaticContinuous)
            m_ui->comboBoxLayoutingMode->addItem(tr("Automatic"));
    }

    m_ui->comboBoxLayouter->addItems(Factory::GetServantNames(Factory::sLayouter));

    m_ui->comboBoxValidator->addItems(Factory::GetServantNames(Factory::sValidator));

    m_ui->comboBoxNodeDescription->addItem(tr("Entity ID"));
    m_ui->comboBoxNodeDescription->addItem(tr("Node ID"));  
    m_ui->comboBoxNodeDescription->addItem(tr("Node ID : Entity ID"));
    m_ui->comboBoxNodeDescription->addItem(tr("Empty"));

    connect(m_ui->spinBoxAnimationLength, SIGNAL(valueChanged(int)), this, SLOT(ChangeTrigger(int)));
    connect(m_ui->comboBoxAnimationStyle, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangeTrigger(int)));
    connect(m_ui->comboBoxLayoutingMode, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangeTrigger(int)));
    connect(m_ui->comboBoxLayouter, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangeTrigger(int)));
    connect(m_ui->spinBoxNodeDisplacement, SIGNAL(valueChanged(int)), this, SLOT(ChangeTrigger(int)));
    connect(m_ui->spinBoxGridOffset, SIGNAL(valueChanged(int)), this, SLOT(ChangeTrigger(int)));
    connect(m_ui->comboBoxValidator, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangeTrigger(int)));
    connect(m_ui->comboBoxNodeDescription, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangeTrigger(int)));
    connect(m_ui->checkBoxControlsVisibility, SIGNAL(clicked()), this, SLOT(ChangeTrigger()));
    connect(m_ui->checkBoxInteractiveCapturing, SIGNAL(clicked()), this, SLOT(ChangeTrigger()));
    connect(this, SIGNAL(Changed(bool)), m_ui->buttonDefault, SLOT(setEnabled(bool)));
}

SetupDialog::~SetupDialog()
{
    delete m_ui;
}

int SetupDialog::GetAnimationLength() const
{
    return m_ui->spinBoxAnimationLength->value();
}

void SetupDialog::SetAnimationLength(int length)
{
    m_ui->spinBoxAnimationLength->setValue(length);
    m_ui->buttonDefault->setEnabled(false);
}

QTimeLine::CurveShape SetupDialog::GetAnimationStyle() const
{
    switch (m_ui->comboBoxAnimationStyle->currentIndex())
    {
        case 0:
            return QTimeLine::LinearCurve;
        case 1:
            return QTimeLine::EaseInOutCurve;
        default:
            return QTimeLine::LinearCurve;
    }
    return QTimeLine::LinearCurve;
}

void SetupDialog::SetAnimationStyle(QTimeLine::CurveShape style)
{
    switch (style)
    {
        case QTimeLine::LinearCurve:
            m_ui->comboBoxAnimationStyle->setCurrentIndex(0);
            break;
        case QTimeLine::EaseInOutCurve:
            m_ui->comboBoxAnimationStyle->setCurrentIndex(1);
            break;
        default:
            m_ui->comboBoxAnimationStyle->setCurrentIndex(0);
            break;
    }
    m_ui->buttonDefault->setEnabled(false);
}

SetupDialog::LayoutingMode SetupDialog::GetLayoutingMode() const
{
    switch (m_ui->comboBoxLayoutingMode->currentIndex())
    {
        case SetupDialog::lmManualContinuous:
            return SetupDialog::lmManualContinuous;
        case SetupDialog::lmManualDiscrete:
            return SetupDialog::lmManualDiscrete;
        case SetupDialog::lmAutomaticContinuous:
            return SetupDialog::lmAutomaticContinuous;
        default:
            return SetupDialog::lmManualContinuous;
    }
    return SetupDialog::lmManualContinuous;
}

void SetupDialog::SetLayoutingMode(SetupDialog::LayoutingMode mode)
{
    m_ui->comboBoxLayoutingMode->setCurrentIndex(mode);
    m_ui->buttonDefault->setEnabled(false);
}

QString SetupDialog::GetLayouterName() const
{
    return m_ui->comboBoxLayouter->currentText();
}

void SetupDialog::SetLayouterName(const QString& name)
{
    int index = m_ui->comboBoxLayouter->findText(name);
    if (index >= 0)
        m_ui->comboBoxLayouter->setCurrentIndex(index);
    m_ui->buttonDefault->setEnabled(false);
}

int SetupDialog::GetNodeDisplacement() const
{
    return m_ui->spinBoxNodeDisplacement->value();
}

void SetupDialog::SetNodeDisplacement(int displacement)
{
    m_ui->spinBoxNodeDisplacement->setValue(displacement);
    m_ui->buttonDefault->setEnabled(false);
}

int SetupDialog::GetGridOffset() const
{
    return m_ui->spinBoxGridOffset->value();
}

void SetupDialog::SetGridOffset(int offset)
{
    m_ui->spinBoxGridOffset->setValue(offset);
    m_ui->buttonDefault->setEnabled(false);
}

QString SetupDialog::GetValidatorName() const
{
    return m_ui->comboBoxValidator->currentText();
}

void SetupDialog::SetValidatorName(const QString& name)
{
    int index = m_ui->comboBoxValidator->findText(name);
    if (index >= 0)
        m_ui->comboBoxValidator->setCurrentIndex(index);
    m_ui->buttonDefault->setEnabled(false);
}

Node::NodeDescription SetupDialog::GetNodeDescription() const
{
    switch (m_ui->comboBoxNodeDescription->currentIndex())
    {
        case 0:
            return Node::EntityID;
        case 1:
            return Node::NodeID;
        case 2:
            return Node::NodeIDEntityID;
        case 3:
            return Node::Empty;
        default:
            return Node::EntityID;
    }
    return Node::EntityID;
}
void SetupDialog::SetNodeDescription(Node::NodeDescription type)
{
    switch (type)
    {
        case Node::EntityID:
            m_ui->comboBoxNodeDescription->setCurrentIndex(0);
            break;
        case Node::NodeID:
            m_ui->comboBoxNodeDescription->setCurrentIndex(1);
            break;
        case Node::NodeIDEntityID:
            m_ui->comboBoxNodeDescription->setCurrentIndex(2);
            break;
        case Node::Empty:
            m_ui->comboBoxNodeDescription->setCurrentIndex(3);
            break;
        default:
            m_ui->comboBoxNodeDescription->setCurrentIndex(1);
            break;
    }
    m_ui->buttonDefault->setEnabled(false);
}

bool SetupDialog::GetControlsVisibility() const
{
    return m_ui->checkBoxControlsVisibility->isChecked();
}

void SetupDialog::SetControlsVisibility(bool areVisible)
{
    m_ui->checkBoxControlsVisibility->setChecked(areVisible);
    m_ui->buttonDefault->setEnabled(false);
}

bool SetupDialog::GetInteractiveCapturing() const
{
    return m_ui->checkBoxInteractiveCapturing->isChecked();
}

void SetupDialog::SetInteractiveCapturing(bool isInteractive)
{
    m_ui->checkBoxInteractiveCapturing->setChecked(isInteractive);
    m_ui->buttonDefault->setEnabled(false);
}

void SetupDialog::on_comboBoxLayoutingMode_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    switch (m_ui->comboBoxLayoutingMode->currentIndex())
    {
        case SetupDialog::lmManualContinuous:
            m_ui->comboBoxLayouter->setEnabled(false);
            m_ui->spinBoxNodeDisplacement->setEnabled(false);
            m_ui->spinBoxGridOffset->setEnabled(false);
            break;
        case SetupDialog::lmManualDiscrete:
            m_ui->comboBoxLayouter->setEnabled(false);
            m_ui->spinBoxNodeDisplacement->setEnabled(false);
            m_ui->spinBoxGridOffset->setEnabled(true);
            break;
        case SetupDialog::lmAutomaticContinuous:
            m_ui->comboBoxLayouter->setEnabled(true);
            m_ui->spinBoxNodeDisplacement->setEnabled(true);
            m_ui->spinBoxGridOffset->setEnabled(false);
            break;
        default:
            m_ui->comboBoxLayouter->setEnabled(false);
            m_ui->spinBoxNodeDisplacement->setEnabled(false);
            m_ui->spinBoxGridOffset->setEnabled(false);
            break;
    }
}

void SetupDialog::on_buttonDefault_clicked()
{
    m_ui->buttonDefault->setEnabled(false);
    QSettings settings;
    settings.setValue("GraphView/AnimationLength", m_ui->spinBoxAnimationLength->value());
    if (m_ui->comboBoxAnimationStyle->currentIndex() == 0)
        settings.setValue("GraphView/AnimationStyle", QTimeLine::LinearCurve);
    else if (m_ui->comboBoxAnimationStyle->currentIndex() == 1)
        settings.setValue("GraphView/AnimationStyle", QTimeLine::EaseInOutCurve);
    if (m_ui->comboBoxLayoutingMode->currentIndex() == SetupDialog::lmManualContinuous)
    {
        settings.setValue("GraphView/LayoutingEnabled", false);
        settings.setValue("GraphView/DiscreteDisplacementEnabled", false);
    }
    else if (m_ui->comboBoxLayoutingMode->currentIndex() == SetupDialog::lmManualDiscrete)
    {
        settings.setValue("GraphView/LayoutingEnabled", false);
        settings.setValue("GraphView/DiscreteDisplacementEnabled", true);
    }
    else if (m_ui->comboBoxLayoutingMode->currentIndex() == SetupDialog::lmAutomaticContinuous)
    {
        settings.setValue("GraphView/LayoutingEnabled", true);
        settings.setValue("GraphView/DiscreteDisplacementEnabled", false);
    }
    settings.setValue("GraphView/Layouter", m_ui->comboBoxLayouter->currentText());
    settings.setValue("GraphView/NodeDisplacement", m_ui->spinBoxNodeDisplacement->value());
    settings.setValue("GraphView/DiscreteDisplacementOffset", m_ui->spinBoxGridOffset->value());
    settings.setValue("GraphView/Validator", m_ui->comboBoxValidator->currentText());
    if (m_ui->comboBoxNodeDescription->currentIndex() == 0)
        settings.setValue("GraphView/NodeDescription", Node::EntityID);
    else if (m_ui->comboBoxNodeDescription->currentIndex() == 1)
        settings.setValue("GraphView/NodeDescription", Node::NodeID);
    else if (m_ui->comboBoxNodeDescription->currentIndex() == 2)
        settings.setValue("GraphView/NodeDescription", Node::NodeIDEntityID);
    settings.setValue("GraphView/Controls", m_ui->checkBoxControlsVisibility->isChecked());
    settings.setValue("GraphView/CapturingInteractivity", m_ui->checkBoxInteractiveCapturing->isChecked());
}

void SetupDialog::on_buttonOK_clicked()
{
    this->accept();
}

void SetupDialog::on_buttonCancel_clicked()
{
    this->reject();
}

void SetupDialog::ChangeTrigger(int index)
{
    Q_UNUSED(index)
    emit Changed(true);
}

void SetupDialog::changeEvent(QEvent* e)
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
