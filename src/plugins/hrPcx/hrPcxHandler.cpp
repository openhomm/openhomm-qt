#include "qimage.h"
#include "qpixmap.h"
#include "qpainter.h"
#include "qvariant.h"
#include "qdebug.h"

#include "hrPcxHandler.hpp"

struct hrRgb
{
    quint8 r,g,b;
};

hrPcxHandler::hrPcxHandler() : hasPallete(false)
{
}

bool hrPcxHandler::canRead() const
{
    if ( !canRead(device()) )
        return false;

    return true;
}

bool hrPcxHandler::read(QImage *image)
{
    if ( !canRead() )
        return false;

    quint32 size, width, height;

    device()->read( (char *) &size, 4);
    device()->read( (char *) &width, 4);
    device()->read( (char *) &height, 4);

    quint8 * data = NULL;
    data = new quint8[size];
    device()->read( (char *) data, size);

    if ( size == width * height )
    {
        *image = QImage(data, width, height, width, QImage::Format_Indexed8);
        image->setColorTable(pallete);
    }
    else if ( size == width * height * 3 )
    {
        *image = QImage(data, width, height, width*3, QImage::Format_RGB888);
    }
    else
    {
        *image = QImage();
    }

    if ( image->isNull() )
        return false;

    return true;
}

bool hrPcxHandler::write(const QImage &)
{
    qWarning("%s: not supported", Q_FUNC_INFO);

    if (!device()->isWritable())
        return false;

    return false;
}

QByteArray hrPcxHandler::name() const
{
    return "pcx";
}

bool hrPcxHandler::canRead(QIODevice *device)
{
    if (!device) {
        qWarning("hrPcxHandler::canRead() called with no device");
        return false;
    }

    return true;
}

QVariant hrPcxHandler::option(ImageOption) const
{
    qWarning("%s: not supported", Q_FUNC_INFO);
    return QVariant();
}

void hrPcxHandler::setOption(ImageOption, const QVariant &)
{
    qWarning("%s: not supported", Q_FUNC_INFO);
}

bool hrPcxHandler::supportsOption(ImageOption option) const
{
    qDebug() << option;
    return option == Size;
}

void hrPcxHandler::readPallete()
{
    device()->seek(device()->size() - 256*3);

    for ( int i = 0; i < 256; i = i++ )
    {
        hrRgb rgb;
        device()->read( (char *) &rgb, 3);
        pallete.push_back(qRgb(rgb.r, rgb.g, rgb.b));
    }
}
