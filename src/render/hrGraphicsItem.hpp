#ifndef HRGRAPHICSITEM_H
#define HRGRAPHICSITEM_H

#include "precompiled.hpp"

class hrGraphicsItem
{
private:
    int id;
    QVector<QImage> frames;
    QVector<QImage> framesH;
    QVector<QImage> framesV;
    QVector<QImage> framesHV;
    int curFrame;
    bool isNextFrame;
    QRect rect;
public:
    hrGraphicsItem();
    hrGraphicsItem(int id);

    int getId() const;
    int getFramesCount() const;
    void nextFrame();
    QImage getNextFrame();
    QImage getFrame(int frame, bool horizontal = false, bool vertical = false) const;
    QImage getFrame();
    QRect getRect() const;
    void modifyFrame(QImage im);

    void addImage(QImage im);
    void addImageMirrored(QImage im);
};

#endif // HRGRAPHICSITEM_H
