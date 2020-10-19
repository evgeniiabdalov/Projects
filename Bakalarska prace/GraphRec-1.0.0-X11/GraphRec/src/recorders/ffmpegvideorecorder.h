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

#ifndef FFMPEGVIDEORECORDER_H
#define FFMPEGVIDEORECORDER_H

#include <QtCore/QString>
#include <QtCore/QThread>

#include <stdint.h>

#include "recorders/videorecorder.h"

class QWidget;
class AVOutputFormat;
class AVFormatContext;
class AVStream;
class AVFrame;

class FFmpegVideoRecorder : public VideoRecorder
{
    Q_OBJECT

public:
    FFmpegVideoRecorder();
    virtual ~FFmpegVideoRecorder();
    static QString GetName() { return "FFmpeg"; }
    virtual QString Name() const { return FFmpegVideoRecorder::GetName(); }
    virtual QString Description() const { return "FFmpeg Library"; }

    virtual QWidget* GetSettingsWidget();
    virtual int GetFPS() const;
    virtual bool Start(const QString& path, const QString& name, int width, int height);
    virtual void Stop();

protected:
    bool CreateStream();
    void DestroyStream();
    bool OpenVideo();
    void CloseVideo();
    AVFrame* AllocateFrame(int format);

protected slots:
    void SetFPS(int fps);
    void SetBitrate(int bitrate);
    void SetGOP(int gop);
    void SetBufferSize(int size);
    void SetFormatString(const QString& format);

private:
    struct Data
    {
        int width;
        int height;
        int fps;
        int bitrate;
        int gop;
        QString formatString;
        AVOutputFormat* format;
        AVFormatContext* context;
        AVStream* stream;
        AVFrame* frameFinal;
        AVFrame* frameTemp;
        uint8_t* buffer;
        int bufferSize;
    };

    class EncoderThread : public QThread
    {
    public:
        EncoderThread();
        void SetData(FFmpegVideoRecorder::Data* data);
        void SafelyTerminate();
    protected:
        virtual void run();
    private:
        bool m_terminate;
        FFmpegVideoRecorder::Data* m_recdata;
    };

    FFmpegVideoRecorder::Data* m_data;
    EncoderThread* m_thread;
};

#endif
