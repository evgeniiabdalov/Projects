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

#ifndef NODE_H
#define NODE_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QPointer>
#include <QtGui/QGraphicsItem>
#include <QtGui/QColor>

class Entity;
class Edge;
class QGraphicsSceneMouseEvent;
class QVariant;
class QRectF;
class QPainterPath;
class QPainter;
class QWidget;
class QStyleOptionGraphicsItem;

class Node : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

signals:
    void NodePositionChanged();

public:
    enum NodeColorType { clBackground = 0, clForeground, clBoundary };
    enum NodeDescription { EntityID = 0, NodeID, NodeIDEntityID, Empty };

    Node(int id, Entity* entity = NULL, double radius = 14.0, Node::NodeDescription description = Node::EntityID);
    Node* CloneShallow();
    int GetID() const;
    Entity* GetEntity() const;
    void SetEntity(Entity* entity);
    double GetRadius() const;
    Node::NodeDescription GetDescription() const;
    void AddEdge(Edge* edge);
    QList<QPointer<Edge> > GetEdges() const;
    QColor GetColor(Node::NodeColorType type) const;
    void AlignPosition();

    enum { Type = UserType + 1 };
    int type() const { return Type; }

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

public slots:
    void SetRadius(double radius);
    void SetDiscreteDisplacementEnabled(bool enable = true);
    void SetDiscreteDisplacementOffset(int offset);
    void SetDescription(Node::NodeDescription description);
    void SetTimeStep(int timestep);
    void SetColor(Node::NodeColorType type, const QColor& color);

protected:
    QPointF AlignPoint(const QPointF& point);
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

private:
    int m_id;
    QPointer<Entity> m_entity;
    double m_radius;
    bool m_discreteDisplacementEnabled;
    int m_discreteDisplacementOffset;
    NodeDescription m_description;
    QList<QPointer<Edge> > m_edges;
    int m_timestep;
    QColor m_clBackground;
    QColor m_clForeground;
    QColor m_clBoundary;
};

#endif
