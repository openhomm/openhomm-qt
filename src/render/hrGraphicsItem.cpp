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
#include "precompiled.hpp"
#include "hrGraphicsItem.hpp"

/*!
  \class hrGraphicsItem
  \brief The hrGraphicsItem class
*/
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

const QImage& hrGraphicsItem::getNextFrame()
{
    const Block &b = blocks.at(curBlock);
    curFrame < b.frames.size() - 1 ? curFrame++ : curFrame = 0;
    return b.frames.at(curFrame);
}

const QImage& hrGraphicsItem::getFrame(int frame) const
{
    const Block &b = blocks.at(curBlock);
    Q_ASSERT(frame >= 0 && frame < b.frames.size());

    return b.frames.at(frame);
}

const QImage& hrGraphicsItem::getFrame()
{
    if (isNextFrame)
        isNextFrame = false;
    return blocks.at(curBlock).frames.at(curFrame);
}

void hrGraphicsItem::addImage(const QImage &im)
{
    rect = im.rect();
    blocks[curBlock].frames.append(im);
}

void hrGraphicsItem::modifyFrame(const QImage& im)
{
    rect = im.rect();
    blocks[curBlock].frames.replace(curFrame, im);
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
