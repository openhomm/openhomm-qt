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
#include "hrRender.hpp"


hrRender::hrRender(QWidget *parent, hrScene *s)
    : QGLWidget(parent), target(0)
{
    setMouseTracking(true);

    setScene(s);
}

hrRender::~hrRender()
{
}

void hrRender::setScene(hrScene *s)
{
    Q_ASSERT(s);

    scene = s;
    connect(scene, SIGNAL(sceneChanged()), this, SLOT(onSceneChanged()));
    connect(scene, SIGNAL(cursorChaged(int)), this, SLOT(onCursorChanged(int)));
}

void hrRender::onSceneChanged()
{
    updateGL();
}

void hrRender::onCursorChanged(int type)
{
    setCursor(scene->getCursor(type));
}

void hrRender::initializeGL()
{
    checkExtensions();
    hrCache& cache = hrCache::Get();
    cache.setTarget(target);

    glViewport(0, 0, width(), height());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width(), height(), 0);

    // Make sure depth testing and lighting are disabled for 2D rendering
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_DITHER);
    glShadeModel(GL_FLAT);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

void hrRender::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, h, 0);

    scene->setViewport(w, h);
}

void hrRender::paintGL()
{
    //QTime timer;
    //timer.start();
    glClear(GL_COLOR_BUFFER_BIT);

    hrCache& cache = hrCache::Get();
    scene->collect();
    QListIterator<hrGraphicsItem> it = scene->getItems();

    while (it.hasNext())
    {
        const hrGraphicsItem item = it.next();
        GLuint tx = cache.getTexture(item);
        drawSprite(tx, item.rect, item.isHorizontal, item.isVertical);
    }

    //qDebug() << timer.elapsed();
}

void hrRender::drawSprite(GLuint tx, QRect r, bool horizontal, bool vertical)
{
    int x1, y1, x2, y2;

    if (target == GL_TEXTURE_2D)
    {
        int s = hrCache::NearestGLTextureSize(qMax(r.width(), r.height()));
        r.setWidth(s);
        r.setHeight(s);

        x1 = 0;
        x2 = 1;
        y1 = 0;
        y2 = 1;
    }
    else
    {
        x1 = 0;
        x2 = r.width();
        y1 = 0;
        y2 = r.height();
    }

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

    glBindTexture(target, tx);

    glEnable(target);
    GLint v[] = {  r.x()             , r.y()
                 , r.x() + r.width() , r.y()
                 , r.x() + r.width() , r.y() + r.height()
                 , r.x()             , r.y() + r.height()
               };
    GLint t[] = { x1, y2, x2, y2, x2, y1, x1, y1 };

    glVertexPointer(2, GL_INT, 0, v);
    glTexCoordPointer(2, GL_INT, 0, t);

    glDrawArrays(GL_QUADS, 0, 4);
    glDisable(target);
}

void hrRender::mouseMoveEvent(QMouseEvent *event)
{
    scene->onMouseEvent(event);
}

void hrRender::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    scene->onMouseLeave();
}

void hrRender::checkExtensions()
{
    QString extensions(reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS)));

    if (extensions.contains("GL_ARB_texture_rectangle"))
    {
        qWarning("GL_ARB_texture_rectangle");
        target = GL_TEXTURE_RECTANGLE_ARB;
    }
    else if (extensions.contains("GL_NV_texture_rectangle"))
    {
        qWarning("GL_NV_texture_rectangle");
        target = GL_TEXTURE_RECTANGLE_NV;
    }
    else if (extensions.contains("GL_EXT_texture_rectangle"))
    {
        qWarning("GL_EXT_texture_rectangle");
        target = GL_TEXTURE_RECTANGLE_EXT;
    }
    else
    {
        qWarning("GL_TEXTURE_2D");
        target = GL_TEXTURE_2D;
    }
}
