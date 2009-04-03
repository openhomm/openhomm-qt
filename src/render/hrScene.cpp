#include "hrScene.hpp"

void hrScene::addItem(int id, QString name)
{
    if (!items.contains(id))
    {
        hrGraphicsItem *item = new hrGraphicsItem(id);
        QImageReader ir("lod:/data/h3sprite.lod/" + name);
        QImage im;
        for (int i = 0; ir.jumpToImage(i); i++)
            if (ir.read(&im))
                item->addImage(im);

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

void hrScene::addTile(int id, QString name, int frame)
{
    addItem(id, name);

    static int x = 0;
    static int y = 0;

    if (x < size.width())
    {
        tiles[y].append(hrTile(id, frame));
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

void hrScene::addObject(int id, QString name, int x, int y)
{
    addItem(id, name);

    QRect r = items.value(id)->getRect();
    objects.append( hrObject(id, name, x, y
                             , toCell(r.width())
                             , toCell(r.height())
                             )
                   );
}

void hrScene::removeObject(int x, int y)
{
    ;
}

QImage hrScene::getItem(int id, int frame) const
{
    return items.value(id)->getFrame(frame);
}

QImage hrScene::getItem(hrTile &tile) const
{
    return getItem(tile.getId(), tile.getFrame());
}

QImage hrScene::getItem(hrObject &object) const
{
    return items.value(object.getId())->getFrame();
}

void hrScene::setItemNextFrame(hrObject &object) const
{
    items.value(object.getId())->nextFrame();
}

void hrScene::modifyItem(hrObject &object, QImage im) const
{
    items.value(object.getId())->modifyFrame(im);
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

hrTile hrScene::getViewportTile(int x, int y) const
{
    return tiles.at(viewport.y() + y).at(viewport.x() + x);
}

hrTile hrScene::getTile(int x, int y) const
{
    return tiles.at(y).at(x);
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

