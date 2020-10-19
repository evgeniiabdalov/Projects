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

#include <QtCore/QEventLoop>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QList>
#include <QtCore/QSet>
#include <QtCore/QPair>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QXmlStreamReader>
#include <QtGui/QApplication>
#include <QtGui/QProgressDialog>
#include <QtGui/QColor>

#include "primitives/entity.h"
#include "primitives/node.h"
#include "primitives/edge.h"
#include "parsers/parser.h"
#include "parsers/graphrecparser.h"
#include "auxiliary/treewidgetitem.h"
#include "context.h"

GraphRecParser::GraphRecParser()
{
}

GraphRecParser::~GraphRecParser()
{
}

TreeWidgetItem* GraphRecParser::ParseFile(QFile& file, const QList<Parser::HeaderItem>& header)
{
    QFileInfo fileInfo(file);
    TreeWidgetItem* items = new TreeWidgetItem(QStringList(fileInfo.fileName()));
    items->setToolTip(0, fileInfo.canonicalFilePath());
    items->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    QXmlStreamReader xml(&file);
    Stats stats;
    stats.startLine = 1;
    stats.startPosition = 0;
    stats.entities = 0;
    stats.nodes = 0;
    stats.edges = 0;
    stats.moves = 0;

    QProgressDialog progress(fileInfo.fileName(), tr("Cancel"), 0, 100);
    progress.setWindowTitle(tr("Loading..."));
    progress.setWindowFlags(Qt::WindowTitleHint);
    progress.setWindowModality(Qt::ApplicationModal);
    progress.setWindowIcon(QIcon(":/icons/images/graphrec.png"));
    progress.setMinimumDuration(4000);
    qint64 step = file.size() / 100;
    if (step == 0)
        step = 1;

    while (!xml.atEnd())
    {
        xml.readNext();

        if (xml.isStartElement())
        {
            if (xml.name() == "graphrec" && xml.attributes().value("version") == "1.0")
            {
                while (!xml.atEnd())
                {
                    stats.startPosition = xml.characterOffset();
                    xml.readNext();

                    if (xml.isEndElement())
                        break;

                    if (xml.isStartElement())
                    {
                        if (xml.name() == "solution")
                        {
                            readSolution(xml, stats);

                            TreeWidgetItem* item = new TreeWidgetItem();
                            item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
                            for (int i = 0; i < header.size(); i++)
                            {
                                Parser::HeaderItem headerItem = header[i];
                                switch (headerItem)
                                {
                                    case Parser::hLine:
                                        item->setText(i, QString::number(stats.startLine));
                                        item->setTextAlignment(i, Qt::AlignRight);
                                        break;
                                    case Parser::hID:
                                        item->setText(i, stats.id);
                                        item->setTextAlignment(i, Qt::AlignRight);
                                        break;
                                    case Parser::hNodes:
                                        item->setText(i, QString::number(stats.nodes));
                                        item->setTextAlignment(i, Qt::AlignRight);
                                        break;
                                    case Parser::hEdges:
                                        item->setText(i, QString::number(stats.edges));
                                        item->setTextAlignment(i, Qt::AlignRight);
                                        break;
                                    case Parser::hEntities:
                                        item->setText(i, QString::number(stats.entities));
                                        item->setTextAlignment(i, Qt::AlignRight);
                                        break;
                                    case Parser::hTimeSteps:
                                        item->setText(i, QString::number(stats.timesteps.size()));
                                        item->setTextAlignment(i, Qt::AlignRight);
                                        break;
                                    case Parser::hMovements:
                                        item->setText(i, QString::number(stats.moves));
                                        item->setTextAlignment(i, Qt::AlignRight);
                                        break;
                                    case Parser::hFilePosition:
                                        item->setText(i, QString::number(stats.startPosition));
                                        item->setTextAlignment(i, Qt::AlignRight);
                                        break;
                                    case Parser::hParser:
                                        item->setText(i, GraphRecParser::GetName());
                                        item->setTextAlignment(i, Qt::AlignLeft);
                                        break;
                                    case Parser::hValidator:
                                        item->setText(i, stats.validator);
                                        item->setTextAlignment(i, Qt::AlignLeft);
                                        break;
                                    default:
                                        item->setText(i, "");
                                        item->setTextAlignment(i, Qt::AlignLeft);
                                        break;
                                }
                            }
                            items->addChild(item);
                            stats.id.clear();
                            stats.entities = 0;
                            stats.nodes = 0;
                            stats.edges = 0;
                            stats.timesteps.clear();
                            stats.moves = 0;
                            stats.validator.clear();
                            qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
                            progress.setValue(file.pos() / step);
                            if (progress.wasCanceled())
                                break;
                        }
                    }
                }
            }
        }
    }

    progress.setValue(progress.maximum());
    return items;
}

void GraphRecParser::ParseGraph(QFile& file, Context& context)
{
    QXmlStreamReader xml(&file);

    while (!xml.atEnd() && context.filePosition >= xml.characterOffset())
        xml.readNext();
    if (xml.isStartElement())
    {
        if (xml.name() == "solution")
            readSolution(xml, context);
    }
}

void GraphRecParser::readSolution(QXmlStreamReader& xml, Stats& stats)
{
    if (xml.name() == "solution")
    {
        stats.startLine = xml.lineNumber();

        if (xml.attributes().hasAttribute("id"))
            stats.id = xml.attributes().value("id").toString();
        else
            stats.id = tr("-");

        while (!xml.atEnd())
        {
            xml.readNext();

            if (xml.isEndElement())
                break;

            if (xml.isStartElement())
            {
                if (xml.name() == "graph")
                    readGraph(xml, stats);
                else if (xml.name() == "scenario")
                    readScenario(xml, stats);
                else
                    readUnknown(xml);
            }
        }
    }
}

void GraphRecParser::readGraph(QXmlStreamReader& xml, Stats& stats)
{
    if (xml.name() == "graph")
    {
        while (!xml.atEnd())
        {
            xml.readNext();

            if (xml.isEndElement())
                break;

            if (xml.isStartElement())
            {
                if (xml.name() == "entity")
                    stats.entities++;
                else if (xml.name() == "node")
                    stats.nodes++;
                else if (xml.name() == "edge")
                    stats.edges++;
                readUnknown(xml);
            }
        }
    }
}

void GraphRecParser::readScenario(QXmlStreamReader& xml, Stats& stats)
{
    if (xml.name() == "scenario")
    {
        if (xml.attributes().hasAttribute("validator"))
            stats.validator = xml.attributes().value("validator").toString();
        else
            stats.validator.clear();

        while (!xml.atEnd())
        {
            xml.readNext();

            if (xml.isEndElement())
                break;

            if (xml.isStartElement())
            {
                if (xml.name() == "move")
                {
                    stats.moves++;
                    if (xml.attributes().hasAttribute("tms"))
                        stats.timesteps.insert(xml.attributes().value("tms").toString().toInt());
                }
                readUnknown(xml);
            }
        }
    }
}

void GraphRecParser::readSolution(QXmlStreamReader& xml, Context& context)
{
    if (xml.name() == "solution")
    {
        if (xml.attributes().hasAttribute("id"))
            context.graphName = tr("ID: %1").arg(xml.attributes().value("id").toString());
        else
            context.graphName = tr("ID: -");

        while (!xml.atEnd())
        {
            xml.readNext();

            if (xml.isEndElement())
                break;

            if (xml.isStartElement())
            {
                if (xml.name() == "scene")
                    readScene(xml, context);
                else if (xml.name() == "graph")
                    readGraph(xml, context);
                else if (xml.name() == "scenario")
                    readScenario(xml, context);
                else
                    readUnknown(xml);
            }
        }
    }
}

void GraphRecParser::readScene(QXmlStreamReader& xml, Context& context)
{
    if (xml.name() == "scene")
    {
        if (xml.attributes().hasAttribute("bg"))
            context.sceneBackground = QColor(xml.attributes().value("bg").toString());

        while (!xml.atEnd())
        {
            xml.readNext();

            if (xml.isEndElement())
                break;

            if (xml.isStartElement())
            {
                if (xml.name() == "viewport")
                {
                    if (xml.attributes().hasAttribute("x") && xml.attributes().hasAttribute("y"))
                        context.sceneViewCenter = QPointF(xml.attributes().value("x").toString().toDouble(), xml.attributes().value("y").toString().toDouble());
                }
                else if (xml.name() == "matrix")
                {
                    if (xml.attributes().hasAttribute("m11") && xml.attributes().hasAttribute("m12") &&
                            xml.attributes().hasAttribute("m21") && xml.attributes().hasAttribute("m22") &&
                            xml.attributes().hasAttribute("dx") && xml.attributes().hasAttribute("dy"))
                        context.sceneMatrix = QMatrix(xml.attributes().value("m11").toString().toDouble(),
                                                      xml.attributes().value("m12").toString().toDouble(),
                                                      xml.attributes().value("m21").toString().toDouble(),
                                                      xml.attributes().value("m22").toString().toDouble(),
                                                      xml.attributes().value("dx").toString().toDouble(),
                                                      xml.attributes().value("dy").toString().toDouble());
                }
                readUnknown(xml);
            }
        }
    }
}

void GraphRecParser::readGraph(QXmlStreamReader& xml, Context& context)
{
    if (xml.name() == "graph")
    {
        while (!xml.atEnd())
        {
            xml.readNext();

            if (xml.isEndElement())
                break;

            if (xml.isStartElement())
            {
                if (xml.name() == "entity")
                {
                    if (xml.attributes().hasAttribute("id") && xml.attributes().value("id").toString().toInt() > 0)
                    {
                        Entity* entity = new Entity(xml.attributes().value("id").toString().toInt());
                        context.entities.insert(entity->GetID(), entity);
                        if (xml.attributes().hasAttribute("bg"))
                        {
                            entity->SetColor(Entity::clBackground, QColor(xml.attributes().value("bg").toString()));
                            context.enabledColoring = false;
                        }
                        if (xml.attributes().hasAttribute("bgf"))
                        {
                            entity->SetColor(Entity::clBackgroundFinal, QColor(xml.attributes().value("bgf").toString()));
                            context.enabledColoring = false;
                        }
                        if (xml.attributes().hasAttribute("fg"))
                        {
                            entity->SetColor(Entity::clForeground, QColor(xml.attributes().value("fg").toString()));
                            context.enabledColoring = false;
                        }
                        if (xml.attributes().hasAttribute("fgf"))
                        {
                            entity->SetColor(Entity::clForegroundFinal, QColor(xml.attributes().value("fgf").toString()));
                            context.enabledColoring = false;
                        }
                    }
                }
                else if (xml.name() == "node")
                {
                    if (xml.attributes().hasAttribute("id"))
                    {
                        Entity* entity = NULL;
                        if (xml.attributes().hasAttribute("ent"))
                            entity = context.entities[xml.attributes().value("ent").toString().toInt()];
                        Node* node = new Node(xml.attributes().value("id").toString().toInt(), entity);
                        context.nodes.insert(node->GetID(), node);
                        if (xml.attributes().hasAttribute("x") && xml.attributes().hasAttribute("y"))
                        {
                            node->setPos(xml.attributes().value("x").toString().toDouble(), xml.attributes().value("y").toString().toDouble());
                            context.enabledLayouting = false;
                        }
                        if (xml.attributes().hasAttribute("bg"))
                        {
                            node->SetColor(Node::clBackground, QColor(xml.attributes().value("bg").toString()));
                            context.enabledColoring = false;
                        }
                        if (xml.attributes().hasAttribute("fg"))
                        {
                            node->SetColor(Node::clForeground, QColor(xml.attributes().value("fg").toString()));
                            context.enabledColoring = false;
                        }
                        if (xml.attributes().hasAttribute("bnd"))
                        {
                            node->SetColor(Node::clBoundary, QColor(xml.attributes().value("bnd").toString()));
                            context.enabledColoring = false;
                        }
                    }
                }
                else if (xml.name() == "edge")
                {
                    if (xml.attributes().hasAttribute("n1") && xml.attributes().hasAttribute("n2"))
                    {
                        int sourceNodeID = xml.attributes().value("n1").toString().toInt();
                        int destinationNodeID = xml.attributes().value("n2").toString().toInt();
                        Node* source = context.nodes[sourceNodeID];
                        Node* destination = context.nodes[destinationNodeID];
                        if (source && destination)
                        {
                            Edge* edge = new Edge(source, destination);
                            context.edges.insert(Context::Ends(sourceNodeID, destinationNodeID), edge);
                            if (xml.attributes().hasAttribute("ln"))
                            {
                                edge->SetColor(Edge::clLine, QColor(xml.attributes().value("ln").toString()));
                                context.enabledColoring = false;
                            }
                            if (xml.attributes().hasAttribute("hgl"))
                            {
                                edge->SetColor(Edge::clHighlight, QColor(xml.attributes().value("hgl").toString()));
                                context.enabledColoring = false;
                            }
                        }
                        else
                            emit Error(tr("Edge {%1, %2} cannot be added. Missing node.").arg(xml.attributes().value("n1").toString()).arg(xml.attributes().value("n2").toString()));
                    }
                }
                readUnknown(xml);
            }
        }
    }
}

void GraphRecParser::readScenario(QXmlStreamReader& xml, Context& context)
{
    if (xml.name() == "scenario")
    {
        while (!xml.atEnd())
        {
            xml.readNext();

            if (xml.isEndElement())
                break;

            if (xml.isStartElement())
            {
                if (xml.name() == "move")
                {
                    if (xml.attributes().hasAttribute("tms") && xml.attributes().hasAttribute("src") && xml.attributes().hasAttribute("dst"))
                    {
                        Node* source = context.nodes[xml.attributes().value("src").toString().toInt()];
                        Node* destination = context.nodes[xml.attributes().value("dst").toString().toInt()];
                        if (source && destination)
                        {
                            Context::CalendarEvent event;
                            event.move = Context::Move(source->GetID(), destination->GetID());
                            event.valid = false;
                            event.reverse = false;
                            event.timestep = xml.attributes().value("tms").toString().toInt();
                            context.calendar.append(event);
                        }
                        else
                            emit Error(tr("Event %1 ---> %2 (%3) cannot be added. Missing node.").arg(xml.attributes().value("src").toString().toInt()).arg(xml.attributes().value("dst").toString().toInt()).arg(xml.attributes().value("tms").toString().toInt()));
                    }
                }
                readUnknown(xml);
            }
        }

        context.SortCalendar();
    }
}

void GraphRecParser::readUnknown(QXmlStreamReader& xml)
{
    while (!xml.atEnd())
    {
        xml.readNext();

        if (xml.isEndElement())
            break;

        if (xml.isStartElement())
            readUnknown(xml);
     }
}
