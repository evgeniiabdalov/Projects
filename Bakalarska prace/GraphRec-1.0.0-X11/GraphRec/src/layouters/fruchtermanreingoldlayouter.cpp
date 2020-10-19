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

#include <QtCore/QEventLoop>
#include <QtCore/QLineF>
#include <QtCore/QPointF>
#include <QtCore/QList>
#include <QtCore/QPair>
#include <QtGui/QApplication>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsItem>
#include <cmath>

#include "layouters/fruchtermanreingoldlayouter.h"
#include "context.h"
#include "primitives/node.h"
#include "primitives/edge.h"

FruchtermanReingoldLayouter::FruchtermanReingoldLayouter()
{
}

FruchtermanReingoldLayouter::~FruchtermanReingoldLayouter()
{
}

bool FruchtermanReingoldLayouter::Layout(Context& context)
{
    int iterations = 1; // Iterations per timer tick
    bool stop;
    for (int i = 0; i < iterations; i++)
    {
        stop = true;
        double displacement = context.layoutDisplacement;
        QList<QPair<Node*, QPointF> > positions;
        foreach (Node* nodeV, context.nodes)
        {
            if (!nodeV || !nodeV->scene() || nodeV->scene()->mouseGrabberItem() == nodeV)
                continue;

            double dispX = 0;
            double dispY = 0;

            // Repulsive force
            foreach (Node* nodeU, context.nodes)
            {
                if (nodeU && nodeV != nodeU)
                {
                    QLineF spring(nodeV->mapFromItem(nodeU, 0, 0), QPointF(0, 0));
                    double dx = spring.dx();
                    double dy = spring.dy();
                    // double distance = sqrt(dx * dx + dy * dy);
                    double distance = dx * dx + dy * dy;
                    if (distance > 0)
                    {
                        /*
                        double fr = (displacement * displacement) / distance;
                        dispX += (dx / distance) * fr;
                        dispY += (dy / distance) * fr;
                        */
                        double fr = (displacement * displacement) / sqrt(distance);
                        dispX += (dx * fr) / distance;
                        dispY += (dy * fr) / distance;
                    }
                }
            }

            // Attractive force
            foreach (Edge* edge, nodeV->GetEdges())
            {
                Node* sourceNode;
                Node* destinationNode;
                if (edge && (sourceNode = edge->GetNodeSource()) && (destinationNode = edge->GetNodeDestination()))
                {
                    QLineF spring;
                    if (sourceNode == nodeV)
                        spring = QLineF(nodeV->mapFromItem(destinationNode, 0, 0), QPointF(0, 0));
                    else
                        spring = QLineF(nodeV->mapFromItem(sourceNode, 0, 0), QPointF(0, 0));
                    double dx = spring.dx();
                    double dy = spring.dy();
                    // double distance = sqrt(dx * dx + dy * dy);
                    double distance = dx * dx + dy * dy;
                    if (displacement > 0)
                    {
                        /*
                        double fa = (distance * distance) / displacement;
                        dispX -= (dx / distance) * fa;
                        dispY -= (dy / distance) * fa;
                        */
                        double edgeCount = nodeV->GetEdges().size();
                        double fa = distance / displacement;
                        dispX -= dx * (fa / distance) * edgeCount;
                        dispY -= dy * (fa / distance) * edgeCount;
                    }
                }
            }

            /*
            double dispAbs = sqrt(dispX * dispX + dispY * dispY);
            if (dispAbs == 0)
                dispAbs = 0.1;
            double dx = dispX / dispAbs;
            double dy = dispY / dispAbs;
            */
            double dx = dispX;
            double dy = dispY;
            if (fabs(dx) > 0.1 || fabs(dy) > 0.1)
            {
                positions.append(QPair<Node*, QPointF>(nodeV, QPointF(nodeV->pos().x() + dx, nodeV->pos().y() + dy)));
                stop = false;
            }
            else
                positions.append(QPair<Node*, QPointF>(nodeV, QPointF(nodeV->pos().x(), nodeV->pos().y())));
        }

        //qApp->processEvents();

        // Update positions
        if (!stop)
        {
            for (int j = 0; j < positions.size(); j++)
            {
                QPair<Node*, QPointF> update = positions[j];
                if (Node* node = update.first)
                    node->setPos(update.second);
            }
        }
        else
            break;
    }
    return stop;
}
