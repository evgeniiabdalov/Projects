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

#ifndef PEBBLEVALIDATOR_H
#define PEBBLEVALIDATOR_H

#include <QtCore/QString>
#include <QtGui/QColor>

#include "validators/multirobotvalidator.h"

class Context;
class Node;

class PebbleValidator : public MultirobotValidator
{
    Q_OBJECT

public:
    PebbleValidator();
    virtual ~PebbleValidator();
    static QString GetName() { return "Pebble"; }
    virtual QString Name() const { return PebbleValidator::GetName(); }
    virtual QString Description() const { return "Pebble motion on graph"; }

    virtual void Validate(Context& context);

    virtual QColor GetColor(Validator::ColorScheme type)
    {
        switch (type)
        {
            case Validator::clBackground: return Qt::white;
            case Validator::clBoundary: return Qt::black;
            case Validator::clHighlight: return Qt::cyan;
            case Validator::clNodeBackground: return Qt::yellow;
            case Validator::clNodeForeground: return Qt::black;
            case Validator::clEntityBackground: return Qt::green;
            case Validator::clEntityForeground: return Qt::black;
            case Validator::clEntityFinalBackground: return Qt::blue;
            case Validator::clEntityFinalForeground: return Qt::white;
            default: return Qt::black;
        }
        return Qt::black;
    }

protected:
    virtual bool IsMoveValid(Context& context, MultirobotValidator::CandidatesMap& candidates, int timestep, Node* sourceNode, Node* destinationNode, QString& message);
};

#endif
