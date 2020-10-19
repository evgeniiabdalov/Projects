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
#include <QtCore/QFile>
#include <QtCore/QXmlStreamWriter>
#include <QtCore/QRegExp>

#include "savers/graphrecsaver.h"
#include "context.h"
#include "primitives/entity.h"
#include "primitives/node.h"
#include "primitives/edge.h"

GraphRecSaver::GraphRecSaver()
{
}

GraphRecSaver::~GraphRecSaver()
{
}

void GraphRecSaver::Open(QFile& file)
{
    xml.setDevice(&file);
    xml.setAutoFormatting(true);

    // Header
    xml.writeStartDocument();
    xml.writeDTD("<!DOCTYPE graphrec>");
    xml.writeStartElement("graphrec");
    xml.writeAttribute("version", "1.0");
}

void GraphRecSaver::Close()
{
    // Footer
    xml.writeEndElement();
    xml.writeEndDocument();
}

void GraphRecSaver::Save(Context& context)
{
    // Solution
    xml.writeStartElement("solution");
    QRegExp id("ID:\\s([0-9]+)\\s*$", Qt::CaseInsensitive);
    if (id.exactMatch(context.graphName))
        xml.writeAttribute("id", id.cap(1));

    // Scene
    xml.writeStartElement("scene");
        xml.writeAttribute("bg", context.sceneBackground.name());
        xml.writeEmptyElement("viewport");
            xml.writeAttribute("x", QString::number(context.sceneViewCenter.x()));
            xml.writeAttribute("y", QString::number(context.sceneViewCenter.y()));
        xml.writeEmptyElement("matrix");
            xml.writeAttribute("m11", QString::number(context.sceneMatrix.m11()));
            xml.writeAttribute("m12", QString::number(context.sceneMatrix.m12()));
            xml.writeAttribute("m21", QString::number(context.sceneMatrix.m21()));
            xml.writeAttribute("m22", QString::number(context.sceneMatrix.m22()));
            xml.writeAttribute("dx", QString::number(context.sceneMatrix.dx()));
            xml.writeAttribute("dy", QString::number(context.sceneMatrix.dy()));
    xml.writeEndElement(); // scene

    // Graph
    xml.writeStartElement("graph");

        // Entities
        foreach (Entity* entity, context.entities)
        {
            if (entity)
            {
                xml.writeEmptyElement("entity");
                xml.writeAttribute("id", QString::number(entity->GetID()));
                xml.writeAttribute("bg", entity->GetColor(Entity::clBackground).name());
                xml.writeAttribute("bgf", entity->GetColor(Entity::clBackgroundFinal).name());
                xml.writeAttribute("fg", entity->GetColor(Entity::clForeground).name());
                xml.writeAttribute("fgf", entity->GetColor(Entity::clForegroundFinal).name());
            }
        }

        // Nodes
        Context::Frame* frame = context.frames[0];
        foreach (Node* node, context.nodes)
        {
            if (node)
            {
                xml.writeEmptyElement("node");
                xml.writeAttribute("id", QString::number(node->GetID()));
                if (frame)
                {
                    if (Entity* entity = frame->value(node))
                        xml.writeAttribute("ent", QString::number(entity->GetID()));
                }
                xml.writeAttribute("x", QString::number(node->pos().x()));
                xml.writeAttribute("y", QString::number(node->pos().y()));
                xml.writeAttribute("bg", node->GetColor(Node::clBackground).name());
                xml.writeAttribute("fg", node->GetColor(Node::clForeground).name());
                xml.writeAttribute("bnd", node->GetColor(Node::clBoundary).name());
            }
        }

        // Edges
        foreach (Edge* edge, context.edges)
        {
            if (edge)
            {
                Node* sourceNode = edge->GetNodeSource();
                Node* destinationNode = edge->GetNodeDestination();
                if (sourceNode && destinationNode)
                {
                    xml.writeEmptyElement("edge");
                    xml.writeAttribute("n1", QString::number(sourceNode->GetID()));
                    xml.writeAttribute("n2", QString::number(destinationNode->GetID()));
                    xml.writeAttribute("ln", edge->GetColor(Edge::clLine).name());
                    xml.writeAttribute("hgl", edge->GetColor(Edge::clHighlight).name());
                }
            }
        }

    xml.writeEndElement(); // graph

    // Scenario
    xml.writeStartElement("scenario");
        xml.writeAttribute("validator", context.validatorName);
        for (int i = 0; i < context.calendar.size(); ++i)
        {
            Context::CalendarEvent event = context.calendar[i];
            xml.writeEmptyElement("move");
            xml.writeAttribute("tms", QString::number(event.timestep));
            xml.writeAttribute("src", QString::number(event.move.first));
            xml.writeAttribute("dst", QString::number(event.move.second));
        }
    xml.writeEndElement(); // scenario

    xml.writeEndElement(); // solution
}
