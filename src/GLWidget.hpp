#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QPainter>
#include <QGLWidget>
#include <QImageReader>

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent);

public slots:
    void animate();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    //void paintEvent(QPaintEvent *event);

private:
    QWidget *parent;
    QImageReader ir;
    QVector<QImage> obj;
    QVector<QImage> tile;

    QMap<QImage *, GLuint> txImageIntMap;

    int curFrame;

    void begin();

    void textureFromImage(QImage *im);
    void drawImage(const QRect &r, QImage *im, const QRect &src, int alpha);

    int q_gl_texture;
    bool texture_rects;
};

#endif // GLWIDGET_H

