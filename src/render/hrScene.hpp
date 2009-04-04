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
    QPixmap getCursor(int frame);

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
    QVector<QPixmap> cursor;
    QRect viewport;
    QRect size;

    void addItem(int id, QString name, bool mirrored = false);
};

#endif // HRSCENE_H
