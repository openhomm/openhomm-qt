#include "hrDefHandler.hpp"
#include <QImage>
#include <QtEndian>
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
    quint32 countFrames;
    char junk[8];
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

class DefReader
{
public:
    DefReader(QIODevice *device);
    ~DefReader();
    bool read(QImage *image);
    int currentImageNumber() const;
    bool jumpToImage(int imageNumber);
    bool jumpToNextImage();

    int count();
    int getWidth();
    int getHeight();

private:
    bool readHeader();
    bool readPalette();
    bool readBlockHeaders();

    bool readFrame0(QImage & image, FrameHeader & fh);
    bool readFrame1(QImage & image, FrameHeader & fh);

    QIODevice *dev;

    QVector<Block> blocks;
    QVector<QRgb> colors;
    uchar *imageBuffer;
    int curFrame;
    int countFrames;
    int curBlock;
    int countBlocks;
    int height;
    int width;

    bool isHeaderRead;
};

DefReader::DefReader(QIODevice *device)
{
    dev = device;
    imageBuffer = NULL;
    curFrame = 0;
    countFrames = 0;
    curBlock = 0;
    countBlocks = 0;
    height = 0;
    width = 0;
    isHeaderRead = false;
}

DefReader::~DefReader()
{
    if (!imageBuffer) delete imageBuffer;
}

int DefReader::count()
{
    if (countFrames == 0 && !isHeaderRead)
        if (readHeader())
            isHeaderRead = true;
    return countFrames;
}

int DefReader::getWidth()
{
    if (!isHeaderRead)
        readHeader();
    return width;
}

int DefReader::getHeight()
{
    if (!isHeaderRead)
        readHeader();
    return width;
}

int DefReader::currentImageNumber() const
{
    return curFrame;
}

bool DefReader::jumpToImage(int imageNumber)
{
    if (!count())
        return false;

    if (imageNumber < 0)
    {
        if (abs(imageNumber) > countBlocks)
            return false;
        curBlock = abs(imageNumber) - 1;
        curFrame = 0;
        countFrames = blocks[curBlock].countFrames;
    }
    else if (imageNumber >= 0 && imageNumber < countFrames)
        curFrame = imageNumber;
    else return false;
    return true;
}

bool DefReader::jumpToNextImage()
{
    if (!count())
        return false;

    curFrame < countFrames ? curFrame++ : curFrame = 0;
    return true;
}

bool DefReader::readPalette()
{
    uchar buf[256 * 3];
    if (dev->read((char*)buf, 256 * 3) == 256 * 3)
    {
        QRgb rgb;
        for(int i = 0, j = 0; i < 256; i++, j += 3)
        {
            rgb = qRgb(buf[j], buf[j + 1], buf[j + 2]);
            colors.append(rgb);
        }

        return true;
    }
    return false;
}

bool DefReader::readBlockHeaders()
{
    BlockHeader bh;
    Block block;
    for (int i = 0; i < countBlocks; i++)
    {
        if (dev->read((char*)&bh, sizeof(bh)) == sizeof(bh))
        {
            block.countFrames = bh.countFrames;

            if (!dev->seek(dev->pos() + bh.countFrames * 13)) // skip names
                return false;

            int cnt = bh.countFrames;
            quint32 *offsets = new quint32[cnt];
            if (dev->read((char*)offsets, cnt * 4) == cnt * 4)
            {
                for (int j = 0; j < cnt; j++)
                    block.offsets.append(offsets[j]);
                blocks.append(block);

                countFrames = blocks[0].countFrames;
            }
            else
            {
                delete [] offsets;
                return false;
            }
            delete [] offsets;
        }
        else return false;
    }
    return true;
}

bool DefReader::readHeader()
{
    DefHeader dh;
    if (dev->read((char*)&dh, sizeof(dh)) == sizeof(dh))
    {
        width = dh.width;
        height = dh.height;
        countBlocks = dh.countBlocks;

        return readPalette() && readBlockHeaders();
    }
    return false;
}

bool DefReader::readFrame0(QImage & image, FrameHeader & fh)
{
    if (!imageBuffer) delete imageBuffer;
    imageBuffer = new uchar[fh.size];

    if (dev->read((char*)imageBuffer, fh.size) == fh.size)
    {
        image = QImage(imageBuffer, width, height, QImage::Format_Indexed8);
        image.setColorTable(colors);
    }
    else return false;
    return true;
}

bool DefReader::readFrame1(QImage & image, FrameHeader & fh)
{
    int sizeFull = fh.widthFull * fh.heightFull;

    if (!imageBuffer) delete imageBuffer;
    imageBuffer = new uchar[sizeFull];

    memset(imageBuffer, 0, sizeFull);

    uchar *buf = new uchar[fh.size];
    dev->read((char*)buf, fh.size);

    int numScanLines = fh.heightFrame;
    quint32 *offsets = (quint32*)&buf[0];

    for (int i = 0; i < numScanLines; i++)
    {
        uchar *imageLine = imageBuffer + (fh.marginTop + i) * fh.widthFull;
        imageLine += fh.marginLeft;

        uchar *line = &buf[offsets[i]];

        int lenLine = 0;
        if (i < numScanLines - 1)
            lenLine = offsets[i + 1] - offsets[i];
        else
            lenLine = fh.size - offsets[i];

        for (int j = 0; j < lenLine; j++)
        {
            uchar typeSegment = line[j];
            if (typeSegment == 0xFF)
            {
                uchar lenSegment = line[++j];
                for (uchar k = 0; k < lenSegment; k++)
                {
                    *imageLine++ = line[++j];
                }
            }
            else if (typeSegment == 0x00)
            {
                uchar lenSegment = line[++j];
                for (uchar k = 0; k < lenSegment; k++)
                {
                    *imageLine++ = 0;
                }
            }
        }
    }
    image = QImage(imageBuffer, fh.widthFull, fh.heightFull, fh.widthFull, QImage::Format_Indexed8);
    image.setColorTable(colors);
    delete [] buf;
    return true;
}


bool DefReader::read(QImage *image)
{
    if (!count()) // forces header to be read
        return false;

    if (!dev->seek(blocks[curBlock].offsets[curFrame]))
        return false;

    FrameHeader fh;
    dev->read((char*)&fh, sizeof(fh));

    if (fh.type == 0)
    {
        return readFrame0(*image, fh);
    }
    if (fh.type == 1)
    {
        return readFrame1(*image, fh);
    }

    return false;
}


hrDefHandler::hrDefHandler(QIODevice *device)
{
    setDevice(device);
    dr = new DefReader(device);
}

hrDefHandler::~hrDefHandler()
{
    if (!dr) delete dr;
}

bool hrDefHandler::read(QImage *image)
{
    if (!canRead())
        return false;

    return dr->read(image);
}

bool hrDefHandler::write(const QImage &image)
{
    qWarning("%s: not supported", Q_FUNC_INFO);
    return false;
}

int hrDefHandler::currentImageNumber() const
{
    return dr->currentImageNumber();
}

int hrDefHandler::imageCount() const
{
    return dr->count();
}

bool hrDefHandler::jumpToImage(int imageNumber)
{
    return dr->jumpToImage(imageNumber);
}

bool hrDefHandler::jumpToNextImage()
{
    return dr->jumpToNextImage();
}

int hrDefHandler::loopCount() const
{
    return 1;
}

int hrDefHandler::nextImageDelay() const
{
    return 0;
}

bool hrDefHandler::canRead() const
{
    if (!canRead(device()))
        return false;
    return true;
}

bool hrDefHandler::canRead(QIODevice *device)
{
    if (!device)
    {
        qWarning("hrDefHandler::canRead() called with no device");
        return false;
    }
    return true;
}

QVariant hrDefHandler::option(ImageOption option) const
{
    if (option == Size && canRead())
    {
        QSize imageSize;
        imageSize = QSize(dr->getWidth(), dr->getHeight());

        if ( imageSize.isValid() )
            return imageSize;
    }

    return QVariant();
}

void hrDefHandler::setOption(ImageOption option, const QVariant &value)
{
    qWarning("%s: not supported", Q_FUNC_INFO);
}

bool hrDefHandler::supportsOption(ImageOption option) const
{
    return option == Size;
}

QByteArray hrDefHandler::name() const
{
    return "def";
}
