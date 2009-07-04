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
#include "precompiled.hpp"
#include "hrSceneObject.hpp"

bool hrSceneObject::operator<(const hrSceneObject& s) const
{
    if ( this->isOverlay() && !s.isOverlay() )
        return true;

    if ( !this->isOverlay() && s.isOverlay() )
        return false;

    int x1 = rect.bottomRight().x();
    int y1 = rect.bottomRight().y();
    int x2 = s.getRect().bottomRight().x();
    int y2 = s.getRect().bottomRight().y();

    if (y1 < y2)
        return true;

    if (y1 > y2)
        return false;

    if ( !this->isVisitable() && s.isVisitable() )
        return true;

    if ( this->isVisitable() && !s.isVisitable() )
        return false;

    if (x1 < x2)
        return true;

    return false;
}
