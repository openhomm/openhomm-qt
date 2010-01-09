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
#include "hrFilesystem.hpp"
#include "hrLodEngine.hpp"

fileSystemCache hrFilesystem::_cache;

hrFilesystem::hrFilesystem()
{
}

hrFilesystem::~hrFilesystem()
{
}

bool hrFilesystem::mount(const QString &path)
{
    QStringList pathElements = path.split('/');

    // FIXME: don't change current dir before this code!!!
    QDir current(QDir::current());

    QString normalPath;

    for ( int j = 0; j < pathElements.size(); j++ )
    {
        QStringList el = current.entryList(QStringList() << pathElements[j], QDir::Dirs | QDir::Files );
        if ( el.size() != 0 )
        {
            normalPath += el[0] + '/';
            if ( current.exists(el[0]) )
                current.cd(el[0]);
        }
    }
    normalPath.remove(normalPath.length() -1, 1); // remove last slash
    qDebug("Try to mount: %s", qPrintable(normalPath));
    hrLodEngine::fillInternalCache(normalPath);

    return true;
}

bool hrFilesystem::umount(const QString &path)
{
    Q_UNUSED(path);
    return true;
}

void hrFilesystem::fillGeneralCache(const QString &filename, const QString &archive)
{
    _cache.insert(filename, archive);
}

bool hrFilesystem::findInCache(const QString &filename, QString &archive)
{
    if ( _cache.find(filename) != _cache.end() )
    {
        archive = _cache[filename];

        return true;
    }

    qWarning() << "Not found " << filename << __FILE__ << __LINE__;
    return false;
}
