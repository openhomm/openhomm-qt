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
    isAnimate = true;
    dx = 0; dy = 0;
    connect(&scrollTimer, SIGNAL(timeout()), this, SLOT(scroll()));
    connect(&animateTimer, SIGNAL(timeout()), this, SLOT(animate()));

    setMouseTracking(true);

    makeCurrent();
    setAutoBufferSwap(true);

    // todo: use it
    GLint param;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &param);
    qWarning("maxsize: %d", param);
    if (param != 0)
        MaxTexDim = param;
    else
        MaxTexDim = 512;

    QString extensions(reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS)));

    texture_rects = true;

    if (extensions.contains("GL_NV_texture_rectangle"))
    {
        qWarning("GL_NV_texture_rectangle");
        q_gl_texture = GL_TEXTURE_RECTANGLE_NV;
    }
    else if (extensions.contains("GL_ARB_texture_rectangle"))
    {
        qWarning("GL_ARB_texture_rectangle");
        q_gl_texture = GL_TEXTURE_RECTANGLE_ARB;
    }
    else if (extensions.contains("GL_EXT_texture_rectangle"))
    {
        qWarning("GL_EXT_texture_rectangle");
        q_gl_texture = GL_TEXTURE_RECTANGLE_EXT;
    }
    else
    {
        qWarning("GL_TEXTURE_2D");
        texture_rects = false;
        q_gl_texture = GL_TEXTURE_2D;
    }
}

void hrGLWidget::startAnimate(int delay)
{
    animateTimer.start(delay);
}

void hrGLWidget::initializeGL()
{
    Begin();
}

void hrGLWidget::resizeGL(int w, int h)
{
    viewport = rect();
    scene->setSceneViewport(coord::toCell(viewport));
    objects = scene->getViewportObjects();

    Begin();
    //glViewport(0, 0, w, h);
}

void hrGLWidget::Begin()
{
    glViewport(0, 0, width(), height());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width(), height(), 0, -999999, 999999);

    /*glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();*/

    // Make sure depth testing and lighting are disabled for 2D rendering
    /*glPushAttrib(GL_DEPTH_BUFFER_BIT | GL_LIGHTING_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);*/
}

void hrGLWidget::End()
{
    /*glPopAttrib();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();*/
}

void hrGLWidget::paintGL()
{
    //Begin();
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    GLuint id = 0;
    QImage im;

    QRect r = scene->getSceneViewport();

    for (int y = r.y(); y < r.y() + r.height(); y++)
        for (int x = r.x(); x < r.x() + r.width(); x++)
        {
            hrTile tile = scene->getTile(x, y);
            im = scene->getItem(tile);
            if (im.isNull()) continue;
            id = bindTexture(im, q_gl_texture, GL_RGBA8 );
            drawTexture(coord::toPix(QPoint(x, y)), id , q_gl_texture);
        }


    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    QLinkedListIterator<hrObject> it(objects);

    if (isAnimate)
    {
        while (it.hasNext())
        {
            hrObject obj = it.next();
            scene->setItemNextFrame(obj);
        }
        it.toFront();
    }

    while (it.hasNext())
    {
        hrObject obj = it.next();
        im = scene->getItem(obj);
        if (im.isNull()) continue;
        ImageToRect(im);
        id = bindTexture(im, q_gl_texture, GL_RGBA8);
        drawTexture(coord::toPix(obj.getPoint()), id, q_gl_texture);
    }

    glDisable(GL_BLEND);
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
    QPoint newPos = oldPos - QPoint(dx, dy);

    viewport.moveTo(newPos);
    if (size.contains(viewport))
    {
        if (!sceneViewport.contains(viewport))
        {
            scene->setSceneViewport(coord::toCell(viewport));
            objects = scene->getViewportObjects();
        }
        glTranslatef(dx, dy, 0);
        updateGL();
    }
    else
    {
        viewport.moveTo(oldPos);
    }
}

void hrGLWidget::ImageToRect(QImage &im)
{
    if (!texture_rects)
    {
        int s = NearestGLTextureSize(qMax(im.width(), im.height()));
        if (im.height() < s || im.width() < s)
        {
            im = im.copy(0, 0, s, s);
        }
        else if (im.height() > s || im.width() > s)
        {
            // todo
            qWarning("too big texture");
            im = im.copy(0, 0, s, s);
        }
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

    return qMin(s, MaxTexDim);
}

void hrGLWidget::mouseMoveEvent(QMouseEvent * event)
{
    QPoint pos = event->pos();
    const int border = 25;
    const int c = 16;
    const int delay = 20;
    bool startScrollTimer = true;


    if (pos.x() < border && pos.y() < border)
    {
        // top left
        dx = c; dy = c;
    }
    else if (pos.x() > width() - border && pos.y() < border)
    {
        // top right
        dx = -c; dy = c;
    }
    else if (pos.x() > width() - border && pos.y() > height() - border)
    {
        // bottom right
        dx = -c; dy = -c;
    }
    else if (pos.x() < border && pos.y() > height() - border)
    {
        // bottom left
        dx = c; dy = -c;
    }
    else if (pos.x() < border)
    {
        // left
        dx = c; dy = 0;
    }
    else if (pos.x() > width() - border)
    {
        // right
        dx = -c; dy = 0;
    }
    else if (pos.y() < border)
    {
        // up
        dx = 0; dy = c;
    }
    else if (pos.y() > height() - border)
    {
        // down
        dx = 0; dy = -c;
    }
    else
    {
        // stop
        dx = 0; dy = 0;
        startScrollTimer = false;
    }

    if (startScrollTimer)
    {
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
