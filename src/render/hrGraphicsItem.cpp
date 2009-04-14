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

hrGraphicsItem::hrGraphicsItem() : curFrame(0), curBlock(0)
{
    isNextFrame = false;
    blocks.append(Block());
}

void hrGraphicsItem::nextFrame()
{
    const Block &b = blocks.at(curBlock);
    if (!isNextFrame)
    {
        curFrame < b.frames.size() - 1 ? curFrame++ : curFrame = 0;
        isNextFrame = true;
    }
}

QImage hrGraphicsItem::getNextFrame()
{
    const Block &b = blocks.at(curBlock);
    curFrame < b.frames.size() - 1 ? curFrame++ : curFrame = 0;
    return b.frames.at(curFrame);
}

QImage hrGraphicsItem::getFrame(int frame, bool horizontal, bool vertical) const
{
    const Block &b = blocks.at(curBlock);
    Q_ASSERT(frame >= 0 && frame < b.frames.size());

    if (horizontal && vertical)
        return b.framesHV.at(frame);
    else if (horizontal)
        return b.framesH.at(frame);
    else if (vertical)
        return b.framesV.at(frame);
    else
        return b.frames.at(frame);
}

QImage hrGraphicsItem::getFrame()
{
    if (isNextFrame)
        isNextFrame = false;
    return blocks.at(curBlock).frames.at(curFrame);
}

void hrGraphicsItem::addImage(QImage im)
{
    rect = im.rect();
    blocks[curBlock].frames.append(im);
}

void hrGraphicsItem::addImageMirrored(QImage im)
{
    rect = im.rect();
    Block &b = blocks[curBlock];
    b.framesHV.append(im.mirrored(true, true));
    b.framesH.append(im.mirrored(true, false));
    b.framesV.append(im.mirrored(false, true));
    b.frames.append(im);
}

void hrGraphicsItem::modifyFrame(QImage im)
{
    blocks[curBlock].frames[curFrame] = im;
}

int hrGraphicsItem::getFramesCount() const
{
    return blocks.at(curBlock).frames.size();
}

const QRect& hrGraphicsItem::getRect() const
{
    return rect;
}

void hrGraphicsItem::addBlock()
{
    curBlock++;
    blocks.append(Block());
}

void hrGraphicsItem::setCurBlock(int i)
{
    Q_ASSERT(i >= 0 && i < blocks.size());
    curBlock = i;
}

int hrGraphicsItem::getBlocksCount() const
{
    return blocks.size();
}
