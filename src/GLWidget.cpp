#include "GLWidget.hpp"

#ifndef GL_TEXTURE_RECTANGLE_ARB
#define GL_TEXTURE_RECTANGLE_ARB 0x84F5
#endif

#ifndef GL_TEXTURE_RECTANGLE_EXT
#define GL_TEXTURE_RECTANGLE_EXT 0x84F5
#endif

#ifndef GL_TEXTURE_RECTANGLE_NV
#define GL_TEXTURE_RECTANGLE_NV 0x84F5
#endif


GLWidget::GLWidget(QWidget *parent)
 : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    //this->parent = parent;
    curFrame = 0;
    MaxTexDim = 512;

    initializeGL();
}

void GLWidget::initializeGL()
{
    makeCurrent();
    setAutoBufferSwap(true);

    static bool init = true;
    if (init)
    {
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
        init = false;
    }
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void GLWidget::Begin()
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

void GLWidget::End()
{
    glPopAttrib();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void GLWidget::paintGL()
{
    Begin();
    QImage im = tile.at(0);
    GLuint id = 0;
    for (int i = 0, y = 0; i < 20; i++, y += 32)
        for (int j = 0, x = 0; j < 20; j++, x += 32)
        {
            id = bindTexture(im, q_gl_texture, GL_RGBA8);
            drawTexture(QPoint(x, y), id, q_gl_texture);
        }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    for (int i = 0, y = 0; i < 10; i++, y += 64)
        for (int j = 0, x = 0; j < 10; j++, x += 64)
        {
            id = bindTexture(obj.at(curFrame), q_gl_texture, GL_RGBA8);
            drawTexture(QPoint(x, y), id, q_gl_texture);
        }
    glDisable(GL_BLEND);
    End();
}

void GLWidget::animate()
{
    curFrame < ir.imageCount() - 1 ? curFrame++ : curFrame = 0;
    paintGL();
    updateGL();
}

void GLWidget::setScene()
{
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
qint32 GLWidget::NearestGLTextureSize(qint32 v)
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

