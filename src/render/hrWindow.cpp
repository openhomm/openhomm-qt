// openhomm - open source clone of Heroes of Might and Magic III
// Copyright (C) 2009-2010 openhomm developers team (see AUTHORS)
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
#include "hrH3MReader.hpp"
#include "hrSettings.hpp"
#include "hrAdventureScreen.hpp"
#include "hrRender.hpp"

/*!
  \class hrWindow
  \brief The hrWindow class
*/
hrWindow::hrWindow(QMainWindow *parent):
        QMainWindow(parent), scene(NULL), render(NULL)
{
    setWindowTitle("OpenHoMM, fullscreen - F11, menu - F12");

    createMenu();

    int gameX = hrSettings::get().x(),
        gameY = hrSettings::get().y();

    if ( gameX > 0 && gameY > 0 )
        move(gameX, gameY);

    scene = new hrAdventureScreen();
    render = new hrRender(this, scene);

    setCentralWidget(render);
    resize(800,600);

    if ( hrSettings::get().isFullscreen() )
    {
        QSize windowmode_resolution(800,600);
        hrFullscreenWrapper::enableFullscreen(windowmode_resolution);
        setWindowState(windowState() | Qt::WindowFullScreen );
        menuBar->hide();
    }
    else if ( hrSettings::get().isShowmenu() )
        menuBar->show();
}

hrWindow::~hrWindow()
{
    delete render;
    delete scene;
}

void hrWindow::AvdventureScreen()
{
    hrH3MReader reader;

    QString filename = hrApplication::getMapName();

    bool isMapLoad;
    if ( filename.isEmpty() )
        isMapLoad = reader.load("Maps/Back For Revenge.h3m");
    else
        isMapLoad = reader.load(filename);

    if (isMapLoad)
    {
        scene->loadMap(&reader);
    }
}

void hrWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
}

void hrWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_F11)
    {
        hrSettings::get().setFullscreen( !hrSettings::get().isFullscreen() );
        if(hrSettings::get().isFullscreen())
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

            if ( hrSettings::get().isShowmenu() )
                menuBar->show();
        }
    }
    if (event->key() == Qt::Key_F12)
    {
        hrSettings::get().setShowmenu(!hrSettings::get().isShowmenu());
        if ( hrSettings::get().isShowmenu() && !hrSettings::get().isFullscreen())
            menuBar->show();
        else
            menuBar->hide();
    }
}

void hrWindow::moveEvent(QMoveEvent *event)
{
    if (hrSettings::get().isFullscreen())
    {
        hrSettings::get().setX(event->pos().x());
        hrSettings::get().setY(event->pos().y());
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
