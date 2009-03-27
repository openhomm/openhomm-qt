#ifndef WINDOW_H
#define WINDOW_H

#include "precompiled.hpp"
#include "hrGLWidget.hpp"

class hrWindow : public QWidget
{
    Q_OBJECT

public:
    hrWindow();
    ~hrWindow();

private:
    hrGLWidget *w;
    QTimer timer;

private slots:
    void resizeEvent(QResizeEvent *event);

};
#endif // WINDOW_H
