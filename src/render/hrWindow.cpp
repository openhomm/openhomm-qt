#include "hrWindow.hpp"

hrWindow::hrWindow(): QWidget()
{
    w = new hrGLWidget(this);
    w->resize(800, 600);
    w->setScene();
    connect(&timer, SIGNAL(timeout()), w, SLOT(animate()));
    timer.start(200);
}

hrWindow::~hrWindow()
{
    delete w;
}

void hrWindow::resizeEvent(QResizeEvent *event)
{
    w->resize(width(), height());
}
