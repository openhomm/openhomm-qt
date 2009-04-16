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
#include "hrWindow.hpp"
#include "hrApplication.hpp"

hrWindow::hrWindow(): QWidget()
{
    setWindowTitle("OpenHoMM, fullscreen - F11, zoom - +/-");

    hrH3MReader reader;

    QString filename = hrApplication::getMapName();
    qDebug() << filename;
    if ( filename.isEmpty() )
        reader.load("maps/Back For Revenge.h3m");
    else
        reader.load(filename);

    int size = reader.getSize();

    scene = new hrScene(size, size);

    for (int i = 0; i < size * size; i++)
    {
        hrTile tile = reader.getTile(i);
        scene->addTile(tile);
    }

    int cnt = reader.getObjectsCount();
    for (int i = 0; i < cnt; i++)
    {
        hrSceneObject object = reader.getObject(i);
        scene->addObject(object);
    }
    scene->sortObjects();

    scene->setCursor("cradvntr.def");

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
    static int i = 0;
    if (event->key() == Qt::Key_F11)
    {
        setWindowState(windowState() ^ Qt::WindowFullScreen);
    }
    else if (event->key() == Qt::Key_Plus || event->key() == Qt::Key_Equal)
    {
        i > 3 ? i = 0 : i++;
        w->setZoom(i);
    }
    else if (event->key() == Qt::Key_Minus)
    {
        i > 0 ? i-- : i = 0;
        w->setZoom(i);
    }
}
