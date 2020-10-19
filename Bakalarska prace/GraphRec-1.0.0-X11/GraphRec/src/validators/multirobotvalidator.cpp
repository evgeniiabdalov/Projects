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

#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtCore/QPair>
#include <QtCore/QString>
#include <QtCore/QPointer>

#include "validators/multirobotvalidator.h"
#include "context.h"
#include "primitives/entity.h"
#include "primitives/node.h"
#include "primitives/edge.h"

MultirobotValidator::MultirobotValidator()
{
}

MultirobotValidator::~MultirobotValidator()
{
}

void MultirobotValidator::Validate(Context& context)
{
    context.validatorName = MultirobotValidator::GetName();

    // Reset data structures.
    foreach (Entity* entity, context.entities)
    {
        if (entity)
            entity->SetTimeStepFinal(-1);
    }
    foreach (Context::Frame* frame, context.frames)
    {
        delete frame;
    }
    context.frames.clear();
    context.timesteps.clear();

    // Animate all timesteps.
    MultirobotValidator::CandidatesMap candidates;
    MultirobotValidator::NodeBuffer nodeBuffer;
    for (int i = 0; i < context.calendar.size(); ++i)
    {
        if (i == 0 || (i > 0 && (context.calendar[i].timestep != context.calendar[i - 1].timestep)))
        {
            for (int j = 0; j < nodeBuffer.size(); ++j)
            {
                Node* animatedNode = nodeBuffer[j].first;
                Node* destinationNode = nodeBuffer[j].second;
                if (animatedNode && destinationNode)
                    destinationNode->SetEntity(animatedNode->GetEntity());
                delete animatedNode;
            }
            nodeBuffer.clear();

            Context::Frame* frame = new Context::Frame();
            foreach (Node* node, context.nodes)
            {
                if (node)
                    frame->insert(node, node->GetEntity());
            }
            context.frames.append(frame);
            context.timesteps.insert(context.calendar[i].timestep, i);
            candidates.clear();
            FindCandidateMoves(context, candidates, context.calendar[i].timestep);
        }
        Context::Move move = context.calendar[i].move;
        Node* sourceNode = context.nodes[move.first];
        Node* destinationNode = context.nodes[move.second];
        QString errorMessage = "";
        if (IsMoveValid(context, candidates, context.calendar[i].timestep, sourceNode, destinationNode, errorMessage))
        {
            context.calendar[i].valid = true;
            if (sourceNode && destinationNode)
            {
                if (destinationNode->GetEntity() && sourceNode->GetEntity() == NULL)
                {
                    context.calendar[i].reverse = true;
                    Node* temp = destinationNode;
                    destinationNode = sourceNode;
                    sourceNode = temp;
                }
                else
                    context.calendar[i].reverse = false;
                Node* animatedNode = sourceNode->CloneShallow();
                sourceNode->SetEntity(NULL);
                nodeBuffer.append(MultirobotValidator::Animation(animatedNode, destinationNode));
            }
        }
        else
            context.calendar[i].valid = false;

        if (errorMessage.size() > 0)
        {
            sourceNode = context.nodes[move.first];
            destinationNode = context.nodes[move.second];
            if (sourceNode && destinationNode)
            {
                int sourceEntityID = -1;
                if (Entity* entity = sourceNode->GetEntity())
                    sourceEntityID = entity->GetID();
                int destinationEntityID = -1;
                if (Entity* entity = destinationNode->GetEntity())
                    destinationEntityID = entity->GetID();
                emit Error(tr("Timestep %1: Event %2(%3) ---> %4(%5). %6").arg(
                        QString::number(context.calendar[i].timestep), QString::number(sourceNode->GetID()), QString::number(sourceEntityID),
                        QString::number(destinationNode->GetID()), QString::number(destinationEntityID), errorMessage
                           ));
            }
        }
    }
    for (int j = 0; j < nodeBuffer.size(); ++j)
    {
        Node* animatedNode = nodeBuffer[j].first;
        Node* destinationNode = nodeBuffer[j].second;
        if (animatedNode && destinationNode)
            destinationNode->SetEntity(animatedNode->GetEntity());
        delete animatedNode;
    }
    nodeBuffer.clear();

    // Final frame
    Context::Frame* final = new Context::Frame();
    foreach (Node* node, context.nodes)
    {
        if (node)
            final->insert(node, node->GetEntity());
    }
    context.frames.append(final);
    if (!context.calendar.empty())
        context.timesteps.insert(context.calendar[context.calendar.size() - 1].timestep + 1, context.calendar.size());

    // Reset back to first timestep.
    foreach (Node* node, context.nodes)
    {
        if (node)
        {
            Entity* entity = context.frames[0]->value(node);
            node->SetEntity(entity);
        }
    }

    // Determine final timesteps.
    QMap<QPointer<Node>, QPointer<Entity> > finalEntities;
    QList<QPointer<Node> > finalNodes;
    QList<QPointer<Node> > garbage;
    foreach (Node* node, context.nodes)
    {
        Entity* entity = context.frames[context.frames.size() - 1]->value(node);
        if (entity)
        {
            finalNodes.append(node);
            finalEntities.insert(node, entity);
        }
    }
    int i = context.frames.size() - 1;
    while (!finalNodes.empty() && i >= 0)
    {
        foreach (Node* node, finalNodes)
        {
            if (context.frames[i]->value(node) != finalEntities[node] || i == 0)
            {
                if (i == 0)
                    finalEntities[node]->SetTimeStepFinal(i);
                else
                    finalEntities[node]->SetTimeStepFinal(i + 1);
                garbage.append(node);
            }
        }
        foreach (Node* node, garbage)
        {
            finalNodes.removeAt(finalNodes.indexOf(node));
        }
        garbage.clear();

        --i;
    }
}

bool MultirobotValidator::IsMoveValid(Context& context, MultirobotValidator::CandidatesMap& candidates, int timestep, Node* sourceNode, Node* destinationNode, QString& message)
{
    if (sourceNode && destinationNode)
    {
        Context::Frame* frame = context.frames[timestep];
        Entity* sourceEntity = NULL;
        Entity* destinationEntity = NULL;
        if (frame)
        {
            sourceEntity = frame->value(sourceNode);
            destinationEntity = frame->value(destinationNode);
        }
        if (candidates.contains(sourceNode) && candidates[sourceNode] == destinationNode && sourceEntity && destinationEntity == NULL)
        {
            message = "";
            return true;
        }
        else if (candidates.contains(destinationNode) && candidates[destinationNode] == sourceNode && destinationEntity && sourceEntity == NULL)
        {
            message = "";
            return true;
        }
        else if (candidates.contains(sourceNode) && candidates[sourceNode] == destinationNode && sourceEntity && destinationEntity)
        {
            if (m_cycleTemp.contains(sourceNode) && m_cycleTemp[sourceNode])
                return true;
            else
            {
                m_cycleTemp[sourceNode] = true;
                bool result = IsMoveValid(context, candidates, timestep, destinationNode, candidates[destinationNode], message);
                m_cycleTemp[sourceNode] = false;
                return result;
            }
        }
        else if (candidates.contains(sourceNode) && candidates[sourceNode] == destinationNode && sourceEntity == NULL && destinationEntity == NULL)
        {
            message = tr("Both nodes are not occupied.");
            return false;
        }
        else
            return false;
    }
    else
    {
        message = tr("Transition chain is not closed.");
        return false;
    }
    return false;
}

void MultirobotValidator::FindCandidateMoves(Context& context, MultirobotValidator::CandidatesMap& candidates, int timestep)
{
    if (context.frames.size() <= timestep)
        return;
    Context::Frame* frame = context.frames[timestep];
    int i = context.timesteps[timestep];
    while (i < context.calendar.size())
    {
        if (context.calendar[i].timestep == timestep)
        {
            QString errorMessage;
            Node* sourceNode = context.nodes[context.calendar[i].move.first];
            Node* destinationNode = context.nodes[context.calendar[i].move.second];
            if (sourceNode && destinationNode)
            {
                Entity* sourceEntity = frame->value(sourceNode);
                Entity* destinationEntity = frame->value(destinationNode);
                if (sourceNode == destinationNode)
                {
                    errorMessage = tr("Loop move.");
                }
                else if (!context.edges.contains(Context::Ends(sourceNode->GetID(), destinationNode->GetID())) &&
                         !context.edges.contains(Context::Ends(destinationNode->GetID(), sourceNode->GetID())))
                {
                    errorMessage = tr("Edge does not exist.");
                }
                else if (sourceEntity && destinationEntity == NULL)
                {
                    if (candidates.contains(sourceNode))
                        errorMessage = tr("Source node is already used in this timestep.");
                    else if (candidates.values().contains(destinationNode))
                        errorMessage = tr("Destination node is already used in this timestep.");
                    else
                        candidates.insert(sourceNode, destinationNode);
                }
                else if (destinationEntity && sourceEntity == NULL)
                {
                    if (candidates.contains(destinationNode))
                        errorMessage = tr("Destination node is already used in this timestep.");
                    else if (candidates.values().contains(sourceNode))
                        errorMessage = tr("Source node is already used in this timestep.");
                    else
                        candidates.insert(destinationNode, sourceNode);
                }
                else if (sourceEntity && destinationEntity)
                {
                    if (candidates.contains(destinationNode) && candidates[destinationNode] == sourceNode)
                        errorMessage = tr("Inverse move already found.");
                    else if (candidates.contains(sourceNode))
                        errorMessage = tr("Source node is already used in this timestep.");
                    else if (candidates.values().contains(destinationNode))
                        errorMessage = tr("Destination node is already used in this timestep.");
                    else
                        candidates.insert(sourceNode, destinationNode);
                }

                if (errorMessage.size() > 0)
                {
                    int sourceEntityID = -1;
                    if (sourceEntity)
                        sourceEntityID = sourceEntity->GetID();
                    int destinationEntityID = -1;
                    if (destinationEntity)
                        destinationEntityID = destinationEntity->GetID();
                    emit Error(tr("Timestep %1: Event %2(%3) ---> %4(%5). %6").arg(
                            QString::number(timestep), QString::number(sourceNode->GetID()), QString::number(sourceEntityID),
                            QString::number(destinationNode->GetID()), QString::number(destinationEntityID), errorMessage
                               ));
                }
            }
            i++;
        }
        else
            break;
    }
}
