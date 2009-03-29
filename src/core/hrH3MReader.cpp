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
#include "hrH3MReader.hpp"

hrH3MReader::hrH3MReader()
{
}

hrH3MReader::~hrH3MReader()
{
}

bool hrH3MReader::load(const QString &name)
{
    QByteArray data;
    bool result = true;

    {
        QFile map(name);
        if ( ! map.open(QIODevice::ReadOnly) )
        {
            qWarning("Can't open file: %s", qPrintable(name));
            return false;
        }

        data = map.readAll();
        map.close();
    }

    QBuffer map(&data);
    if ( !map.open(QIODevice::ReadOnly) )
    {
        qWarning() << map.errorString();
        return false;
    }

    map.read( (char *) &basic.version, 4);
    if ( basic.version != 0x0000001C )
    {
        qWarning("File (%s) is not a Heroes III SoD Map. Template stopped.", qPrintable(name));
        map.close();
        return false;
    }

    map.read( (char *) &basic.junk, 1);
    map.read( (char *) &basic.size, 4);
    map.read( (char *) &basic.under, 1);

    basic.name = this->loadString(&map);
    basic.description = this->loadString(&map);

    map.read( (char *) &basic.difficult, 1);
    map.read( (char *) &basic.levelLimit, 1);

    return true;
}

QString hrH3MReader::loadString(QIODevice *device)
{

    quint32 len = 0;
    device->read( (char *) &len, 4);

    if ( len > 0)
    {
        QByteArray str = device->read(len);
        return QString(str);
    }

    return QString("");
}
