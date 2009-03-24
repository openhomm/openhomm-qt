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
#include "hrLodEngineIterator.hpp"
#include "hrLodEngine.hpp"

static LodFiles static_lodFiles;

// TODO: refactor

QAbstractFileEngine* hrLodEngineHandler::create(const QString &filename) const
{
    //qDebug() << Q_FUNC_INFO;
    if ( filename.size() > 0 && filename.startsWith("lod:/", Qt::CaseInsensitive) )
    {
        return new hrLodEngine(filename);
    }
    return 0;
}

hrLodEngine::hrLodEngine(const QString& path) : QAbstractFileEngine(), _buffer(NULL), _lf(NULL)
{
    this->setFileName(path);
    if ( ! _archivename.isEmpty() )
    {
        if ( static_lodFiles.find ( _archivename ) == static_lodFiles.end())
        {
            static_lodFiles.insert(_archivename, new LodFile);
        }
        _lf = static_lodFiles[_archivename];
        preload_fat();
    }
}

hrLodEngine::~hrLodEngine()
{
    this->close();
}

void hrLodEngine::setFileName(const QString &file)
{
    int c = file.count('/');

    if ( file.endsWith(".lod", Qt::CaseInsensitive) )
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

bool hrLodEngine::open(QIODevice::OpenMode flags)
{
    if ( flags & QIODevice::WriteOnly )
        qWarning("Write mode not supported. Ignored");

    if ( static_lodFiles.find ( _archivename ) == static_lodFiles.end())
    {
        static_lodFiles.insert(_archivename, new LodFile);
    }

    _lf = static_lodFiles[_archivename];

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
bool hrLodEngine::close()
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
bool hrLodEngine::flush()
{
    return false;
}
qint64 hrLodEngine::size() const
{
    if ( _buffer != NULL )
        return _buffer->size();

    return 0;
}
qint64 hrLodEngine::pos() const
{
    if ( _buffer != NULL )
        return _buffer->pos();

    return 0;
}
bool hrLodEngine::atEnd() const
{
    if ( _buffer != NULL )
        return _buffer->atEnd();

    return true;
}
bool hrLodEngine::seek(qint64 offset)
{
    if ( _buffer != NULL )
        return _buffer->seek(offset);

    return false;
}
qint64 hrLodEngine::read(char *data, qint64 maxlen)
{
    if ( _buffer != NULL )
        return _buffer->read(data, maxlen);

    return 0;
}
qint64 hrLodEngine::write(const char *, qint64)
{
    qWarning("%s: not supported", Q_FUNC_INFO);
    return -1;
}

bool hrLodEngine::remove()
{
    qWarning("%s: not supported", Q_FUNC_INFO);
    return false;
}
bool hrLodEngine::copy(const QString &)
{
    qWarning("Native %s: not supported. Using standart Qt implementation", Q_FUNC_INFO);
    return false;
}
bool hrLodEngine::rename(const QString &)
{
    qWarning("Native %s: not supported. Using standart Qt implementation", Q_FUNC_INFO);
    return false;
}
bool hrLodEngine::link(const QString &)
{
    qWarning("%s: not supported", Q_FUNC_INFO);
    return false;
}

bool hrLodEngine::isSequential() const
{
    return false;
}

bool hrLodEngine::isRelativePath() const
{
    qWarning("%s: not supported", Q_FUNC_INFO);
    return false;
}

bool hrLodEngine::mkdir(const QString &, bool) const
{
    qWarning("%s: not supported", Q_FUNC_INFO);
    return false;
}
bool hrLodEngine::rmdir(const QString &, bool) const
{
    qWarning("%s: not supported", Q_FUNC_INFO);
    return false;
}

bool hrLodEngine::setSize(qint64)
{
    qWarning("%s: not supported", Q_FUNC_INFO);
    return false;
}

QStringList hrLodEngine::entryList(QDir::Filters filters, const QStringList &filterNames) const
{
    if ( _lf != NULL )
        return _lf->fat.keys();

    return QAbstractFileEngine::entryList(filters, filterNames);
}

bool hrLodEngine::caseSensitive() const
{
    return false;
}

QAbstractFileEngine::FileFlags hrLodEngine::fileFlags(QAbstractFileEngine::FileFlags type) const
{
    QAbstractFileEngine::FileFlags ret = 0;

    if(type & TypesMask)
        ret |= FileType;

    if(type & FlagsMask)
        ret |= ExistsFlag;

    return ret;
}

bool hrLodEngine::setPermissions(uint)
{
    qWarning("%s: not supported", Q_FUNC_INFO);
    return false;
}

QString hrLodEngine::fileName(QAbstractFileEngine::FileName) const
{
    return _filename;
}

uint hrLodEngine::ownerId(QAbstractFileEngine::FileOwner) const
{
    qWarning("%s: not supported", Q_FUNC_INFO);
    return 0;
}
QString hrLodEngine::owner(QAbstractFileEngine::FileOwner) const
{
    qWarning("%s: not supported", Q_FUNC_INFO);
    return "";
}

QDateTime hrLodEngine::fileTime(QAbstractFileEngine::FileTime) const
{
    qWarning("%s: not supported", Q_FUNC_INFO);
    return QDateTime();
}

QAbstractFileEngine::Iterator *hrLodEngine::beginEntryList(QDir::Filters filters, const QStringList &filterNames)
{
    return new hrLodEngineIterator(filters, filterNames);
}
QAbstractFileEngine::Iterator *hrLodEngine::endEntryList()
{
    return 0;
}

bool hrLodEngine::extension(Extension, const ExtensionOption *, ExtensionReturn *)
{
    return false;
}
bool hrLodEngine::supportsExtension(Extension ext) const
{
    return ext == QAbstractFileEngine::AtEndExtension;
}

bool hrLodEngine::preload_fat()
{
    Q_ASSERT(_lf != NULL);
    if ( _lf->file != NULL )
    {
        if ( _lf->file->isOpen() )
        {
            if ( !_lf->fat.isEmpty() )
            {
                return true;
            }
        }
    }
    else
    {
        _lf->file = new QFile(_archivename);

        if ( _lf->file->open(QIODevice::ReadOnly) )
        {
            LodHeader head;
            _lf->file->read((char *)&head, sizeof(head));
            if ( head.magic != LOD_MAGIC )
            {
                setError(QFile::OpenError, QString("%1 is not LOD archive").arg(_archivename));
                return false;
            }

            _lf->file->seek(0x5C);

            for ( quint32 i = 0; i < head.files; ++i )
            {
                LodEntry entry;
                _lf->file->read((char *)&entry, sizeof(entry));
                _lf->fat.insert(QString(entry.name).toLower(), entry);
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

bool hrLodEngine::preload_file()
{
    if ( _buffer == NULL )
        _buffer = new QBuffer;

    LodEntry entry = _lf->fat.value(_filename);
    _lf->file->seek(entry.offset);

    if ( entry.csize == 0 )
    {
        QByteArray ba = _lf->file->read(entry.size);
        _buffer->setData(ba);
        _buffer->open(QIODevice::ReadOnly);

        return true;
    }
    else
    {
        QByteArray ba = _lf->file->read(entry.csize);

        char *l = (char*)&entry.size;

        ba.prepend(l[0]);
        ba.prepend(l[1]);
        ba.prepend(l[2]);
        ba.prepend(l[3]);

        QByteArray unc = qUncompress(ba);

        _buffer->setData(unc);
        _buffer->open(QIODevice::ReadOnly);

        return true;
    }

    return false;
}
