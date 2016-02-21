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
#include "precompiled.hpp"
#include "hrString.hpp"

bool loadHString(QDataStream &in, HString &str)
{
    quint32 len = 0;
    in>>len;
    if ( len == 0 )
        str.clear();
    else if ( len > 0 )
    {
        char *t = new char[len];
        in.readRawData( t, len );
        str = QString::fromLocal8Bit(t, len);
        delete[] t;
    }
    return true;

}
