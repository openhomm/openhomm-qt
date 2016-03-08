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

Q_LOGGING_CATEGORY(fsCat, "fs.main");

/**
*  Find a case-correct path.
 *  @param path a case-insensitive path
 *  @param baseDir a existing base directory. Certainly does not require case correction.
 *  @return Right path or null string on error.
 */
QString hrFilesystem::adjustCaseInPath(const QString &path, const QDir &baseDir)
{
    QStringList pathElements = path.split('/');
    QDir current = baseDir;

    for ( int j = 0; j < pathElements.size()-1; j++ )
    {
        QString next = pathElements[j];

        if (!current.exists(next))
        {
            QStringList candidates = current.entryList(QStringList() << next, QDir::Dirs | QDir::Readable ); // flag QDir::CaseSensitive does not set by default and this is that we need

            if (candidates.size() == 0)
            {
                qCCritical(fsCat) << QString("Directory `%1` not found in directory `%2`").arg(next).arg(current.path());
                return QString();
            }

            if (candidates.size() != 1)
                qCWarning(fsCat) << QString("Ambiguous element `%1` in directory `%2`, selecting `%3`").arg(next).arg(current.path()).arg(candidates.first());

            next = candidates.first();
        }

        if (!current.cd(next))
        {
            qCCritical(fsCat) << QString("Cannot enter directory `%1` in directory `%2`").arg(next).arg(current.path());
            return QString();
        }
    }

    QString last = pathElements.last();

    if (!current.exists(last))
    {
        QStringList candidates = current.entryList(QStringList() << last, QDir::Dirs | QDir::Files | QDir::Readable ); // flag QDir::CaseSensitive does not set by default and this is that we need

        if (candidates.size() == 0)
        {
            qCCritical(fsCat) << QString("File or directory `%1` not found in directory `%2`").arg(last).arg(current.path());
            return QString();
        }

        if (candidates.size() != 1)
            qCWarning(fsCat) << QString("Ambiguous element `%1` in directory `%2`, selecting `%3`").arg(last).arg(current.path()).arg(candidates.first());

        last = candidates.first();
    }
    return current.filePath(last);
}

bool hrFilesystem::mount(const QString &path)
{
    QDir gameRoot = hrSettings::get().gameDir();

    Q_ASSERT(gameRoot.exists());

    QString normalPath = adjustCaseInPath(path,gameRoot);

    if (normalPath.isNull() || !QFile::exists(normalPath))
        return false;

    qCInfo(fsCat) << QString("Trying to mount: %1").arg(normalPath);

    if ( normalPath.endsWith(".lod", Qt::CaseInsensitive) )
    {
        if ( hrLodEngine::fillInternalCache(normalPath) )
            qCInfo(fsCat) << MOUNT_SUCCESSFULLY;
        else
            qCCritical(fsCat) << MOUNT_FAILED;
    }
    else if ( normalPath.endsWith(".snd", Qt::CaseInsensitive) )
    {
        if ( hrSndEngine::fillInternalCache(normalPath) )
            qCInfo(fsCat) << MOUNT_SUCCESSFULLY;
        else
            qCCritical(fsCat) << MOUNT_FAILED;
    }
    else
    {
        if ( !mountDir(normalPath))
            qCCritical(fsCat) << "\tUnsupported archive type";
        else
            qCInfo(fsCat) << MOUNT_SUCCESSFULLY;
    }

    return true;
}
/*!
  \overload
*/

void hrFilesystem::mount(const QStringList &path_list)
{
    for ( int i = 0; i < path_list.size(); i++)
        mount(path_list[i]);
}

/*!
  not implemented yet
*/
bool hrFilesystem::umount(const QString &path)
{
    Q_UNUSED(path);
    return false;
}

void hrFilesystem::fillGeneralCache(const QString &filename, const QString &archive)
{
    _cache.insert(filename, archive);
}

void hrFilesystem::walkDirectory(const QString &path, QStringList &fileList)
{
    QFileInfo info(path);

    if ( info.isDir() )
    {
        QDir mounted(path);
        QFileInfoList entries = mounted.entryInfoList();

        for ( int i = 0; i < entries.size(); i++)
        {
            if ( entries[i].fileName() == "." || entries[i].fileName() == "..")
                continue;

            if ( entries[i].isDir() )
            {
                walkDirectory(entries[i].absoluteFilePath(), fileList);
                continue;
            }

            fileList.append(entries[i].absoluteFilePath());
        }
    }
}

bool hrFilesystem::mountDir(const QString &path)
{
    QFileInfo info(path);

    if ( info.isDir() )
    {
        QStringList fileList;
        walkDirectory(path, fileList);

        for ( int i = 0; i < fileList.size(); i++)
        {
            fileList[i].remove(path);
            fileList[i].remove(0, 1); // remove first '/'
            hrFilesystem::fillGeneralCache(fileList[i], path);
        }
    }

    return info.isDir();
}

/*!
  Try to find archive where the file located.
  @param filename Filename to found.
  @return Archive name or null string on error.
*/
QString hrFilesystem::findInCache(const QString &filename)
{
    if ( _cache.find(filename) != _cache.end() )
    {
        return _cache[filename];
    }

    QList<QString> keys = _cache.keys();
    QStringList keys_string(keys);
    QStringList results;

    results = keys_string.filter(filename,  Qt::CaseInsensitive);

    if ( results.size() > 0 )
        return _cache[results[0]];

    return QString();
}

/**
 *  Extract archive name from full path.
 *  @param path a case-sensitive path
 *  @param ext Must be ".lod" or ".snd" or something else
 *  @return Archive name or null string on error.
 */
QString hrFilesystem::extractArchnameFromPath(const QString& path, const char* ext)
{
    QString archive;
    int index = path.indexOf(ext);

    if ( index == -1 )
        return QString();

    archive = path.left(index + qstrlen(ext));
    archive.remove(0, qstrlen(ext) + 1);

    return archive;
}

/**
 *  Extract filename with path from full path.
 *  @param path a case-sensitive path
 *  @param ext Must be ".lod" or ".snd" or something else
 *  @return Archive name or null string on error.
 */
QString hrFilesystem::extractFilenameFromPath(const QString& path, const char* ext)
{
    int index = path.indexOf(ext);

    if ( index == -1 )
        return QString();

    return path.right(path.length() - index - qstrlen(ext) - 1 );
}
