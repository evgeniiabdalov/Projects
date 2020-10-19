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

#include <QtCore/QString>

#include "auxiliary/treewidgetitem.h"

bool TreeWidgetItem::operator<(const QTreeWidgetItem& other) const
{
    int column = this->treeWidget() ? this->treeWidget()->sortColumn() : 0;
    QString thisText = this->text(column);
    QString otherText = other.text(column);
    bool thisIsNumber = false;
    bool otherIsNumber = false;
    qlonglong thisNumber = thisText.toLongLong(&thisIsNumber);
    qlonglong otherNumber = otherText.toLongLong(&otherIsNumber);
    if (thisIsNumber && otherIsNumber)
        return thisNumber < otherNumber;
    else if (thisIsNumber && !otherIsNumber)
        return false;
    else if (!thisIsNumber && otherIsNumber)
        return true;
    else
        return thisText < otherText;
}
