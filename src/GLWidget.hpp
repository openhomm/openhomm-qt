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
    void setScene();

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

    int curFrame;
    int MaxTexDim;

    void Begin();
    void End();

    //void LoadViewport();

    int NearestGLTextureSize(int v);
    int q_gl_texture;
    bool texture_rects;
};

#endif // GLWIDGET_H

