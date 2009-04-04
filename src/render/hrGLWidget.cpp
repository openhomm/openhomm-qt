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


hrGLWidget::hrGLWidget(QWidget *parent, hrScene *scene)
 : QGLWidget(QGLFormat(QGL::SampleBuffers), parent), scene(scene)
{
    isAnimate = false;
    dx = 0; dy = 0;
    connect(&scrollTimer, SIGNAL(timeout()), this, SLOT(scroll()));
    connect(&animateTimer, SIGNAL(timeout()), this, SLOT(animate()));

    setMouseTracking(true);
    setCursor(QCursor(scene->getCursor(0), 0, 0));

    makeCurrent();
    setAutoBufferSwap(true);

    // todo: use it
    GLint param;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &param);
    qWarning("maxsize: %d", param);
    if (param != 0)
        maxTexDim = param;
    else
        maxTexDim = 512;

    textureTarget = getTextureTarget();
    if (textureTarget == GL_TEXTURE_2D)
    {
        objects = scene->getAllObjects();
        QLinkedListIterator<hrObject> it(objects);
        while (it.hasNext())
        {
            hrGraphicsItem* item = scene->getItem(it.next());
            QImage im;
            int cnt = item->getFramesCount();
            for (int i = 0; i < cnt; i++)
            {
                im = item->getNextFrame();
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
    viewport = rect();
    scene->setSceneViewport(coord::toCellRect(viewport));
    objects = scene->getViewportObjects();
    tilesSecondLayer = scene->getViewportTilesSecondLayer();

    Begin();
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
}

void hrGLWidget::End()
{
}

void hrGLWidget::paintGL()
{
    //Begin();
    glClear(GL_COLOR_BUFFER_BIT);
    GLuint id = 0;

    QRect r = scene->getSceneViewport();

    for (int y = r.y(); y < r.y() + r.height(); y++)
        for (int x = r.x(); x < r.x() + r.width(); x++)
        {
            hrTile tile = scene->getTile(x, y);
            QImage im = scene->getImage(tile);
            if (im.isNull()) continue;
            id = bindTexture(im, textureTarget, GL_RGBA8);
            drawTexture(coord::toPixPoint(QPoint(x, y)), id , textureTarget);
        }


    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    QLinkedListIterator<hrTile> itTiles(tilesSecondLayer);

    while (itTiles.hasNext())
    {
        hrTile tile = itTiles.next();
        QImage im = scene->getImage(tile);
        if (im.isNull()) continue;
        id = bindTexture(im, textureTarget, GL_RGBA8);
        drawTexture(coord::toPixPoint(tile.getPoint()), id, textureTarget);
    }

    QLinkedListIterator<hrObject> it(objects);

    if (isAnimate)
    {
        while (it.hasNext())
        {
            hrGraphicsItem *item = scene->getItem(it.next());
            item->nextFrame();
        }
        it.toFront();
    }

    while (it.hasNext())
    {
        hrObject obj = it.next();
        QImage im = scene->getImage(obj);
        if (im.isNull()) continue;
        id = bindTexture(im, textureTarget, GL_RGBA8);
        drawTexture(coord::toPixPoint(obj.getPoint()), id, textureTarget);
    }

    glDisable(GL_BLEND);
    //End();
}

void hrGLWidget::ImageToPOT(hrGraphicsItem *item, QImage im)
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
qint32 hrGLWidget::NearestGLTextureSize(qint32 v)
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
            objects = scene->getViewportObjects();
            tilesSecondLayer = scene->getViewportTilesSecondLayer();
        }
        glTranslatef(dx, dy, 0);
        updateGL();
    }
    else
    {
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
        setCursor(QCursor(scene->getCursor(39), 0, 0));
    }
    else if (pos.x() > width() - border && pos.y() < border)
    {
        // top right
        dx = -c; dy = c;
        setCursor(QCursor(scene->getCursor(33), 0, 0));
    }
    else if (pos.x() > width() - border && pos.y() > height() - border)
    {
        // bottom right
        dx = -c; dy = -c;
        setCursor(QCursor(scene->getCursor(35), 0, 0));
    }
    else if (pos.x() < border && pos.y() > height() - border)
    {
        // bottom left
        dx = c; dy = -c;
        setCursor(QCursor(scene->getCursor(37), 0, 0));
    }
    else if (pos.x() < border)
    {
        // left
        dx = c; dy = 0;
        setCursor(QCursor(scene->getCursor(38), 0, 0));
    }
    else if (pos.x() > width() - border)
    {
        // right
        dx = -c; dy = 0;
        setCursor(QCursor(scene->getCursor(34), 0, 0));
    }
    else if (pos.y() < border)
    {
        // up
        dx = 0; dy = c;
        setCursor(QCursor(scene->getCursor(32), 0, 0));
    }
    else if (pos.y() > height() - border)
    {
        // down
        dx = 0; dy = -c;
        setCursor(QCursor(scene->getCursor(36), 0, 0));
    }
    else
    {
        // stop
        dx = 0; dy = 0;
        startScrollTimer = false;
        setCursor(QCursor(scene->getCursor(0), 0, 0));
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

int hrGLWidget::getTextureTarget()
{
    QString extensions(reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS)));

    if (extensions.contains("GL_NV_texture_rectangle"))
    {
        qWarning("GL_NV_texture_rectangle");
        return GL_TEXTURE_RECTANGLE_NV;
    }
    else if (extensions.contains("GL_ARB_texture_rectangle"))
    {
        qWarning("GL_ARB_texture_rectangle");
        return GL_TEXTURE_RECTANGLE_ARB;
    }
    else if (extensions.contains("GL_EXT_texture_rectangle"))
    {
        qWarning("GL_EXT_texture_rectangle");
        return GL_TEXTURE_RECTANGLE_EXT;
    }
    else
    {
        qWarning("GL_TEXTURE_2D");
        return GL_TEXTURE_2D;
    }
}

