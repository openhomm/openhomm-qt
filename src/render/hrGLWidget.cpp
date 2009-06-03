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
#include "hrGLWidget.hpp"

#ifndef GL_TEXTURE_RECTANGLE_ARB
#define GL_TEXTURE_RECTANGLE_ARB 0x84F5
#endif

#ifndef GL_TEXTURE_RECTANGLE_EXT
#define GL_TEXTURE_RECTANGLE_EXT 0x84F5
#endif

#ifndef GL_TEXTURE_RECTANGLE_NV
#define GL_TEXTURE_RECTANGLE_NV 0x84F5
#endif

#ifndef GL_SGIS_generate_mipmap
#define GL_GENERATE_MIPMAP_SGIS           0x8191
#define GL_GENERATE_MIPMAP_HINT_SGIS      0x8192
#endif

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

hrGLWidget::hrGLWidget(QWidget *parent, hrScene *scene)
 : QGLWidget(parent)
 , scene(scene)
{
    isAnimate = false;
    dx = 0; dy = 0;
    connect(&scrollTimer, SIGNAL(timeout()), this, SLOT(scroll()));
    connect(&animateTimer, SIGNAL(timeout()), this, SLOT(animate()));

    setMouseTracking(true);
    setCursor(scene->getCursor(0));

    makeCurrent();
    texs.setMaxCost(1000);

    // todo: use it
    GLint param;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &param);
    qWarning("maxsize: %d", param);
    if (param != 0)
        textureMaxDim = param;
    else
        textureMaxDim = 512;

    checkExtensions(); // set flags

    if (!textureRects)
    {
        objects = scene->getAllObjects();
        QListIterator<hrSceneObject> it(objects);
        while (it.hasNext())
        {
            hrGraphicsItem* item = scene->getItem(it.next());
            int cnt = item->getFramesCount();
            for (int i = 0; i < cnt; i++)
            {
                const QImage &im = item->getNextFrame();
                ImageToPOT(item, im);
            }
        }
    }
}

hrGLWidget::~hrGLWidget()
{
}

void hrGLWidget::startAnimate(int delay)
{
    animateTimer.start(delay);
}

void hrGLWidget::stopAnimate()
{
    if (animateTimer.isActive())
        animateTimer.stop();
}

void hrGLWidget::initializeGL()
{
    Begin();
}

void hrGLWidget::resizeGL(int w, int h)
{
    viewport = QRect(0, 0, w, h);
    scene->setSceneViewport(coord::toCell(viewport));
    objects = scene->getViewportObjects();
    tiles = scene->getViewportTiles();
    oldTileId = -1;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, h, 0, -999999, 999999);

    //Begin();
    //glViewport(0, 0, w, h);
}

void hrGLWidget::Begin()
{
    glViewport(0, 0, width(), height());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width(), height(), 0, -999999, 999999);

     // Make sure depth testing and lighting are disabled for 2D rendering
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
}

void hrGLWidget::End()
{
}

void hrGLWidget::animateObjects() const
{
    QListIterator<hrSceneObject> it(objects);
    while (it.hasNext())
    {
        hrGraphicsItem *item = scene->getItem(it.next());
        item->nextFrame();
    }
}

void hrGLWidget::drawObjects()
{
    QListIterator<hrSceneObject> it(objects);
    while (it.hasNext())
    {
        const hrSceneObject &obj = it.next();
        const QImage &im = scene->getImage(obj);
        bindImage(im, textureTarget);
        drawImage(coord::toPix(obj.getPoint()), im.rect());
    }
}

GLuint hrGLWidget::bindImage(const QImage &im, GLuint target)
{
    GLTexture *tx;
    quint64 key = im.cacheKey();
    if (texs.contains(key))
    {
        tx = texs[key];
        glBindTexture(target, tx->getId());
        return tx->getId();
    }

    QImage txim = QGLWidget::convertToGLFormat(im);

    GLuint format = GL_RGBA;
    GLuint param;
    glGenTextures(1, &param);
    tx = new GLTexture(param);
    glBindTexture(target, tx->getId());

    /*if (generateMipmap) // it's slow
    {
        glHint(GL_GENERATE_MIPMAP_HINT_SGIS, GL_NICEST);
        glTexParameteri(textureTarget, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
        glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    }*/

    if (!textureRects)
    {
        glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(target, 0, format, txim.width(), txim.height(), 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, txim.bits());

    texs.insert(key, tx);
    return tx->getId();
}

void hrGLWidget::drawImage(const QPoint &point, const QRect &src)
{
    int x1, y1, x2, y2;
    const QRect r(point.x(), point.y(), src.width(), src.height());

    if (!textureRects)
    {
        x1 = 0;
        x2 = 1;
        y1 = 0;
        y2 = 1;
    }
    else
    {
        x1 = 0;
        x2 = src.width();
        y1 = 0;
        y2 = src.height();
    }

    glEnable(textureTarget);
    glBegin(GL_QUADS);
    {
        glTexCoord2i(x1, y2); glVertex2i(r.x(), r.y());
        glTexCoord2i(x2, y2); glVertex2i(r.x() + r.width(), r.y());
        glTexCoord2i(x2, y1); glVertex2i(r.x() + r.width(), r.y() + r.height());
        glTexCoord2i(x1, y1); glVertex2i(r.x(), r.y() + r.height());
    }
    glEnd();
    glDisable(textureTarget);
}

void hrGLWidget::drawAtlasTiles()
{
    QListIterator<hrSceneTile> it(tiles);
    while (it.hasNext())
    {
        const hrSceneTile &tile = it.next();
        hrTileAtlas* atlas = scene->getAtlas(tile);
        if (tile.getId() != oldTileId)
        {
            oldTileId = tile.getId();
            if (isAnimate)
                atlas->nextFrame();
            bindImage(atlas->getImage(), GL_TEXTURE_2D);
        }
        drawAtlasImage(tile.getPoint()
                      , atlas->getFrame(tile.getFrame())
                      , atlas->getDim()
                      , tile.isHorizontal()
                      , tile.isVertical()
                      );
    }
}

void hrGLWidget::drawAtlasImage(const QPoint &point
                               , const QRect &src
                               , int dim
                               , bool horizontal
                               , bool vertical
                               )
{
    double x1, y1, x2, y2;
    const QRect r(point.x(), point.y(), coord::dim, coord::dim);

    x1 = src.x() / (double) dim;
    x2 = x1 + src.width() / (double) dim;

    y2 = 1 - src.y() / (double) dim;
    y1 = y2 - src.height() / (double) dim;

    if (horizontal && vertical)
    {
        qSwap(x1, x2);
        qSwap(y1, y2);
    }
    else if (horizontal)
    {
        qSwap(x1, x2);
    }
    else if (vertical)
    {
        qSwap(y1, y2);
    }

    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    {
        glTexCoord2f(x1, y2); glVertex2f(r.x(), r.y());
        glTexCoord2f(x2, y2); glVertex2f(r.x() + r.width(), r.y());
        glTexCoord2f(x2, y1); glVertex2f(r.x() + r.width(), r.y() + r.height());
        glTexCoord2f(x1, y1); glVertex2f(r.x(), r.y() + r.height());
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void hrGLWidget::paintGL()
{
    //Begin();
    glClear(GL_COLOR_BUFFER_BIT);

    drawAtlasTiles();

    if (isAnimate)
        animateObjects();

    drawObjects();

    //End();
}


void hrGLWidget::animate()
{
    isAnimate = true;
    updateGL();
}

void hrGLWidget::scroll()
{
    isAnimate = false;
    QRect size = coord::toPix(scene->getSize());
    QRect sceneViewport = coord::toPix(scene->getSceneViewport());

    QPoint oldPos = viewport.topLeft();

    viewport.moveTo(oldPos - QPoint(dx, dy));
    if (!size.contains(viewport))
    {
        viewport.moveTo(oldPos - QPoint(dx, 0));
        if (!size.contains(viewport))
        {
            viewport.moveTo(oldPos - QPoint(0, dy));
            if (!size.contains(viewport))
            {
                viewport.moveTo(oldPos);
                return;
            }
            else
                dx = 0;
        }
        else
            dy = 0;
    }

    if (!sceneViewport.contains(viewport))
    {
        scene->setSceneViewport(coord::toCell(viewport));
        objects = scene->getViewportObjects();
        tiles = scene->getViewportTiles();
        oldTileId = -1;
    }
    glTranslatef(dx, dy, 0);
    updateGL();
}


void hrGLWidget::mouseMoveEvent(QMouseEvent * event)
{
    QPoint pos = event->pos();
    const int border = 50;
    const int c = 16;
    const int delay = 20;
    bool startScrollTimer = true;

    if (pos.x() < border && pos.y() < border)
    {
        // top left
        dx = c; dy = c;
        setCursor(scene->getCursor(39));
    }
    else if (pos.x() > width() - border && pos.y() < border)
    {
        // top right
        dx = -c; dy = c;
        setCursor(scene->getCursor(33));
    }
    else if (pos.x() > width() - border && pos.y() > height() - border)
    {
        // bottom right
        dx = -c; dy = -c;
        setCursor(scene->getCursor(35));
    }
    else if (pos.x() < border && pos.y() > height() - border)
    {
        // bottom left
        dx = c; dy = -c;
        setCursor(scene->getCursor(37));
    }
    else if (pos.x() < border)
    {
        // left
        dx = c; dy = 0;
        setCursor(scene->getCursor(38));
    }
    else if (pos.x() > width() - border)
    {
        // right
        dx = -c; dy = 0;
        setCursor(scene->getCursor(34));
    }
    else if (pos.y() < border)
    {
        // up
        dx = 0; dy = c;
        setCursor(scene->getCursor(32));
    }
    else if (pos.y() > height() - border)
    {
        // down
        dx = 0; dy = -c;
        setCursor(scene->getCursor(36));
    }
    else
    {
        // stop
        dx = 0; dy = 0;
        startScrollTimer = false;
        setCursor(scene->getCursor(0));
    }

    if (startScrollTimer)
    {
        if (!scrollTimer.isActive())
            scrollTimer.start(delay);
    }
    else
    {
        if (scrollTimer.isActive())
            scrollTimer.stop();
    }
}

void hrGLWidget::leaveEvent(QEvent * event)
{
    if (scrollTimer.isActive())
    {
        scrollTimer.stop();
    }
}

void hrGLWidget::checkExtensions()
{
    QString extensions(reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS)));

    textureRects = true;
    if (extensions.contains("GL_NV_texture_rectangle"))
    {
        qWarning("GL_NV_texture_rectangle");
        textureTarget = GL_TEXTURE_RECTANGLE_NV;
    }
    else if (extensions.contains("GL_ARB_texture_rectangle"))
    {
        qWarning("GL_ARB_texture_rectangle");
        textureTarget = GL_TEXTURE_RECTANGLE_ARB;
    }
    else if (extensions.contains("GL_EXT_texture_rectangle"))
    {
        qWarning("GL_EXT_texture_rectangle");
        textureTarget = GL_TEXTURE_RECTANGLE_EXT;
    }
    else
    {
        qWarning("GL_TEXTURE_2D");
        textureTarget = GL_TEXTURE_2D;
        textureRects = false;
    }

    if (!textureRects)
        generateMipmap = extensions.contains("GL_SGIS_generate_mipmap");
    else
        generateMipmap = false;
}


void hrGLWidget::ImageToPOT(hrGraphicsItem *item, const QImage &im) const
{
    int s = NearestGLTextureSize(qMax(im.width(), im.height()));
    if (im.height() < s || im.width() < s)
    {
        item->modifyFrame(im.copy(0, 0, s, s));
    }
    else if (im.height() > s || im.width() > s)
    {
        // todo
        qWarning("too big texture");
        item->modifyFrame(im.copy(0, 0, s, s));
    }
}

// returns the highest number closest to v, which is a power of 2
qint32 hrGLWidget::NearestGLTextureSize(qint32 v) const
{
    qint32 n = 0, last = 0;
    qint32 s;

    for (s = 0; s < 32; ++s)
    {
        if (((v >> s) & 1) == 1)
        {
            ++n;
            last = s;
        }
    }

    if (n > 1)
        s = 1 << (last + 1);
    else
        s = 1 << last;

    return qMin(s, textureMaxDim);
}
