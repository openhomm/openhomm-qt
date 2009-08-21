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
#include "hrPushButton.hpp"
#include "hrLodEngine.hpp"
#include "hrSndEngine.hpp"

QString hrApplication::mapName = "";

/*!
  \class hrApplication
*/

/*!
  Constructs application and loads application settings.
  \sa hrSettings
*/
hrApplication::hrApplication(int &argc, char **argv):
        QApplication(argc, argv),
        lodHandler(NULL),
        sndHandler(NULL)
{
    hrSettings::get();
    mapName = "";
    this->createFileEngineHandlers();

    if ( argc > 1 ) {
        mapName = argv[1];
        qDebug() << mapName;
    }
}

/*!
  Destroys the hrApplication object.
  \sa hrSettings
  */
hrApplication::~hrApplication()
{
    this->destroyFileEngineHandlers();
}

void hrApplication::createFileEngineHandlers()
{
    lodHandler = new hrLodEngineHandler;
    sndHandler = new hrSndEngineHandler;
}

void hrApplication::destroyFileEngineHandlers()
{
    delete lodHandler;
    lodHandler = NULL;
    delete sndHandler;
    sndHandler = NULL;
}
