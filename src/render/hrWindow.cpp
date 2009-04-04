#include "hrWindow.hpp"

hrWindow::hrWindow(): QWidget()
{
    setWindowTitle("OpenHOMM, fullscreen - F11");

    scene = new hrScene(128, 128);
    QString path = "lod:/data/h3sprite.lod/";
    QString s = path + "grastl.def";
    for (int i = 0; i < 128 * 128; i++)
        i % 2 == 0 ? scene->addTile(0, 0, s)
                   : scene->addTile(0, 1, s, true);

    s = path + "cobbrd.def";
    scene->addTileSecondLayer(4, 0, 9, 9, s);

    s = path + "adcfra.def";
    for (int i = 0; i < 128; i += 2)
        for (int j = 0; j < 128; j += 2)
            scene->addObject(3, i, j, s);

    s = path + "advmwind.def";
    scene->addObject(2, 0, 0, s);
    scene->addObject(2, 15, 15, s);
    scene->addObject(2, 30, 15, s);
    scene->addObject(2, 15, 30, s);
    scene->addObject(2, 30, 30, s);

    s = path + "cradvntr.def";
    scene->setCursor(s);

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

void hrWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_F11)
    {
        setWindowState(windowState() ^ Qt::WindowFullScreen);
    }
}
