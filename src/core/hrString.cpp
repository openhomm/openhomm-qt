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
#include "hrString.hpp"

QDataStream &operator<<(QDataStream &out, const hrString &str)
{
    if ( str.isEmpty() || str.isNull() )
    {
        out << (quint32)0x00;
    }
    else
    {
        out << (quint32)str.length();
        out.writeRawData(str.toLocal8Bit().data(), str.length());
    }
    return out;
}

QDataStream &operator>>(QDataStream &in, hrString &str)
{
    quint32 len = 0;
    in >> len;

    if ( len == 0 )
    {
        str.clear();
    }
    else if ( len > 0 )
    {
        char *b = new char[len];
        in.readRawData(b, len);
        str = QString::fromLocal8Bit(b, len);
        delete [] b;
    }

    return in;
}
