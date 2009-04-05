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

#include "hrGraphicsItem.hpp"
#include "hrCoord.hpp"

class hrTile
{
    int id;
    int frame;
    QPoint point;
    bool horizontal;
    bool vertical;
public:
    hrTile() {}

    hrTile(int id, int frame = 0, bool horizontal = false, bool vertical = false)
        : id(id), frame(frame), horizontal(horizontal), vertical(vertical) {}

    hrTile(int id, int frame, int x, int y)
        : id(id), frame(frame), horizontal(false), vertical(false)
    {
        point.setX(x);
        point.setY(y);
    }
    int getId() const
    {
        return id;
    }
    int getFrame() const
    {
        return frame;
    }
    QPoint getPoint() const
    {
        return point;
    }
    int x() const
    {
        return point.x();
    }
    int y() const
    {
        return point.y();
    }
    bool getHorizontal() const
    {
        return horizontal;
    }
    bool getVertical() const
    {
        return vertical;
    }
};

class hrObject
{
    int id;
    QRect rect;
public:
    hrObject() {}
    hrObject(int id, QRect rect) : id(id), rect(rect) {}
    hrObject(int id, int x, int y, int width, int height) : id(id)
    {
        rect.setRect(x, y, width, height);
    }
    hrObject(int id, int x, int y) : id(id)
    {
        rect.setX(x);
        rect.setY(y);
    }
    int getId() const
    {
        return id;
    }
    QPoint getPoint() const
    {
        return QPoint(x(), y());
    }
    int x() const
    {
        return rect.x();
    }
    int y() const
    {
        return rect.y();
    }
    QRect getRect() const
    {
        return rect;
    }
};

class hrScene
{
public:
    hrScene(int width, int height);
    ~hrScene();

    void addTile(int id
                 , int frame
                 , QString path
                 , bool horizontal = false
                 , bool vertical = false);
    void addTileSecondLayer(int id, int frame, int x, int y, QString path);
    void addObject(int id, int x, int y, QString path);
    void removeObject(int x, int y);

    void setCursor(QString path);
    const QCursor& getCursor(int frame);

    QRect getSize() const;
    void setSceneViewport(QRect r);
    QRect getSceneViewport() const;

    QImage getImage(const hrTile &tile) const;
    QImage getImage(const hrObject &object) const;
    hrGraphicsItem* getItem(const hrObject &object) const;

    QVector<hrTile> getViewportTiles() const;
    hrTile getTile(int x, int y) const;
    QLinkedList<hrTile> getViewportTilesSecondLayer() const;
    QLinkedList<hrObject> getViewportObjects() const;
    QLinkedList<hrObject> getAllObjects() const;

public slots:
//mouseClick()
//mouseMove()


private:
    QVector< QVector<hrTile> > tiles;
    QLinkedList<hrObject> objects;
    QLinkedList<hrTile> tilesSecondLayer;
    QMap<int, hrGraphicsItem*> items;
    QVector<QCursor> cursor;
    QRect viewport;
    QRect size;

    void addItem(int id, QString name, bool mirrored = false);
};

#endif // HRSCENE_H
