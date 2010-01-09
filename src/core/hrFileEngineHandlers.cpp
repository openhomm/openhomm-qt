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
#include "precompiled.hpp"
#include "hrFileEngineHandlers.hpp"
#include "hrLodEngine.hpp"
#include "hrSndEngine.hpp"
#include "hrFilesystem.hpp"

QAbstractFileEngine* hrLodEngineHandler::create(const QString &filename) const
{
    if ( filename.size() > 0 && filename.startsWith("lod:/", Qt::CaseInsensitive) )
    {
        return new hrLodEngine(filename);
    }
    return NULL;
}

QAbstractFileEngine* hrVfsEngineHandler::create(const QString &fileName) const
{
    if ( fileName.size() > 0 && fileName.startsWith("vfs:/", Qt::CaseInsensitive) )
    {
        QString file = fileName, archive;

        file.remove(0,5); // remove 'vfs:/' at the begining
        if ( hrFilesystem::findInCache(file.toLower(),archive) )
        {
            return new hrLodEngine(QString("lod:/") + archive + "/" + file);
        }
    }
    return NULL;
}

QAbstractFileEngine* hrSndEngineHandler::create(const QString &filename) const
{
    if ( filename.size() > 0 && filename.startsWith("snd:/", Qt::CaseInsensitive) )
    {
        return new hrSndEngine(filename);
    }
    return 0;
}
