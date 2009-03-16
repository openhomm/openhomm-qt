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
#pragma once

#include <qimageiohandler.h>

class QImage;
class QByteArray;
class QIODevice;
class QVariant;
class DefReader;

class hrDefHandler : public QImageIOHandler
{
public:
    hrDefHandler(QIODevice *device);
    ~hrDefHandler();

    virtual bool canRead() const;
    virtual bool read(QImage *image);
    virtual bool write(const QImage &image);
    virtual int currentImageNumber() const;
    virtual int imageCount() const;
    virtual bool jumpToImage(int imageNumber);
    virtual bool jumpToNextImage();
    virtual int loopCount() const;
    virtual int nextImageDelay() const;
    static bool canRead(QIODevice *device);

    QVariant option(ImageOption option) const;
    void setOption(ImageOption option, const QVariant &value);
    bool supportsOption(ImageOption option) const;

    virtual QByteArray name() const;

private:
    DefReader *dr;
};

