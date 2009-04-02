#ifndef HRSCENE_H
#define HRSCENE_H

#include "hrGraphicsItem.hpp"
#include "hrCoord.hpp"

class hrTile
{
    QString name;
    int frame;
public:
    hrTile() {}
    hrTile(QString name, int frame = 0) : name(name), frame(frame) {}
    QString getName() const
    {
        return name;
    }
    int getFrame() const
    {
        return frame;
    }
};

class hrObject
{
    QString name;
    QRect rect;
public:
    hrObject() {}
    hrObject(QString name, QRect rect) : name(name), rect(rect) {}
    hrObject(QString name, int x, int y) : name(name)
    {
        rect.setRect(x, y, 0, 0);
    }
    hrObject(QString name, int x, int y, int width, int height) : name(name)
    {
        rect.setRect(x, y, width, height);
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
    QMap<QString, hrGraphicsItem*> items;
    QRect viewport;
    QRect size;

    void addItem(QString name);
public:
    hrScene(int width, int height);
    ~hrScene();

    void addTile(QString name, int frame = 0);
    void addObject(QString name, int x, int y);
    void removeObject(int x, int y);

    QRect getSize() const;
    void setSceneViewport(QRect r);
    QRect getSceneViewport() const;

    QImage getItem(QString name, int frame) const;
    QImage getItem(hrTile &tile) const;
    QImage getItem(hrObject &object) const;
    void setItemNextFrame(hrObject &object) const;
    void modifyItem(hrObject &object, QImage im);
    QVector<hrTile> getViewportTiles() const;
    hrTile getViewportTile(int x, int y) const;
    hrTile getTile(int x, int y) const;
    QLinkedList<hrObject> getViewportObjects() const;


public slots:
//mouseClick()
//mouseMove()
};

#endif // HRSCENE_H
