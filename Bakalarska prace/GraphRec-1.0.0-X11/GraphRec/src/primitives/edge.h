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

#ifndef EDGE_H
#define EDGE_H

#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtCore/QPointF>
#include <QtCore/QRectF>
#include <QtGui/QGraphicsItem>
#include <QtGui/QColor>

class Node;
class GraphView;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;

class Edge : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    enum EdgeColorType{ clLine = 0, clHighlight };

    Edge(Node* source, Node* destination);
    Edge* CloneShallow();
    Node* GetNodeSource() const;
    void SetNodeSource(Node* node);
    Node* GetNodeDestination() const;
    void SetNodeDestination(Node* node);
    void SetHighlight(bool highlight = true);
    QColor GetColor(Edge::EdgeColorType type) const;
    void Adapt();

    enum { Type = UserType + 2 };
    int type() const { return Type; }

    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

public slots:
    void SetColor(Edge::EdgeColorType type, const QColor& color);

private:
    QPointer<Node> m_nodeSource;
    QPointer<Node> m_nodeDestination;
    QPointF m_ptSource;
    QPointF m_ptDestination;
    QColor m_clLine;
    QColor m_clHighlight;
    bool m_highlight;
};

#endif
