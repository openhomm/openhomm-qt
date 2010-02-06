// openhomm - open source clone of Heroes of Might and Magic III
// Copyright (C) 2009-2010 openhomm developers team (see AUTHORS)
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
#include "hrDefHandler.hpp"
#include <QtDebug>

int hrDefHandler::imageCount() const
{
    return countFrames;
}

int hrDefHandler::currentImageNumber() const
{
    return curFrame;
}

bool hrDefHandler::jumpToImage(int imageNumber)
{
    if (!readHeader())
        return false;

    if (imageNumber < 0)
    {
        if (!blocks.contains(qAbs(imageNumber) - 1))
            return false;
        curBlock = qAbs(imageNumber) - 1;
        curFrame = 0;
        countFrames = blocks[curBlock].countFrames;
    }
    else if (imageNumber >= 0 && imageNumber < countFrames)
        curFrame = imageNumber;
    else
        return false;
    return true;
}

bool hrDefHandler::jumpToNextImage()
{
    if (!readHeader())
        return false;

    curFrame < countFrames - 1 ? curFrame++ : curFrame = 0;
    return true;
}

bool hrDefHandler::readPalette()
{
    quint8 buf[256 * 3];
    if (device()->read((char*)buf, 256 * 3) == 256 * 3)
    {
        QRgb rgb;
        // first color is alpha
        rgb = qRgba(0, 0, 0, 0);
        colors.append(rgb);
        // second is light shadow
        rgb = qRgba(0, 0, 0, 64);
        colors.append(rgb);
        // only used in Tshre.def and AvGnoll.def
        rgb = /*qRgba(0, 0, 0, 128);*/qRgb(buf[6], buf[7], buf[8]);
        colors.append(rgb);
        // only used in Tshre.def
        rgb = /*qRgba(0, 0, 0, 64);*/qRgb(buf[9], buf[10], buf[11]);
        colors.append(rgb);
        // strong shadow
        rgb = qRgba(0, 0, 0, 128);
        colors.append(rgb);

        for(int i = 5, j = 15; i < 256; i++, j += 3)
        {
            rgb = qRgb(buf[j], buf[j + 1], buf[j + 2]);
            colors.append(rgb);
        }

        return true;
    }
    return false;
}

bool hrDefHandler::readBlockHeaders()
{
    BlockHeader bh;
    for (int i = 0; i < countBlocks; i++)
    {
        Block block;
        if (device()->read((char*)&bh, sizeof(bh)) == sizeof(bh))
        {
            block.countFrames = bh.countFrames;
            if (i == 0)
                countFrames = bh.countFrames;

            if (!device()->seek(device()->pos() + bh.countFrames * 13)) // skip names
                return false;

            int cnt = bh.countFrames;
            quint32 *offsets = new quint32[cnt];
            if (device()->read((char*)offsets, cnt * 4) == cnt * 4)
            {
                for (int j = 0; j < cnt; j++)
                    block.offsets.append(offsets[j]);

                blocks[bh.index] = block;
            }
            else
            {
                delete [] offsets;
                return false;
            }
            delete [] offsets;
        }
    }
    return true;
}

bool hrDefHandler::readHeader()
{
    if (isHeaderRead)
        return true;

    DefHeader dh;
    if (device()->read((char*)&dh, sizeof(dh)) == sizeof(dh))
    {
        width = dh.width;
        height = dh.height;
        countBlocks = dh.countBlocks;

        if (readPalette() && readBlockHeaders())
        {
            isHeaderRead = true;
            return true;
        }
    }
    return false;
}

bool hrDefHandler::checkFrame(const FrameHeader &fh) const
{
    if (fh.marginLeft + fh.widthFrame <= fh.widthFull
        && fh.marginTop + fh.heightFrame <= fh.heightFull)
        return true;
    else
        return false;
}

bool hrDefHandler::readFrame0(quint8 *imageBuffer, const FrameHeader &fh) const
{
    if (device()->read((char*)imageBuffer, fh.size) == fh.size)
    {
        return true;
    }
    return false;
}

void hrDefHandler::fillFrameBorders(quint8 *imageBuffer, const FrameHeader &fh) const
{
    if (fh.widthFrame == fh.widthFull
        && fh.heightFrame == fh.heightFull)
        return;
    quint8 *line = imageBuffer;
    quint32 i = 0;
    quint32 j = 0;

    for (; i < fh.marginTop; i++)
        for (j = 0; j < fh.widthFull; j++)
            *line++ = 0;

    for (; i < fh.marginTop + fh.heightFrame; i++)
    {
        for (j = 0; j < fh.marginLeft; j++)
            *line++ = 0;
        line += fh.widthFrame;
        for (j += fh.widthFrame; j < fh.widthFull; j++)
            *line++ = 0;
    }

    for (; i < fh.heightFull; i++)
        for (j = 0; j < fh.widthFull; j++)
            *line++ = 0;
}

bool hrDefHandler::readFrame1(quint8 *imageBuffer, quint8 *buf, const FrameHeader & fh) const
{
    quint32 *offsets = (quint32*)&buf[0];

    for (quint32 curLine = 0; curLine < fh.heightFrame; curLine++)
    {
        int offsetImageBuffer = (fh.marginTop + curLine) * fh.widthFull + fh.marginLeft;

        quint8 *imageLine = imageBuffer + offsetImageBuffer;
        quint32 offsetImageLine = 0;

        if (offsets[curLine] > fh.size)
            return false;

        quint8 *line = buf + offsets[curLine];

        quint32 lenLine = 0;
        if (curLine < fh.heightFrame - 1)
            lenLine = offsets[curLine + 1] - offsets[curLine];
        else
            lenLine = fh.size - offsets[curLine];

        if (offsets[curLine] + lenLine > fh.size)
            return false;

        quint32 offset = 0;

        while (offset < lenLine)
        {
            quint8 typeSegment = line[offset];
            quint32 lenSegment = line[++offset] + 1;
            if (offsetImageLine + lenSegment > fh.widthFrame)
            {
                qWarning("offsetImageLine + lenSegment > fh.widthFrame");
                return false;
            }

            if (typeSegment == 0xFF)
            {
                offset++;
                for (quint32 k = 0; k < lenSegment; k++)
                {
                    imageLine[offsetImageLine++] = line[offset++];
                }
            }
            else
            {
                for (quint32 k = 0; k < lenSegment; k++)
                {
                    imageLine[offsetImageLine++] = typeSegment;
                }
                offset++;
            }
        }
    }
    return true;
}

bool hrDefHandler::readFrame2(quint8 *imageBuffer, quint8 *buf, const FrameHeader & fh) const
{
    quint16 *offsets = (quint16*)&buf[0];

    for (quint32 curLine = 0; curLine < fh.heightFrame; curLine++)
    {
        quint32 offsetImageBuffer = (fh.marginTop + curLine) * fh.widthFull + fh.marginLeft;

        quint8 *imageLine = imageBuffer + offsetImageBuffer;
        quint32 offsetImageLine = 0;

        if (offsets[curLine] > fh.size)
            return false;

        quint8 *line = buf + offsets[curLine];

        quint32 lenLine = 0;
        if (curLine < fh.heightFrame - 1)
            lenLine = offsets[curLine + 1] - offsets[curLine];
        else
            lenLine = fh.size - offsets[curLine];

        if (offsets[curLine] + lenLine > fh.size)
            return false;

        quint32 offset = 0;

        while (offset < lenLine)
        {
            int typeSegment = line[offset] >> 5;
            int lenSegment = (line[offset] & 0x1F) + 1;
            if (offsetImageLine + lenSegment > fh.widthFrame)
                return false;

            if (typeSegment == 0x07)
            {
                offset++;
                for (int k = 0; k < lenSegment; k++)
                {
                    imageLine[offsetImageLine++] = line[offset++];
                }
            }
            else
            {
                for (int k = 0; k < lenSegment; k++)
                {
                    imageLine[offsetImageLine++] = typeSegment;
                }
                offset++;
            }
        }
    }
    return true;
}

bool hrDefHandler::readFrame3(quint8 *imageBuffer, quint8 *buf, const FrameHeader &fh) const
{
    quint16 *offsets = (quint16*)&buf[0];

    int offsetImageBuffer = fh.marginTop * fh.widthFull + fh.marginLeft;
    quint8 *imageLine = imageBuffer + offsetImageBuffer;
    quint32 curImageBufferLine = 0;
    quint32 offsetImageLine = 0;

    int countLines = fh.heightFrame * fh.widthFrame / 32;

    for (int curLine = 0; curLine < countLines; curLine++)
    {
        if (offsetImageLine == fh.widthFrame)
        {
            curImageBufferLine++;
            if (curImageBufferLine > fh.heightFrame)
                return false;

            offsetImageBuffer = (fh.marginTop + curImageBufferLine) * fh.widthFull + fh.marginLeft;
            imageLine = imageBuffer + offsetImageBuffer;
            offsetImageLine = 0;
        }

        if (offsets[curLine] > fh.size)
            return false;

        quint8 *line = buf + offsets[curLine];

        quint32 lenLine = 0;
        if (curLine < countLines - 1)
            lenLine = offsets[curLine + 1] - offsets[curLine];
        else
            lenLine = fh.size - offsets[curLine];

        if (offsets[curLine] + lenLine > fh.size)
            return false;

        quint32 offset = 0;

        while (offset < lenLine)
        {
            int typeSegment = line[offset] >> 5;
            int lenSegment = (line[offset] & 0x1F) + 1;
            if (offsetImageLine + lenSegment > fh.widthFrame)
                return false;

            if (typeSegment == 0x07)
            {
                offset++;
                for (int k = 0; k < lenSegment; k++)
                {
                    imageLine[offsetImageLine++] = line[offset++];
                }
            }
            else
            {
                for (int k = 0; k < lenSegment; k++)
                {
                    imageLine[offsetImageLine++] = typeSegment;
                }
                offset++;
            }
        }
    }
    return true;
}


bool hrDefHandler::read(QImage *image)
{
    if (!readHeader())
        return false;

    if (!device()->seek(blocks[curBlock].offsets.at(curFrame)))
        return false;

    FrameHeader fh;
    if (device()->read((char*)&fh, sizeof(fh)) != sizeof(fh))
        return false;

    bool isFrameRead = false;
    if (checkFrame(fh))
    {
        quint8 *imageBuffer = new quint8[fh.widthFull * fh.heightFull];

        if (fh.type == 0)
        {
            isFrameRead = readFrame0(imageBuffer, fh);
        }
        else
        {
            fillFrameBorders(imageBuffer, fh);
            quint8 *buf = new quint8[fh.size];
            if (device()->read((char*)buf, fh.size) == fh.size)
            {
                if (fh.type == 1)
                {
                    isFrameRead = readFrame1(imageBuffer, buf, fh);
                }
                else if (fh.type == 2)
                {
                    isFrameRead = readFrame2(imageBuffer, buf, fh);
                }
                else if (fh.type == 3)
                {
                    isFrameRead = readFrame3(imageBuffer, buf, fh);
                }
            }
            delete [] buf;
        }
        if (isFrameRead)
        {
            QImage im(imageBuffer, fh.widthFull, fh.heightFull, fh.widthFull, QImage::Format_Indexed8);
            *image = im;
            image->setColorTable(colors);
        }
        else
        {
            *image = QImage();
        }
        delete [] imageBuffer;
    }
    return isFrameRead;
}

bool hrDefHandler::canRead(QIODevice *device)
{
    if (!device)
    {
        qWarning("hrDefHandler::canRead() empty device");
        return false;
    }
    if (device->isSequential())
    {
        qWarning("hrDefHandler::canRead() random access devices only");
        return false;
    }

    bool isProbablyDef = false;
    DefHeader dh;
    if (device->peek((char*)&dh, sizeof(dh)) == sizeof(dh))
    {
        if (dh.type >= 64 && dh.type <= 73 // from tests
            && dh.width <= 800
            && dh.height <= 600 // heroes 3 original resolution
            && dh.countBlocks < 0xFFFF // probably..
            )
        {
            isProbablyDef = true;
        }
    }
    return isProbablyDef;
}


hrDefHandler::hrDefHandler() :
        curFrame(0)
        , countFrames(0)
        , curBlock(0)
        , countBlocks(0)
        , height(0)
        , width(0)
        , isHeaderRead(false)
{
}

hrDefHandler::~hrDefHandler()
{
}

bool hrDefHandler::write(const QImage &)
{
    qWarning("%s: not supported", Q_FUNC_INFO);
    return false;
}

bool hrDefHandler::canRead() const
{
    return canRead(device());
}

QVariant hrDefHandler::option(ImageOption option) const
{
    return QVariant();
}

void hrDefHandler::setOption(ImageOption, const QVariant &)
{
    qWarning("%s: not supported", Q_FUNC_INFO);
}

bool hrDefHandler::supportsOption(ImageOption option) const
{
    return false;
}

QByteArray hrDefHandler::name() const
{
    return "def";
}
