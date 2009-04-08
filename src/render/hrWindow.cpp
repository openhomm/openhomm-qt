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

    scene = new hrScene(128, 128);

    hrTile tile;
    for (int i = 0; i < 128 * 128; i++)
    {
        if (i % 2 == 0)
        {
            tile.terrainId = 0;
            scene->addTile(tile);
        }
        else
        {
            tile.terrainId = 2;
            scene->addTile(tile);
        }
    }

    for (int i = 0; i < 128; i += 2)
        for (int j = 0; j < 128; j += 2)
        {
            hrObject object("adcfra.def", i, j);
            scene->addObject(object);
        }

    scene->addObject(hrObject("advmwind.def", 0, 0));
    scene->addObject(hrObject("advmwind.def", 15, 15));
    scene->addObject(hrObject("advmwind.def", 30, 15));
    scene->addObject(hrObject("advmwind.def", 15, 30));
    scene->addObject(hrObject("advmwind.def", 30, 30));

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
