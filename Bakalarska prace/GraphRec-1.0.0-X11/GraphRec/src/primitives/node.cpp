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

#include <QtCore/QPointer>
#include <QtCore/QRectF>
#include <QtCore/QPointF>
#include <QtGui/QWidget>
#include <QtGui/QGraphicsItem>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QStyleOptionGraphicsItem>
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include <QtGui/QPen>
#include <QtGui/QBrush>
#include <QtGui/QColor>
#include <QtGui/QFont>
#include <cmath>

#include "primitives/entity.h"
#include "primitives/node.h"
#include "primitives/edge.h"

Node::Node(int id, Entity* entity, double radius, Node::NodeDescription description) :
        m_id(id),
        m_entity(entity),
        m_radius(radius),
        m_discreteDisplacementEnabled(false),
        m_discreteDisplacementOffset(14),
        m_description(description),
        m_timestep(0),
        m_clBackground(Qt::yellow),
        m_clForeground(Qt::black),
        m_clBoundary(Qt::black)
{
    setCacheMode(QGraphicsItem::NoCache);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setZValue(3);
    setAcceptedMouseButtons(Qt::LeftButton);
}

Node* Node::CloneShallow()
{
    Node* clone = new Node(m_id, NULL, m_radius, m_description);
    clone->setZValue(6);
    clone->setFlag(QGraphicsItem::ItemIsMovable, false);
    clone->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    clone->setPos(this->pos());
    clone->m_entity = this->m_entity;
    clone->m_timestep = this->m_timestep;
    clone->m_clBackground = this->m_clBackground;
    clone->m_clForeground = this->m_clForeground;
    clone->m_clBoundary = this->m_clBoundary;
    return clone;
}

int Node::GetID() const
{
    return m_id;
}

Entity* Node::GetEntity() const
{
    return m_entity;
}

void Node::SetEntity(Entity* entity)
{
    m_entity = entity;
}

double Node::GetRadius() const
{
    return m_radius;
}

Node::NodeDescription Node::GetDescription() const
{
    return m_description;
}

void Node::AddEdge(Edge* edge)
{
    m_edges.append(QPointer<Edge>(edge));
    edge->Adapt();
}

QList<QPointer<Edge> > Node::GetEdges() const
{
    return m_edges;
}

QColor Node::GetColor(Node::NodeColorType type) const
{
    switch (type)
    {
        case Node::clBackground:
            return m_clBackground;
        case Node::clForeground:
            return m_clForeground;
        case Node::clBoundary:
            return m_clBoundary;
        default:
            return Qt::black;
    }
}

void Node::AlignPosition()
{
    if (m_discreteDisplacementEnabled)
        setPos(AlignPoint(pos()));
}

void Node::SetRadius(double radius)
{
    m_radius = radius;
}

void Node::SetDiscreteDisplacementEnabled(bool enable)
{
    m_discreteDisplacementEnabled = enable;
}

void Node::SetDiscreteDisplacementOffset(int offset)
{
    m_discreteDisplacementOffset = offset;
}

void Node::SetDescription(Node::NodeDescription description)
{
    m_description = description;
}

void Node::SetTimeStep(int timeStep)
{
    m_timestep = timeStep;
}

void Node::SetColor(Node::NodeColorType type, const QColor& color)
{
    switch (type)
    {
        case Node::clBackground:
            m_clBackground = color;
            break;
        case Node::clForeground:
            m_clForeground = color;
            break;
        case Node::clBoundary:
            m_clBoundary = color;
            break;
        default:
            break;
    }
}

QRectF Node::boundingRect() const
{
    return QRectF(1.25 * -m_radius, 1.25 * -m_radius, 2.5 * m_radius, 2.5 * m_radius);
}

QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(1.25 * -m_radius, 1.25 * -m_radius, 2.5 * m_radius, 2.5 * m_radius);
    return path;
}

void Node::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    QColor background;
    QColor boundary;
    QColor label;

    boundary = m_clBoundary;
    if (!this->isSelected() && zValue() == 4)
        setZValue(3);
    if (m_entity)
    {
        int timestepFinal = m_entity->GetTimeStepFinal();
        if (timestepFinal >= 0 && timestepFinal <= m_timestep)
        {
            background = m_entity->GetColor(Entity::clBackgroundFinal);
            label = m_entity->GetColor(Entity::clForegroundFinal);
        }
        else
        {
            background = m_entity->GetColor(Entity::clBackground);
            label = m_entity->GetColor(Entity::clForeground);
        }
    }
    else
    {
        background = m_clBackground;
        label = m_clForeground;
    }

    if (this->isSelected())
    {
        painter->setBrush(QBrush(background.dark(135)));
        painter->setPen(QPen(boundary, 3));
        if (zValue() < 4)
            setZValue(4);
    }
    else
    {
        painter->setBrush(QBrush(background));
        painter->setPen(QPen(boundary, 1));
    }

    painter->drawEllipse(QRectF(-m_radius, -m_radius, 2 * m_radius, 2 * m_radius));
    painter->setPen(QPen(label, 1));
    switch(m_description)
    {
        case Node::EntityID:
            if (m_entity)
                painter->drawText(QRectF(-m_radius, -m_radius, 2 * m_radius, 2 * m_radius), Qt::AlignCenter | Qt::AlignCenter, QString::number(m_entity->GetID()));
            break;
        case Node::NodeID:
            painter->drawText(QRectF(-m_radius, -m_radius, 2 * m_radius, 2 * m_radius), Qt::AlignCenter | Qt::AlignCenter, QString::number(m_id));
            break;
        case Node::NodeIDEntityID:
            if (m_entity)
            {
                QFont old = painter->font();
                QFont small;
                small.setPointSizeF(0.6 * old.pointSizeF());
                painter->setFont(small);
                painter->drawText(QRectF(1.25 * -m_radius, 1.25 * -m_radius, 2.5 * m_radius, 2.5 * m_radius), Qt::AlignCenter | Qt::AlignCenter, QString::number(m_id) + ":" + QString::number(m_entity->GetID()));
                painter->setFont(old);
            }
            else
                painter->drawText(QRectF(1.25 * -m_radius, 1.25 * -m_radius, 2.5 * m_radius, 2.5 * m_radius), Qt::AlignCenter | Qt::AlignCenter, QString::number(m_id));
            break;
        default:
            break;
    }

}

QPointF Node::AlignPoint(const QPointF& point)
{
    int offset = m_discreteDisplacementOffset;

    int left = static_cast<int>(floor(point.x()));
    int leftAligned = left - (left % offset);
    int rightAligned = leftAligned + offset;
    int x = (left - leftAligned) < (rightAligned - left) ? leftAligned : rightAligned;

    int top = static_cast<int>(floor(point.y()));
    int topAligned = top - (top % offset);
    int bottomAligned = topAligned + offset;
    int y = (top - topAligned) < (bottomAligned - top) ? topAligned : bottomAligned;

    return QPointF(x, y);
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant& value)
{
    switch (change)
    {
        case ItemPositionChange:
            foreach (Edge* edge, m_edges)
                edge->Adapt();
            emit NodePositionChanged();
            if (m_discreteDisplacementEnabled)
                return (AlignPoint(value.toPointF()));
            break;
        case ItemPositionHasChanged:
            if (m_discreteDisplacementEnabled)
            {
                foreach (Edge* edge, m_edges)
                    edge->Adapt();
            }
            break;
        default:
            break;
    }
    return QGraphicsItem::itemChange(change, value);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem::mousePressEvent(event);
    setZValue(5);
    update();
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    if (isSelected())
        setZValue(4);
    else
        setZValue(3);
    update();
}
