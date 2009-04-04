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
    hrGLWidget(QWidget *parent, hrScene *scene);
    ~hrGLWidget();
    void startAnimate(int delay);
    void stopAnimate();

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

    QLinkedList<hrObject> objects;
    QLinkedList<hrTile> tilesSecondLayer;

    QTimer scrollTimer;
    QTimer animateTimer;
    QRect viewport;
    int dx;
    int dy;

    bool isAnimate;

    void Begin();
    void End();

    void ImageToPOT(hrGraphicsItem *item, QImage im);
    qint32 NearestGLTextureSize(qint32 v);

    int getTextureTarget();
    int maxTexDim;
    int textureTarget;

private slots:
    void mouseMoveEvent(QMouseEvent * event);
    void leaveEvent(QEvent * event);
};

#endif // GLWIDGET_H

