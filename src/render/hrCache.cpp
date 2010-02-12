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
#include "hrCache.hpp"

const int HR_FILE_SIZE = 50000;

hrCache::hrCache() : inc(0), target(0)
{
    cache.setMaxCost(300);

    QDir dir;
    dir.mkdir("cache/");
    cacheFile.setFileName("cache/cache0");
    cacheFile.open(QIODevice::ReadOnly | QIODevice::Truncate | QIODevice::Append);

    /*QFile fileFat("cache/cache1");
    if (fileFat.open(QIODevice::ReadOnly))
    {
        QDataStream in(&fileFat);
        in >> fat;
    }*/
}

hrCache::~hrCache()
{
    /*QFile fileFat("cache/cache1");
    if (fileFat.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QDataStream out(&fileFat);
        out << fat;
    }*/
}

void hrCache::setTarget(GLuint textureTarget)
{
    target = textureTarget;
}

hrGraphicsItem hrCache::loadItem(const QString &name, bool notDeletable)
{
    hrCacheKey key = inc++;

    hrCacheItem *item;
    if (fat.contains(name))
    {
        item = LoadPrepared(fat[name]);
    }
    else
    {
        item = LoadAndPrepare(name);
    }

    files[key] = name;

    if (notDeletable)
    {
        map[key] = item;
    }
    else
    {
        cache.insert(key, item);
    }

    return hrGraphicsItem(key, item->countFrames(), item->getSize());
}


hrCacheItem* hrCache::Load(const QString &name) const
{
    hrCacheItem *item = new hrCacheItem();

    QImageReader ir("vfs:/" + name);
    QImage im;
    for (int i = 0; ir.jumpToImage(i); i++)
        if (ir.read(&im))
        {
            QImage conv = QGLWidget::convertToGLFormat(target == GL_TEXTURE_2D
                                                    ? ImageToPOT(im)
                                                    : im
                                                    );
            GLuint tx = bindImage(conv.bits(), conv.width(), conv.height());
            item->addFrame(tx);
        }
    item->setSize(im.size());

    if (ir.error())
    {
        qFatal("hrCache: %s", qPrintable(ir.errorString() + name));
    }

    return item;
}

hrCacheItem* hrCache::LoadAndPrepare(const QString &name)
{
    hrCacheItem *item = new hrCacheItem();

    QFile file("vfs:/" + name);
    file.open(QIODevice::ReadOnly);

    bool isHeaderWrite = true;
    bool isFileLarge = false;
    if (file.size() > HR_FILE_SIZE)
    {
        isFileLarge = true;

        if (!cacheFile.atEnd())
        {
            cacheFile.seek(cacheFile.size());
        }
        fat[name] = cacheFile.pos();
    }
    QDataStream out(&cacheFile);

    QImageReader ir(&file);
    QImage im;
    for (int i = 0; ir.jumpToImage(i); i++)
    {
        if (ir.read(&im))
        {
            QImage conv = QGLWidget::convertToGLFormat(target == GL_TEXTURE_2D
                                                    ? ImageToPOT(im)
                                                    : im
                                                    );
            GLuint tx = bindImage(conv.bits(), conv.width(), conv.height());
            item->addFrame(tx);
            
            if (isFileLarge)
            {
                if (isHeaderWrite)
                {
                    out << im.size();
                    out << ir.imageCount();
                    isHeaderWrite = false;
                }

                out << QByteArray::fromRawData((char*)conv.bits(), conv.byteCount());
            }
        }
    }
    item->setSize(im.size());

    if (ir.error())
    {
        qFatal("hrCache: %s", qPrintable(ir.errorString() + name));
    }
    if (cacheFile.error())
    {
        qFatal("hrCache: %s", qPrintable(cacheFile.errorString()));
    }
    
    return item;
}

hrCacheItem* hrCache::LoadPrepared(qint64 pos)
{
    hrCacheItem *item = new hrCacheItem();

    cacheFile.seek(pos);

    QDataStream in(&cacheFile);

    QSize size;
    in >> size;
    item->setSize(size);
    int count;
    in >> count;
    QByteArray image;
    for (int i = 0; i < count; i++)
    {
        in >> image;
        GLuint tx = bindImage(image.data(), size.width(), size.height());
        item->addFrame(tx);
    }

    if (cacheFile.error())
    {
        qFatal("hrCache: %s", qPrintable(cacheFile.errorString()));
    }
    return item;
}

GLuint hrCache::getTexture(const hrGraphicsItem &item)
{
    hrCacheItem *cacheItem;
    if (map.contains(item.key))
    {
        cacheItem = map[item.key];
    }
    else if (cache.contains(item.key))
    {
        cacheItem = cache[item.key];
    }
    else if (files.contains(item.key))
    {
        const QString &name = files[item.key];
        if (fat.contains(name))
        {
            cacheItem = LoadPrepared(fat[name]);
        }
        else
        {
            cacheItem = Load(name);
        }
        cache.insert(item.key, cacheItem);
    }
    else
    {
       qFatal("hrCache: key not found");
    }

    GLuint tx = cacheItem->getFrame(item.curFrame);
    return tx;
}

GLuint hrCache::bindImage(const GLvoid* image, int width, int height) const
{
    Q_ASSERT(target);
    GLuint format = GL_RGBA;
    GLuint tx;
    glGenTextures(1, &tx);
    glBindTexture(target, tx);


    if (target == GL_TEXTURE_2D)
    {
        int s = NearestGLTextureSize(qMax(width, height));
        width = s;
        height = s;

        glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(target, 0, format, width, height, 0
                 , GL_RGBA
                 , GL_UNSIGNED_BYTE
                 , image
                 );

    GLenum error = glGetError();

    if (error == GL_OUT_OF_MEMORY)
        qFatal("Out of texture memory");

    return tx;
}

QImage hrCache::ImageToPOT(const QImage &im) const
{
    int s = NearestGLTextureSize(qMax(im.width(), im.height()));
    return im.copy(0, 0, s, s);
}

/*!
  returns the highest number closest to v, which is a power of 2

  \sa http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
*/
qint32 hrCache::NearestGLTextureSize(qint32 v)
{
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}
