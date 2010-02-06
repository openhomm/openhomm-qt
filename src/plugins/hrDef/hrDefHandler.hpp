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
#pragma once

#include <qimageiohandler.h>
#include <QImage>
#include <QVector>
#include <QRgb>
#include <QVariant>

struct DefHeader
{
    quint32 type;
    quint32 width;
    quint32 height;
    quint32 countBlocks;
};

struct BlockHeader
{
    quint32 index;
    /*
      0 - moving
      1 - mouse over
      02 - standing
      03 - getting hit
      04 - defend
      05 - death
      06 - ?
      07 - turn left
      08 - turn right
      09 - turn left
      10 - turn right
      11 - attack up
      12 - attack straight
      13 - attack down
      14 - shoot up
      15 - shoot straight
      16 - shoot down
      17 - 2 hex attack up
      18 - 2 hex attack straight
      19 - 2 hex attack down
      20 - start moving
      21 - stop moving
      */
    quint32 countFrames;
    quint8 junk[8];
};


struct FrameHeader
{
    quint32 size;
    quint32 type;
    quint32 widthFull;
    quint32 heightFull;
    quint32 widthFrame;
    quint32 heightFrame;
    quint32 marginLeft;
    quint32 marginTop;
};

struct Block
{
    int countFrames;
    QVector<quint32> offsets;
};

class hrDefHandler : public QImageIOHandler
{
public:
    hrDefHandler();
    ~hrDefHandler();

    virtual bool canRead() const;
    virtual bool read(QImage *image);
    virtual bool write(const QImage &image);
    virtual int currentImageNumber() const;
    virtual int imageCount() const;
    virtual bool jumpToImage(int imageNumber);
    virtual bool jumpToNextImage();

    static bool canRead(QIODevice *device);

    QVariant option(ImageOption option) const;
    void setOption(ImageOption option, const QVariant &value);
    bool supportsOption(ImageOption option) const;

    virtual QByteArray name() const;

private:

    bool readHeader();
    bool readPalette();
    bool readBlockHeaders();

    bool readFrame0(quint8 *imageBuffer, const FrameHeader &fh) const;
    bool readFrame1(quint8 *imageBuffer, quint8 *buf, const FrameHeader &fh) const;
    bool readFrame2(quint8 *imageBuffer, quint8 *buf, const FrameHeader &fh) const;
    bool readFrame3(quint8 *imageBuffer, quint8 *buf, const FrameHeader &fh) const;

    bool checkFrame(const FrameHeader &fh) const;
    void fillFrameBorders(quint8 *imageBuffer, const FrameHeader &fh) const;

    QMap<int, Block> blocks;
    QVector<QRgb> colors;
    int curFrame;
    int countFrames;
    int curBlock;
    int countBlocks;
    int height;
    int width;

    bool isHeaderRead;
};

