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
#include "precompiled.hpp"
#include "hrWindow.hpp"
#include "hrApplication.hpp"
#include "version.hpp"
#include "hrFullscreenWrapper.hpp"

//hrWindow::hrWindow(QWidget *parent): QWidget(parent), scene(NULL), w(NULL)
hrWindow::hrWindow(QMainWindow *parent): QMainWindow(parent), scene(NULL), w(NULL)
{
    setWindowTitle("OpenHoMM, fullscreen - F11, menu - F12");//, zoom - +/-");

    createMenu();

    int gameX = hrSettings::get().gameX,
        gameY = hrSettings::get().gameY;

    if ( gameX > 0 && gameY > 0 )
        move(gameX, gameY);

    hrH3MReader reader;

    QString filename = hrApplication::getMapName();
    qDebug() << filename;
    bool isMapLoad;
    if ( filename.isEmpty() )
        isMapLoad = reader.load("maps/Back For Revenge.h3m");
    else
        isMapLoad = reader.load(filename);

    int size = reader.getSize();
    scene = new hrScene(size, size);

    if (isMapLoad)
    {
        for (int i = 0; i < size * size; i++)
        {
            hrTile tile = reader.getTile(i);
            scene->addTile(tile);
        }

        int cnt = reader.getObjectsCount();
        for (int i = 0; i < cnt; i++)
        {
            hrSceneObject object = reader.getObject(i);
            if (!object.isUnderground())
                scene->addObject(object);
        }
        scene->sortObjects();
    }

    scene->setCursor("cradvntr.def");

    w = new hrGLWidget(this, scene);
    setCentralWidget(w);
    resize(800,600);

    if ( hrSettings::get().isGameFullscreen )
        {
        QSize windowmode_resolution(800,600);
        hrFullscreenWrapper::enableFullscreen(windowmode_resolution);
        setWindowState(windowState() | Qt::WindowFullScreen );
        menuBar->hide();
        }
    else if ( hrSettings::get().isGameShowMenu )
        menuBar->show();

    w->startAnimate(200);
}

hrWindow::~hrWindow()
{
    delete w;
    delete scene;
}

void hrWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
}

void hrWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_F11)
        {
        hrSettings::get().isGameFullscreen = !hrSettings::get().isGameFullscreen;
        if(hrSettings::get().isGameFullscreen)
            {
            QSize windowmode_resolution(800,600);
            hrFullscreenWrapper::enableFullscreen(windowmode_resolution);
            setWindowState(windowState() | Qt::WindowFullScreen );
            menuBar->hide();
            }
        else
            {
            hrFullscreenWrapper::disableFullscreen();
            setWindowState(windowState() ^ Qt::WindowFullScreen );
            if ( hrSettings::get().isGameShowMenu ) menuBar->show();
            }
        }
    if (event->key() == Qt::Key_F12)
        {
        hrSettings::get().isGameShowMenu = !hrSettings::get().isGameShowMenu;
        if ( hrSettings::get().isGameShowMenu && !hrSettings::get().isGameFullscreen) menuBar->show();
        else menuBar->hide();
        }
}

void hrWindow::moveEvent(QMoveEvent *event)
{
    if (hrSettings::get().isGameFullscreen)
        {
        hrSettings::get().gameX = event->pos().x();
        hrSettings::get().gameY = event->pos().y();
        }
}

void hrWindow::createMenu()
{
    menuBar = new QMenuBar(this);
    menuBar->hide();
    QMenu *file = new QMenu("File", this);
    QMenu *help = new QMenu("Help", this);

    actFileExit = new QAction("Exit", this);
    connect(actFileExit, SIGNAL(triggered()), hrApplication::instance(), SLOT(quit()));
    actHelpAboutQt = new QAction("About Qt", this);
    connect(actHelpAboutQt, SIGNAL(triggered()), hrApplication::instance(), SLOT(aboutQt()));
    actHelpAbout = new QAction("About", this);
    connect(actHelpAbout, SIGNAL(triggered()), this, SLOT(about()));

    file->addAction(actFileExit);
    help->addAction(actHelpAbout);
    help->addAction(actHelpAboutQt);

    menuBar->addMenu(file);
    menuBar->addMenu(help);
    this->setMenuBar(menuBar);
}

void hrWindow::about()
{
    QMessageBox::information(this, "About OpenHoMM", VERSION_RELEASE_STR);
}
