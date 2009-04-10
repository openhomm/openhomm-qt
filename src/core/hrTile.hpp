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
    /*bool riverHorizontal : 1;
    bool riverVertical : 1;
    bool roadHorizontal : 1;
    bool roadVertical : 1;
    bool terrainHorizontal : 1;
    bool terrainVertical : 1;*/
    union
    {
        struct
        {
            bool riverHorizontal : 1;
            bool riverVertical : 1;
            bool roadHorizontal : 1;
            bool roadVertical : 1;
            bool terrainHorizontal : 1;
            bool terrainVertical : 1;
        };
        quint8 mirror;
    };
    /*hrTile() : terrainId(0xFF)
            , terrainFrame(0)
            , riverId(0)
            , riverFrame(0)
            , roadId(0)
            , roadFrame(0)
            , riverHorizontal(false)
            , riverVertical(false)
            , roadHorizontal(false)
            , roadVertical(false)
            , terrainHorizontal(false)
            , terrainVertical(false)
            {}*/
};
