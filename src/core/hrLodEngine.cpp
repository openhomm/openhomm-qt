#include "precompiled.hpp"
#include "hrLodEngineIterator.hpp"
#include "hrLodEngine.hpp"

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
    qWarning("%s: not implemented", __FUNCTION__);
}

hrLodEngine::~hrLodEngine()
{
    qWarning("%s: not implemented", __FUNCTION__);
}

void hrLodEngine::setFileName(const QString &file)
{
    Q_UNUSED(file);
    qWarning("%s: not implemented", __FUNCTION__);
}

bool hrLodEngine::open(QIODevice::OpenMode flags)
{
    Q_UNUSED(flags);
    qWarning("%s: not implemented", __FUNCTION__);
    return false;
}
bool hrLodEngine::close()
{
    qWarning("%s: not implemented", __FUNCTION__);
    return false;
}
bool hrLodEngine::flush()
{
    qWarning("%s: not implemented", __FUNCTION__);
    return false;
}
qint64 hrLodEngine::size() const
{
    qWarning("%s: not implemented", __FUNCTION__);
    return 0;
}
qint64 hrLodEngine::pos() const
{
    qWarning("%s: not implemented", __FUNCTION__);
    return 0;
}
bool hrLodEngine::atEnd() const
{
    qWarning("%s: not implemented", __FUNCTION__);
    return false;
}
bool hrLodEngine::seek(qint64)
{
    qWarning("%s: not implemented", __FUNCTION__);
    return false;
}
qint64 hrLodEngine::read(char *data, qint64 maxlen)
{
    Q_UNUSED(data);
    Q_UNUSED(maxlen);
    qWarning("%s: not implemented", __FUNCTION__);
    return 0;
}
qint64 hrLodEngine::write(const char *data, qint64 len)
{
    Q_UNUSED(data);
    Q_UNUSED(len);
    qWarning("%s: not implemented", __FUNCTION__);
    return 0;
}

bool hrLodEngine::remove()
{
    qWarning("%s: not implemented", __FUNCTION__);
    return false;
}
bool hrLodEngine::copy(const QString &)
{
    qWarning("%s: not implemented", __FUNCTION__);
    return false;
}
bool hrLodEngine::rename(const QString &)
{
    qWarning("%s: not implemented", __FUNCTION__);
    return false;
}
bool hrLodEngine::link(const QString &)
{
    qWarning("%s: not implemented", __FUNCTION__);
    return false;
}

bool hrLodEngine::isSequential() const
{
    qWarning("%s: not implemented", __FUNCTION__);
    return false;
}

bool hrLodEngine::isRelativePath() const
{
    qWarning("%s: not implemented", __FUNCTION__);
    return false;
}

bool hrLodEngine::mkdir(const QString &, bool) const
{
    qWarning("%s: not implemented", __FUNCTION__);
    return false;
}
bool hrLodEngine::rmdir(const QString &, bool) const
{
    qWarning("%s: not implemented", __FUNCTION__);
    return false;
}

bool hrLodEngine::setSize(qint64 size)
{
    Q_UNUSED(size);
    qWarning("%s: not implemented", __FUNCTION__);
    return false;
}

QStringList hrLodEngine::entryList(QDir::Filters filters, const QStringList &filterNames) const
{
    qWarning("%s: not implemented", __FUNCTION__);
    return QAbstractFileEngine::entryList(filters, filterNames);
}

bool hrLodEngine::caseSensitive() const
{
    qWarning("%s: not implemented", __FUNCTION__);
    return false;
}

QAbstractFileEngine::FileFlags hrLodEngine::fileFlags(QAbstractFileEngine::FileFlags) const
{
    qWarning("%s: not implemented", __FUNCTION__);
    return 0;
}

bool hrLodEngine::setPermissions(uint)
{
    qWarning("%s: not implemented", __FUNCTION__);
    return false;
}

QString hrLodEngine::fileName(QAbstractFileEngine::FileName file) const
{
    Q_UNUSED(file);
    qWarning("%s: not implemented", __FUNCTION__);
    return "";
}

uint hrLodEngine::ownerId(QAbstractFileEngine::FileOwner) const
{
    qWarning("%s: not implemented", __FUNCTION__);
    return 0;
}
QString hrLodEngine::owner(QAbstractFileEngine::FileOwner) const
{
    qWarning("%s: not implemented", __FUNCTION__);
    return "";
}

QDateTime hrLodEngine::fileTime(QAbstractFileEngine::FileTime) const
{
    qWarning("%s: not implemented", __FUNCTION__);
    return QDateTime();
}

QAbstractFileEngine::Iterator *hrLodEngine::beginEntryList(QDir::Filters filters, const QStringList &filterNames)
{
    qWarning("%s: not implemented", __FUNCTION__);
    return new hrLodEngineIterator(filters, filterNames);
}
QAbstractFileEngine::Iterator *hrLodEngine::endEntryList()
{
    qWarning("%s: not implemented", __FUNCTION__);
    return 0;
}

bool hrLodEngine::extension(Extension, const ExtensionOption *, ExtensionReturn *)
{
    qWarning("%s: not implemented", __FUNCTION__);
    return false;
}
bool hrLodEngine::supportsExtension(Extension) const
{
    qWarning("%s: not implemented", __FUNCTION__);
    return false;
}
