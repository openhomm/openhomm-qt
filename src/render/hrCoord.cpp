// openhomm - open source clone of Heroes of Might and Magic III
// Copyright (C) 2009 openhomm developers team (see AUTHORS)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#include "hrCoord.hpp"

int coord::toPix(int x)
{
    return x * 32;
}
int coord::toCell(int x)
{
    return x / 32;
}
QPoint coord::toPix(const QPoint &p)
{
    return QPoint(toPix(p.x()), toPix(p.y()));
}
QPoint coord::toCell(const QPoint &p)
{
    return QPoint(toCell(p.x()), toCell(p.y()));
}
QRect coord::toPix(const QRect &r)
{
    return QRect(toPix(r.x()), toPix(r.y()), toPix(r.width()), toPix(r.height()));
}
QRect coord::toCell(const QRect &r)
{
    return QRect(toCell(r.x()), toCell(r.y()), toCell(r.width()), toCell(r.height()));
}

