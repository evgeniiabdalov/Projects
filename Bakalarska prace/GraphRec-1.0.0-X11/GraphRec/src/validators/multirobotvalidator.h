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

#ifndef MULTIROBOTVALIDATOR_H
#define MULTIROBOTVALIDATOR_H

#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtCore/QPair>
#include <QtCore/QPointer>
#include <QtGui/QColor>

#include "validators/validator.h"

class Context;
class Node;

class MultirobotValidator : public Validator
{
    Q_OBJECT

public:  
    MultirobotValidator();
    virtual ~MultirobotValidator();
    static QString GetName() { return "Multirobot"; }
    virtual QString Name() const { return MultirobotValidator::GetName(); }
    virtual QString Description() const { return "Multirobot path planning"; }

    virtual void Validate(Context& context);

    virtual QColor GetColor(Validator::ColorScheme type)
    {
        switch (type)
        {
            case Validator::clBackground: return Qt::white;
            case Validator::clBoundary: return Qt::black;
            case Validator::clHighlight: return Qt::cyan;
            case Validator::clNodeBackground: return QColor(255, 170, 0, 255); // orange
            case Validator::clNodeForeground: return Qt::black;
            case Validator::clEntityBackground: return QColor(170, 255, 0, 255); // lime
            case Validator::clEntityForeground: return Qt::black;
            case Validator::clEntityFinalBackground: return QColor(170, 0, 255, 255); // purple
            case Validator::clEntityFinalForeground: return Qt::white;
            default: return Qt::black;
        }
        return Qt::black;
    }

protected:
    typedef QMap<QPointer<Node>, QPointer<Node> > CandidatesMap;
    typedef QPair<QPointer<Node>, QPointer<Node> > Animation; // QPair<animatedNode, destinationNode>
    typedef QList<Animation> NodeBuffer;
    QMap<QPointer<Node>, bool> m_cycleTemp;

    virtual bool IsMoveValid(Context& context, MultirobotValidator::CandidatesMap& candidates, int timestep, Node* sourceNode, Node* destinationNode, QString& message);
    virtual void FindCandidateMoves(Context& context, MultirobotValidator::CandidatesMap& candidates, int timestep);
};

#endif
