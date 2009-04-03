#include "hrWindow.hpp"

hrWindow::hrWindow(): QWidget()
{
    scene = new hrScene(128, 128);
    for (int i = 0; i < 128 * 128; i++)
        i % 2 == 0 ? scene->addTile(0, "grastl.def", 0) : scene->addTile(0, "grastl.def", 1);
    scene->addObject(2, "advmwind.def", 0, 0);
    scene->addObject(2, "advmwind.def", 15, 15);
    scene->addObject(2, "advmwind.def", 30, 15);
    scene->addObject(2, "advmwind.def", 15, 30);
    scene->addObject(2, "advmwind.def", 30, 30);
    w = new hrGLWidget(this, scene);
    w->resize(800, 600);
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
