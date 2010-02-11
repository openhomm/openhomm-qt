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
#include "hrCache.hpp"

const int HR_FILE_SIZE = 0;//200000;

hrCache::hrCache() : inc(0), target(0)
{
    cache.setMaxCost(200);

    QDir dir;
    dir.mkdir("cache/");
    file.setFileName("cache/cache0");
    file.open(QIODevice::ReadOnly | QIODevice::Append);

    QFile fileFat("cache/cache1");
    if (fileFat.open(QIODevice::ReadOnly))
    {
        QDataStream in(&fileFat);
        in >> fat;
    }
}

hrCache::~hrCache()
{
    QFile fileFat("cache/cache1");
    if (fileFat.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QDataStream out(&fileFat);
        out << fat;
    }
}

void hrCache::setTarget(GLuint textureTarget)
{
    target = textureTarget;
}

hrGraphicsItem hrCache::loadItem(const QString &name, bool notDeletable)
{
    hrCacheKey key = inc++;

    hrCacheItem *item = new hrCacheItem();
    if (fat.contains(name))
    {
        qint64 pos = fat[name];
        offsets[key] = pos;

        loadFromDiscPrepared(pos, item);
    }
    else
    {
        int size = loadFromDisc(name, item);

        if (size > HR_FILE_SIZE)
        {
            if (!file.atEnd())
            {
                file.seek(file.size());
            }
            fat[name] = file.pos();
            offsets[key] = file.pos();

            saveToDisc(name);
        }
        else
        {
            files[key] = name;
        }
    }

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

int hrCache::loadFromDisc(QString name, hrCacheItem* item)
{
    Q_ASSERT(item);

    QImageReader ir("vfs:/" + name);
    QImage im;
    int bytes = 0;
    for (int i = 0; ir.jumpToImage(i); i++)
        if (ir.read(&im))
        {
            QImage conv = QGLWidget::convertToGLFormat(target == GL_TEXTURE_2D
                                                    ? ImageToPOT(im)
                                                    : im
                                                    );
            GLuint tx = bindImage(conv.bits(), conv.width(), conv.height());
            item->addFrame(tx);
            bytes += conv.byteCount();
        }

    if (ir.error())
    {
        qFatal("hrCache: %s", qPrintable(ir.errorString() + name));
    }
    item->setSize(im.size());
    return bytes;
}

void hrCache::loadFromDiscPrepared(qint64 pos, hrCacheItem *item)
{
    Q_ASSERT(item);

    file.seek(pos);
    QDataStream in(&file);
    QSize size;
    in >> size;
    item->setSize(size);
    QVector<QByteArray> images;
    in >> images;
    for (int i = 0; i < images.size(); i++)
    {
        GLuint tx = bindImage(images.at(i).data()
                              , size.width()
                              , size.height()
                              );
        item->addFrame(tx);
    }
    if (file.error())
    {
        qFatal("hrCache: %s", qPrintable(file.errorString()));
    }
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
    else if (offsets.contains(item.key))
    {
        cacheItem = new hrCacheItem();
        loadFromDiscPrepared(offsets[item.key], cacheItem);
        cache.insert(item.key, cacheItem);
    }
    else if (files.contains(item.key))
    {
        cacheItem = new hrCacheItem();
        loadFromDisc(files[item.key], cacheItem);
        cache.insert(item.key, cacheItem);
    }
    else
    {
       qFatal("hrCache: key not found");
    }
    
    GLuint tx = cacheItem->getFrame(item.curFrame);
    return tx;
}

GLuint hrCache::bindImage(const GLvoid* image, int width, int height)
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

void hrCache::saveToDisc(QString name)
{
    QImageReader ir("vfs:/" + name);
    QVector<QByteArray> images;
    QImage im;
    for (int i = 0; ir.jumpToImage(i); i++)
    {
        if (ir.read(&im))
        {
            QImage conv = QGLWidget::convertToGLFormat(target == GL_TEXTURE_2D
                                                       ? ImageToPOT(im)
                                                       : im
                                                       );
            QByteArray bytes((char*)conv.bits(), conv.byteCount());
            images.append(bytes);
        }
    }
    if (ir.error())
    {
        qFatal("hrCache: %s", qPrintable(ir.errorString() + name));
    }

    QSize size = im.size();

    QDataStream out(&file);
    out << size;
    out << images;

    if (file.error())
    {
        qFatal("hrCache: %s", qPrintable(file.errorString()));
    }
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
