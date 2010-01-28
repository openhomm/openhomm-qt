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
#include "hrTileAtlas.hpp"
#include "hrCoord.hpp"

/*!
  \class hrTileAtlas
  \brief The hrTileAtlas class
*/
hrTileAtlas::hrTileAtlas(int dim)
    : x(0), y(0), dim(dim), curFrame(0)
{
    tilesPerLine = dim / coord::dim;
    frames.append(QImage());
}

int hrTileAtlas::getDim() const
{
    return dim;
}

void hrTileAtlas::CyclShiftPalette(int a, int b, QImage &im)
{
    QRgb c = im.color(b);
    for (int i = b; i > a; i--)
    {
        im.setColor(i, im.color(i - 1));
    }
    im.setColor(a, c);
}

void hrTileAtlas::addImage(const QImage &im)
{
    Q_ASSERT(x < tilesPerLine && y < tilesPerLine);

    QImage &atlas = frames[0];
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
    return frames.at(curFrame);
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
    curFrame < frames.size() - 1 ? curFrame++ : curFrame = 0;
}

void hrTileAtlas::animate(int animation)
{
    QImage im = frames[0];
    switch (animation)
    {
        case noanim:
             break;
        case watrtl:
             for (int i = 0; i < 11; i++)
             {
                 CyclShiftPalette(229, 240, im);
                 CyclShiftPalette(242, 253, im);
                 frames.append(im);
             }
             break;
        case clrrvr:
             for (int i = 0; i < 11; i++)
             {
                CyclShiftPalette(189, 200, im);
                frames.append(im);
             }
             break;
        case mudrvr:
             for (int i = 0; i < 11; i++)
             {
                CyclShiftPalette(228, 239, im);
                frames.append(im);
             }
             break;
        case lavrvr:
             for (int i = 0; i < 8; i++)
             {
                CyclShiftPalette(240, 248, im);
                frames.append(im);
             }
             break;
    }
}
