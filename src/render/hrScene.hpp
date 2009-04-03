#ifndef HRSCENE_H
#define HRSCENE_H

#include "hrGraphicsItem.hpp"
#include "hrCoord.hpp"

class hrTile
{
    int id;
    int frame;
public:
    hrTile() {}
    hrTile(int id, int frame = 0) : id(id), frame(frame) {}
    int getId() const
    {
        return id;
    }
    int getFrame() const
    {
        return frame;
    }
};

class hrObject
{
    int id;
    QString name;
    QRect rect;
public:
    hrObject() {}
    hrObject(int id, QString name, QRect rect) : id(id), name(name), rect(rect) {}
    hrObject(int id, QString name, int x, int y, int width, int height) : id(id), name(name)
    {
        rect.setRect(x, y, width, height);
    }
    hrObject(int id, QString name, int x, int y) : id(id), name(name)
    {
        rect.setX(x);
        rect.setY(y);
    }
    int getId() const
    {
        return id;
    }
    QString getName() const
    {
        return name;
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
private:
    QVector< QVector<hrTile> > tiles;
    QLinkedList<hrObject> objects;
    QMap<int, hrGraphicsItem*> items;
    QRect viewport;
    QRect size;

    void addItem(int id, QString name);
public:
    hrScene(int width, int height);
    ~hrScene();

    void addTile(int id, QString name, int frame = 0);
    void addObject(int id, QString name, int x, int y);
    void removeObject(int x, int y);

    QRect getSize() const;
    void setSceneViewport(QRect r);
    QRect getSceneViewport() const;

    QImage getItem(int id, int frame) const;
    QImage getItem(hrTile &tile) const;
    QImage getItem(hrObject &object) const;
    void setItemNextFrame(hrObject &object) const;
    void modifyItem(hrObject &object, QImage im) const;
    QVector<hrTile> getViewportTiles() const;
    hrTile getViewportTile(int x, int y) const;
    hrTile getTile(int x, int y) const;
    QLinkedList<hrObject> getViewportObjects() const;


public slots:
//mouseClick()
//mouseMove()
};

#endif // HRSCENE_H
