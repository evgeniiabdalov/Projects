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

#ifndef MULTIROBOTSAVER_H
#define MULTIROBOTSAVER_H

#include <QtCore/QString>
#include <QtCore/QTextStream>

#include "savers/saver.h"

class QFile;
class Context;

class MultirobotSaver : public Saver
{
    Q_OBJECT

public:
    MultirobotSaver();
    virtual ~MultirobotSaver();
    static QString GetName() { return "Multirobot (*.txt)"; }
    virtual QString Name() const { return MultirobotSaver::GetName(); }
    virtual QString Description() const { return "Multirobot Saver"; }

    virtual void Open(QFile& file);
    virtual void Close();
    virtual void Save(Context& context);

private:
    QTextStream out;
};

#endif
