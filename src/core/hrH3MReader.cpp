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

hrH3MReader::hrH3MReader() : data(NULL)
{
}

hrH3MReader::~hrH3MReader()
{
}

bool hrH3MReader::load(const QString &name)
{
    QFile map(name);

    if ( !map.open(QIODevice::ReadOnly) )
        return false;

    quint32 real_size = 0;
    map.seek(map.size() - 4);
    map.read( ( char* ) &real_size, 4);
    qDebug() << real_size;
    QByteArray cData = map.read(map.size() - 4);
    char *l = (char*)real_size;

    cData.prepend(l[0]);
    cData.prepend(l[1]);
    cData.prepend(l[2]);
    cData.prepend(l[3]);

    data = new QByteArray;
    //*data = qUncompress(cData);
    //qDebug() << data->size();

//    quint32 mapVersion;
//
//    map.read( (char *) &mapVersion, 4);
//
//    if ( mapVersion != 0x0000001C )
//    {
//        qWarning("File is not a Heroes III SoD Map.");
//        return false;
//    }

    map.close();

    return false;
}

QString hrH3MReader::loadString(QIODevice *device)
{
    return QString();
}
