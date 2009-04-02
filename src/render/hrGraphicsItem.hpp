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
    QRect rect;
    qint32 NearestGLTextureSize(qint32 v);
public:
    hrGraphicsItem();
    hrGraphicsItem(QString name);

    QString getName() const;
    void nextFrame();
    QImage getFrame(int frame) const;
    QImage getFrame();
    QRect getRect() const;
    void modifyFrame(QImage im);

    void addImage(QImage im);
};

#endif // HRGRAPHICSITEM_H
