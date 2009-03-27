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


hrGLWidget::hrGLWidget(QWidget *parent)
 : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    //this->parent = parent;
    curFrame = 0;
    x0 = 0; y0 = 0;
    dx = 0; dy = 0;
    connect(&scrollTimer, SIGNAL(timeout()), this, SLOT(scroll()));

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

void hrGLWidget::initializeGL()
{
    Begin();
}

void hrGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void hrGLWidget::Begin()
{
    glViewport(0, 0, width(), height());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width(), height(), 0, -999999, 999999);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Make sure depth testing and lighting are disabled for 2D rendering
    glPushAttrib(GL_DEPTH_BUFFER_BIT | GL_LIGHTING_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
}

void hrGLWidget::End()
{
    glPopAttrib();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void hrGLWidget::paintGL()
{
    //Begin();
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    QImage im = tile.at(0);
    GLuint id = 0;
    for (int i = 0, y = y0; i < 20; i++, y += 32)
        for (int j = 0, x = x0; j < 20; j++, x += 32)
        {
            id = bindTexture(im, q_gl_texture, GL_RGBA8);
            drawTexture(QPoint(x, y), id, q_gl_texture);
        }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    for (int i = 0, y = y0; i < 10; i++, y += 64)
        for (int j = 0, x = x0; j < 10; j++, x += 64)
        {
            id = bindTexture(obj.at(curFrame), q_gl_texture, GL_RGBA8);
            drawTexture(QPoint(x, y), id, q_gl_texture);
        }
    glDisable(GL_BLEND);
    //End();
}

void hrGLWidget::animate()
{
    curFrame < ir.imageCount() - 1 ? curFrame++ : curFrame = 0;
    updateGL();
}

void hrGLWidget::scroll()
{
    //qWarning("repaint");
    x0 += dx;
    y0 += dy;
    updateGL();
}

void hrGLWidget::setScene(/*hrScene &scene*/)
{
    this->scene = scene;
    QImage im("lod:/data/h3sprite.lod/grastl.def");
    tile.append(im);

    ir.setFileName("lod:/data/h3sprite.lod/advmwind.def");
    for (int i = 0; ir.jumpToImage(i); i++)
    {
        if (ir.read(&im))
        {
            if (!texture_rects)
            {
                int s = NearestGLTextureSize(qMax(im.width(), im.height()));
                if (im.height() < s || im.width() < s)
                {
                    QImage tmp = im.copy(0, 0, s, s);
                    obj.append(tmp);
                }
                else if (im.height() > s || im.width() > s)
                {
                    // todo
                    for (int j = 0; j < im.width() / s + 1; j++)
                        for (int k = 0; k < im.height() / s + 1; k++)
                        {
                            QImage tmp = im.copy(j * s, k * s, s, s);
                            obj.append(tmp);
                        }
                }
                else
                    obj.append(im);
            }
            else
            {
                obj.append(im);
            }
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
    const int c = 15;
    const int delay = 20;

    if (pos.x() < border && pos.y() < border)
    {
        //qWarning("top left");
        dx = c; dy = c;
        scrollTimer.start(delay);
    }
    else if (pos.x() > width() - border && pos.y() < border)
    {
        //qWarning("top right");
        dx = -c; dy = c;
        scrollTimer.start(delay);
    }
    else if (pos.x() > width() - border && pos.y() > height() - border)
    {
        //qWarning("bottom right");
        dx = -c; dy = -c;
        scrollTimer.start(delay);
    }
    else if (pos.x() < border && pos.y() > height() - border)
    {
        //qWarning("bottom left");
        dx = c; dy = -c;
        scrollTimer.start(delay);
    }
    else if (pos.x() < border)
    {
        //qWarning("left");
        dx = c; dy = 0;
        scrollTimer.start(delay);
    }
    else if (pos.x() > width() - border)
    {
        //qWarning("right");
        dx = -c; dy = 0;
        scrollTimer.start(delay);
    }
    else if (pos.y() < border)
    {
        //qWarning("up");
        dx = 0; dy = c;
        scrollTimer.start(delay);
    }
    else if (pos.y() > height() - border)
    {
        //qWarning("down");
        dx = 0; dy = -c;
        scrollTimer.start(delay);
    }
    else
    {
        if (scrollTimer.isActive())
        {
            //qWarning("stop");
            dx = 0; dy = 0;
            scrollTimer.stop();
        }
    }

}
