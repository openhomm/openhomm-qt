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
#include "hrFilesystem.hpp"
#include "hrLodEngine.hpp"
#include "hrSndEngine.hpp"
#include "hrSettings.hpp"

fileSystemCache hrFilesystem::_cache;

const char * MOUNT_SUCCESSFULLY = "Successfully mounted: %s";
const char * MOUNT_FAILED       = "Failed to mount: %s";

hrFilesystem::hrFilesystem()
{
}

hrFilesystem::~hrFilesystem()
{
}

bool hrFilesystem::mount(const QString &path)
{
    QStringList pathElements = path.split('/');

    QDir current(hrSettings::get().gameDir());

    QString normalPath = hrSettings::get().gameDir() + '/' ;

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
    qDebug("Trying to mount: %s", qPrintable(normalPath));

    if ( normalPath.indexOf(".lod", 0, Qt::CaseInsensitive) != -1 )
    {
        if ( hrLodEngine::fillInternalCache(normalPath) )
            qDebug(MOUNT_SUCCESSFULLY, qPrintable(normalPath));
        else
            qCritical(MOUNT_FAILED, qPrintable(normalPath));
    }
    else if ( normalPath.indexOf(".snd", 0, Qt::CaseInsensitive) != -1 )
    {
        if ( hrSndEngine::fillInternalCache(normalPath) )
            qDebug(MOUNT_SUCCESSFULLY, qPrintable(normalPath));
        else
            qCritical(MOUNT_FAILED, qPrintable(normalPath));
    }

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
