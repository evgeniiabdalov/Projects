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

#include <QtCore/QLineF>
#include <QtCore/QRectF>
#include <QtGui/QWidget>
#include <QtGui/QGraphicsItem>
#include <QtGui/QStyleOptionGraphicsItem>
#include <QtGui/QPainter>
#include <QtGui/QPen>

#include "primitives/edge.h"
#include "primitives/node.h"

Edge::Edge(Node* source, Node* destination) :
        m_nodeSource(source),
        m_nodeDestination(destination),
        m_clLine(Qt::black),
        m_clHighlight(Qt::cyan),
        m_highlight(false)
{
    setCacheMode(QGraphicsItem::NoCache);
    setAcceptedMouseButtons(0);
    setZValue(2);

    if (m_nodeSource)
        m_nodeSource->AddEdge(this);
    if (m_nodeDestination)
        m_nodeDestination->AddEdge(this);
    Adapt();
}

Edge* Edge::CloneShallow()
{
    Edge* clone = new Edge(NULL, NULL);
    clone->SetNodeSource(m_nodeSource);
    clone->SetNodeDestination(m_nodeDestination);
    clone->Adapt();
    clone->setZValue(1);
    clone->m_clLine = this->m_clLine;
    clone->m_clHighlight = this->m_clHighlight;
    clone->m_highlight = this->m_highlight;
    clone->setPos(this->pos());
    return clone;
}

Node* Edge::GetNodeSource() const
{
    return m_nodeSource;
}

void Edge::SetNodeSource(Node* node)
{
    m_nodeSource = node;
    Adapt();
}

Node* Edge::GetNodeDestination() const
{
    return m_nodeDestination;
}

void Edge::SetNodeDestination(Node* node)
{
    m_nodeDestination = node;
    Adapt();
}

void Edge::SetHighlight(bool highlight)
{
    m_highlight = highlight;
}

QColor Edge::GetColor(Edge::EdgeColorType type) const
{
    switch (type)
    {
        case Edge::clLine:
            return m_clLine;
        case Edge::clHighlight:
            return m_clHighlight;
        default:
            return Qt::black;
    }
}

void Edge::Adapt()
{
    if (!m_nodeSource || !m_nodeDestination)
        return;

    QLineF edge(mapFromItem(m_nodeSource, 0, 0), mapFromItem(m_nodeDestination, 0, 0));
    prepareGeometryChange();
    m_ptSource = edge.p1();
    m_ptDestination = edge.p2();
}

void Edge::SetColor(Edge::EdgeColorType type, const QColor& color)
{
    switch (type)
    {
        case Edge::clLine:
            m_clLine = color;
            break;
        case Edge::clHighlight:
            m_clHighlight = color;
            break;
        default:
            break;
    }
}

QRectF Edge::boundingRect() const
{
    if (!m_nodeSource || !m_nodeDestination)
        return QRectF();

    double left;
    double top;
    double width;
    double height;
    if (m_ptSource.x() <= m_ptDestination.x())
    {
        width = m_ptDestination.x() - m_ptSource.x() + 2 * m_nodeSource->GetRadius() + 2 * m_nodeDestination->GetRadius();
        left = m_ptSource.x() - 2 * m_nodeSource->GetRadius();
    }
    else
    {
        width = m_ptSource.x() - m_ptDestination.x() + 2 * m_nodeSource->GetRadius() + 2 * m_nodeDestination->GetRadius();
        left = m_ptDestination.x() - 2 * m_nodeDestination->GetRadius();
    }
    if (m_ptSource.y() <= m_ptDestination.y())
    {
        height = m_ptDestination.y() - m_ptSource.y() + 2 * m_nodeSource->GetRadius() + 2 * m_nodeDestination->GetRadius();
        top = m_ptSource.y() - 2 * m_nodeSource->GetRadius();
    }
    else
    {
        height = m_ptSource.y() - m_ptDestination.y() + 2 * m_nodeSource->GetRadius() + 2 * m_nodeDestination->GetRadius();
        top = m_ptDestination.y() - 2 * m_nodeDestination->GetRadius();
    }

    return QRectF(left, top, width, height);
}

void Edge::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    if (!m_nodeSource || !m_nodeDestination)
        return;

    QLineF edge(m_ptSource, m_ptDestination);
    if (m_highlight)
    {
        QPen* pen = new QPen(m_clHighlight, 2 * m_nodeSource->GetRadius());
        pen->setCapStyle(Qt::RoundCap);
        painter->setPen(*pen);
        painter->drawLine(edge);
        delete pen;
    }
    else
    {
        painter->setPen(QPen(m_clLine, 1));
        painter->drawLine(edge);
    }
}
