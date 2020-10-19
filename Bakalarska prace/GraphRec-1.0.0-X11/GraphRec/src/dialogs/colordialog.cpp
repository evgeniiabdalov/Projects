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

#include <QtCore/QSignalMapper>
#include <QtCore/QEvent>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QPoint>
#include <QtCore/QPointF>
#include <QtGui/QWidget>
#include <QtGui/QColorDialog>
#include <QtGui/QColor>
#include <QtGui/QMenu>
#include <QtGui/QAction>
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QAbstractItemView>

#include "dialogs/colordialog.h"
#include "ui_colordialog.h"
#include "context.h"
#include "primitives/entity.h"
#include "primitives/node.h"
#include "primitives/edge.h"
#include "auxiliary/treewidgetitem.h"

ColorDialog::ColorDialog(Context* context, QWidget* parent) :
        QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint),
        m_ui(new Ui::ColorDialog),
        m_context(context)
{
    m_ui->setupUi(this);
    m_ui->tabWidget->removeTab(0);

    QStringList nodeHeaderLabels;
    nodeHeaderLabels.append(tr("ID"));
    nodeHeaderLabels.append(tr("Background"));
    nodeHeaderLabels.append(tr("Foreground"));
    nodeColorTypes.insert(1, Node::clBackground);
    nodeColorTypes.insert(2, Node::clForeground);

    QStringList entityHeaderLabels;
    entityHeaderLabels.append(tr("ID"));
    entityHeaderLabels.append(tr("Background"));
    entityHeaderLabels.append(tr("Foreground"));
    entityHeaderLabels.append(tr("Final Background"));
    entityHeaderLabels.append(tr("Final Foreground"));
    entityColorTypes.insert(1, Entity::clBackground);
    entityColorTypes.insert(2, Entity::clForeground);
    entityColorTypes.insert(3, Entity::clBackgroundFinal);
    entityColorTypes.insert(4, Entity::clForegroundFinal);

    QStringList graphHeaderLabels;
    graphHeaderLabels.append(tr("Legend"));
    graphHeaderLabels.append(tr("Color"));

    QTreeWidget* treeWidgetNodes = new QTreeWidget(this);
    treeWidgetNodes->setEditTriggers(QAbstractItemView::NoEditTriggers);
    treeWidgetNodes->setSelectionMode(QAbstractItemView::ExtendedSelection);
    treeWidgetNodes->setSelectionBehavior(QAbstractItemView::SelectRows);
    treeWidgetNodes->setUniformRowHeights(true);
    treeWidgetNodes->setSortingEnabled(true);
    treeWidgetNodes->setColumnCount(nodeHeaderLabels.size());
    treeWidgetNodes->setHeaderLabels(nodeHeaderLabels);
    treeWidgetNodes->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(treeWidgetNodes, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowContextMenu(const QPoint&)));
    if (m_context)
    {
        foreach(Node* node, m_context->nodes)
        {
            if (node)
            {
                TreeWidgetItem* item = new TreeWidgetItem();
                item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
                item->setText(0, QString::number(node->GetID()));
                item->setTextAlignment(0, Qt::AlignRight);
                for (int i = 0; i < nodeColorTypes.size(); i++)
                {
                    item->setText(i + 1, node->GetColor(nodeColorTypes[i + 1]).name());
                    item->setData(i + 1, Qt::DecorationRole, node->GetColor(nodeColorTypes[i + 1]));
                }
                treeWidgetNodes->addTopLevelItem(item);
            }
        }
    }
    for (int i = 0; i < nodeHeaderLabels.size(); i++)
        treeWidgetNodes->resizeColumnToContents(i);
    treeWidgetNodes->sortByColumn(0, Qt::AscendingOrder);

    QTreeWidget* treeWidgetEntities = new QTreeWidget(this);
    treeWidgetEntities->setEditTriggers(QAbstractItemView::NoEditTriggers);
    treeWidgetEntities->setSelectionMode(QAbstractItemView::ExtendedSelection);
    treeWidgetEntities->setSelectionBehavior(QAbstractItemView::SelectRows);
    treeWidgetEntities->setUniformRowHeights(true);
    treeWidgetEntities->setSortingEnabled(true);
    treeWidgetEntities->setColumnCount(entityHeaderLabels.size());
    treeWidgetEntities->setHeaderLabels(entityHeaderLabels);
    treeWidgetEntities->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(treeWidgetEntities, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowContextMenu(const QPoint&)));
    if (m_context)
    {
        foreach(Entity* entity, m_context->entities)
        {
            if (entity)
            {
                TreeWidgetItem* item = new TreeWidgetItem();
                item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
                item->setText(0, QString::number(entity->GetID()));
                item->setTextAlignment(0, Qt::AlignRight);
                for (int i = 0; i < entityColorTypes.size(); i++)
                {
                    item->setText(i + 1, entity->GetColor(entityColorTypes[i + 1]).name());
                    item->setData(i + 1, Qt::DecorationRole, entity->GetColor(entityColorTypes[i + 1]));
                }
                treeWidgetEntities->addTopLevelItem(item);
            }
        }
    }
    for (int i = 0; i < entityHeaderLabels.size(); i++)
        treeWidgetEntities->resizeColumnToContents(i);
    treeWidgetEntities->sortByColumn(0, Qt::AscendingOrder);

    QTreeWidget* treeWidgetGraph = new QTreeWidget(this);
    treeWidgetGraph->setEditTriggers(QAbstractItemView::NoEditTriggers);
    treeWidgetGraph->setSelectionMode(QAbstractItemView::ExtendedSelection);
    treeWidgetGraph->setSelectionBehavior(QAbstractItemView::SelectRows);
    treeWidgetGraph->setUniformRowHeights(true);
    treeWidgetGraph->setSortingEnabled(true);
    treeWidgetGraph->setColumnCount(graphHeaderLabels.size());
    treeWidgetGraph->setHeaderLabels(graphHeaderLabels);
    treeWidgetGraph->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(treeWidgetGraph, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowContextMenu(const QPoint&)));
    if (m_context)
    {
        m_itemBackground = new TreeWidgetItem();
        m_itemBackground->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_itemBackground->setText(0, tr("Graph background"));
        m_itemBackground->setText(1, m_context->sceneBackground.name());
        m_itemBackground->setData(1, Qt::DecorationRole, m_context->sceneBackground);
        treeWidgetGraph->addTopLevelItem(m_itemBackground);
        Edge* edge = NULL;
        if (!m_context->edges.empty())
            edge = m_context->edges.value(m_context->edges.keys().at(0));
        if (edge)
        {
            m_itemBoundary = new TreeWidgetItem();
            m_itemBoundary->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            m_itemBoundary->setText(0, tr("Outlines and edges"));
            m_itemBoundary->setText(1, edge->GetColor(Edge::clLine).name());
            m_itemBoundary->setData(1, Qt::DecorationRole, edge->GetColor(Edge::clLine));
            treeWidgetGraph->addTopLevelItem(m_itemBoundary);
            m_itemHighlight = new TreeWidgetItem();
            m_itemHighlight->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            m_itemHighlight->setText(0, tr("Edge highlight"));
            m_itemHighlight->setText(1, edge->GetColor(Edge::clHighlight).name());
            m_itemHighlight->setData(1, Qt::DecorationRole, edge->GetColor(Edge::clHighlight));
            treeWidgetGraph->addTopLevelItem(m_itemHighlight);
        }
    }
    for (int i = 0; i < graphHeaderLabels.size(); i++)
        treeWidgetGraph->resizeColumnToContents(i);
    treeWidgetGraph->sortByColumn(0, Qt::AscendingOrder);

    for (int i = 0; i < 3; i++)
    {
        if (i == ColorDialog::tabNodes)
            m_ui->tabWidget->addTab(treeWidgetNodes, tr("Nodes"));
        else if (i == ColorDialog::tabEntities)
            m_ui->tabWidget->addTab(treeWidgetEntities, tr("Entities"));
        else if (i == ColorDialog::tabGraph)
            m_ui->tabWidget->addTab(treeWidgetGraph, tr("Graph"));
    }
}

ColorDialog::~ColorDialog()
{
    delete m_ui;
}

void ColorDialog::SelectItems(const QList<Node*>& selectedNodes)
{
    QTreeWidget* treeWidget = qobject_cast<QTreeWidget*>(m_ui->tabWidget->currentWidget());
    if (!treeWidget)
        return;
    treeWidget->clearSelection();
    int index = m_ui->tabWidget->currentIndex();
    foreach(Node* node, selectedNodes)
    {
        if (node)
        {
            int id = -1;
            Entity* entity = NULL;
            switch (index)
            {
                case ColorDialog::tabNodes:
                    id = node->GetID();
                    break;
                case ColorDialog::tabEntities:
                    entity = node->GetEntity();
                    if (entity)
                        id = entity->GetID();
                    break;
                default:
                    return;
            }
            QList<QTreeWidgetItem*> items =  treeWidget->findItems(QString::number(id), Qt::MatchFixedString, 0);
            if (!items.empty())
            {
                QTreeWidgetItem* item = items[0];
                item->setSelected(true);
            }
        }
    }
}

void ColorDialog::on_buttonSelectAll_clicked()
{
    QTreeWidget* treeWidget = qobject_cast<QTreeWidget*>(m_ui->tabWidget->currentWidget());
    if (treeWidget)
        treeWidget->selectAll();
}

void ColorDialog::on_buttonClose_clicked()
{
    this->close();
}

void ColorDialog::on_tabWidget_currentChanged(int index)
{
    Q_UNUSED(index)
    QMenu* menu =  m_ui->buttonSetColor->menu();
    m_ui->buttonSetColor->setMenu(NULL);
    delete menu;
    menu = CreateMenu();
    m_ui->buttonSetColor->setMenu(menu);
}

void ColorDialog::changeEvent(QEvent* e)
{
    QDialog::changeEvent(e);
    switch (e->type())
    {
        case QEvent::LanguageChange:
            m_ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

QMenu* ColorDialog::CreateMenu()
{
    QMenu* menu = new QMenu(this);
    QTreeWidget* treeWidget = qobject_cast<QTreeWidget*>(m_ui->tabWidget->currentWidget());
    QTreeWidgetItem* header = NULL;
    if (treeWidget)
        header = treeWidget->headerItem();
    if (!header)
        return menu;
    QSignalMapper* mapper = new QSignalMapper(this);  
    for (int i = 1; i < header->columnCount(); i++)
    {
        QString label = header->text(i) + "...";
        QAction* action = new QAction(label, menu);
        connect(action, SIGNAL(triggered()), mapper, SLOT(map()));
        mapper->setMapping(action, i);
        menu->addAction(action);
    }
    connect(mapper, SIGNAL(mapped(int)), this, SLOT(SetColor(int)));
    return menu;
}

void ColorDialog::ShowContextMenu(const QPoint& position)
{
    QTreeWidget* senderTreeWidget = qobject_cast<QTreeWidget*>(sender());
    if (!senderTreeWidget)
        return;

    QTreeWidgetItem* item = senderTreeWidget->itemAt(position);
    if (!item)
        return;
    if (!item->isSelected())
        item->setSelected(true);

    QMenu* menu = m_ui->buttonSetColor->menu();
    if (menu)
        menu->exec(senderTreeWidget->viewport()->mapToGlobal(position));
}

void ColorDialog::SetColor(int column)
{
    QTreeWidget* treeWidget = qobject_cast<QTreeWidget*>(m_ui->tabWidget->currentWidget());
    if (!treeWidget || treeWidget->selectedItems().empty())
        return;

    QColor color;
    if (treeWidget->selectedItems().size() == 1)
        color = QColorDialog::getColor(QColor(treeWidget->selectedItems().at(0)->text(column)), this);
    else
        color = QColorDialog::getColor(QColor(Qt::white), this);
    if (color.isValid() && m_context)
    {
        m_context->enabledColoring = false;
        foreach (QTreeWidgetItem* item, treeWidget->selectedItems())
        {
            if (item)
            {
                item->setText(column, color.name());
                item->setData(column, Qt::DecorationRole, color);

                if (item == m_itemBackground)
                    emit BackgroundColorChanged(color);
                else if (item == m_itemBoundary && m_context)
                {
                    foreach (Node* node, m_context->nodes)
                    {
                        if (node)
                            node->SetColor(Node::clBoundary, color);
                    }
                    foreach (Edge* edge, m_context->edges)
                    {
                        if (edge)
                            edge->SetColor(Edge::clLine, color);
                    }
                }
                else if (item == m_itemHighlight && m_context)
                {
                    foreach (Edge* edge, m_context->edges)
                    {
                        if (edge)
                            edge->SetColor(Edge::clHighlight, color);
                    }
                }
                else if (m_ui->tabWidget->currentIndex() == ColorDialog::tabNodes)
                {
                    Node* node = m_context->nodes[item->text(0).toInt()];
                    Node::NodeColorType type = nodeColorTypes[column];
                    if (node)
                        node->SetColor(type, color);
                }
                else if (m_ui->tabWidget->currentIndex() == ColorDialog::tabEntities)
                {
                    Entity* entity = m_context->entities[item->text(0).toInt()];
                    Entity::EntityColorType type = entityColorTypes[column];
                    if (entity)
                        entity->SetColor(type, color);
                }
            }
        }
        foreach (Node* node, m_context->nodes)
        {
            if (node)
                node->update(node->boundingRect());
        }
        foreach (Edge* edge, m_context->edges)
        {
            if (edge)
                edge->update(edge->boundingRect());
        }
    }
}
