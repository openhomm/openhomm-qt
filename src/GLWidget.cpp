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
    this->parent = parent;
    curFrame = 0;
    //setFixedSize(800, 600);

    QImage im("lod:/data/h3sprite.lod/grastl.def");
    QImage tx;
    tx = QGLWidget::convertToGLFormat(im);
    tile.append(tx);

    ir.setFileName("lod:/data/h3sprite.lod/adcfra.def");
    for (int i = 0; ir.jumpToImage(i); i++)
    {
        if (ir.read(&im))
        {
            tx = QGLWidget::convertToGLFormat(im);
            obj.append(tx);
        }
    }
}

void GLWidget::initializeGL()
{
    makeCurrent();
    setAutoBufferSwap(true);

/*
    glViewport(0, 0, parent->width(), parent->height());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, parent->width(), parent->height(), 0, -999999, 999999);

    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();

    // Make sure depth testing and lighting are disabled for 2D rendering
    glPushAttrib( GL_DEPTH_BUFFER_BIT | GL_LIGHTING_BIT );
    glDisable( GL_DEPTH_TEST );
    glDisable( GL_LIGHTING );*/
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = w/(float)(h ? h : 1);
    glFrustum(-aspect, aspect, -1, 1, 10, 100);
    glTranslatef(-0.5f, -0.5f, -0.5f);
    glTranslatef(0.0f, 0.0f, -15.0f);
}

void GLWidget::begin()
{
    //glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glShadeModel(GL_FLAT);
    glViewport(0, 0, parent->width(), parent->height());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, parent->width(), parent->height(), 0, -999999, 999999);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //glDisable( GL_DEPTH_TEST );
    //glDisable( GL_LIGHTING );
}

void GLWidget::paintGL()
{
    begin();
    QImage tx = tile.at(0);
    for (int i = 0, y = 0; i < 20; i++, y += 32)
        for (int j = 0, x = 0; j < 20; j++, x += 32)
            drawImage(QRect(x, y, 32, 32), &tx, tx.rect(), 0);

    for (int i = 0, y = 0; i < 10; i++, y += 64)
        for (int j = 0, x = 0; j < 10; j++, x += 64)
        {
            //QImage tx_obj = obj.at(curFrame);
            drawImage(QRect(x, y, 32, 32)
                      , const_cast<QImage*>(&obj.at(curFrame))
                      , obj.at(curFrame).rect()
                      , 0);
        }
}

void GLWidget::animate()
{
    curFrame < ir.imageCount() - 1 ? curFrame++ : curFrame = 0;
    //qWarning("next frame");
    paintGL();
    updateGL();
}

void GLWidget::textureFromImage(QImage *tx)
{
    static bool init_extensions = true;

    if (init_extensions)
    {
        QString extensions(reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS)));

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
            qWarning("extension fail");
            return;
        }

        init_extensions = false;
    }

    if (txImageIntMap.contains(tx))
    {
        GLuint val = txImageIntMap[tx];
        glBindTexture(q_gl_texture, val);
        return;
    }


    //QImage tx;

    //tx = QGLWidget::convertToGLFormat(*im);

    GLuint format = GL_RGBA8;
    //GLuint format = GL_RGBA;

    GLuint tx_id;
    glGenTextures(1, &tx_id);
    glBindTexture(q_gl_texture, tx_id);
    glTexParameteri(q_gl_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameterf(q_gl_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(q_gl_texture, 0, format, tx->width(), tx->height(), 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, tx->bits());



    txImageIntMap[tx] = tx_id;

    //qWarning("somethin load");
}

void GLWidget::drawImage(const QRect &r, QImage *im,
                                  const QRect &src, int alpha)
{
    double x1, y1, x2, y2;

    glClearDepth(1.0f);

    textureFromImage(im);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushAttrib(GL_CURRENT_BIT);

    //glColor4f(1.0, 1.0, 1.0, 0.0);
    glColor3f(1.0, 1.0, 1.0);
    glEnable(q_gl_texture);
    glEnable(GL_BLEND);

    glBegin(GL_QUADS);
    {
        x1 = src.x();
        x2 = src.width();
        y1 = src.y();
        y2 = src.height();

        glTexCoord2f(x1, y2); glVertex2f(r.x(), r.y());
        glTexCoord2f(x2, y2); glVertex2f(r.x() + r.width(), r.y());
        glTexCoord2f(x2, y1); glVertex2f(r.x() + r.width(), r.y() + r.height());
        glTexCoord2f(x1, y1); glVertex2f(r.x(), r.y() + r.height());
    }
    glEnd();

    glDisable(GL_BLEND);
    glDisable(q_gl_texture);

    glPopAttrib();

    //qWarning("somethin draw");
}



