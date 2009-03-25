#include "Window.h"

Window::Window(): QWidget()
{
    w = new GLWidget(this);
    w->resize(800, 600);
    w->setScene();
    connect(&timer, SIGNAL(timeout()), w, SLOT(animate()));
    timer.start(200);
}

Window::~Window()
{
    delete w;
}

void Window::resizeEvent(QResizeEvent *event)
{
    w->resize(width(), height());
}
