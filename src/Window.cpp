#include "Window.h"

Window::Window(): QWidget()
{
    w = new GLWidget(this);
    w->resize(800, 600);
    connect(&timer, SIGNAL(timeout()), w, SLOT(animate()));
    timer.start(200);
}

Window::~Window()
{
    delete w;
}
