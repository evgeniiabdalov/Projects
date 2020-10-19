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

#ifndef ENTITY_H
#define ENTITY_H

#include <QtCore/QObject>
#include <QtGui/QColor>

class Entity : public QObject
{
    Q_OBJECT

public:
    enum EntityColorType { clBackground = 0, clForeground, clBackgroundFinal, clForegroundFinal };

    Entity(int id);
    int GetID() const;
    int GetTimeStepFinal() const;
    void SetTimeStepFinal(int timestep);
    QColor GetColor(Entity::EntityColorType type) const;

public slots:
    void SetColor(Entity::EntityColorType type, const QColor& color);

private:
    int m_id;
    int m_timestepFinal;
    QColor m_clBackground;
    QColor m_clBackgroundFinal;
    QColor m_clForeground;
    QColor m_clForegroundFinal;
};

#endif
