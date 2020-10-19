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

#ifndef KAMADAKAWAILAYOUTER_H
#define KAMADAKAWAILAYOUTER_H

#include <QtCore/QList>

#include "layouters/layouter.h"

class Context;

class KamadaKawaiLayouter : public Layouter
{
    Q_OBJECT

public:
    KamadaKawaiLayouter();
    virtual ~KamadaKawaiLayouter();

    static QString GetName() { return "Kamada-Kawai"; }
    virtual QString Name() const { return KamadaKawaiLayouter::GetName(); }
    virtual QString Description() const { return "Kamada-Kawai"; }

    virtual bool Layout(Context& context);

private:
    bool init;
    QList<int> m; // Node ID mapper
    double** d; // Shortest paths
    double** k; // Spring strengths
};

#endif
