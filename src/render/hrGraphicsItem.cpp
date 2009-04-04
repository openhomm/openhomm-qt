#include "hrGraphicsItem.hpp"

hrGraphicsItem::hrGraphicsItem()
{
    isNextFrame = false;
}

hrGraphicsItem::hrGraphicsItem(int id) : id(id), curFrame(0)
{
    isNextFrame = false;
}

void hrGraphicsItem::nextFrame()
{
    if (!isNextFrame)
    {
        curFrame < frames.size() - 1 ? curFrame++ : curFrame = 0;
        isNextFrame = true;
    }
}

QImage hrGraphicsItem::getNextFrame()
{
    curFrame < frames.size() - 1 ? curFrame++ : curFrame = 0;
    return frames.at(curFrame);
}

QImage hrGraphicsItem::getFrame(int frame, bool horizontal, bool vertical) const
{
    if (frame < frames.size())
    {
        QImage im;
        if (horizontal && vertical)
            im = framesHV.at(frame);
        else if (horizontal)
            im = framesH.at(frame);
        else if (vertical)
            im = framesV.at(frame);
        else
            im = frames.at(frame);
        return im;
    }
    return QImage();
}

QImage hrGraphicsItem::getFrame()
{
    if (isNextFrame)
        isNextFrame = false;
    return frames.at(curFrame);
}

void hrGraphicsItem::addImage(QImage im)
{
    rect = im.rect();
    frames.append(im);
}

void hrGraphicsItem::addImageMirrored(QImage im)
{
    rect = im.rect();
    framesHV.append(im.mirrored(true, true));
    framesH.append(im.mirrored(true));
    framesV.append(im.mirrored(false, true));
    frames.append(im);
}

void hrGraphicsItem::modifyFrame(QImage im)
{
    frames[curFrame] = im;
}

int hrGraphicsItem::getId() const
{
    return id;
}

int hrGraphicsItem::getFramesCount() const
{
    return frames.size();
}

QRect hrGraphicsItem::getRect() const
{
    return rect;
}
