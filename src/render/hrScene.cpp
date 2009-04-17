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

void hrScene::CyclShiftPalette(int a, int b, QImage &im)
{
    QRgb c = im.color(b);
    for (int i = b; i > a; i--)
    {
        im.setColor(i, im.color(i - 1));
    }
    im.setColor(a, c);
}

void hrScene::addItem(int id, const QString &name, bool mirrored)
{
    if (!items.contains(id))
    {
        hrGraphicsItem *item = new hrGraphicsItem();
        QImageReader ir("lod:/data/h3sprite.lod/" + name);
        QImage im;
        for (int i = 0; ir.jumpToImage(i); i++)
        {
            if (ir.read(&im))
            {
                mirrored ? item->addImageMirrored(im) : item->addImage(im);

                if (name == "watrtl.def")
                {
                    for (int j = 0; j < 11; j++)
                    {
                        CyclShiftPalette(229, 240, im);
                        CyclShiftPalette(242, 253, im);
                        mirrored ? item->addImageMirrored(im) : item->addImage(im);
                    }
                    item->addBlock();
                }
                else if (name == "clrrvr.def")
                {
                    for (int j = 0; j < 11; j++)
                    {
                        CyclShiftPalette(189, 200, im);
                        mirrored ? item->addImageMirrored(im) : item->addImage(im);
                    }
                    item->addBlock();
                }
                else if (name == "mudrvr.def")
                {
                    for (int j = 0; j < 11; j++)
                    {
                        CyclShiftPalette(228, 239, im);
                        mirrored ? item->addImageMirrored(im) : item->addImage(im);
                    }
                    item->addBlock();
                }
                else if (name == "lavrvr.def")
                {
                    for (int j = 0; j < 8; j++)
                    {
                        CyclShiftPalette(240, 248, im);
                        mirrored ? item->addImageMirrored(im) : item->addImage(im);
                    }
                    item->addBlock();
                }
            }
        }

        items[id] = item;
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
    tiles.append(QVector<hrTile>());
    addItem(0xFF, "default.def");
}

hrScene::~hrScene()
{
    items.clear();
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
    addItem(tile.terrainId, name, true);

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
        addItem(tile.getRiverId(), name, true);
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
        addItem(tile.getRoadId(), name, true);
    }


    static int x = 0;
    static int y = 0;

    if (x < size.width() - 1)
    {
        tiles[y].append(tile);
        x++;
    }
    else
    {
        x = 0;
        tiles[y].append(tile);
        if (y < size.height())
        {
            tiles.append(QVector<hrTile>());
        }
        y++;
    }
}

void hrScene::addObject(hrSceneObject &object)
{
    addItem(object.getName());

    QRect r = items_obj.value(object.getName())->getRect();
    objects.append( hrSceneObject(object.getName()
                             , object.getVisitable()
                             , object.isOverlay()
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

const QImage& hrScene::getImage(int id
                         , int frame
                         , bool horizontal
                         , bool vertical) const
{
    return items.value(id)->getFrame(frame, horizontal, vertical);
}

const QImage& hrScene::getImageTerrain(const hrTile &tile) const
{
    if (tile.terrainId == 8) // "watrtl.def"
    {
        hrGraphicsItem *item = items.value(tile.terrainId);
        item->setCurBlock(tile.terrainFrame);
        return item->getFrame(tile.isTerrainHorizontal(), tile.isTerrainVertical());
    }
    return getImage(tile.terrainId
                    , tile.getTerrainFrame()
                    , tile.isTerrainHorizontal()
                    , tile.isTerrainVertical()
                    );
}

const QImage& hrScene::getImageRiver(const hrTile &tile) const
{
    if (tile.riverId != 2) // not "icyrvr.def"
    {
        hrGraphicsItem *item = items.value(tile.getRiverId());
        item->setCurBlock(tile.riverFrame);
        return item->getFrame(tile.isRiverHorizontal(), tile.isRiverVertical());
    }
    else
    {
        return getImage(tile.getRiverId()
                        , tile.getRiverFrame()
                        , tile.isRiverHorizontal()
                        , tile.isRiverVertical()
                        );
    }
}

const QImage& hrScene::getImageRoad(const hrTile &tile) const
{
    return getImage(tile.getRoadId()
                    , tile.getRoadFrame()
                    , tile.isRoadHorizontal()
                    , tile.isRoadVertical()
                    );
}

const QImage& hrScene::getImage(const hrSceneObject &object) const
{
    return items_obj.value(object.getName())->getFrame();
}

hrGraphicsItem* hrScene::getItem(const hrSceneObject &object) const
{
    return items_obj.value(object.getName());
}

hrGraphicsItem* hrScene::getItem(int id) const
{
    return items.value(id);
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

const hrTile& hrScene::getTile(int x, int y) const
{
    Q_ASSERT(y < tiles.size() && x < tiles.at(y).size());
    return tiles.at(y).at(x);
}

QList<hrSceneObject> hrScene::getViewportObjects() const
{
    QList<hrSceneObject> l;
    QListIterator<hrSceneObject> it(objects);
    hrSceneObject obj;
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
