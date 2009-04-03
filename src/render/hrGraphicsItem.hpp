#ifndef HRGRAPHICSITEM_H
#define HRGRAPHICSITEM_H

#include "precompiled.hpp"

class hrGraphicsItem
{
private:
    int id;
    //QString name;
    QVector<QImage> frames;
    int curFrame;
    bool isNextFrame;
    QRect rect;
public:
    hrGraphicsItem();
    hrGraphicsItem(int id);

    int getId() const;
    void nextFrame();
    QImage getFrame(int frame) const;
    QImage getFrame();
    QRect getRect() const;
    void modifyFrame(QImage im);

    void addImage(QImage im);
};

#endif // HRGRAPHICSITEM_H
