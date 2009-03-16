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
#include <qimageiohandler.h>
#include <qdebug.h>

#include "hrPcxHandler.hpp"

class hrPcxPlugin : public QImageIOPlugin
{
public:
    Capabilities capabilities(QIODevice * device, const QByteArray & format) const;
    QImageIOHandler * create(QIODevice * device, const QByteArray & format = QByteArray()) const;
    QStringList keys() const;
};

QImageIOPlugin::Capabilities hrPcxPlugin::capabilities(QIODevice *device, const QByteArray &format) const
{
    if (format == "pcx")
        return CanRead;
    if (!format.isEmpty())
        return 0;
    if (!device->isOpen())
        return 0;

    Capabilities cap;

    if ( device->isReadable() && hrPcxHandler::canRead(device) )
        cap |= CanRead;

    return cap;
}

QImageIOHandler* hrPcxPlugin::create(QIODevice *device, const QByteArray &format) const
{
    QImageIOHandler *pcxHandler = new hrPcxHandler();
    pcxHandler->setDevice(device);
    pcxHandler->setFormat(format);
    return pcxHandler;
}

QStringList hrPcxPlugin::keys() const
{
    return QStringList() << QLatin1String("pcx");
}

Q_EXPORT_STATIC_PLUGIN(hrPcxPlugin)
Q_EXPORT_PLUGIN2(hrPcx, hrPcxPlugin)
