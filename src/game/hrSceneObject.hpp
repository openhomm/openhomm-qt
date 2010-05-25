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

class hrSceneObject
{
    int id;
    QRect rect;
    quint8 visitable[6];
    bool overlay;
    bool underground;

    int curFrame;
    int countFrames;
public:
    hrSceneObject(int id
                  , quint8 *visit
                  , bool overlay
                  , bool underground
                  , const QPoint &bottomRight)
        : id(id), overlay(overlay), underground(underground)
        , curFrame(0), countFrames(0)
    {
        memcpy(visitable, visit, 6);
        rect.setBottomRight(bottomRight);
    }

    void setRect(int x, int y, int width, int height)
    {
        rect.setRect(x, y, width, height);
    }

    quint8* getVisitable()
    {
        return visitable;
    }
    bool isVisitable() const
    {
        for ( quint8 i = 0; i < 6; i++)
            if ( visitable[i] != 0 )
                return true;

        return false;
    }

    bool operator<(const hrSceneObject& s) const;

    bool isOverlay() const
    {
        return overlay;
    }
    bool isUnderground() const
    {
        return underground;
    }
    int getId() const
    {
        return id;
    }
    QPoint getPoint() const
    {
        return rect.topLeft();
    }
    const QRect& getRect() const
    {
        return rect;
    }
    void setSize(QSize size)
    {
        QPoint p = rect.bottomRight();
        rect.setRect(p.x() - size.width() + 1
                   , p.y() - size.height() + 1
                   , size.width()
                   , size.height()
                   );
    }
    QSize getSize() const
    {
        return QSize(rect.width(), rect.height());
    }
    void setFrames(int count)
    {
        countFrames = count;

        curFrame = qrand() % countFrames;
    }
    int getNextFrame()
    {
        curFrame < countFrames - 1 ? curFrame++ : curFrame = 0;
        return curFrame;
    }
    int getCurFrame() const
    {
        return curFrame;
    }
};
