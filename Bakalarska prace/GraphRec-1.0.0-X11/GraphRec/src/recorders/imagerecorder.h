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

#ifndef IMAGERECORDER_H
#define IMAGERECORDER_H

#include "recorders/recorder.h"

class QString;
class QPaintDevice;

class ImageRecorder : public Recorder
{
    Q_OBJECT

public:
    ImageRecorder() { }
    virtual ~ImageRecorder() { }

    virtual QPaintDevice* GetPaintDevice(const QString& path, const QString& name, int width, int height) = 0;
    virtual void SaveImage(const QString& path, const QString& name, QPaintDevice* device) = 0;
};

#endif
