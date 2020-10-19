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

#include "validators/pebblevalidator.h"
#include "validators/multirobotvalidator.h"
#include "context.h"
#include "primitives/entity.h"
#include "primitives/node.h"

PebbleValidator::PebbleValidator()
{
}

PebbleValidator::~PebbleValidator()
{
}

void PebbleValidator::Validate(Context& context)
{
    MultirobotValidator::Validate(context);
    context.validatorName = PebbleValidator::GetName();
}

bool PebbleValidator::IsMoveValid(Context& context, MultirobotValidator::CandidatesMap& candidates, int timestep, Node* sourceNode, Node* destinationNode, QString& message)
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
            message = tr("Both nodes are occupied.");
            return false;
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
