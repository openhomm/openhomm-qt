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


hrGLWidget::hrGLWidget(QWidget *parent, hrScene *scene)
 : QGLWidget(parent)
 , scene(scene)
{
    zoom = 1.0;
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
        maxTexDim = param;
    else
        maxTexDim = 512;

    checkExtensions(); // set flags

    if (!textureRects)
    {
        objects = scene->getAllObjects();
        QLinkedListIterator<hrSceneObject> it(objects);
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

void hrGLWidget::setZoom(int i)
{
    switch (i)
    {
        case 0:
            zoom = 1.0;
            break;
        case 1:
            zoom = 1.28;
            break;
        case 2:
            zoom = 1.6;
            break;
        default:
            zoom = 1.0;
    }
    resizeGL(width(), height());
}

void hrGLWidget::initializeGL()
{
    Begin();
}

void hrGLWidget::resizeGL(int w, int h)
{
    w = (int)(w / zoom);
    h = (int)(h / zoom);
    viewport = QRect(0, 0, w, h);
    //viewport = rect();
    scene->setSceneViewport(coord::toCellRect(viewport));
    objects.clear();
    objects = scene->getViewportObjects();

    glViewport(0, 0, (int)(w * zoom), (int)(h * zoom));
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

void hrGLWidget::animateTiles() const
{
    QRect r = scene->getSceneViewport();
    int bottom = r.y() + r.height();
    int right = r.x() + r.width();

    for (int y = r.y(); y < bottom; y++)
        for (int x = r.x(); x < right; x++)
        {
            const hrTile &tile = scene->getTile(x, y);
            hrGraphicsItem *item = scene->getItem(tile.terrainId);
            item->nextFrame();

            if (tile.hasRiver())
            {
                item = scene->getItem(tile.getRiverId());
                item->nextFrame();
            }
        }
}

void hrGLWidget::drawTiles()
{
    QRect r = scene->getSceneViewport();
    int bottom = r.y() + r.height();
    int right = r.x() + r.width();

    for (int y = r.y(); y < bottom; y++)
        for (int x = r.x(); x < right; x++)
        {
            QPoint point = coord::toPixPoint(QPoint(x, y));
            const hrTile &tile = scene->getTile(x, y);

            drawImage(point, scene->getImageTerrain(tile));
            if (tile.hasRiver())
            {
                drawImage(point, scene->getImageRiver(tile));
            }
            if (tile.hasRoad())
            {
                drawImage(point, scene->getImageRoad(tile));
            }
        }
}

void hrGLWidget::animateObjects() const
{
    QLinkedListIterator<hrSceneObject> it(objects);
    while (it.hasNext())
    {
        hrGraphicsItem *item = scene->getItem(it.next());
        item->nextFrame();
    }
}

void hrGLWidget::drawObjects()
{
    QLinkedListIterator<hrSceneObject> it(objects);
    while (it.hasNext())
    {
        const hrSceneObject &obj = it.next();
        drawImage(coord::toPixPoint(obj.getPoint()), scene->getImage(obj));
    }
}

GLuint hrGLWidget::bindImage(const QImage &im)
{
    GLTexture *tx;
    quint64 key = im.cacheKey();
    if (texs.contains(key))
    {
        tx = texs[key];
        glBindTexture(textureTarget, tx->getId());
        return tx->getId();
    }

    const QImage &txim = QGLWidget::convertToGLFormat(im);

    GLuint format = GL_RGBA;
    GLuint param;
    glGenTextures(1, &param);
    tx = new GLTexture(param);
    glBindTexture(textureTarget, tx->getId());

    /*if (generateMipmap) // it's slow
    {
        glHint(GL_GENERATE_MIPMAP_HINT_SGIS, GL_NICEST);
        glTexParameteri(textureTarget, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
        glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }
    else*/
    {
        glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    glTexImage2D(textureTarget, 0, format, txim.width(), txim.height(), 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, txim.bits());

    texs.insert(key, tx);
    return tx->getId();
}

void hrGLWidget::drawImage(const QPoint &point, const QImage &im)
{
    double x1, y1, x2, y2;
    const QRect r(point.x(), point.y(), im.width(), im.height());
    const QRect src(0, 0, im.width(), im.height());
    
    bindImage(im);

    glEnable(textureTarget);

    glBegin(GL_QUADS);
    {
        if (!textureRects)
        {
            x1 = 0;//src.x() / (double)im.width();
            x2 = 1;//x1 + src.width() / (double)im.width();
            y1 = 0;//src.y() / (double)im.height();
            y2 = 1;//y1 + src.height() / (double)im.height();
        }
        else
        {
            x1 = src.x();
            x2 = src.width();
            y1 = src.y();
            y2 = src.height();
        }

        glTexCoord2f(x1, y2); glVertex2f(r.x(), r.y());
        glTexCoord2f(x2, y2); glVertex2f(r.x() + r.width(), r.y());
        glTexCoord2f(x2, y1); glVertex2f(r.x() + r.width(), r.y() + r.height());
        glTexCoord2f(x1, y1); glVertex2f(r.x(), r.y() + r.height());
    }
    glEnd();

    glDisable(textureTarget);
}

void hrGLWidget::paintGL()
{
    //Begin();
    glClear(GL_COLOR_BUFFER_BIT);

    if (isAnimate)
        animateTiles();

    drawTiles();

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
    QRect size = coord::toPixRect(scene->getSize());
    QRect sceneViewport = coord::toPixRect(scene->getSceneViewport());

    QPoint oldPos = viewport.topLeft();
    QPoint newPos = oldPos - QPoint(dx, dy);

    viewport.moveTo(newPos);
    if (size.contains(viewport))
    {
        if (!sceneViewport.contains(viewport))
        {
            scene->setSceneViewport(coord::toCellRect(viewport));
            objects.clear();
            objects = scene->getViewportObjects();
        }
        glTranslatef(dx, dy, 0);
        updateGL();
    }
    else
    {
        viewport.moveTo(oldPos - QPoint(dx, 0));
        size.contains(viewport) ? dy = 0 : dx = 0;
        viewport.moveTo(oldPos);
    }
}


void hrGLWidget::mouseMoveEvent(QMouseEvent * event)
{
    QPoint pos = event->pos();
    const int border = 50;
    const int c = 20;
    const int delay = 30;
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
        {
            scrollTimer.stop();
        }
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

    return qMin(s, maxTexDim);
}
