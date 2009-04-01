#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "precompiled.hpp"
#include <QGLWidget>

#include "hrScene.hpp"
#include "hrCoord.hpp"

class hrGLWidget : public QGLWidget
{
    Q_OBJECT

public:
    hrGLWidget(QWidget *parent);
    void setScene(hrScene *scene);
    void startAnimate(int delay);

public slots:
    void animate();
    void scroll();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    //void paintEvent(QPaintEvent *event);
private:
    hrScene *scene;

    QVector<hrTile> tiles;
    QLinkedList<hrObject> objects;

    QTimer scrollTimer;
    QTimer animateTimer;
    QRect viewport;
    int dx;
    int dy;

    bool isAnimate;

    int MaxTexDim;

    void Begin();
    void End();

    void ImageToRect(QImage &image);
    qint32 NearestGLTextureSize(qint32 v);
    int q_gl_texture;
    bool texture_rects;

private slots:
    void mouseMoveEvent(QMouseEvent * event);
};

#endif // GLWIDGET_H

