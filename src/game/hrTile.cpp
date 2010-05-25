// openhomm - open source clone of Heroes of Might and Magic III
// Copyright (C) 2009-2010 openhomm developers team (see AUTHORS)
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
#include "hrTile.hpp"

/*!
  \class hrTile
  \brief The hrTile class
*/
QString hrTile::getTerrainName() const
{
    QString name;
    switch (terrainId)
    {
        case 0:
            name = "dirttl.def";
            break;
        case 1:
            name = "sandtl.def";
            break;
        case 2:
            name = "grastl.def";
            break;
        case 3:
            name = "snowtl.def";
            break;
        case 4:
            name = "swmptl.def";
            break;
        case 5:
            name = "rougtl.def";
            break;
        case 6:
            name = "subbtl.def";
            break;
        case 7:
            name = "lavatl.def";
            break;
        case 8:
            name = "watrtl.def";
            break;
        case 9:
            name = "rocktl.def";
            break;
        default:
            name = "default.def";
            break;
    }
    return name;
}

QString hrTile::getRiverName() const
{
    QString name;
    switch (riverId)
    {
        case 1:
            name = "clrrvr.def";
            break;
        case 2:
            name = "icyrvr.def";
            break;
        case 3:
            name = "mudrvr.def";
            break;
        case 4:
            name = "lavrvr.def";
            break;
        default:
            name = "default.def";
            break;
    }
    return name;
}

QString hrTile::getRoadName() const
{
    QString name;
    switch (roadId)
    {
        case 1:
            name = "dirtrd.def";
            break;
        case 2:
            name = "gravrd.def";
            break;
        case 3:
            name = "cobbrd.def";
            break;
        default:
            name = "default.def";
            break;
    }
    return name;
}
