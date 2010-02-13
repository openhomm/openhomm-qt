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
#include "hrSndEngineIterator.hpp"
#include "hrSndEngine.hpp"
#include "hrFilesystem.hpp"

SndFiles hrSndEngine::_cache;
/*!
  \class hrSndEngine
  \brief The hrSndEngine class
*/
hrSndEngine::hrSndEngine(const QString& path) : QAbstractFileEngine(), _sf(NULL), _buffer(NULL)
{
    this->setFileName(path);
}

hrSndEngine::~hrSndEngine()
{
    this->close();
}

void hrSndEngine::setFileName(const QString &file)
{
    _archivename = hrFilesystem::extractArchnameFromPath(file, ".lod");
    _filename    = hrFilesystem::extractFilenameFromPath(file, ".lod");

    _filename = _filename.toLower();
}

bool hrSndEngine::open(QIODevice::OpenMode flags)
{
    if ( flags & QIODevice::WriteOnly )
        qWarning("Write mode not supported. Ignored");

    if ( _cache.find(_archivename) == _cache.end() )
    {
        qWarning("File %s not found in cache", qPrintable(_archivename));
        return false;
    }

    _sf = _cache[_archivename];

    bool res = false;

    if ( !_filename.isEmpty() )
        res = preloadFile();

    return res;
}
bool hrSndEngine::close()
{
    if ( _buffer != NULL )
    {
        if ( _buffer->isOpen() )
            _buffer->close();

        delete _buffer;
        _buffer = NULL;
    }
    return true;
}
qint64 hrSndEngine::size() const
{
    if ( _buffer != NULL )
        return _buffer->size();

    return 0;
}
qint64 hrSndEngine::pos() const
{
    if ( _buffer != NULL )
        return _buffer->pos();

    return 0;
}
bool hrSndEngine::atEnd() const
{
    if ( _buffer != NULL )
        return _buffer->atEnd();

    return true;
}
bool hrSndEngine::seek(qint64 offset)
{
    if ( _buffer != NULL )
        return _buffer->seek(offset);

    return false;
}
qint64 hrSndEngine::read(char *data, qint64 maxlen)
{
    if ( _buffer != NULL )
        return _buffer->read(data, maxlen);

    return 0;
}

QStringList hrSndEngine::entryList(QDir::Filters filters, const QStringList &filterNames) const
{
    if ( _sf != NULL )
        return _sf->fat.keys();

    return QAbstractFileEngine::entryList(filters, filterNames);
}

QAbstractFileEngine::FileFlags hrSndEngine::fileFlags(QAbstractFileEngine::FileFlags type) const
{
    QAbstractFileEngine::FileFlags ret = 0;

    if(type & TypesMask)
        ret |= FileType;

    if(type & FlagsMask)
        ret |= ExistsFlag;

    return ret;
}

QString hrSndEngine::fileName(QAbstractFileEngine::FileName file) const
{
    Q_UNUSED(file);
    return _filename;
}

QAbstractFileEngine::Iterator *hrSndEngine::beginEntryList(QDir::Filters filters, const QStringList &filterNames)
{
    return new hrSndEngineIterator(filters, filterNames);
}

bool hrSndEngine::supportsExtension(Extension ext) const
{
    return ext == QAbstractFileEngine::AtEndExtension;
}

bool hrSndEngine::fillInternalCache(const QString &filename)
{
    SndFile *sf = new SndFile;
    sf->file = new QFile(filename);

    if ( sf->file->open(QIODevice::ReadOnly ) )
    {
        quint32 count = 0;

        if ( sf->file->read( (char *) &count, 4 ) == 4 )
        {
            for ( quint32 i = 0; i < count; ++i )
            {
                SndEntry entry;
                memset(&entry, 0, sizeof(entry) );

                sf->file->read( (char *) &entry, sizeof(SndEntry));
                for ( int i = 0; i < 40; ++i )
                {
                    if ( entry.name[i] == '\0' )
                    {
                        entry.name[i] = '.';
                        break;
                    }
                }

                sf->fat.insert(QString(entry.name).toLower(), entry);
                hrFilesystem::fillGeneralCache(QString(entry.name), filename);
            }
        }
        _cache.insert(filename, sf);
        return true;
    }
    else
    {
        qCritical("Can't open %s", qPrintable(filename));
        delete sf;
    }

    return false;
}

bool hrSndEngine::preloadFile()
{
    if ( _buffer == NULL )
        _buffer = new QBuffer;

    SndEntry entry = _sf->fat.value(_filename);

    if ( _sf->file->seek(entry.offset) == true)
    {
        QByteArray ba = _sf->file->read(entry.size);
        _buffer->setData(ba);
        _buffer->open(QIODevice::ReadOnly);

        return true;
    }

    return false;
}
