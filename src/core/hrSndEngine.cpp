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
#include "hrSndEngineIterator.hpp"
#include "hrSndEngine.hpp"

static SndFiles static_sndFiles;

QAbstractFileEngine* hrSndEngineHandler::create(const QString &filename) const
{
    if ( filename.size() > 0 && filename.startsWith("snd:/", Qt::CaseInsensitive) )
    {
        return new hrSndEngine(filename);
    }
    return 0;
}

hrSndEngine::hrSndEngine(const QString& path) : QAbstractFileEngine(), _sf(NULL), _buffer(NULL)
{
    this->setFileName(path);
    if ( ! _archivename.isEmpty() )
    {
        if ( static_sndFiles.find ( _archivename ) == static_sndFiles.end())
        {
            static_sndFiles.insert(_archivename, new SndFile);
        }
        _sf = static_sndFiles[_archivename];
        preload_fat();
    }
}

hrSndEngine::~hrSndEngine()
{
    this->close();
}

void hrSndEngine::setFileName(const QString &file)
{
    int c = file.count('/');

    if ( file.endsWith(".snd", Qt::CaseInsensitive) )
    {
        _archivename = file.section('/', 1, c);
        _filename = "";
    }
    else
    {
        _archivename = file.section('/', c-(c-1), c-1);
        _filename = file.section('/', c, c).toLower();
    }
}

bool hrSndEngine::open(QIODevice::OpenMode flags)
{
    if ( flags & QIODevice::WriteOnly )
        qWarning("Write mode not supported. Ignored");

    if ( static_sndFiles.find ( _archivename ) == static_sndFiles.end())
    {
        static_sndFiles.insert(_archivename, new SndFile);
    }

    _sf = static_sndFiles[_archivename];

    bool res = preload_fat();

    if ( res == true )
    {
        if ( !_filename.isEmpty() )
            res = preload_file();
        else
            res = false;
    }

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
bool hrSndEngine::flush()
{
    return false;
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
qint64 hrSndEngine::write(const char *, qint64)
{
    qWarning("%s: not supported", Q_FUNC_INFO);
    return -1;
}

bool hrSndEngine::remove()
{
    qWarning("%s: not supported", Q_FUNC_INFO);
    return false;
}
bool hrSndEngine::copy(const QString &)
{
    qWarning("Native %s: not supported. Using standart Qt implementation", Q_FUNC_INFO);
    return false;
}
bool hrSndEngine::rename(const QString &)
{
    qWarning("Native %s: not supported. Using standart Qt implementation", Q_FUNC_INFO);
    return false;
}
bool hrSndEngine::link(const QString &)
{
    qWarning("%s: not supported", Q_FUNC_INFO);
    return false;
}

bool hrSndEngine::isSequential() const
{
    return false;
}

bool hrSndEngine::isRelativePath() const
{
    return false;
}

bool hrSndEngine::mkdir(const QString &, bool) const
{
    qWarning("%s: not supported", Q_FUNC_INFO);
    return false;
}
bool hrSndEngine::rmdir(const QString &, bool) const
{
    qWarning("%s: not supported", Q_FUNC_INFO);
    return false;
}

bool hrSndEngine::setSize(qint64)
{
    qWarning("%s: not supported", Q_FUNC_INFO);
    return false;
}

QStringList hrSndEngine::entryList(QDir::Filters filters, const QStringList &filterNames) const
{
    if ( _sf != NULL )
        return _sf->fat.keys();

    return QAbstractFileEngine::entryList(filters, filterNames);
}

bool hrSndEngine::caseSensitive() const
{
    return false;
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

bool hrSndEngine::setPermissions(uint)
{
    qWarning("%s: not supported", Q_FUNC_INFO);
    return false;
}

QString hrSndEngine::fileName(QAbstractFileEngine::FileName file) const
{
    Q_UNUSED(file);
    return _filename;
}

uint hrSndEngine::ownerId(QAbstractFileEngine::FileOwner) const
{
    qWarning("%s: not supported", Q_FUNC_INFO);
    return 0;
}
QString hrSndEngine::owner(QAbstractFileEngine::FileOwner) const
{
    qWarning("%s: not supported", Q_FUNC_INFO);
    return "";
}

QDateTime hrSndEngine::fileTime(QAbstractFileEngine::FileTime) const
{
    qWarning("%s: not supported", Q_FUNC_INFO);
    return QDateTime();
}

QAbstractFileEngine::Iterator *hrSndEngine::beginEntryList(QDir::Filters filters, const QStringList &filterNames)
{
    return new hrSndEngineIterator(filters, filterNames);
}
QAbstractFileEngine::Iterator *hrSndEngine::endEntryList()
{
    return 0;
}

bool hrSndEngine::extension(Extension, const ExtensionOption *, ExtensionReturn *)
{
    return false;
}
bool hrSndEngine::supportsExtension(Extension ext) const
{
    return ext == QAbstractFileEngine::AtEndExtension;
}

bool hrSndEngine::preload_fat()
{
    Q_ASSERT(_sf != NULL);
    if ( _sf->file != NULL )
    {
        if ( _sf->file->isOpen() )
        {
            if ( !_sf->fat.isEmpty() )
            {
                return true;
            }
        }
    }
    else
    {
        _sf->file = new QFile(_archivename);
        QFile *file = _sf->file;

        if ( file->open(QIODevice::ReadOnly) )
        {
            quint32 count;

            if ( file->read( (char *) &count, 4) == 4)
            {
                for ( quint32 i = 0; i < count; ++i )
                {
                    SndEntry entry;
                    memset(&entry,0,sizeof(entry));

                    file->read( (char *) &entry, sizeof(SndEntry));

                    for ( int i = 0; i < 40; ++i)
                    {
                        if ( entry.name[i] == '\0' )
                        {
                            entry.name[i] = '.';
                            break;
                        }
                    }

                    _sf->fat.insert(QString(entry.name).toLower(), entry);
                }
            }

            return true;
        }
        else
        {
            setError(QFile::OpenError, QString("Can't open: %1").arg(_archivename));
            return false;
        }
    }
    return false;
}

bool hrSndEngine::preload_file()
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
