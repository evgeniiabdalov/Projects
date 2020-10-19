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

#ifndef GRAPHRECPARSER_H
#define GRAPHRECPARSER_H

#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QSet>

#include "parsers/parser.h"

class TreeWidgetItem;
class QFile;
class QXmlStreamReader;
class Context;

class GraphRecParser : public Parser
{
    Q_OBJECT

public:
    GraphRecParser();
    virtual ~GraphRecParser();
    static QString GetName() { return "GraphRec (*.xml)"; }
    virtual QString Name() const { return GraphRecParser::GetName(); }
    virtual QString Description() const { return "GraphRec Parser"; }

    virtual TreeWidgetItem* ParseFile(QFile& file, const QList<Parser::HeaderItem>& header);
    virtual void ParseGraph(QFile& file, Context& context);

protected:
    struct Stats
    {
        QString id;
        qint64 startLine;
        qint64 startPosition;
        uint entities;
        uint nodes;
        uint edges;
        QSet<uint> timesteps;
        uint moves;
        QString validator;
    };

    void readSolution(QXmlStreamReader& xml, Stats& stats);
    void readGraph(QXmlStreamReader& xml, Stats& stats);
    void readScenario(QXmlStreamReader& xml, Stats& stats);
    void readSolution(QXmlStreamReader& xml, Context& context);
    void readScene(QXmlStreamReader& xml, Context& context);
    void readGraph(QXmlStreamReader& xml, Context& context);
    void readScenario(QXmlStreamReader& xml, Context& context);
    void readUnknown(QXmlStreamReader& xml);
};

#endif
