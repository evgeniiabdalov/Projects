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
#include <QtCore/QRegExp>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtGui/QApplication>
#include <QtGui/QProgressDialog>
#include <QtGui/QColor>

#include "primitives/entity.h"
#include "primitives/node.h"
#include "primitives/edge.h"
#include "parsers/parser.h"
#include "parsers/multirobotparser.h"
#include "validators/multirobotvalidator.h"
#include "validators/pebblevalidator.h"
#include "auxiliary/treewidgetitem.h"
#include "context.h"

MultirobotParser::MultirobotParser()
{
    id = QRegExp("id:([-0-9]+)\\s*$", Qt::CaseInsensitive); // id:5
    v = QRegExp("^V =\\s*$", Qt::CaseInsensitive); // V =
    c = QRegExp("^C =\\s*$", Qt::CaseInsensitive); // C =
    col = QRegExp("^COL =\\s*$", Qt::CaseInsensitive); // COL =
    pos = QRegExp("^POS =\\s*$", Qt::CaseInsensitive); // POS =
    vertex = QRegExp("^\\(([-0-9]+):[-0-9]+\\)\\[([-0-9]+):[-0-9]+:[-0-9]+\\][-\\s0-9]*$", Qt::CaseInsensitive); // (53:12)[40:12:12] 14 52
    edge = QRegExp("^\\{([-0-9]+),([-0-9]+)\\}\\s+\\([-0-9]+\\)\\s*$", Qt::CaseInsensitive); // {0,1} (0)
    circle = QRegExp("^([-0-9]+)\\s\\(([-0-9]+),([-0-9]+)\\):[-\\s0-9]*\\[([-\\s0-9]+)\\]\\s\\{[-\\s0-9]*\\}\\s*$"); // 12 (1,14): 14 15 0 1 52 53  [52 53 ] {14 15 0 1 }
    color = QRegExp("^([BP])_([SBHEIF][CRLMNI][NDTPH]):([-A0-9]+):(#[0-9a-f]+)\\s*$", Qt::CaseInsensitive); // B_SCN:A:#ffffff
    mode = QRegExp("^M:([a-z]+)\\s*$", Qt::CaseInsensitive); // M:IMMEDIATE | M:TRANSITIVE
    matrix = QRegExp("^MATRIX:([-.0-9]+):([-.0-9]+):([-.0-9]+):([-.0-9]+):([-.0-9]+):([-.0-9]+)\\s*$", Qt::CaseInsensitive); // MATRIX:1.00:1.00:1.00:1.00:0.00:0.00
    angle = QRegExp("^ANGLE:([-.0-9]+)\\s*$", Qt::CaseInsensitive); // ANGLE:0.00
    center = QRegExp("^CENTER:X([-.0-9]+):Y([-.0-9]+)\\s*$", Qt::CaseInsensitive); // CENTER:X0.00,Y0.00
    position = QRegExp("^([-0-9]+):X([-.0-9]+):Y([-.0-9]+)\\s*$", Qt::CaseInsensitive); // 0:X-1053.64:Y80.1456
    movement = QRegExp("^([-0-9]+)\\s+-+>\\s+([-0-9]+)\\s+[\\s\\[\\]-0-9>]*\\(([-0-9]+)\\)\\s*$", Qt::CaseInsensitive); // 15 ---> 32 [24 ---> -17] (5)
    length = QRegExp("^Length:([-0-9]+)\\s*$", Qt::CaseInsensitive); // Length:1654
}

MultirobotParser::~MultirobotParser()
{
}

TreeWidgetItem* MultirobotParser::ParseFile(QFile& file, const QList<Parser::HeaderItem>& header)
{
    QFileInfo fileInfo(file);
    TreeWidgetItem* items = new TreeWidgetItem(QStringList(fileInfo.fileName()));
    items->setToolTip(0, fileInfo.canonicalFilePath());
    items->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    QString line;
    QString name;
    ulong startLine = 0;
    qint64 start = 0;
    ulong lineNumber = 1;
    uint vertices = 0;
    uint entities = 0;
    uint edges = 0;
    QSet<uint> timesteps;
    uint movements = 0;
    QString validator;

    QProgressDialog progress(fileInfo.fileName(), tr("Cancel"), 0, 100);
    progress.setWindowTitle(tr("Loading..."));
    progress.setWindowFlags(Qt::WindowTitleHint);
    progress.setWindowModality(Qt::ApplicationModal);
    progress.setWindowIcon(QIcon(":/icons/images/graphrec.png"));
    progress.setMinimumDuration(4000);
    qint64 step = file.size() / 100;
    if (step == 0)
        step = 1;

    bool newGraph = true;
    bool isID = false;
    bool isV = false;
    bool ignoreNextV = false;

    while (!file.atEnd())
    {
        qint64 position = file.pos();
        line = file.readLine();

        if ((isID = id.exactMatch(line)) || (isV = v.exactMatch(line)) || length.exactMatch(line))
        {
            if (isV && ignoreNextV)
            {
                ignoreNextV = false;
                continue;
            }
            ignoreNextV = false;

            if (!newGraph)
            {
                TreeWidgetItem* item = new TreeWidgetItem();
                item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
                for (int i = 0; i < header.size(); i++)
                {
                    Parser::HeaderItem headerItem = header[i];
                    switch (headerItem)
                    {
                        case Parser::hLine:
                            item->setText(i, QString::number(startLine));
                            item->setTextAlignment(i, Qt::AlignRight);
                            break;
                        case Parser::hID:
                            item->setText(i, name);
                            item->setTextAlignment(i, Qt::AlignRight);
                            break;
                        case Parser::hNodes:
                            item->setText(i, QString::number(vertices));
                            item->setTextAlignment(i, Qt::AlignRight);
                            break;
                        case Parser::hEdges:
                            item->setText(i, QString::number(edges));
                            item->setTextAlignment(i, Qt::AlignRight);
                            break;
                        case Parser::hEntities:
                            item->setText(i, QString::number(entities));
                            item->setTextAlignment(i, Qt::AlignRight);
                            break;
                        case Parser::hTimeSteps:
                            item->setText(i, QString::number(timesteps.size()));
                            item->setTextAlignment(i, Qt::AlignRight);
                            break;
                        case Parser::hMovements:
                            item->setText(i, QString::number(movements));
                            item->setTextAlignment(i, Qt::AlignRight);
                            break;
                        case Parser::hFilePosition:
                            item->setText(i, QString::number(start));
                            item->setTextAlignment(i, Qt::AlignRight);
                            break;
                        case Parser::hParser:
                            item->setText(i, MultirobotParser::GetName());
                            item->setTextAlignment(i, Qt::AlignLeft);
                            break;
                        case Parser::hValidator:
                            item->setText(i, validator);
                            item->setTextAlignment(i, Qt::AlignLeft);
                            break;
                        default:
                            item->setText(i, "");
                            item->setTextAlignment(i, Qt::AlignLeft);
                            break;
                    }
                }
                items->addChild(item);
                vertices = 0;
                edges = 0;
                entities = 0;
                timesteps.clear();
                movements = 0;
                validator.clear();
                newGraph = true;
                qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
                progress.setValue(position / step);
                if (progress.wasCanceled())
                    break;
            }
            if (newGraph  && (isID || isV))
            {
                newGraph = false;
                startLine = lineNumber;
                if (isID)
                {
                    name = id.cap(1);
                    ignoreNextV = true;
                }
                else
                    name = tr("-");
                start = position;
            }
        }
        else if (mode.exactMatch(line))
        {
            if (mode.cap(1) == "IMMEDIATE")
                validator = PebbleValidator::GetName();
            else if (mode.cap(1) == "TRANSITIVE")
                validator = MultirobotValidator::GetName();
            else
                validator.clear();
        }
        else if (vertex.exactMatch(line))
        {
            vertices++;
            if (vertex.cap(2).toInt() > 0)
                entities++;
        }
        else if (edge.exactMatch(line))
        {
            edges++;
        }
        else if (movement.exactMatch(line))
        {
            movements++;
            timesteps.insert(movement.cap(3).toInt());
        }
        lineNumber++;
    }

    progress.setValue(progress.maximum());
    return items;
}

void MultirobotParser::ParseGraph(QFile& file, Context& context)
{
    file.seek(context.filePosition);
    QString line;

    bool newGraph = true;
    bool isID = false;
    bool isV = false;
    bool ignoreNextV = false;
    bool positions = false;
    double x = 0;
    double y = 0;
    double dx = 40.0;
    double dy = 60.0;
    while (!file.atEnd())
    {
        line = file.readLine();

        if ((isID = id.exactMatch(line)) || (isV = v.exactMatch(line)) || length.exactMatch(line))
        {
            if (isV && ignoreNextV)
            {
                ignoreNextV = false;
                continue;
            }
            ignoreNextV = false;

            if (!newGraph)
            {
                context.SortCalendar();
                newGraph = true;
                break;
            }

            if (newGraph && (isID || isV))
            {
                newGraph = false;
                if (isID)
                {
                    context.graphName = tr("ID: %1").arg(id.cap(1));
                    ignoreNextV = true;
                }
                else
                    context.graphName = tr("ID: -");
            }
        }
        else if (vertex.exactMatch(line))
        {
            int nodeID = vertex.cap(1).toInt();
            int entityID = vertex.cap(2).toInt();
            Entity* entity = NULL;
            if (entityID > 0)
            {
                entity = new Entity(entityID);
                context.entities.insert(entityID, entity);
            }
            Node* node = new Node(nodeID, entity);
            context.nodes.insert(nodeID, node);
        }
        else if (edge.exactMatch(line))
        {
            int sourceNodeID = edge.cap(1).toInt();
            int destinationNodeID = edge.cap(2).toInt();
            Node* source = context.nodes[sourceNodeID];
            Node* destination = context.nodes[destinationNodeID];
            if (source && destination)
                context.edges.insert(Context::Ends(sourceNodeID, destinationNodeID), new Edge(source, destination));
            else
                emit Error(tr("Edge {%1, %2} cannot be added. Missing node.").arg(edge.cap(1)).arg(edge.cap(2)));
        }
        else if (circle.exactMatch(line) && !positions)
        {
            if (circle.cap(1) == "0")
            {
                Node* node = context.nodes[circle.cap(2).toInt()];
                if (node)
                    node->setPos(x, y);
                x += dx;
                node = context.nodes[circle.cap(3).toInt()];
                if (node)
                    node->setPos(x, y);
                y += dy;
                x -= dx;
            }
            else
            {
                Node* node = context.nodes[circle.cap(2).toInt()];
                if (node)
                    x = node->pos().x();
                y += dy;
            }
            QStringList list = circle.cap(4).split(" ", QString::SkipEmptyParts);
            foreach (QString string, list)
            {
                Node* node = context.nodes[string.toInt()];
                if (node)
                {
                    node->setPos(x, y);
                    x += dx;
                }
            }
        }
        else if (color.exactMatch(line))
        {
            if (color.cap(2) == "SCN")
                context.sceneBackground = QColor(color.cap(4));
            else if (color.cap(2) == "BRD")
            {
                foreach (Node* node, context.nodes)
                {
                    if (node)
                        node->SetColor(Node::clBoundary, QColor(color.cap(4)));
                }
                foreach (Edge* edge, context.edges)
                {
                    if (edge)
                        edge->SetColor(Edge::clLine, QColor(color.cap(4)));
                }
            }
            else if (color.cap(2) == "HLT")
            {
                foreach (Edge* edge, context.edges)
                {
                    if (edge)
                        edge->SetColor(Edge::clHighlight, QColor(color.cap(4)));
                }
            }
            else if (color.cap(2) == "EMP")
            {
                Node* node = context.nodes[color.cap(3).toInt()];
                if (node && color.cap(1) == "B")
                    node->SetColor(Node::clBackground, QColor(color.cap(4)));
                else if (node && color.cap(1) == "P")
                    node->SetColor(Node::clForeground, QColor(color.cap(4)));
            }
            else if (color.cap(2) == "INH")
            {
                Entity* entity = context.entities[color.cap(3).toInt()];
                if (entity && color.cap(1) == "B")
                    entity->SetColor(Entity::clBackground, QColor(color.cap(4)));
                else if (entity && color.cap(1) == "P")
                    entity->SetColor(Entity::clForeground, QColor(color.cap(4)));
            }
            else if (color.cap(2) == "FIN")
            {
                Entity* entity = context.entities[color.cap(3).toInt()];
                if (entity && color.cap(1) == "B")
                    entity->SetColor(Entity::clBackgroundFinal, QColor(color.cap(4)));
                else if (entity && color.cap(1) == "P")
                    entity->SetColor(Entity::clForegroundFinal, QColor(color.cap(4)));
            }
        }
        else if (c.exactMatch(line))
        {
            context.enabledLayouting = false;
        }
        else if (col.exactMatch(line))
        {
            context.enabledColoring = false;
        }
        else if (pos.exactMatch(line))
        {
            positions = true;
            context.enabledLayouting = false;
        }
        else if (matrix.exactMatch(line))
        {
            context.sceneMatrix = QMatrix(matrix.cap(1).toDouble(), matrix.cap(2).toDouble(), matrix.cap(3).toDouble(), matrix.cap(4).toDouble(), matrix.cap(5).toDouble(), matrix.cap(6).toDouble());
        }
        else if (angle.exactMatch(line))
        {
            context.sceneAngle = angle.cap(1).toDouble();
        }
        else if (center.exactMatch(line))
        {
            context.sceneViewCenter = QPointF(center.cap(1).toDouble(), center.cap(2).toDouble());
        }
        else if (position.exactMatch(line))
        {
            Node* node = context.nodes[position.cap(1).toInt()];
            if (node)
                node->setPos(position.cap(2).toDouble(), position.cap(3).toDouble());
        }
        else if (movement.exactMatch(line))
        {
            Node* source = context.nodes[movement.cap(1).toInt()];
            Node* destination = context.nodes[movement.cap(2).toInt()];
            if (source && destination)
            {
                Context::CalendarEvent event;
                event.move = Context::Move(movement.cap(1).toInt(), movement.cap(2).toInt());
                event.valid = false;
                event.reverse = false;
                event.timestep = movement.cap(3).toInt();
                context.calendar.append(event);
            }
            else
                emit Error(tr("Event %1 ---> %2 (%3) cannot be added. Missing node.").arg(movement.cap(1)).arg(movement.cap(2)).arg(movement.cap(3)));
        }
        else
            continue;
    }
}
