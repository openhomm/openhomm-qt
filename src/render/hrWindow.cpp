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

hrWindow::hrWindow(): QWidget()
{
    setWindowTitle("OpenHoMM, fullscreen - F11");

    hrH3MReader reader;
    reader.load("maps/Mutare's Apprentice");
    int size = reader.getSize();

    scene = new hrScene(size, size);

    for (int i = 0; i < size * size; i++)
    {
        hrTile tile = reader.getTile(i);
        qDebug() << "r tile: " << tile.terrainId;
        scene->addTile(tile);
    }

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
    if (event->key() == Qt::Key_F11)
    {
        setWindowState(windowState() ^ Qt::WindowFullScreen);
    }
}
