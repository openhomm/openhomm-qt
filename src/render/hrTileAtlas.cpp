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
#include "hrTileAtlas.hpp"

hrTileAtlas::hrTileAtlas(int animation) : x(0), y(0), animation(animation)
{
    tilesPerLine = dim / coord::dim;
}

void hrTileAtlas::CyclShiftPalette(int a, int b)
{
    QRgb c = atlas.color(b);
    for (int i = b; i > a; i--)
    {
        atlas.setColor(i, atlas.color(i - 1));
    }
    atlas.setColor(a, c);
}

void hrTileAtlas::addImage(const QImage &im)
{
    Q_ASSERT(x < tilesPerLine && y < tilesPerLine);

    if (y == 0 && x == 0)
    {
        atlas = im.copy(0, 0, dim, dim);
        x++;
        return;
    }

    for (int i = 0; i < coord::dim; i++)
    {
        quint8 *lineAtlas = atlas.scanLine(coord::toPix(y) + i) + coord::toPix(x);
        const quint8 *line = im.scanLine(i);
        memcpy(lineAtlas, line, coord::dim);
    }

    if (x < tilesPerLine - 1)
    {
        x++;
    }
    else
    {
        x = 0;
        y++;
    }
}

const QImage& hrTileAtlas::getImage() const
{
    return atlas;
}

QRect hrTileAtlas::getFrame(int frame) const
{
    Q_ASSERT(frame >= 0 && frame < tilesPerLine * tilesPerLine);

    int y0 = frame / tilesPerLine;
    int x0 = frame - y0 * tilesPerLine;
    QRect r(x0, y0, 1, 1);
    return coord::toPix(r);
}

void hrTileAtlas::nextFrame()
{
    switch (animation)
    {
        case noanim:
             break;
        case watrtl:
             CyclShiftPalette(229, 240);
             CyclShiftPalette(242, 253);
             break;
        case clrrvr:
             CyclShiftPalette(189, 200);
             break;
        case mudrvr:
             CyclShiftPalette(228, 239);
             break;
        case lavrvr:
             CyclShiftPalette(240, 248);
             break;
    }
}
