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

#include <QtCore/QString>
#include <QtCore/QSettings>
#include <QtCore/QSize>
#include <QtCore/QRect>
#include <QtCore/QDir>
#include <QtGui/QWidget>
#include <QtGui/QPaintDevice>
#include <QtGui/QFormLayout>
#include <QtGui/QSpinBox>
#include <QtGui/QLineEdit>
#include <QtSvg/QSvgGenerator>

#include "recorders/svgimagerecorder.h"

SVGImageRecorder::SVGImageRecorder()
{
    QSettings settings;
    m_dpi = settings.value("SVGImageRecorder/DPI", 72).toInt();
}

SVGImageRecorder::~SVGImageRecorder()
{
}

QWidget* SVGImageRecorder::GetSettingsWidget()
{
    QWidget* widget = new QWidget();
    QFormLayout* formLayout = new QFormLayout();
    QLineEdit* lineEditTitle = new QLineEdit();
    lineEditTitle->setText(m_title);
    connect(lineEditTitle, SIGNAL(textChanged(const QString&)), this, SLOT(SetTitle(const QString&)));
    formLayout->addRow(tr("Title:"), lineEditTitle);
    QLineEdit* lineEditDescription = new QLineEdit();
    lineEditDescription->setText(m_description);
    connect(lineEditDescription, SIGNAL(textChanged(const QString&)), this, SLOT(SetDescription(const QString&)));
    formLayout->addRow(tr("Description:"), lineEditDescription);
    QSpinBox* spinBoxDPI = new QSpinBox();
    spinBoxDPI->setMinimum(1);
    spinBoxDPI->setMaximum(10000);
    spinBoxDPI->setValue(m_dpi);
    connect(spinBoxDPI, SIGNAL(valueChanged(int)), this, SLOT(SetDPI(int)));
    formLayout->addRow(tr("DPI:"), spinBoxDPI);
    widget->setLayout(formLayout);
    return widget;
}

QPaintDevice* SVGImageRecorder::GetPaintDevice(const QString& path, const QString& name, int width, int height)
{
    QDir directory(path);
    QString completePath;
    QString safeName = name;
    if (directory.exists(name + ".svg"))
    {
        int counter = 1;
        do
            safeName = name + "_" + QString::number(counter);
        while (directory.exists(name + "_" + QString::number(counter++) + ".svg"));
    }
    completePath = directory.absoluteFilePath(safeName + ".svg");

    QSvgGenerator* generator = new QSvgGenerator();
    generator->setFileName(completePath);
    generator->setSize(QSize(width, height));
    generator->setViewBox(QRect(0, 0, width, height));
    generator->setResolution(m_dpi);
    generator->setTitle(m_title);
    generator->setDescription(m_description);

    return generator;
}

void SVGImageRecorder::SaveImage(const QString& path, const QString& name, QPaintDevice* device)
{
    Q_UNUSED(path)
    Q_UNUSED(name)
    Q_UNUSED(device)
}

void SVGImageRecorder::SetTitle(const QString& text)
{
    m_title = text;
}

void SVGImageRecorder::SetDescription(const QString& text)
{
    m_description = text;
}

void SVGImageRecorder::SetDPI(int dpi)
{
    m_dpi = dpi;
    QSettings settings;
    settings.setValue("SVGImageRecorder/DPI", m_dpi);
}
