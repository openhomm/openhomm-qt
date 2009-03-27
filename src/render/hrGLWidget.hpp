#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "precompiled.hpp"
#include <QGLWidget>

#include "hrScene.hpp"

class hrGLWidget : public QGLWidget
{
    Q_OBJECT

public:
    hrGLWidget(QWidget *parent);
    void setScene(/*hrScene &scene*/);

public slots:
    void animate();
    void scroll();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    //void paintEvent(QPaintEvent *event);

private:
    //QWidget *parent;

    hrScene scene;

    QTimer scrollTimer;
    int x0;
    int y0;
    int dx;
    int dy;

    QImageReader ir;
    QVector<QImage> obj;
    QVector<QImage> tile;

    int curFrame;
    int MaxTexDim;

    void Begin();
    void End();

    //void LoadViewport();

    qint32 NearestGLTextureSize(qint32 v);
    int q_gl_texture;
    bool texture_rects;

private slots:
    void mouseMoveEvent(QMouseEvent * event);
};

#endif // GLWIDGET_H

