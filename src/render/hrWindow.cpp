#include "hrWindow.hpp"

hrWindow::hrWindow(): QWidget()
{
    w = new hrGLWidget(this);
    w->resize(800, 600);
    scene = new hrScene(QRect(0, 0, 128, 128));
    scene->setViewport(QRect(0, 0, 32, 32));
    for (int i = 0; i < 128 * 128; i++)
        scene->addTile("grastl.def", 0);
    scene->addObject("advmwind.def", 0, 0);
    scene->addObject("advmwind.def", 15, 15);
    scene->addObject("advmwind.def", 30, 15);
    scene->addObject("advmwind.def", 15, 30);
    scene->addObject("advmwind.def", 30, 30);
    w->setScene(scene);
    w->startAnimate(200);
}

hrWindow::~hrWindow()
{
    delete w;
    delete scene;
}

void hrWindow::resizeEvent(QResizeEvent *event)
{
    w->resize(width(), height());
}
