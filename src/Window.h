#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QTimer>
#include "GLWidget.hpp"

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();
    ~Window();

private:
    GLWidget *w;
    QTimer timer;

private slots:
    void resizeEvent(QResizeEvent *event);

};
#endif // WINDOW_H
