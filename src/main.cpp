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
#include "hrApplication.hpp"
#include "hrH3MReader.hpp"

#include "hrWindow.hpp"

int main(int argc, char** argv)
{
    QT_REQUIRE_VERSION(argc, argv, "4.5.0");
    hrApplication app(argc, argv);

    hrWindow w;
    w.show();

    //hrH3MReader reader;
    //reader.load("maps/All for One.h3m");

    return app.exec();
}
