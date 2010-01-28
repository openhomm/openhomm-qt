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
#include <qimage.h>
#include <qpixmap.h>
#include <qpainter.h>
#include <qvariant.h>
#include <qdebug.h>

#include "hrPcxHandler.hpp"

struct hrRgb
{
    quint8 r,g,b;
};

hrPcxHandler::hrPcxHandler() : QImageIOHandler()
{
}

bool hrPcxHandler::canRead() const
{
    if ( canRead(device()) )
    {
        setFormat("pcx");
        return true;
    }

    return false;
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
        QImage im(data, width, height, width, QImage::Format_Indexed8);
        *image = im;
        this->readPallete();
        image->setColorTable(pallete);
    }
    else if ( size == width * height * 3 )
    {
        QImage im(data, width, height, width*3, QImage::Format_RGB888);
        *image = im.rgbSwapped();
    }
    else
    {
        *image = QImage();
    }

    delete [] data;

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

    quint32 s, w, h;

    device->read( (char *) &s, 4);
    device->read( (char *) &w, 4);
    device->read( (char *) &h, 4);
    device->seek(0);
    
    if ( s == w * h )
        return true;
    if ( s == w * h * 3 )
        return true;

    return false;
}

QVariant hrPcxHandler::option(ImageOption option) const
{
    if (option == Size && canRead())
    {
        QSize imageSize;
        quint32 width, height;
        quint64 pos = device()->pos();
        device()->seek(4);

        device()->read( (char *) &width, 4);
        device()->read( (char *) &height, 4);
        device()->seek(pos);

        imageSize = QSize(width, height);

        if ( imageSize.isValid() )
            return imageSize;
    }
    return QVariant();
}

void hrPcxHandler::setOption(ImageOption, const QVariant &)
{
    qWarning("%s: not supported", Q_FUNC_INFO);
}

bool hrPcxHandler::supportsOption(ImageOption option) const
{
    return option == Size;
}

void hrPcxHandler::readPallete()
{
    device()->seek(device()->size() - 256*3);

    for ( int i = 0; i < 256; i++ )
    {
        hrRgb rgb;
        device()->read( (char *) &rgb, 3);
        pallete.push_back(qRgb(rgb.r, rgb.g, rgb.b));
    }
}
