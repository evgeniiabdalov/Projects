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
#include <QtCore/QTextStream>
#include <QtCore/QRegExp>

#include "savers/multirobotsaver.h"
#include "context.h"
#include "primitives/entity.h"
#include "primitives/node.h"
#include "primitives/edge.h"
#include "validators/multirobotvalidator.h"
#include "validators/pebblevalidator.h"

MultirobotSaver::MultirobotSaver()
{
}

MultirobotSaver::~MultirobotSaver()
{
}

void MultirobotSaver::Open(QFile& file)
{
    out.setDevice(&file);
}

void MultirobotSaver::Close()
{
    out.flush();
}

void MultirobotSaver::Save(Context& context)
{
    // ID
    QRegExp idLabel("ID:\\s([-0-9]+)\\s*$", Qt::CaseInsensitive); // ID: 5
    if (idLabel.exactMatch(context.graphName))
        out << "id:" << idLabel.cap(1) << endl;
    else
        out << "id:-" << endl;

    // Vertices
    out << "V =" << endl;
    Context::Frame* frame = context.frames[0];
    foreach (Node* node, context.nodes)
    {
        if (node && frame)
        {
            if (Entity* entity = frame->value(node))
                out << "(" << node->GetID() << ":0)[" << entity->GetID() << ":0:0]" << endl;
            else
                out << "(" << node->GetID() << ":0)[-1:0:0]" << endl;
        }
    }

    // Edges
    out << "E =" << endl;
    foreach (Edge* edge, context.edges)
    {
        if (edge)
        {
            Node* sourceNode = edge->GetNodeSource();
            Node* destinationNode = edge->GetNodeDestination();
            if (sourceNode && destinationNode)
                out << "{" << sourceNode->GetID() << "," << destinationNode->GetID() << "} (0)" << endl;
        }
    }

    // Mode
    if (context.validatorName == MultirobotValidator::GetName())
        out << "MOD =" << endl << "M:TRANSITIVE" << endl;
    else if (context.validatorName == PebbleValidator::GetName())
        out << "MOD =" << endl << "M:IMMEDIATE" << endl;

    // Colors
    out << "COL =" << endl;
    out << "B_SCN:A:" << context.sceneBackground.name() << endl;
    if (context.edges.begin() != context.edges.end())
    {
        Edge* edge = context.edges.begin().value();
        if (edge)
        {
            out << "P_BRD:A:" << edge->GetColor(Edge::clLine).name() << endl;
            out << "P_HLT:A:" << edge->GetColor(Edge::clHighlight).name() << endl;
        }
    }
    foreach (Node* node, context.nodes)
    {
        if (node)
            out << "B_EMP:" << node->GetID() << ":" << node->GetColor(Node::clBackground).name() << endl;
    }
    foreach (Node* node, context.nodes)
    {
        if (node)
            out << "P_EMP:" << node->GetID() << ":" << node->GetColor(Node::clForeground).name() << endl;
    }
    foreach (Entity* entity, context.entities)
    {
        if (entity)
            out << "B_INH:" << entity->GetID() << ":" << entity->GetColor(Entity::clBackground).name() << endl;
    }
    foreach (Entity* entity, context.entities)
    {
        if (entity)
            out << "P_INH:" << entity->GetID() << ":" << entity->GetColor(Entity::clForeground).name() << endl;
    }
    foreach (Entity* entity, context.entities)
    {
        if (entity)
            out << "B_FIN:" << entity->GetID() << ":" << entity->GetColor(Entity::clBackgroundFinal).name() << endl;
    }
    foreach (Entity* entity, context.entities)
    {
        if (entity)
            out << "P_FIN:" << entity->GetID() << ":" << entity->GetColor(Entity::clForegroundFinal).name() << endl;
    }

    // Positions
    out << "POS =" << endl;
    out << "MATRIX" << ":" << context.sceneMatrix.m11() << ":" << context.sceneMatrix.m12()
                    << ":" << context.sceneMatrix.m21() << ":" << context.sceneMatrix.m22()
                    << ":" << context.sceneMatrix.dx() << ":" << context.sceneMatrix.dy() << endl;
    out << "ANGLE:" << context.sceneAngle << endl;
    out << "CENTER:X" << context.sceneViewCenter.x() << ":Y" << context.sceneViewCenter.y() << endl;
    foreach (Node* node, context.nodes)
    {
        if (node)
            out << node->GetID() << ":X" << node->pos().x() << ":Y" << node->pos().y() << endl;
    }

    // Solution
    out << "Solution" << endl;
    for (int i = 0; i < context.calendar.size(); ++i)
    {
        Context::CalendarEvent event = context.calendar[i];
        out << event.move.first << " ---> " << event.move.second << " [0 ---> 0] (" << event.timestep << ")" << endl;
    }
    out << "Length:" << context.calendar.size() << endl;
}
