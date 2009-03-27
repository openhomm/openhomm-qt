#ifndef HRSCENE_H
#define HRSCENE_H

#include "hrObject.hpp"

class hrScene
{
private:
    QVector<QString> tiles;
    QLinkedList<hrObject*> objects;
    QRect viewport;
    QRect size;

public:
    hrScene();
    ~hrScene();

    void addTile(QString name);
    void addObject(int x, int y, QString name);
    void removeObject(int x, int y);

    void setSize(QRect r);
    QRect getSize();
    void setViewport(QRect r);
    QRect getViewport();

    QVector<QString> getViewportTiles();
    QLinkedList<hrObject*> getViewportObjects();

    void setSizeWindow(QRect);
    QRect getSizeWindow();

public slots:
//mouseClick()
//mouseMove()
};

#endif // HRSCENE_H
