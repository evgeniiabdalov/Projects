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

#include <QtGui/QColor>

#include "primitives/entity.h"

Entity::Entity(int id) :
        m_id(id),
        m_timestepFinal(-1),
        m_clBackground(Qt::green),
        m_clBackgroundFinal(Qt::darkGreen),
        m_clForeground(Qt::white),
        m_clForegroundFinal(Qt::white)
{
}

int Entity::GetID() const
{
    return m_id;
}

int Entity::GetTimeStepFinal() const
{
    return m_timestepFinal;
}

void Entity::SetTimeStepFinal(int timestep)
{
    m_timestepFinal = timestep;
}

QColor Entity::GetColor(Entity::EntityColorType type) const
{
    switch (type)
    {
        case Entity::clBackground:
            return m_clBackground;
        case Entity::clBackgroundFinal:
            return m_clBackgroundFinal;
        case Entity::clForeground:
            return m_clForeground;
        case Entity::clForegroundFinal:
            return m_clForegroundFinal;
        default:
            return Qt::black;
    }
}

void Entity::SetColor(Entity::EntityColorType type, const QColor& color)
{
    switch (type)
    {
        case Entity::clBackground:
            m_clBackground = color;
            break;
        case Entity::clBackgroundFinal:
            m_clBackgroundFinal = color;
            break;
        case Entity::clForeground:
            m_clForeground = color;
            break;
        case Entity::clForegroundFinal:
            m_clForegroundFinal = color;
            break;
        default:
            break;
    }
}
