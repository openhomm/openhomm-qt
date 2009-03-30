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
#include "hrString.hpp"

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

    if ( basic.load(&map) == false )
    {
        map.close();
        return false;
    }

    return true;
}

bool BasicParametres_t::load(QIODevice *device)
{
    device->read( (char *) &version, 4);

    if ( version != 0x0000001C )
    {
        qWarning("File is not a HoMM III : SoD Map.");
        return false;
    }

    device->read( (char *) &junk, 1);
    device->read( (char *) &size, 4);
    device->read( (char *) &under, 1);

    name = hrString::deserialize(device);
    description = hrString::deserialize(device);

    device->read( (char *) &difficult, 1);
    device->read( (char *) &levelLimit, 1);

    return true;
}
