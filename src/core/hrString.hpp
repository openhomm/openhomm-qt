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

class hrString3
{
public:
    hrString3();
    hrString3(const char* str);
    hrString3(const hrString3& str);
    //hrString3(const QString& str);
    ~hrString3();

    static void serialize(QIODevice *device);
    static void desrialize(QIODevice *device);

// data
protected:
    char * _str;
    size_t size;
};
