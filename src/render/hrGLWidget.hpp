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

    QTimer scrollTimer;
    QTimer animateTimer;
    QRect viewport;
    int dx;
    int dy;

    bool isAnimate;

    void Begin();
    void End();

    void PaletteAnimation(QImage &im);
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

