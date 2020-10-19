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
#include <QtGui/QApplication>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsItem>
#include <cmath>
#include <limits>

#include "layouters/kamadakawailayouter.h"
#include "context.h"
#include "primitives/node.h"

KamadaKawaiLayouter::KamadaKawaiLayouter() :
        init(true)
{
}

KamadaKawaiLayouter::~KamadaKawaiLayouter()
{
    for (int i = 0; i < m.count(); ++i)
        delete[] d[i];
    for (int i = 0; i < m.count(); ++i)
        delete[] k[i];
    delete[] d;
    delete[] k;
}

bool KamadaKawaiLayouter::Layout(Context& context)
{
    // First call only
    if (init)
    {
        // Node ID mapper
        foreach (Node* node, context.nodes)
        {
            if (node)
                m.append(node->GetID());
        }

        // Allocation
        d = new double*[m.count()];
        k = new double*[m.count()];
        for (int i = 0; i < m.count(); ++i)
        {
            d[i] = new double[m.count()];
            k[i] = new double[m.count()];
        }

        // Initialization
        for (int i = 0; i < m.count(); ++i)
        {
            for (int j = 0; j < m.count(); ++j)
            {
                if (i == j)
                {
                    d[i][j] = 0;
                    k[i][j] = 0;
                }
                else if (context.edges.contains(Context::Ends(m[i],m[j])) || context.edges.contains(Context::Ends(m[j],m[i])))
                {
                    d[i][j] = 1;
                    k[i][j] = 0;
                }
                else
                {
                    d[i][j] = std::numeric_limits<double>::max();
                    k[i][j] = 0;
                }
            }
        }

        // Floyd-Warshall algorithm (all-pairs shortest-paths problem)
        for (int s = 0; s < m.count(); ++s)
        {
            for (int i = 0; i < m.count(); ++i)
            {
                for (int j = 0; j < m.count(); ++j)
                {
                    if (d[i][s] + d[s][j] < d[i][j])
                        d[i][j] = d[i][s] + d[s][j];
                }
            }
        }

        // Spring strengths
        for (int i = 0; i < m.count(); ++i)
        {
            for (int j = 0; j < m.count(); ++j)
            {
                k[i][j] = 40.0 / (d[i][j] * d[i][j]);
            }
        }

        init = false;
    }

    // Gradients
    int M = 0;
    double gradM = 0;
    for (int i = 0; i < m.count(); ++i)
    {
        double dEdx = 0; // dE/dx
        double dEdy = 0; // dE/dy
        for (int j = 0; j < m.count(); ++j)
        {
            QLineF separation(context.nodes[m[i]]->mapFromItem(context.nodes[m[j]], 0, 0), QPointF(0, 0));
            double dx = separation.dx();
            double dy = separation.dy();
            double distance = sqrt(dx * dx + dy * dy);
            if (distance > 0)
            {
                dEdx += k[i][j] * (dx - ((double)context.layoutDisplacement * d[i][j] * dx) / distance);
                dEdy += k[i][j] * (dy - ((double)context.layoutDisplacement * d[i][j] * dy) / distance);
            }
        }
        double gradI = sqrt(dEdx * dEdx + dEdy * dEdy);
        if (gradI > gradM)
        {
            gradM = gradI;
            M = i;
        }
    }

    // Layout is done.
    if (gradM <= 5)
        return true;

    int cycles = 0;
    // Newton–Raphson method (finding zero of nonlinear systems of equations)
    while (gradM > 0.5)
    {
        // Prevent crash when switching layouters.
        if (M >= m.count())
            break;

        Node* nodeM = context.nodes[m[M]];
        double x = nodeM->pos().x();
        double y = nodeM->pos().y();

        // Jacobian, first-order partial derivatives
        double dEdxdx = 0; // dE/dxdx
        double dEdxdy = 0; // dE/dxdy
        double dEdydx = 0; // dE/dydx
        double dEdydy = 0; // dE/dydy
        double dEdx = 0; // dE/dx
        double dEdy = 0; // dE/dy
        for (int i = 0; i < m.count(); ++i)
        {
            QLineF separation(nodeM->mapFromItem(context.nodes[m[i]], 0, 0), QPointF(0, 0));
            double dx = separation.dx();
            double dy = separation.dy();
            double distance = sqrt(dx * dx + dy * dy);
            double cubedDistance = distance * distance * distance;
            if (cubedDistance > 0)
            {
                dEdxdx += k[M][i] * (1 - ((double)context.layoutDisplacement * d[M][i] * dy * dy) / cubedDistance);
                dEdxdy += k[M][i] * ((double)context.layoutDisplacement * d[M][i] * dx * dy) / cubedDistance;
                dEdydx = dEdxdy;
                dEdydy += k[M][i] * (1 - ((double)context.layoutDisplacement * d[M][i] * dx * dx) / cubedDistance);
                dEdx += k[M][i] * (dx - ((double)context.layoutDisplacement * d[M][i] * dx) / distance);
                dEdy += k[M][i] * (dy - ((double)context.layoutDisplacement * d[M][i] * dy) / distance);
            }
        }

        // Gradient for next round
        gradM = sqrt(dEdx * dEdx + dEdy * dEdy);

        // Prepare dEdx, dEdy for Newton-Raphson.
        dEdx = -dEdx;
        dEdy = -dEdy;

        // Cramer's rule
        double denom = dEdxdx * dEdydy - dEdxdy * dEdydx;
        if (denom != 0)
        {
            x += (dEdx * dEdydy - dEdxdy * dEdy) / denom;
            y += (dEdxdx * dEdy - dEdx * dEdydx) / denom;
        }

        // Update position
        nodeM->setPos(x, y);
        //qApp->processEvents();

        // Detect lock-ups when method fails to converge.
        cycles++;
        if (cycles > 100)
            break;
    }

    return false;
}
