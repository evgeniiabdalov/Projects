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

#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <QtGui/QColor>

#include "servant.h"

class QString;
class Context;

class Validator : public Servant
{
    Q_OBJECT

signals:
    void Error(const QString& message);

public:
    enum ColorScheme
    {
        clBackground = 0,
        clBoundary,
        clHighlight,
        clNodeBackground,
        clNodeForeground,
        clEntityBackground,
        clEntityForeground,
        clEntityFinalBackground,
        clEntityFinalForeground
    };

    Validator() { }
    virtual ~Validator() { }

    virtual void Validate(Context& context) = 0;

    virtual QColor GetColor(Validator::ColorScheme type)
    {
        switch (type)
        {
            case Validator::clBackground: return Qt::white;
            case Validator::clBoundary: return Qt::black;
            case Validator::clHighlight: return Qt::cyan;
            case Validator::clNodeBackground: return Qt::darkGray;
            case Validator::clNodeForeground: return Qt::black;
            case Validator::clEntityBackground: return Qt::green;
            case Validator::clEntityForeground: return Qt::black;
            case Validator::clEntityFinalBackground: return Qt::darkGreen;
            case Validator::clEntityFinalForeground: return Qt::black;
            default: return Qt::black;
        }
        return Qt::black;
    }
};

#endif
