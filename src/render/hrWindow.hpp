#ifndef WINDOW_H
#define WINDOW_H

#include "precompiled.hpp"
#include "hrGLWidget.hpp"
#include "hrScene.hpp"

class hrWindow : public QWidget
{
    Q_OBJECT

public:
    hrWindow();
    ~hrWindow();

private:
    hrGLWidget *w;
    hrScene *scene;

private slots:
    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *event);

};
#endif // WINDOW_H
