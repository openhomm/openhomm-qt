#include "hrScene.hpp"

hrScene::hrScene()
{
}

hrScene::~hrScene()
{
}

void hrScene::addTile(QString name)
{
    tiles.append(name);
}

void hrScene::addObject(int x, int y, QString name)
{
    objects.append(new hrObject(x, y, name));
}

void hrScene::removeObject(int x, int y)
{
    //for (int i = 0; objects.at(i)->x == x && objects.at(i)->y == y; i++);
    //objects.removeAt(i);
}

QVector<QString> hrScene::getViewportTiles()
{
    QVector<QString> v;
    for (int i = 0; i < viewport.height(); i++)
        for (int j = 0; j < viewport.width(); j++)
        {
            v.append(tiles.at((viewport.y() + i) * size.width() + (viewport.x() + j)));
        }
    return v;
}


QLinkedList<hrObject*> hrScene::getViewportObjects()
{
    QLinkedList<hrObject*> l;
    QLinkedListIterator<hrObject*> it(objects);
    hrObject *obj = NULL;
    while (it.hasNext())
    {
        obj = it.next();
        if (viewport.contains(obj->point))
        {
            l.append(new hrObject(obj->x - viewport.topLeft().x()
                                  , obj->y - viewport.topLeft().y()
                                  , obj->name)
                     );
        }
    }
    return l;
}

void hrScene::setSize(QRect r)
{
    size = r;
}

QRect hrScene::getSize()
{
    return size;
}

void hrScene::setViewport(QRect r)
{
    viewport = r;
}

QRect hrScene::getViewport()
{
    return viewport;
}

void hrScene::setSizeWindow(QRect r)
{
    ;
}

QRect hrScene::getSizeWindow()
{
    return QRect();
}

