#include "hrScene.hpp"

void hrScene::addItem(QString name)
{
    if (!items.contains(name))
    {
        hrGraphicsItem *item = new hrGraphicsItem(name);
        QImageReader ir("lod:/data/h3sprite.lod/" + name);
        QImage im;
        for (int i = 0; ir.jumpToImage(i); i++)
            if (ir.read(&im))
                item->addImage(im);

        items[name] = item;
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

void hrScene::addTile(QString name, int frame)
{
    addItem(name);

    static int x = 0;
    static int y = 0;

    if (x < size.width())
    {
        tiles[y].append(hrTile(name, frame));
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

void hrScene::addObject(QString name, int x, int y)
{
    addItem(name);

    QRect r = items.value(name)->getRect();
    objects.append( hrObject(name, x, y
                             , coord::toCell(r.width())
                             , coord::toCell(r.height())
                             )
                   );
}

void hrScene::removeObject(int x, int y)
{
    ;
}

QImage hrScene::getItem(QString name, int frame) const
{
    if (items.contains(name))
        return items.value(name)->getFrame(frame);
    return QImage();
}

QImage hrScene::getItem(hrTile &tile) const
{
    return getItem(tile.getName(), tile.getFrame());
}

QImage hrScene::getItem(hrObject &object) const
{
    if (items.contains(object.getName()))
        return items.value(object.getName())->getFrame();
    return QImage();
}

void hrScene::setItemNextFrame(hrObject &object) const
{
    if (items.contains(object.getName()))
       items.value(object.getName())->nextFrame();
}

void hrScene::modifyItem(hrObject &object, QImage im)
{
    if (items.contains(object.getName()))
        items.value(object.getName())->modifyFrame(im);
}

QVector<hrTile> hrScene::getViewportTiles() const
{
    QVector<hrTile> v;

    for (int i = 0; i < viewport.height(); i++)
        for (int j = 0; j < viewport.width(); j++)
        {
            v.append(tiles.at(viewport.y() + i).at(viewport.x() + j));
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
            l.append(hrObject(obj.getName()
                              , obj.x()
                              , obj.y()
                              )
                     );
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

