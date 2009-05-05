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
#ifndef HRSCENE_H
#define HRSCENE_H

#include "hrTile.hpp"
#include "hrGraphicsItem.hpp"
#include "hrTileAtlas.hpp"
#include "hrCoord.hpp"
#include "hrSceneObject.hpp"

class hrSceneTile
{
    int id;
    int frame;
    QPoint point;
    bool horizontal;
    bool vertical;
public:
    hrSceneTile(int id
                , int frame
                , int x
                , int y
                , bool horizontal = false
                , bool vertical = false)
        : id(id), frame(frame), horizontal(horizontal), vertical(vertical)
    {
        point.setX(x);
        point.setY(y);
    }
    bool operator<(const hrSceneTile& s) const
    {
        if (this->id < s.getId())
            return true;
        return false;
    }
    int getId() const
    {
        return id;
    }
    int getFrame() const
    {
        return frame;
    }
    bool isHorizontal() const
    {
        return horizontal;
    }
    bool isVertical() const
    {
        return vertical;
    }
    int x() const
    {
        return point.x();
    }
    int y() const
    {
        return point.y();
    }
    QPoint getPoint() const
    {
        return point;
    }
};

class hrScene
{
public:
    hrScene(int width, int height);
    ~hrScene();

    void addTile(const hrTile &tile);
    void addObject(hrSceneObject &object);
    void removeObject(int x, int y);

    void setCursor(const QString &name);
    const QCursor& getCursor(int frame);

    QRect getSize() const;
    void setSceneViewport(const QRect &r);
    QRect getSceneViewport() const;

    const QImage& getImage(const hrSceneObject &object) const;
    hrGraphicsItem* getItem(const hrSceneObject &object) const;
    QList<hrSceneObject> getViewportObjects() const;
    const QList<hrSceneObject>& getAllObjects() const;

    const hrTile& getTile(int x, int y) const;
    hrTileAtlas* getAtlas(const hrSceneTile &tile) const;
    QList<hrSceneTile> getViewportTiles() const;

    void sortObjects();


public slots:
//mouseClick()
//mouseMove()


private:
    QVector<hrTile> tiles;
    QList<hrSceneObject> objects;

    QMap<int, hrTileAtlas*> items_atlas;
    QMap<int, hrGraphicsItem*> items_obj;
    QVector<QCursor> cursor;
    QRect viewport;
    QRect size;

    void addItem(int id, const QString &name);
    void addTileItem(int id, const QString &name);
};

#endif // HRSCENE_H
