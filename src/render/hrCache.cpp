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
#include "hrGL.hpp"
#include "hrCache.hpp"
#include "hrSettings.hpp"

typedef void (APIENTRY *pfn_glCompressedTexImage2DARB) (GLenum, GLint, GLenum, GLsizei,
                                                        GLsizei, GLint, GLsizei, const GLvoid *);
static pfn_glCompressedTexImage2DARB hr_glCompressedTexImage2DARB = 0;

//typedef void (APIENTRY *pfn_glGetCompressedTexImageARB) (GLenum, GLint, void *);
//static pfn_glGetCompressedTexImageARB  hr_glGetCompressedTexImageARB = 0;



static const int HR_FILE_SIZE = 0;

hrCache::hrCache() : target(GL_TEXTURE_2D), format(GL_RGBA)
{
    cache.setMaxCost(1000);

    /*QDir dir;
    dir.mkdir("cache/");
    cacheFile.setFileName("cache/cache0");
    cacheFile.open(QIODevice::Append);*/
}

hrCache::~hrCache()
{
}

void hrCache::setContext(const QGLContext *context)
{
    if (context->isValid())
    {
        hr_glCompressedTexImage2DARB = (pfn_glCompressedTexImage2DARB)context->getProcAddress(QLatin1String("glCompressedTexImage2DARB"));
        if (!hr_glCompressedTexImage2DARB)
        {
            qWarning("hrCache: glCompressed* functions not found");
        }
        checkExtensions();
    }
    else
    {
        qFatal("hrCache: invalid context");
    }
}

hrGraphicsItem hrCache::loadItem(const QString &name, bool notDeletable)
{
    static int inc = 0;
    hrCacheKey key = inc++;

    hrCacheItem *item;
    //if (fat.contains(name))
    {
        //item = LoadPrepared(fat[name]);
    }
    //else
    {
        //item = LoadAndPrepare(name);
        item = Load(name);
    }

    if (notDeletable)
    {
        map[key] = item;
    }
    else
    {
        cache.insert(key, item);
        files[key] = name;
    }

    return hrGraphicsItem(key, item->countFrames(), item->getSize());
}


hrCacheItem* hrCache::Load(const QString &name) const
{
    hrCacheItem *item = new hrCacheItem();

    QImageReader ir("vfs:/" + name);
    QImage im;
    if (ir.read(&im))
    {
        item->addFrame(Load(im));
    }
    for (int i = 1; ir.jumpToImage(i); i++)
        if (ir.read(&im))
        {
            item->addFrame(Load(im));
        }

    item->setSize(im.size());

    if (ir.error())
    {
        qFatal("hrCache: %s", qPrintable(ir.errorString() + name));
    }

    return item;
}

GLuint hrCache::Load(const QImage &im) const
{
    QImage conv = QGLWidget::convertToGLFormat(target == GL_TEXTURE_2D
                                            ? ImageToPOT(im)
                                            : im
                                            );
    GLuint tx = bindImage(conv.bits()
                          , conv.width()
                          , conv.height()
                          , false
                          , 0
                          );
    return tx;
}

/*hrCacheItem* hrCache::LoadAndPrepare(const QString &name)
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
            GLuint tx = bindImage(conv.bits()
                                  , conv.width()
                                  , conv.height()
                                  , false
                                  , 0
                                  );
            item->addFrame(tx);
            
            if (isFileLarge)
            {
                if (isHeaderWrite)
                {
                    out << im.size();
                    out << ir.imageCount();
                    isHeaderWrite = false;
                }

// For compatible with Qt 4.5. Qt 4.5 must be built with qt3support option.
#if QT_VERSION >= 0x040600
                out << QByteArray::fromRawData((char*)conv.bits(), conv.byteCount());
#else
                out << QByteArray::fromRawData((char*)conv.bits(), conv.numBytes());
#endif
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
}*/

/*hrCacheItem* hrCache::LoadPrepared(qint64 pos)
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
        GLuint tx = bindImage(image.data()
                              , size.width()
                              , size.height()
                              , true
                              , image.size()
                              );
        item->addFrame(tx);
    }

    if (cacheFile.error())
    {
        qFatal("hrCache: %s", qPrintable(cacheFile.errorString()));
    }
    return item;
}*/

GLuint hrCache::getTexture(const hrGraphicsItem &item)
{
    hrCacheItem *cacheItem = NULL;
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
        //if (fat.contains(name))
        {
            //cacheItem = LoadPrepared(fat[name]);
        }
        //else
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

GLuint hrCache::bindImage(const GLvoid *image
                          , int width
                          , int height
                          , bool compressed
                          , int size) const
{
    GLuint tx;
    glGenTextures(1, &tx);
    glBindTexture(target, tx);

    if (target == GL_TEXTURE_2D)
    {
        int s = hrgl::NearestGLTextureSize(qMax(width, height));
        width = s;
        height = s;

        glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, /*GL_NEAREST*/GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, /*GL_NEAREST*/GL_LINEAR);

    if (compressed && hr_glCompressedTexImage2DARB)
    {
        hr_glCompressedTexImage2DARB(target, 0, format, width, height, 0
                                     , size
                                     , image
                                     );
    }
    else
    {
        glTexImage2D(target, 0, format, width, height, 0
                     , GL_RGBA
                     , GL_UNSIGNED_BYTE
                     , image
                     );
    }

    GLenum error = glGetError();

    if (error == GL_OUT_OF_MEMORY)
    {
        qFatal("Out of texture memory");
    }
    return tx;
}

int hrCache::getCompressedImageSize() const
{
    GLint param = 0;
    glGetTexLevelParameteriv(target, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &param);
    return param;
}

QImage hrCache::ImageToPOT(const QImage &im) const
{
    int s = hrgl::NearestGLTextureSize(qMax(im.width(), im.height()));
    return im.copy(0, 0, s, s);
}

void hrCache::checkExtensions()
{
    QString extensions(reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS)));

    if (hrSettings::get().isCompression())
    {
        if (extensions.contains("GL_ARB_texture_compression"))
        {
            //qWarning("GL_ARB_texture_compression");
            format = GL_COMPRESSED_RGBA_ARB;
        }
        if (extensions.contains("GL_EXT_texture_compression_s3tc"))
        {
            //qWarning("GL_EXT_texture_compression_s3tc");
            format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        }
    }
}
