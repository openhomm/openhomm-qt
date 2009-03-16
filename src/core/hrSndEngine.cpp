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

QAbstractFileEngine* hrSndEngineHandler::create(const QString &filename) const
{
    if ( filename.size() > 0 && filename.startsWith("snd:/", Qt::CaseInsensitive) )
    {
        return new hrSndEngine(filename);
    }
    return 0;
}

hrSndEngine::hrSndEngine(const QString& path) : QAbstractFileEngine()
{
    qWarning("%s: not implemented", Q_FUNC_INFO);
    this->setFileName(path);
}

hrSndEngine::~hrSndEngine()
{
    qWarning("%s: not implemented", Q_FUNC_INFO);
}

void hrSndEngine::setFileName(const QString &file)
{
    _filename = file;
}

bool hrSndEngine::open(QIODevice::OpenMode flags)
{
    Q_UNUSED(flags);
    qWarning("%s: not implemented", Q_FUNC_INFO);
    return false;
}
bool hrSndEngine::close()
{
    qWarning("%s: not implemented", Q_FUNC_INFO);
    return false;
}
bool hrSndEngine::flush()
{
    qWarning("%s: not implemented", Q_FUNC_INFO);
    return false;
}
qint64 hrSndEngine::size() const
{
    qWarning("%s: not implemented", Q_FUNC_INFO);
    return 0;
}
qint64 hrSndEngine::pos() const
{
    qWarning("%s: not implemented", Q_FUNC_INFO);
    return 0;
}
bool hrSndEngine::atEnd() const
{
    qWarning("%s: not implemented", Q_FUNC_INFO);
    return false;
}
bool hrSndEngine::seek(qint64)
{
    qWarning("%s: not implemented", Q_FUNC_INFO);
    return false;
}
qint64 hrSndEngine::read(char *data, qint64 maxlen)
{
    Q_UNUSED(data);
    Q_UNUSED(maxlen);
    qWarning("%s: not implemented", Q_FUNC_INFO);
    return 0;
}
qint64 hrSndEngine::write(const char *data, qint64 len)
{
    Q_UNUSED(data);
    Q_UNUSED(len);
    qWarning("%s: not implemented", Q_FUNC_INFO);
    return 0;
}

bool hrSndEngine::remove()
{
    qWarning("%s: not implemented", Q_FUNC_INFO);
    return false;
}
bool hrSndEngine::copy(const QString &)
{
    qWarning("%s: not implemented", Q_FUNC_INFO);
    return false;
}
bool hrSndEngine::rename(const QString &)
{
    qWarning("%s: not implemented", Q_FUNC_INFO);
    return false;
}
bool hrSndEngine::link(const QString &)
{
    qWarning("%s: not implemented", Q_FUNC_INFO);
    return false;
}

bool hrSndEngine::isSequential() const
{
    qWarning("%s: not implemented", Q_FUNC_INFO);
    return false;
}

bool hrSndEngine::isRelativePath() const
{
    qWarning("%s: not implemented", Q_FUNC_INFO);
    return false;
}

bool hrSndEngine::mkdir(const QString &, bool) const
{
    qWarning("%s: not implemented", Q_FUNC_INFO);
    return false;
}
bool hrSndEngine::rmdir(const QString &, bool) const
{
    qWarning("%s: not implemented", Q_FUNC_INFO);
    return false;
}

bool hrSndEngine::setSize(qint64 size)
{
    Q_UNUSED(size);
    qWarning("%s: not implemented", Q_FUNC_INFO);
    return false;
}

QStringList hrSndEngine::entryList(QDir::Filters filters, const QStringList &filterNames) const
{
    qWarning("%s: not implemented", Q_FUNC_INFO);
    return QAbstractFileEngine::entryList(filters, filterNames);
}

bool hrSndEngine::caseSensitive() const
{
    qWarning("%s: not implemented", Q_FUNC_INFO);
    return false;
}

QAbstractFileEngine::FileFlags hrSndEngine::fileFlags(QAbstractFileEngine::FileFlags) const
{
    qWarning("%s: not implemented", Q_FUNC_INFO);
    return 0;
}

bool hrSndEngine::setPermissions(uint)
{
    qWarning("%s: not implemented", Q_FUNC_INFO);
    return false;
}

QString hrSndEngine::fileName(QAbstractFileEngine::FileName file) const
{
    Q_UNUSED(file);
    qWarning("%s: not implemented", Q_FUNC_INFO);
    return "";
}

uint hrSndEngine::ownerId(QAbstractFileEngine::FileOwner) const
{
    qWarning("%s: not implemented", Q_FUNC_INFO);
    return 0;
}
QString hrSndEngine::owner(QAbstractFileEngine::FileOwner) const
{
    qWarning("%s: not implemented", Q_FUNC_INFO);
    return "";
}

QDateTime hrSndEngine::fileTime(QAbstractFileEngine::FileTime) const
{
    qWarning("%s: not implemented", Q_FUNC_INFO);
    return QDateTime();
}

QAbstractFileEngine::Iterator *hrSndEngine::beginEntryList(QDir::Filters filters, const QStringList &filterNames)
{
    qWarning("%s: not implemented", Q_FUNC_INFO);
    return new hrSndEngineIterator(filters, filterNames);
}
QAbstractFileEngine::Iterator *hrSndEngine::endEntryList()
{
    qWarning("%s: not implemented", Q_FUNC_INFO);
    return 0;
}

bool hrSndEngine::extension(Extension, const ExtensionOption *, ExtensionReturn *)
{
    qWarning("%s: not implemented", Q_FUNC_INFO);
    return false;
}
bool hrSndEngine::supportsExtension(Extension) const
{
    qWarning("%s: not implemented", Q_FUNC_INFO);
    return false;
}
