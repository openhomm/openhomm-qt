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
#pragma once

struct LodEntry
{
    char name[12];  // null-terminated, sometimes null-padded too, sometimes padded with, well, something after the null
    quint32 junk;   //
    quint32 offset; // includes the header size, no preprocessing required
    quint32 size;   // real size
    quint32 type;   // what's in the file - probably not used by the game directly, more on that below
    quint32 csize;  // compressed size
};

struct LodHeader
{
    quint32 magic;  // always 0x00444f4c, that is, a null-terminated "LOD" string
    quint32 type;   // 200 for base archives, 500 for expansion pack archives, probably completely arbitrary numbers
    quint32 files;  // quantity of files
};

const quint32 LOD_MAGIC = 0x00444f4c;
typedef QHash<QString, LodEntry> LodFat;
typedef QHashIterator<QString, LodEntry> LodFatIterator;

struct LodFile
{
    LodFat fat;
    QFile *file;
    LodFile() : file(NULL){}
};

typedef QHash<QString, LodFile*> LodFiles;

class hrLodEngine: public QAbstractFileEngine
{
public:
    explicit hrLodEngine(const QString &path);
    ~hrLodEngine();

    virtual void setFileName(const QString &file);

    virtual bool open(QIODevice::OpenMode flags) ;
    virtual bool close();
    virtual qint64 size() const;
    virtual qint64 pos() const;
    virtual bool atEnd() const;
    virtual bool seek(qint64);
    virtual qint64 read(char *data, qint64 maxlen);

    virtual QStringList entryList(QDir::Filters filters, const QStringList &filterNames) const;

    virtual QAbstractFileEngine::FileFlags fileFlags(QAbstractFileEngine::FileFlags type) const;

    virtual QString fileName(QAbstractFileEngine::FileName file) const;

    virtual Iterator *beginEntryList(QDir::Filters filters, const QStringList &filterNames);

    bool supportsExtension(Extension) const;
private:
    bool preload_fat();
    bool preload_file();

private:
    LodFile* _lf;
    QString _filename, _archivename;
    QBuffer *_buffer;
    bool compressed;
};

class hrLodEngineHandler : public QAbstractFileEngineHandler
{
public:
    QAbstractFileEngine *create(const QString &fileName) const;
};
