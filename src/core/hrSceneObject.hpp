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

class hrSceneObject
{
    int id;
    QString name;
    QRect rect;
    quint8 visitable[6];
    bool overlay;
    bool underground;
public:
    hrSceneObject() { name = "default.def"; }

    hrSceneObject(int id
                  , const QString &name
                  , quint8 *visit
                  , bool overlay
                  , bool underground
                  , const QPoint &bottomRight)
        : id(id), name(name), overlay(overlay), underground(underground)
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
    const QString& getName() const
    {
        return name;
    }
    QPoint getPoint() const
    {
        return rect.topLeft();
    }
    const QRect& getRect() const
    {
        return rect;
    }
    int x() const
    {
        return rect.x();
    }
    int y() const
    {
        return rect.y();
    }
};
