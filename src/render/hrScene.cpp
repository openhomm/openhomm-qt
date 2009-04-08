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
#include "hrScene.hpp"

void hrScene::addItem(int id, QString name, bool mirrored)
{
    if (!items.contains(id))
    {
        hrGraphicsItem *item = new hrGraphicsItem();
        QImageReader ir("lod:/data/h3sprite.lod/" + name);
        QImage im;
        for (int i = 0; ir.jumpToImage(i); i++)
            if (ir.read(&im))
            {
                mirrored ? item->addImageMirrored(im) : item->addImage(im);
            }

        items[id] = item;
    }
}

void hrScene::addItem(QString name)
{
    if (!items_obj.contains(name))
    {
        hrGraphicsItem *item = new hrGraphicsItem();
        QImageReader ir("lod:/data/h3sprite.lod/" + name);
        QImage im;
        for (int i = 0; ir.jumpToImage(i); i++)
            if (ir.read(&im))
            {
               item->addImage(im);
            }

        items_obj[name] = item;
    }
}

hrScene::hrScene(int width, int height)
{
    size.setRect(0, 0, width - 1, height - 1);
    tiles.append(QVector<hrTile>());
    addItem(-1, "default.def");
}

hrScene::~hrScene()
{
    items.clear();
}

void hrScene::addTile(hrTile tile)
{
    QString name;
    switch (tile.terrainId)
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
    }

    addItem(tile.terrainId, name, true);

    if (tile.riverId != 0)
    {
        switch (tile.riverId)
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
        }
        addItem(tile.riverId, name, true);
    }

    if (tile.roadId != 0)
    {
        switch (tile.roadId)
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
        }
        addItem(tile.roadId, name, true);
    }


    static int x = 0;
    static int y = 0;

    if (x < size.width())
    {
        tiles[y].append(tile);
        x++;
    }
    else
    {
        x = 0;
        if (y < size.height())
        {
            tiles.append(QVector<hrTile>());
        }
        y++;
    }
}

void hrScene::addObject(hrObject object)
{
    addItem(object.getName());

    QRect r = items_obj.value(object.getName())->getRect();
    objects.append( hrObject(object.getName()
                             , object.x()
                             , object.y()
                             , toCell(r.width())
                             , toCell(r.height())
                             )
                   );
}


void hrScene::removeObject(int x, int y)
{
    ;
}

void hrScene::setCursor(QString name)
{
    QImageReader ir("lod:/data/h3sprite.lod/" + name);
    QImage im;
    for (int i = 0; ir.jumpToImage(i); i++)
        if (ir.read(&im))
            cursor.append(QCursor(QPixmap::fromImage(im.copy(0, 0, 32, 32))
                                  , 0
                                  , 0
                                  )
                          );
}

const QCursor& hrScene::getCursor(int frame)
{
    if (frame < cursor.size())
        return cursor.at(frame);
    return cursor.at(0);
}

QImage hrScene::getImage(int id
                         , int frame
                         , bool horizontal
                         , bool vertical) const
{
    return items.value(id)->getFrame(frame, horizontal, vertical);
}

QImage hrScene::getImageTerrain(hrTile tile) const
{
    return getImage(tile.terrainId, tile.terrainFrame, tile.terrainHorizontal, tile.terrainVertical);
}

QImage hrScene::getImageRiver(hrTile tile) const
{
    return getImage(tile.riverId, tile.riverFrame, tile.riverHorizontal, tile.riverVertical);
}

QImage hrScene::getImageRoad(hrTile tile) const
{
    return getImage(tile.roadId, tile.roadFrame, tile.roadHorizontal, tile.roadVertical);
}

QImage hrScene::getImage(const hrObject &object) const
{
    return items_obj.value(object.getName())->getFrame();
}

hrGraphicsItem* hrScene::getItem(const hrObject &object) const
{
    return items_obj.value(object.getName());
}

QVector<hrTile> hrScene::getViewportTiles() const
{
    QVector<hrTile> v;

    for (int y = 0; y < viewport.height(); y++)
        for (int x = 0; x < viewport.width(); x++)
        {
            v.append(tiles.at(viewport.y() + y).at(viewport.x() + x));
        }
    return v;
}

hrTile hrScene::getTile(int x, int y) const
{
    if (y < tiles.size() && x < tiles.at(y).size())
        return tiles.at(y).at(x);
    return hrTile();
}

QLinkedList<hrObject> hrScene::getViewportObjects() const
{
    QLinkedList<hrObject> l;
    QLinkedListIterator<hrObject> it(objects);
    hrObject obj;
    while (it.hasNext())
    {
        obj = it.next();
        if (viewport.intersects(obj.getRect()))
        {            
            l.append(obj);
        }
    }
    return l;
}

QLinkedList<hrObject> hrScene::getAllObjects() const
{
    return objects;
}

QRect hrScene::getSize() const
{
    return size;
}

void hrScene::setSceneViewport(QRect r)
{
    viewport.setRect(r.x() - 2, r.y() - 2, r.width() + 4, r.height() + 4);
    viewport = size.intersected(viewport);
}

QRect hrScene::getSceneViewport() const
{
    return viewport;
}

