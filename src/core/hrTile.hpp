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
#pragma once

#include "precompiled.hpp"

class hrTile
{
public:
    quint8 terrainId;
    quint8 terrainFrame;
    quint8 riverId;
    quint8 riverFrame;
    quint8 roadId;
    quint8 roadFrame;
    quint8 mirror;
private:
    enum
    {
        terrainVertical = 0x1,
        terrainHorizontal = 0x2,
        riverVertical = 0x4,
        riverHorizontal = 0x8,
        roadVertical = 0x10,
        roadHorizontal = 0x20
    };
public:
    hrTile() : terrainId(0xFF)
            , terrainFrame(0)
            , riverId(0)
            , riverFrame(0)
            , roadId(0)
            , roadFrame(0)
            , mirror(0)
            {}
    bool isTerrainVertical()
    {
        return (mirror & terrainVertical);
    }
    bool isTerrainHorizontal()
    {
        return (mirror & terrainHorizontal);
    }
    bool isRiverVertical()
    {
        return (mirror & riverVertical);
    }
    bool isRiverHorizontal()
    {
        return (mirror & riverHorizontal);
    }
    bool isRoadVertical()
    {
        return (mirror & roadVertical);
    }
    bool isRoadHorizontal()
    {
        return (mirror & roadHorizontal);
    }
};
