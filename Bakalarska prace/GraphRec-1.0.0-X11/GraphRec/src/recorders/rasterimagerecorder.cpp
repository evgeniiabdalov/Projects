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
#include <QtCore/QDir>
#include <QtGui/QWidget>
#include <QtGui/QPaintDevice>
#include <QtGui/QImage>
#include <QtGui/QFormLayout>
#include <QtGui/QComboBox>
#include <QtGui/QSlider>
#include <QtGui/QHBoxLayout>
#include <QtGui/QCheckBox>

#include "recorders/rasterimagerecorder.h"

RasterImageRecorder::RasterImageRecorder()
{
    QSettings settings;
    m_format = static_cast<RasterImageRecorder::Format>(settings.value("RasterImageRecorder/Format", RasterImageRecorder::fPNG).toInt());
    m_quality = settings.value("RasterImageRecorder/Quality", 80).toInt();
    m_qualityDefault = settings.value("RasterImageRecorder/DefaultQuality", true).toBool();
}

RasterImageRecorder::~RasterImageRecorder()
{
}

QWidget* RasterImageRecorder::GetSettingsWidget()
{
    QWidget* widget = new QWidget();
    QFormLayout* formLayout = new QFormLayout();
    QComboBox* comboBoxFormat = new QComboBox();
    comboBoxFormat->addItem("PNG");
    comboBoxFormat->addItem("JPG");
    comboBoxFormat->addItem("BMP");
    comboBoxFormat->setCurrentIndex(m_format);
    connect(comboBoxFormat, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(SetFormat(const QString&)));
    formLayout->addRow(tr("Format:"), comboBoxFormat);
    QHBoxLayout* layoutQuality = new QHBoxLayout();
    QSlider* sliderQuality = new QSlider(Qt::Horizontal);
    sliderQuality->setMinimum(0);
    sliderQuality->setMaximum(100);
    sliderQuality->setSingleStep(1);
    sliderQuality->setPageStep(10);
    sliderQuality->setEnabled(!m_qualityDefault);
    sliderQuality->setValue(m_quality);
    connect(sliderQuality, SIGNAL(valueChanged(int)), this, SLOT(SetQuality(int)));
    connect(this, SIGNAL(DefaultQualityChanged(bool)), sliderQuality, SLOT(setEnabled(bool)));
    QCheckBox* checkBoxQuality = new QCheckBox(tr("Default"));
    checkBoxQuality->setChecked(m_qualityDefault);
    connect(checkBoxQuality, SIGNAL(toggled(bool)), this, SLOT(SetDefaultQualityEnabled(bool)));
    layoutQuality->addWidget(sliderQuality);
    layoutQuality->addWidget(checkBoxQuality);
    formLayout->addRow(tr("Quality:"), layoutQuality);
    widget->setLayout(formLayout);
    return widget;
}

QPaintDevice* RasterImageRecorder::GetPaintDevice(const QString& path, const QString& name, int width, int height)
{
    Q_UNUSED(path)
    Q_UNUSED(name)
    return new QImage(width, height, QImage::Format_RGB32);
}

void RasterImageRecorder::SaveImage(const QString& path, const QString& name, QPaintDevice* device)
{
    QImage* image = dynamic_cast<QImage*>(device);
    if (!image)
        return;
    QDir directory(path);
    QString completePath;
    QString safeName = name;
    QString id;
    QString extension;
    switch (m_format)
    {
        case RasterImageRecorder::fPNG:
            id = "PNG";
            extension = ".png";
            break;
        case RasterImageRecorder::fJPG:
            id = "JPG";
            extension = ".jpg";
            break;
        case RasterImageRecorder::fBMP:
            id = "BMP";
            extension = ".bmp";
            break;
        default:
            break;
    }
    if (directory.exists(name + extension))
    {
        int counter = 1;
        do
            safeName = name + "_" + QString::number(counter);
        while (directory.exists(name + "_" + QString::number(counter++) + extension));
    }
    completePath = directory.absoluteFilePath(safeName + extension);
    if (m_qualityDefault)
        image->save(completePath, id.toAscii());
    else
        image->save(completePath, id.toAscii(), m_quality);
}

void RasterImageRecorder::SetFormat(const QString& format)
{
    if (format == "PNG")
        m_format = RasterImageRecorder::fPNG;
    else if (format == "JPG")
        m_format = RasterImageRecorder::fJPG;
    else if (format == "BMP")
        m_format = RasterImageRecorder::fBMP;
    QSettings settings;
    settings.setValue("RasterImageRecorder/Format", m_format);
}

void RasterImageRecorder::SetQuality(int quality)
{
    m_quality = quality;
    QSettings settings;
    settings.setValue("RasterImageRecorder/Quality", m_quality);
}

void RasterImageRecorder::SetDefaultQualityEnabled(bool enable)
{
    m_qualityDefault = enable;
    QSettings settings;
    settings.setValue("RasterImageRecorder/DefaultQuality", m_qualityDefault);
    emit DefaultQualityChanged(!enable);
}
