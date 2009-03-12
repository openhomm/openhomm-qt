#include "precompiled.hpp"
#include "hrLodEngineIterator.hpp"
#include "hrLodEngine.hpp"

static LodFiles static_lodFiles;

// TODO: refactor

QAbstractFileEngine* hrLodEngineHandler::create(const QString &filename) const
{
    if ( filename.size() > 0 && filename.startsWith("lod:/", Qt::CaseInsensitive) )
    {
        return new hrLodEngine(filename);
    }
    return 0;
}

hrLodEngine::hrLodEngine(const QString& path) : QAbstractFileEngine()
{
    _buffer = NULL;
    this->setFileName(path);
}

hrLodEngine::~hrLodEngine()
{
    Q_ASSERT(_buffer != NULL);

    if ( _buffer->isOpen() )
        _buffer->close();

    delete _buffer;
}

void hrLodEngine::setFileName(const QString &file)
{
    int c = file.count('/');
    _archivename = file.section('/', c-(c-1), c-1);
    _archivename += ".lod";
    _filename = file.section('/', c, c);
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
        res = preload_file();

    return res;
}
bool hrLodEngine::close()
{
    Q_ASSERT(_buffer != NULL);
    _buffer->close();
    return true;
}
bool hrLodEngine::flush()
{
    qWarning("%s: not supported", Q_FUNC_INFO);
    return false;
}
qint64 hrLodEngine::size() const
{
    Q_ASSERT(_buffer != NULL);
    return _buffer->size();
}
qint64 hrLodEngine::pos() const
{
    Q_ASSERT(_buffer != NULL);
    return _buffer->pos();
}
bool hrLodEngine::atEnd() const
{
    Q_ASSERT(_buffer != NULL);
    return _buffer->atEnd();
}
bool hrLodEngine::seek(qint64 offset)
{
    Q_ASSERT(_buffer != NULL);
    return _buffer->seek(offset);
}
qint64 hrLodEngine::read(char *data, qint64 maxlen)
{
    Q_ASSERT(_buffer != NULL);
    return _buffer->read(data, maxlen);;
}
qint64 hrLodEngine::write(const char *data, qint64 len)
{
    Q_UNUSED(data);
    Q_UNUSED(len);
    qWarning("%s: not supported", Q_FUNC_INFO);
    return 0;
}

bool hrLodEngine::remove()
{
    qWarning("%s: not supported", Q_FUNC_INFO);
    return false;
}
bool hrLodEngine::copy(const QString &)
{
    qWarning("%s: not supported", Q_FUNC_INFO);
    return false;
}
bool hrLodEngine::rename(const QString &)
{
    qWarning("%s: not supported", Q_FUNC_INFO);
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

bool hrLodEngine::setSize(qint64 size)
{
    Q_UNUSED(size);
    qWarning("%s: not implemented", Q_FUNC_INFO);
    return false;
}

QStringList hrLodEngine::entryList(QDir::Filters filters, const QStringList &filterNames) const
{
    qWarning("%s: not supported", Q_FUNC_INFO);
    return QAbstractFileEngine::entryList(filters, filterNames);
}

bool hrLodEngine::caseSensitive() const
{
    qWarning("%s: not supported", Q_FUNC_INFO);
    return false;
}

QAbstractFileEngine::FileFlags hrLodEngine::fileFlags(QAbstractFileEngine::FileFlags) const
{
    qWarning("%s: not supported", Q_FUNC_INFO);
    return 0;
}

bool hrLodEngine::setPermissions(uint)
{
    qWarning("%s: not supported", Q_FUNC_INFO);
    return false;
}

QString hrLodEngine::fileName(QAbstractFileEngine::FileName file) const
{
    Q_UNUSED(file);
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
    qWarning("%s: not supported", Q_FUNC_INFO);
    return new hrLodEngineIterator(filters, filterNames);
}
QAbstractFileEngine::Iterator *hrLodEngine::endEntryList()
{
    qWarning("%s: not supported", Q_FUNC_INFO);
    return 0;
}

bool hrLodEngine::extension(Extension, const ExtensionOption *, ExtensionReturn *)
{
    qWarning("%s: not supported", Q_FUNC_INFO);
    return false;
}
bool hrLodEngine::supportsExtension(Extension) const
{
    qWarning("%s: not supported", Q_FUNC_INFO);
    return false;
}

bool hrLodEngine::preload_fat()
{
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
                _lf->fat.insert(entry.name, entry);
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
