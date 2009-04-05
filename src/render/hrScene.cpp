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

void hrScene::addItem(int id, QString path, bool mirrored)
{
    if (!items.contains(id))
    {
        hrGraphicsItem *item = new hrGraphicsItem(id);
        QImageReader ir(path);
        QImage im;
        for (int i = 0; ir.jumpToImage(i); i++)
            if (ir.read(&im))
            {
                mirrored ? item->addImageMirrored(im) : item->addImage(im);
            }

        items[id] = item;
    }
}

hrScene::hrScene(int width, int height)
{
    size.setRect(0, 0, width - 1, height - 1);
    tiles.append(QVector<hrTile>());
}

hrScene::~hrScene()
{
    items.clear();
}

void hrScene::addTile(int id
                     , int frame
                     , QString path
                     , bool horizontal
                     , bool vertical)
{
    addItem(id, path, true);

    static int x = 0;
    static int y = 0;

    if (x < size.width())
    {
        tiles[y].append(hrTile(id, frame, horizontal, vertical));
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

void hrScene::addTileSecondLayer(int id, int frame, int x, int y, QString path)
{
    addItem(id, path);

    tilesSecondLayer.append(hrTile(id, frame, x, y));
}

void hrScene::addObject(int id, int x, int y, QString path)
{
    addItem(id, path);

    QRect r = items.value(id)->getRect();
    objects.append( hrObject(id, x, y
                             , toCell(r.width())
                             , toCell(r.height())
                             )
                   );
}


void hrScene::removeObject(int x, int y)
{
    ;
}

void hrScene::setCursor(QString path)
{
    QImageReader ir(path);
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

QImage hrScene::getImage(const hrTile &tile) const
{
    return items.value(tile.getId())->getFrame(tile.getFrame()
                                               , tile.getHorizontal()
                                               , tile.getVertical()
                                               );
}

QImage hrScene::getImage(const hrObject &object) const
{
    return items.value(object.getId())->getFrame();
}

hrGraphicsItem* hrScene::getItem(const hrObject &object) const
{
    return items.value(object.getId());
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

QLinkedList<hrTile> hrScene::getViewportTilesSecondLayer() const
{
    QLinkedList<hrTile> l;
    QLinkedListIterator<hrTile> it(tilesSecondLayer);
    hrTile tile;
    while (it.hasNext())
    {
        tile = it.next();
        if (viewport.contains(tile.getPoint()))
        {
            l.append(tile);
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

