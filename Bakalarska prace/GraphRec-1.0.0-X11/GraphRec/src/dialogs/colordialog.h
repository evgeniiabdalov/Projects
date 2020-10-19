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

#ifndef COLORDIALOG_H
#define COLORDIALOG_H

#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtGui/QDialog>

#include "primitives/entity.h"
#include "primitives/node.h"

class QWidget;
class QColor;
class QEvent;
class QPointF;
class QMenu;
class QTreeWidgetItem;
class Context;
class Node;

namespace Ui
{
    class ColorDialog;
}

class ColorDialog : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(ColorDialog)

signals:
    void BackgroundColorChanged(const QColor& color);

public:
    enum TabOrder
    {
        tabNodes = 0,
        tabEntities,
        tabGraph
    };

    explicit ColorDialog(Context* context, QWidget* parent = 0);
    virtual ~ColorDialog();

    void SelectItems(const QList<Node*>& selectedNodes);

public slots:
    void on_buttonSelectAll_clicked();
    void on_buttonClose_clicked();
    void on_tabWidget_currentChanged(int index);

protected:
    virtual void changeEvent(QEvent* e);

protected slots:
    QMenu* CreateMenu();
    void ShowContextMenu(const QPoint& position);
    void SetColor(int column);

private:
    Ui::ColorDialog* m_ui;
    Context* m_context;
    QTreeWidgetItem* m_itemBackground;
    QTreeWidgetItem* m_itemBoundary;
    QTreeWidgetItem* m_itemHighlight;
    QMap<int, Node::NodeColorType> nodeColorTypes; // QMap<column, Node::NodeColorType>
    QMap<int, Entity::EntityColorType> entityColorTypes; // QMap<column, Entity::EntityColorType>
};

#endif
