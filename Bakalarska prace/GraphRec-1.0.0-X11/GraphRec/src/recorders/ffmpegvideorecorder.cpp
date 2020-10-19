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

extern "C"
{
#define __STDC_CONSTANT_MACROS
#define __STDC_LIMIT_MACROS
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
};

#include <QtCore/QString>
#include <QtCore/QSettings>
#include <QtCore/QDir>
#include <QtCore/QThread>
#include <QtCore/QSemaphore>
#include <QtGui/QImage>
#include <QtGui/QWidget>
#include <QtGui/QMessageBox>
#include <QtGui/QFormLayout>
#include <QtGui/QComboBox>
#include <QtGui/QSpinBox>

#include "recorders/ffmpegvideorecorder.h"

#define QT_PIX_FMT PIX_FMT_RGB32

extern int G_GRVideoBufferSize;
extern QImage G_GRVideoBuffer[];
extern QSemaphore G_GRVideoFree;
extern QSemaphore G_GRVideoUsed;
extern void* G_GRVideoOwner;

// Format list
#define FMT_ITEM_AVIMPEG4 "AVI (MPEG4)"
#define FMT_ITEM_MKVMPEG4 "MKV (MPEG4)"
#define FMT_ITEM_MPGMPEG1 "MPG (MPEG1)"
#define FMT_ITEM_MPGMPEG2 "MPG (MPEG2)"
#define FMT_ITEM_MOVMPEG4 "MOV (MPEG4)"
#define FMT_ITEM_MP4MPEG4 "MP4 (MPEG4)"
#define FMT_ITEM_RMRV10 "RM (RV10)"
#define FMT_ITEM_SWFFLV1 "SWF (FLV1)"
#define FMT_ITEM_3GPH263 "3GP (H.263)"

FFmpegVideoRecorder::FFmpegVideoRecorder()
{
    QSettings settings;
    m_data = new FFmpegVideoRecorder::Data();
    m_data->width = 720;
    m_data->height = 576;
    m_data->fps = settings.value("FFmpegVideoRecorder/FPS", 25).toInt();
    m_data->bitrate = settings.value("FFmpegVideoRecorder/Bitrate", 800000).toInt();
    m_data->gop = settings.value("FFmpegVideoRecorder/GOP", 300).toInt();
    m_data->formatString = settings.value("FFmpegVideoRecorder/Format", FMT_ITEM_AVIMPEG4).toString();
    m_data->format = NULL;
    m_data->context = NULL;
    m_data->stream = NULL;
    m_data->frameFinal = NULL;
    m_data->frameTemp = NULL;
    m_data->buffer = NULL;
    m_data->bufferSize = settings.value("FFmpegVideoRecorder/BufferSize", 8388608).toInt();
    m_thread = new EncoderThread();
    m_thread->SetData(m_data);
    av_register_all();
}

FFmpegVideoRecorder::~FFmpegVideoRecorder()
{
    Stop();
    delete m_thread;
    delete m_data;
}

QWidget* FFmpegVideoRecorder::GetSettingsWidget()
{
    QWidget* widget = new QWidget();
    QFormLayout* formLayout = new QFormLayout();
    QComboBox* comboBoxFormat = new QComboBox();
    comboBoxFormat->addItem(FMT_ITEM_AVIMPEG4);
    comboBoxFormat->addItem(FMT_ITEM_MKVMPEG4);
    comboBoxFormat->addItem(FMT_ITEM_MPGMPEG1);
    comboBoxFormat->addItem(FMT_ITEM_MPGMPEG2);
    comboBoxFormat->addItem(FMT_ITEM_MOVMPEG4);
    comboBoxFormat->addItem(FMT_ITEM_MP4MPEG4);
    comboBoxFormat->addItem(FMT_ITEM_RMRV10);
    comboBoxFormat->addItem(FMT_ITEM_SWFFLV1);
    comboBoxFormat->addItem(FMT_ITEM_3GPH263);
    comboBoxFormat->setCurrentIndex(comboBoxFormat->findText(m_data->formatString, Qt::MatchExactly));
    connect(comboBoxFormat, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(SetFormatString(const QString&)));
    formLayout->addRow(tr("Format:"), comboBoxFormat);
    QSpinBox* spinBoxFPS = new QSpinBox();
    spinBoxFPS->setMinimum(1);
    spinBoxFPS->setMaximum(30);
    spinBoxFPS->setValue(m_data->fps);
    spinBoxFPS->setSuffix(tr(" frames/sec"));
    connect(spinBoxFPS, SIGNAL(valueChanged(int)), this, SLOT(SetFPS(int)));
    formLayout->addRow(tr("FPS:"), spinBoxFPS);
    QSpinBox* spinBoxBitrate = new QSpinBox();
    spinBoxBitrate->setMinimum(1);
    spinBoxBitrate->setMaximum(INT_MAX);
    spinBoxBitrate->setValue(m_data->bitrate);
    spinBoxBitrate->setSuffix(tr(" bits/sec"));
    connect(spinBoxBitrate, SIGNAL(valueChanged(int)), this, SLOT(SetBitrate(int)));
    formLayout->addRow(tr("Bitrate:"), spinBoxBitrate);
    QSpinBox* spinBoxGOP = new QSpinBox();
    spinBoxGOP->setMinimum(1);
    spinBoxGOP->setMaximum(1000);
    spinBoxGOP->setSuffix(tr(" frames"));
    spinBoxGOP->setValue(m_data->gop);
    connect(spinBoxGOP, SIGNAL(valueChanged(int)), this, SLOT(SetGOP(int)));
    formLayout->addRow(tr("GOP:"), spinBoxGOP);
    QSpinBox* spinBoxBufferSize = new QSpinBox();
    spinBoxBufferSize->setMinimum(1);
    spinBoxBufferSize->setMaximum(INT_MAX);
    spinBoxBufferSize->setSuffix(tr(" B"));
    spinBoxBufferSize->setValue(m_data->bufferSize);
    connect(spinBoxBufferSize, SIGNAL(valueChanged(int)), this, SLOT(SetBufferSize(int)));
    formLayout->addRow(tr("Buffer:"), spinBoxBufferSize);
    widget->setLayout(formLayout);
    return widget;
}

int FFmpegVideoRecorder::GetFPS() const
{
    return m_data->fps;
}

bool FFmpegVideoRecorder::Start(const QString& path, const QString& name, int width, int height)
{
    if (!G_GRVideoOwner)
    {
        if (G_GRVideoFree.available() < G_GRVideoBufferSize)
            G_GRVideoFree.release(G_GRVideoBufferSize - G_GRVideoFree.available());
        if (G_GRVideoUsed.available() > 0)
            G_GRVideoUsed.acquire(G_GRVideoFree.available());

        m_data->width = width % 2 ? width + 1 : width;
        m_data->height = height % 2 ? height + 1 : height;

        QString suffix;
        QString formatID;
        if (m_data->formatString == FMT_ITEM_AVIMPEG4)
        {
            suffix = "avi";
            formatID = "avi";
        }
        else if (m_data->formatString == FMT_ITEM_MKVMPEG4)
        {
            suffix = "mkv";
            formatID = "matroska";
        }
        else if (m_data->formatString == FMT_ITEM_MPGMPEG1)
        {
            suffix = "mpg";
            formatID = "mpeg";
        }
        else if (m_data->formatString == FMT_ITEM_MPGMPEG2)
        {
            suffix = "mpg";
            formatID = "dvd";
        }
        else if (m_data->formatString == FMT_ITEM_MOVMPEG4)
        {
            suffix = "mov";
            formatID = "mov";
        }
        else if (m_data->formatString == FMT_ITEM_MP4MPEG4)
        {
            suffix = "mp4";
            formatID = "mp4";
        }
        else if (m_data->formatString == FMT_ITEM_RMRV10)
        {
            suffix = "rm";
            formatID = "rm";
        }
        else if (m_data->formatString == FMT_ITEM_SWFFLV1)
        {
            suffix = "swf";
            formatID = "swf";
        }
        else if (m_data->formatString == FMT_ITEM_3GPH263)
        {
            suffix = "3gp";
            formatID = "3gp";
        }


        // Filename
        QDir directory(path);
        QString completePath;
        QString safeName = name;
        if (directory.exists(name + "." + suffix))
        {
            int counter = 1;
            do
                safeName = name + "_" + QString::number(counter);
            while (directory.exists(name + "_" + QString::number(counter++) + "." + suffix));
        }
        while (directory.exists(safeName + "." + suffix))
            safeName += "_";

        completePath = directory.absoluteFilePath(safeName + "." + suffix);
        int filenameSize = 1024;
        char* filename = (char*)malloc(filenameSize);
        qsnprintf(filename, filenameSize, completePath.toLocal8Bit().constData());
        if (!filename)
        {
            QMessageBox::warning(NULL, tr("Warning"), tr("Invalid filename."));
            return false;
        }

        // Format
        const char* fmtID = formatID.toAscii();
        m_data->format = guess_format(fmtID, NULL, NULL);
        if (!m_data->format)
        {
            QMessageBox::warning(NULL, tr("Warning"), tr("Unknown format."));
            return false;
        }

        // Context
        m_data->context = avformat_alloc_context();
        if (!m_data->context)
            return false;
        m_data->context->oformat = m_data->format;
        qsnprintf(m_data->context->filename, sizeof(m_data->context->filename), "%s", filename);

        // Stream
        if (m_data->format->video_codec != CODEC_ID_NONE)
            if (!CreateStream())
                return false;

        // Parameters
        if (av_set_parameters(m_data->context, NULL) < 0)
            return false;

        // Encoder
        if (!OpenVideo())
            return false;

        // File
        if (!(m_data->format->flags & AVFMT_NOFILE))
        {
            if (url_fopen(&m_data->context->pb, filename, URL_WRONLY) < 0)
            {
                QMessageBox::warning(NULL, tr("Warning"), tr("File cannot be opened."));
                return false;
            }
        }

        // Header
        av_write_header(m_data->context);

        G_GRVideoOwner = this;
        m_thread->start();
        return true;
    }
    else
        return false;
}

void FFmpegVideoRecorder::Stop()
{
    if (this == G_GRVideoOwner)
    {
        m_thread->SafelyTerminate();

        // Trailer
        if (m_data->context)
            av_write_trailer(m_data->context);

        // Encoder
        CloseVideo();

        // Stream
        DestroyStream();

        // File
        if (m_data->format && m_data->context)
        {
            if (!(m_data->format->flags & AVFMT_NOFILE) && m_data->context->pb)
                url_fclose(m_data->context->pb);
        }

        // Context
        if (m_data->context)
            av_free(m_data->context);

        G_GRVideoOwner = NULL;
    }

    m_data->buffer = NULL;
    m_data->context = NULL;
    m_data->format = NULL;
    m_data->frameFinal = NULL;
    m_data->frameTemp = NULL;
    m_data->stream = NULL;
}

bool FFmpegVideoRecorder::CreateStream()
{
    if (!m_data->context || !m_data->format)
        return false;

    m_data->stream = av_new_stream(m_data->context, 0);
    if (!m_data->stream)
        return false;

    // Codec
    AVCodecContext* codec = m_data->stream->codec;
    codec->codec_id = m_data->format->video_codec;
    codec->codec_type = CODEC_TYPE_VIDEO;
    codec->bit_rate = m_data->bitrate;
    codec->width = m_data->width;
    codec->height = m_data->height;
    codec->time_base.den = m_data->fps;
    codec->time_base.num = 1;
    codec->gop_size = m_data->gop;
    codec->pix_fmt = PIX_FMT_YUV420P;
    codec->mb_decision = FF_MB_DECISION_RD;
    codec->trellis = 2;
    codec->me_cmp = 2;
    codec->me_sub_cmp = 2;

    if (codec->codec_id == CODEC_ID_MPEG4)
    {
        codec->max_b_frames = 2;
        codec->flags |= CODEC_FLAG_4MV;
        codec->flags |= CODEC_FLAG_AC_PRED;
        codec->flags |= CODEC_FLAG_GMC;
    }
    if (codec->codec_id == CODEC_ID_MPEG2VIDEO)
        codec->max_b_frames = 2;
    if (codec->codec_id == CODEC_ID_MPEG1VIDEO)
        codec->mb_decision = 2;
    if(m_data->context->oformat->flags & AVFMT_GLOBALHEADER)
        codec->flags |= CODEC_FLAG_GLOBAL_HEADER;

    return true;
}

void FFmpegVideoRecorder::DestroyStream()
{
    if (!m_data->context)
        return;
    for (unsigned int i = 0; i < m_data->context->nb_streams; i++)
    {
        av_freep(&m_data->context->streams[i]->codec);
        av_freep(&m_data->context->streams[i]);
    }
}

bool FFmpegVideoRecorder::OpenVideo()
{
    if (!m_data->context || !m_data->format || !m_data->stream)
        return false;

    // Encoder
    AVCodecContext* codec = m_data->stream->codec;
    AVCodec* encoder = avcodec_find_encoder(codec->codec_id);
    if (!encoder)
        return false;
    if (avcodec_open(codec, encoder) < 0)
    {
        QMessageBox::warning(NULL, tr("Warning"), tr("Could not open codec. Maybe incorrect parameters. See Help."));
        return false;
    }

    // Allocation
    if (!(m_data->context->oformat->flags & AVFMT_RAWPICTURE))
        m_data->buffer = (uint8_t*)av_malloc(m_data->bufferSize);
    m_data->frameFinal = AllocateFrame(codec->pix_fmt);
    if (!m_data->frameFinal)
        return false;
    m_data->frameTemp = AllocateFrame(QT_PIX_FMT);
    if (!m_data->frameTemp)
        return false;

    return true;
}

void FFmpegVideoRecorder::CloseVideo()
{
    if (m_data->stream && m_data->stream->codec && m_data->stream->codec->codec)
        avcodec_close(m_data->stream->codec);
    if (m_data->frameFinal)
    {
        av_free(m_data->frameFinal->data[0]);
        av_free(m_data->frameFinal);
    }
    if (m_data->frameTemp)
    {
        av_free(m_data->frameTemp->data[0]);
        av_free(m_data->frameTemp);
    }
    if (m_data->buffer)
        av_free(m_data->buffer);
}

AVFrame* FFmpegVideoRecorder::AllocateFrame(int format)
{
    AVFrame* frame = avcodec_alloc_frame();
    if (!frame)
        return NULL;
    int size = avpicture_get_size(format, m_data->width, m_data->height);
    uint8_t* buffer = (uint8_t*)av_malloc(size);
    if (!buffer)
    {
        av_free(frame);
        return NULL;
    }
    avpicture_fill((AVPicture*)frame, buffer, format, m_data->width, m_data->height);
    return frame;
}

void FFmpegVideoRecorder::SetFPS(int fps)
{
    m_data->fps = fps;
    QSettings settings;
    settings.setValue("FFmpegVideoRecorder/FPS", fps);
}

void FFmpegVideoRecorder::SetBitrate(int bitrate)
{
    m_data->bitrate = bitrate;
    QSettings settings;
    settings.setValue("FFmpegVideoRecorder/Bitrate", bitrate);
}

void FFmpegVideoRecorder::SetGOP(int gop)
{
    m_data->gop = gop;
    QSettings settings;
    settings.setValue("FFmpegVideoRecorder/GOP", gop);
}

void FFmpegVideoRecorder::SetBufferSize(int size)
{
    m_data->bufferSize = size;
    QSettings settings;
    settings.setValue("FFmpegVideoRecorder/BufferSize", size);
}

void FFmpegVideoRecorder::SetFormatString(const QString& format)
{
    m_data->formatString = format;
    QSettings settings;
    settings.setValue("FFmpegVideoRecorder/Format", format);
}

FFmpegVideoRecorder::EncoderThread::EncoderThread() :
        m_terminate(false)
{
}

void FFmpegVideoRecorder::EncoderThread::SetData(FFmpegVideoRecorder::Data* data)
{
    m_recdata = data;
}

void FFmpegVideoRecorder::EncoderThread::SafelyTerminate()
{
    m_terminate = true;
    wait();
}

void FFmpegVideoRecorder::EncoderThread::run()
{
    bool problem = false;
    int i = 0;
    int notWritten = 0;
    int notBuffered = 0;
    double time = 0.0;
    AVFormatContext* context = NULL;
    AVStream* stream = NULL;
    AVCodecContext* codec = NULL;
    AVFrame* frameFinal = NULL;
    AVFrame* frameTemp = NULL;
    uint8_t* buffer = NULL;
    int bufferSize = 0;
    int pictureSize = 0;
    static SwsContext* swsContext = NULL;

    if (m_recdata && m_recdata->context && m_recdata->stream)
    {
        context = m_recdata->context;
        stream = m_recdata->stream;
        codec = m_recdata->stream->codec;
        frameFinal = m_recdata->frameFinal;
        frameTemp = m_recdata->frameTemp;
        buffer = m_recdata->buffer;
        bufferSize = m_recdata->bufferSize;
        pictureSize = avpicture_get_size(QT_PIX_FMT, m_recdata->width, m_recdata->height);
        swsContext = sws_getContext(codec->width, codec->height, QT_PIX_FMT,
                                                       codec->width, codec->height, codec->pix_fmt,
                                                       SWS_BICUBIC, NULL, NULL, NULL);
        if (!swsContext)
            problem = true;
    }
    else
        problem = true;

    forever
    {
        if (!problem)
            time = (double)stream->pts.val * stream->time_base.num / stream->time_base.den;

        if (G_GRVideoUsed.tryAcquire())
        {
            // Copy QImage (RGB) to AVFrame (RGB)
            memcpy(frameTemp->data[0], G_GRVideoBuffer[i].bits(), pictureSize);
            G_GRVideoFree.release();

            if (!problem)
            {
                // Convert AVFrame (RGB) to AVFrame (YUV)
                sws_scale(swsContext, frameTemp->data, frameTemp->linesize, 0,
                          codec->height, frameFinal->data, frameFinal->linesize);

                // Encode AVFrame
                AVPacket packet;
                av_init_packet(&packet);
                if (context->oformat->flags & AVFMT_RAWPICTURE)
                {
                    // RAW video
                    packet.flags |= PKT_FLAG_KEY;
                    packet.stream_index = stream->index;
                    packet.data = (uint8_t*)frameFinal;
                    packet.size = sizeof(AVPicture);
                    notWritten = av_interleaved_write_frame(context, &packet);
                }
                else
                {
                    // Encoded video
                    notBuffered = avcodec_encode_video(codec, buffer, bufferSize, frameFinal);
                    if (notBuffered)
                    {
                        if (codec->coded_frame->pts != (int64_t)AV_NOPTS_VALUE)
                            packet.pts = av_rescale_q(codec->coded_frame->pts, codec->time_base, stream->time_base);
                        if (codec->coded_frame->key_frame)
                            packet.flags |= PKT_FLAG_KEY;
                        packet.stream_index = stream->index;
                        packet.data = buffer;
                        packet.size = notBuffered;
                        notWritten = av_interleaved_write_frame(context, &packet);
                    }
                    else
                        notWritten = 0;
                }
                if (notWritten)
                    problem = true;
            }

            i++;
            i %= G_GRVideoBufferSize;
        }
        else if (m_terminate)
            break;
    }

    if (problem)
        QMessageBox::warning(NULL, tr("Warning"), tr("Errors occured during encoding."));
}
