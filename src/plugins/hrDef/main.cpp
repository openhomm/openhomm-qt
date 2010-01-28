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
#include <qimageiohandler.h>
#include <qdebug.h>
#include "hrDefHandler.hpp"

class hrDefPlugin : public QImageIOPlugin
{
public:
    Capabilities capabilities(QIODevice * device, const QByteArray & format) const;
    QImageIOHandler * create(QIODevice * device, const QByteArray & format = QByteArray()) const;
    QStringList keys() const;
};

QImageIOPlugin::Capabilities hrDefPlugin::capabilities(QIODevice *device, const QByteArray &format) const
{
    if (format == "def")
        return CanRead;
    if (!format.isEmpty())
        return 0;
    if (!device->isOpen())
        return 0;

    Capabilities cap;

    if ( device->isReadable() && hrDefHandler::canRead(device) )
    {
        cap |= CanRead;
    }

    return cap;
}

QImageIOHandler* hrDefPlugin::create(QIODevice *device, const QByteArray &format) const
{
    hrDefHandler *hand = new hrDefHandler(device);
    hand->setFormat(format);
    return hand;
}

QStringList hrDefPlugin::keys() const
{
    return QStringList() << QLatin1String("def");
}

Q_EXPORT_STATIC_PLUGIN(hrDefPlugin)
Q_EXPORT_PLUGIN2(hrDef, hrDefPlugin)
