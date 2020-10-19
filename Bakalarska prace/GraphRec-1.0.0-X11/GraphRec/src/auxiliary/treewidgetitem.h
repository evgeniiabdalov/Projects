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

#ifndef TREEWIDGETITEM_H
#define TREEWIDGETITEM_H

#include <QtGui/QTreeWidgetItem>

class QStringList;
class QTreeWidget;

class TreeWidgetItem : public QTreeWidgetItem
{
public:
    TreeWidgetItem(int type = UserType) : QTreeWidgetItem(type) { }
    TreeWidgetItem(const QStringList& strings, int type = UserType) : QTreeWidgetItem(strings, type) { }
    TreeWidgetItem(QTreeWidget* parent, int type = UserType) : QTreeWidgetItem(parent, type) { }
    TreeWidgetItem(QTreeWidget* parent, const QStringList& strings, int type = UserType) : QTreeWidgetItem(parent, strings, type) { }
    TreeWidgetItem(QTreeWidget* parent, QTreeWidgetItem* preceding, int type = UserType) : QTreeWidgetItem(parent, preceding, type) { }
    TreeWidgetItem(QTreeWidgetItem* parent, int type = UserType) : QTreeWidgetItem(parent, type) { }
    TreeWidgetItem(QTreeWidgetItem* parent, const QStringList& strings, int type = UserType) : QTreeWidgetItem(parent, strings, type) { }
    TreeWidgetItem(QTreeWidgetItem* parent, QTreeWidgetItem* preceding, int type = UserType) : QTreeWidgetItem(parent, preceding, type) { }
    TreeWidgetItem(const QTreeWidgetItem& other) : QTreeWidgetItem(other) { }

    virtual bool operator<(const QTreeWidgetItem& other) const;
};

#endif
