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

#ifndef MULTIROBOTPARSER_H
#define MULTIROBOTPARSER_H

#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QRegExp>

#include "parsers/parser.h"

class TreeWidgetItem;
class QFile;
class Context;

class MultirobotParser : public Parser
{
    Q_OBJECT

public:
    MultirobotParser();
    virtual ~MultirobotParser();
    static QString GetName() { return "Multirobot (*.txt)"; }
    virtual QString Name() const { return MultirobotParser::GetName(); }
    virtual QString Description() const { return "Multirobot Parser"; }

    virtual TreeWidgetItem* ParseFile(QFile& file, const QList<Parser::HeaderItem>& header);
    virtual void ParseGraph(QFile& file, Context& context);

protected:
    QRegExp id;
    QRegExp v;
    QRegExp c;
    QRegExp col;
    QRegExp pos;
    QRegExp vertex;
    QRegExp edge;
    QRegExp circle;
    QRegExp color;
    QRegExp mode;
    QRegExp matrix;
    QRegExp angle;
    QRegExp center;
    QRegExp position;
    QRegExp movement;
    QRegExp length;
};

#endif
