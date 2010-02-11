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
#pragma once
#include "hrCoord.hpp"

typedef int hrCacheKey;

class hrGraphicsItem
{
public:
    hrGraphicsItem()
        : key(-1)
        , curFrame(0)
        , countFrames(0)
        , isHorizontal(false)
        , isVertical(false)
    { }

    hrGraphicsItem(hrCacheKey key, int count, const QSize &size)
        : key(key)
        , curFrame(0)
        , countFrames(count)
        , isHorizontal(false)
        , isVertical(false)
    {
        rect.setRect(0, 0, size.width(), size.height());
    }

    bool operator < (const hrGraphicsItem& s) const
    {
        if (key < s.key)
            return true;
        return false;
    }

    QSize getSize() const
    {
        return QSize(rect.width(), rect.height());
    }
    void setPoint(int x, int y)
    {
        rect.setRect(x, y, rect.width(), rect.height());
    }
    void setPoint(const QPoint& p)
    {
        rect.setRect(p.x(), p.y(), rect.width(), rect.height());
    }
    void setMirror(bool horizontal, bool vertical)
    {
        isHorizontal = horizontal;
        isVertical = vertical;
    }
    void setCurFrame(int frame)
    {
        curFrame = frame;
    }

    hrCacheKey key;
    int curFrame;
    int countFrames;
    bool isHorizontal;
    bool isVertical;
    QRect rect;
};
