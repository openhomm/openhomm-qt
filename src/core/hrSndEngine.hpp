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

struct SndEntry
{
    char    name[40];
    quint32 offset;
    quint32 size;
};

typedef QHash<QString, SndEntry> SndFat;
typedef QHashIterator<QString, SndEntry> SndFatIterator;

struct SndFile
{
    QFile *file;
    SndFat fat;
    SndFile() : file(NULL){}
};

typedef QHash<QString, SndFile*> SndFiles;

class hrSndEngine: public QAbstractFileEngine
{
public:
    explicit hrSndEngine(const QString &path);
    ~hrSndEngine();

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
    SndFile * _sf;
    QString _filename, _archivename;
    QBuffer *_buffer;
};

class hrSndEngineHandler : public QAbstractFileEngineHandler
{
public:
    QAbstractFileEngine *create(const QString &fileName) const;
};
