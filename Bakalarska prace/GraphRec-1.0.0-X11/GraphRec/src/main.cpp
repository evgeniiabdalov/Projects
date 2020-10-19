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

#ifdef G_GRSTATIC
    #include <QtPlugin>
    Q_IMPORT_PLUGIN(qjpeg)
#endif

#include <QtCore/QTime>
#include <QtCore/QSemaphore>
#include <QtGui/QImage>
#include <QtGui/QApplication>

#include "graphrec.h"

#define G_GRVIDEOBUFFERSIZE 64
int G_GRVideoBufferSize = G_GRVIDEOBUFFERSIZE;
QImage G_GRVideoBuffer[G_GRVIDEOBUFFERSIZE];
QSemaphore G_GRVideoFree(G_GRVideoBufferSize);
QSemaphore G_GRVideoUsed;
void* G_GRVideoOwner = NULL;

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    GraphRec widget;
    widget.show();
    return application.exec();
}
