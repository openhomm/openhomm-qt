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

void hrScene::addTileItem(int id, const QString &name)
{
    if (!items_atlas.contains(id))
    {
        int animation = 0;
        int dim = 256;
        if (name == "watrtl.def")
        {
            animation = hrTileAtlas::watrtl;
        }
        else if (name == "clrrvr.def")
        {
            animation = hrTileAtlas::clrrvr;
            dim = 128;
        }
        else if (name == "mudrvr.def")
        {
            animation = hrTileAtlas::mudrvr;
            dim = 128;
        }
        else if (name == "lavrvr.def")
        {
            animation = hrTileAtlas::lavrvr;
            dim = 128;
        }
        else if (   name == "grastl.def"
                 || name == "lavatl.def"
                 || name == "rougtl.def"
                 || name == "snowtl.def"
                 || name == "subbtl.def"
                 || name == "swmptl.def" )
        {
            dim = 512;
        }

        hrTileAtlas *atlas = new hrTileAtlas(dim);
        QImageReader ir("lod:/data/h3sprite.lod/" + name);
        QImage im;
        for (int i = 0; ir.jumpToImage(i); i++)
            if (ir.read(&im))
            {
                atlas->addImage(im);
            }

        atlas->animate(animation);

        items_atlas[id] = atlas;
    }
}

void hrScene::addItem(const QString &name)
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
    size.setRect(0, 0, width, height);
}

hrScene::~hrScene()
{
}

void hrScene::addTile(const hrTile &tile)
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
    addTileItem(tile.terrainId, name);

    if (tile.hasRiver())
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
        addTileItem(tile.getRiverId(), name);
    }

    if (tile.hasRoad())
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
        addTileItem(tile.getRoadId(), name);
    }

    tiles.append(tile);
}

void hrScene::addObject(hrSceneObject &object)
{
    addItem(object.getName());

    QRect r = coord::toCell( items_obj.value(object.getName())->getRect() );
    QPoint p = object.getRect().bottomRight();
    object.setRect(p.x() - r.width() + 1
                   , p.y() - r.height() + 1
                   , r.width()
                   , r.height()
                   );

    objects.append(object);
}


void hrScene::removeObject(int x, int y)
{
    ;
}

void hrScene::setCursor(const QString &name)
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

const QImage& hrScene::getImage(const hrSceneObject &object) const
{
    return items_obj.value(object.getName())->getFrame();
}

hrGraphicsItem* hrScene::getItem(const hrSceneObject &object) const
{
    return items_obj.value(object.getName());
}

QList<hrSceneTile> hrScene::getViewportTiles() const
{
    QList<hrSceneTile> lst;

    for (int i = 0; i < viewport.height(); i++)
        for (int j = 0; j < viewport.width(); j++)
        {
            int x = viewport.x() + j;
            int y = viewport.y() + i;

            const hrTile &tile = tiles.at(y * size.width() + x);

            lst.append(hrSceneTile(tile.terrainId
                                 , tile.getTerrainFrame()
                                 , coord::toPix(x)
                                 , coord::toPix(y)
                                 , tile.isTerrainHorizontal()
                                 , tile.isTerrainVertical()
                                 )
                     );
            if (tile.hasRiver())
            {
                lst.append(hrSceneTile(tile.getRiverId()
                                     , tile.getRiverFrame()
                                     , coord::toPix(x)
                                     , coord::toPix(y)
                                     , tile.isRiverHorizontal()
                                     , tile.isRiverVertical()
                                     )
                         );
            }
            if (tile.hasRoad())
            {
                lst.append(hrSceneTile(tile.getRoadId()
                                     , tile.getRoadFrame()
                                     , coord::toPix(x)
                                     , coord::toPix(y) + coord::dim / 2
                                     , tile.isRoadHorizontal()
                                     , tile.isRoadVertical()
                                     )
                         );
            }
        }

    qStableSort(lst);
    return lst;
}

hrTileAtlas* hrScene::getAtlas(const hrSceneTile &tile) const
{
    return items_atlas.value(tile.getId());
}

const hrTile& hrScene::getTile(int x, int y) const
{
    Q_ASSERT(x >= 0 && y >= 0 && x < size.width() && y < size.height());
    return tiles.at(y * size.width() + x);
}

QList<hrSceneObject> hrScene::getViewportObjects() const
{
    QList<hrSceneObject> lst;
    QListIterator<hrSceneObject> it(objects);
    hrSceneObject obj;
    while (it.hasNext())
    {
        obj = it.next();
        if (viewport.intersects(obj.getRect()))
        {            
            lst.append(obj);
        }
    }
    return lst;
}

QList<hrSceneObject> hrScene::getAllObjects() const
{
    return objects;
}

QRect hrScene::getSize() const
{
    return size;
}

void hrScene::setSceneViewport(const QRect &r)
{
    viewport.setRect(r.x() - 2, r.y() - 2, r.width() + 4, r.height() + 4);
    viewport = size.intersected(viewport);
}

QRect hrScene::getSceneViewport() const
{
    return viewport;
}

void hrScene::sortObjects()
{
    qStableSort(objects.begin(), objects.end());
}
