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
#include "hrApplication.hpp"
#include "hrH3MReader.hpp"
#include "hrFilesystem.hpp"
#include "hrWindow.hpp"

#if defined(Q_WS_WIN32) && defined(_MSC_VER)
#   include "client/windows/handler/exception_handler.h"
#elif Q_WS_LINUX && !defined(__x86_64__)
#   include "client/linux/handler/exception_handler.h"
#endif

void checkPlugins()
{
    QList<QByteArray> formats = QImageReader::supportedImageFormats();

    if ( !formats.contains("def") || !formats.contains("pcx" )) {
        QMessageBox::critical(NULL, "Critical error!", "Plugins didn't load. Files maybe corrupted. Try to re-install application.");
        exit(1);
    }
}

#ifndef __x86_64__
bool callback(
#if defined Q_WS_WIN32
        const wchar_t *dump_path, const wchar_t *id,
#else
        const char *dump_path, const char *id,
#endif
                     void *context,
#if defined(Q_WS_WIN32) && defined(_MSC_VER)
                     EXCEPTION_POINTERS *exinfo,
                     MDRawAssertionInfo *assertion,
#endif
                     bool succeeded) {

  if (succeeded) {
    qWarning("Dump is successfull");
  } else {
    qWarning("Dump failed");
  }
  return succeeded;
}
#endif

int main(int argc, char** argv)
{
    QT_REQUIRE_VERSION(argc, argv, "4.5.0");

#if defined(Q_WS_WIN32) && defined(_MSC_VER)
    google_breakpad::ExceptionHandler eh(L".", NULL, callback, NULL,
        google_breakpad::ExceptionHandler::HANDLER_ALL );

#endif

#if defined(Q_WS_LINUX) && !defined(__x86_64__)
    google_breakpad::ExceptionHandler eh(".", NULL, callback, NULL, true);
#endif

    hrApplication app(argc, argv);

    checkPlugins();

    hrFilesystem fs;

    fs.mount(QStringList() << "data/h3sprite.lod" << "data/h3bitmap.lod" << "data/heroes3.snd" );

    hrWindow w;
    w.show();
    w.AvdventureScreen();

    return app.exec();
}
