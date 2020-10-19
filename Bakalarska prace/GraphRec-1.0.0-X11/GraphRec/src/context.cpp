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
#include <QtCore/QFileInfo>
#include <QtCore/QtAlgorithms>

#include "context.h"
#include "primitives/entity.h"
#include "primitives/node.h"
#include "primitives/edge.h"

Context::Context(QFileInfo& fileInfo, qint64 filePosition) :
        filePosition(filePosition),
        sceneBackground(Qt::white),
        sceneAngle(0),
        enabledColoring(true),
        enabledLayouting(true),
        layoutDisplacement(40.0)
{
    fileName = fileInfo.fileName();
    filePath = fileInfo.canonicalPath();
    fileCompleteName = fileInfo.canonicalFilePath();
}

Context::~Context()
{
    foreach (Node* node, nodes)
    {
        delete node;
    }
    foreach (Entity* entity, entities)
    {
        delete entity;
    }
    foreach (Edge* edge, edges)
    {
        delete edge;
    }
    foreach (Frame* frame, frames)
    {
        delete frame;
    }
}

void Context::SortCalendar()
{
    qSort(calendar.begin(), calendar.end(), Context::calendarLessThan);
}

bool Context::calendarLessThan(CalendarEvent a, CalendarEvent b)
{
    if (a.timestep < b.timestep)
        return true;
    else
        return false;
}
