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

#ifndef RASTERIMAGERECORDER_H
#define RASTERIMAGERECORDER_H

#include <QtCore/QString>

#include "recorders/imagerecorder.h"

class QWidget;
class QPaintDevice;

class RasterImageRecorder : public ImageRecorder
{
    Q_OBJECT

signals:
    void DefaultQualityChanged(bool notEnabled);

public:
    enum Format
    {
        fPNG = 0,
        fJPG,
        fBMP
    };

    RasterImageRecorder();
    virtual ~RasterImageRecorder();
    static QString GetName() { return "Raster Image"; }
    virtual QString Name() const { return RasterImageRecorder::GetName(); }
    virtual QString Description() const { return "Raster Image Encoder"; }

    virtual QWidget* GetSettingsWidget();
    virtual QPaintDevice* GetPaintDevice(const QString& path, const QString& name, int width, int height);
    virtual void SaveImage(const QString& path, const QString& name, QPaintDevice* device);

protected slots:
    void SetFormat(const QString& format);
    void SetQuality(int quality);
    void SetDefaultQualityEnabled(bool enable);

private:
    RasterImageRecorder::Format m_format;
    int m_quality;
    bool m_qualityDefault;
};

#endif
