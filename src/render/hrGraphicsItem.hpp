#ifndef HRGRAPHICSITEM_H
#define HRGRAPHICSITEM_H

#include "precompiled.hpp"

class hrGraphicsItem
{
private:
    QString name;
    QVector<QImage> frames;
    int curFrame;
    bool isNextFrame;
public:
    hrGraphicsItem();
    hrGraphicsItem(QString name);

    QString getName() const;
    void nextFrame();
    QImage getFrame(int frame) const;
    QImage getFrame();
    QRect getRect() const;

    void addImage(QImage &image);
};

#endif // HRGRAPHICSITEM_H
