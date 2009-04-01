#include "hrGraphicsItem.hpp"

hrGraphicsItem::hrGraphicsItem()
{
    isNextFrame = false;
}

hrGraphicsItem::hrGraphicsItem(QString name) : name(name), curFrame(0)
{
}

void hrGraphicsItem::nextFrame()
{
    if (!isNextFrame)
    {
        curFrame < frames.size() - 1 ? curFrame++ : curFrame = 0;
        isNextFrame = true;
    }
}

QImage hrGraphicsItem::getFrame(int frame) const
{
    if (frame >= 0 && frame < frames.size())
        return frames.at(frame);
    return QImage();
}

QImage hrGraphicsItem::getFrame()
{
    if (isNextFrame)
        isNextFrame = false;
    return frames.at(curFrame);
}

void hrGraphicsItem::addImage(QImage &image)
{
    frames.append(image);
}

QString hrGraphicsItem::getName() const
{
    return name;
}

QRect hrGraphicsItem::getRect() const
{
    if (!frames.isEmpty())
        return frames.at(0).rect();
    return QRect();
}
