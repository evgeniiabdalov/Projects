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

#ifndef CONTEXT_H
#define CONTEXT_H

#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtCore/QPair>
#include <QtCore/QPointF>
#include <QtCore/QRectF>
#include <QtGui/QMatrix>
#include <QtGui/QColor>

class Entity;
class Node;
class Edge;
class QFileInfo;

class Context : public QObject
{
    Q_OBJECT

public:
    Context(QFileInfo& fileInfo, qint64 filePosition);
    ~Context();
    void SortCalendar();

    QString graphName;
    QString fileName;
    QString filePath;
    QString fileCompleteName;
    qint64 filePosition;

    typedef QPair<int, int> Move; // QPair<sourceNodeID, destinationNodeID>
    typedef QPair<int, int> Ends; // QPair<sourceNodeID, destinationNodeID>
    typedef QMap<int, QPointer<Node> > NodeMap; // QMap<nodeID, node>
    typedef QMap<int, QPointer<Entity> > EntityMap; // QMap<entityID, entity>
    typedef QMap<Ends, QPointer<Edge> > EdgeMap; // QMap<QPair<sourceNodeID, destinationNodeID>, edge>
    typedef QMap<int, int> TimeStepMap; // QMap<timestep, calendarPos>
    typedef QMap<QPointer<Node>, QPointer<Entity> > Frame;

    typedef struct
    {
        int timestep;
        Move move;
        bool valid;
        bool reverse;
    } CalendarEvent;

    static bool calendarLessThan(CalendarEvent a, CalendarEvent b);

    NodeMap nodes;
    EntityMap entities;
    EdgeMap edges;
    QList<CalendarEvent> calendar;
    TimeStepMap timesteps;
    QList<Frame*> frames;

    QColor sceneBackground;
    QMatrix sceneMatrix;
    double sceneAngle;
    QPointF sceneViewCenter;

    QString validatorName;

    bool enabledColoring;

    bool enabledLayouting;
    double layoutDisplacement;
};

#endif
