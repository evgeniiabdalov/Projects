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

#ifndef FACTORY_H
#define FACTORY_H

#include <QtCore/QObject>
#include <QtCore/QStringList>

class Servant;
class QString;

class Factory : public QObject
{
    Q_OBJECT

public:
    enum ServantType
    {
        sParser = 0,
        sValidator,
        sLayouter,
        sRecorder,
        sSaver
    };

    Factory();
    static QStringList GetServantNames(Factory::ServantType type);
    static Servant* CreateServant(Factory::ServantType type, const QString& name);
};

#endif
