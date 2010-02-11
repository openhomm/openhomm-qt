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
#include "hrGraphicsItem.hpp"

#ifndef GL_SGIS_generate_mipmap
#define GL_GENERATE_MIPMAP_SGIS           0x8191
#define GL_GENERATE_MIPMAP_HINT_SGIS      0x8192
#endif

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

class hrCacheItem
{
public:
    hrCacheItem() : curBlock(0)
    {
        blocks.append(Block());
    }
    ~hrCacheItem()
    {
        for (int i = 0; i < blocks.size(); i++)
        {
            const Block& b = blocks.at(i);
            for (int j = 0; j < b.frames.size(); j++)
                glDeleteTextures(1, &b.frames.at(j));
        }
    }
    GLuint getFrame(int frame) const
    {
        const Block &b = blocks.at(curBlock);
        Q_ASSERT(frame >= 0 && frame < b.frames.size());
        return b.frames.at(frame);
    }
    void addFrame(GLuint tx)
    {
        blocks[curBlock].frames.append(tx);
    }
    int countFrames() const
    {
        return blocks.at(curBlock).frames.size();
    }
    void setSize(const QSize &s)
    {
        size = s;
    }
    QSize getSize() const
    {
        return size;
    }
private:
    struct Block
    {
        QVector<GLuint> frames;
    };
    QVector<Block> blocks;

    int curBlock;
    QSize size;
};

class hrCache
{
public:
    ~hrCache();

    static hrCache& Get()
    {
        static hrCache cache;
        return cache;
    }

    GLuint getTexture(const hrGraphicsItem& item);
    hrGraphicsItem loadItem(const QString& name, bool notDeletable = false);
    void setTarget(GLuint textureTarget);

    static qint32 NearestGLTextureSize(qint32 v);
private:
    hrCache();
    int inc;
    GLuint target;

    QFile file;

    int loadFromDisc(QString name, hrCacheItem*);
    void loadFromDiscPrepared(qint64 pos, hrCacheItem* item);

    void saveToDisc(QString name);

    GLuint bindImage(const GLvoid* image, int width, int height);

    QCache<hrCacheKey, hrCacheItem> cache;
    QMap<hrCacheKey, hrCacheItem*> map;

    QMap<hrCacheKey, QString> files;
    QMap<hrCacheKey, qint64> offsets;

    QMap<QString, qint64> fat;

    QImage ImageToPOT(const QImage &im) const;
};
