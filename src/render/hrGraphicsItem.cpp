// openhomm - open source clone of Heroes of Might and Magic III
// Copyright (C) 2009 openhomm developers team (see AUTHORS)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#include "hrGraphicsItem.hpp"

hrGraphicsItem::hrGraphicsItem() : curFrame(0)
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
        if (horizontal && vertical)
            return framesHV.at(frame);
        else if (horizontal)
            return framesH.at(frame);
        else if (vertical)
            return framesV.at(frame);
        else
            return frames.at(frame);
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

int hrGraphicsItem::getFramesCount() const
{
    return frames.size();
}

QRect hrGraphicsItem::getRect() const
{
    return rect;
}
