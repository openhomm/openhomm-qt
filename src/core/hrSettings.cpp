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
#include "hrSettings.hpp"

/*!
  \class hrSettings
  \brief The hrSettings class stores settings for further manipulation.
  Used the simple singleton pattern. Non-thread safe.
*/

hrSettings::hrSettings(QObject *parent) : QObject(parent)
{
    _settings = new QSettings("settings.ini", QSettings::IniFormat);
    _isFullscreen   = _settings->value("video/fullscreen", false).toBool();
    _render         = _settings->value("video/render").toString();
    _isShowmenu     = _settings->value("showmenu", true).toBool();
    _x              = _settings->value("x", 0).toInt();
    _y              = _settings->value("y", 0).toInt();
    _windowScrollSpeed = _settings->value("windowScrollSpeed", 2).toUInt();
}

hrSettings::~hrSettings()
{
    delete _settings;
}

void hrSettings::setFullscreen(bool fullscreen)
{
    _isFullscreen = fullscreen;
    _settings->setValue("video/fullscreen", fullscreen);
    _settings->sync();
}

void hrSettings::setShowmenu(bool showmenu)
{
    _isShowmenu = showmenu;
    _settings->setValue("showmenu", showmenu);
    _settings->sync();
}

void hrSettings::setRender(const QString &render)
{
    _render = render;
    _settings->setValue("video/render", render);
    _settings->sync();
}

void hrSettings::setX(qint32 x)
{
    _x = x;
    _settings->setValue("x", x);
    _settings->sync();
}

void hrSettings::setY(qint32 y)
{
    _y = y;
    _settings->setValue("y", y);
    _settings->sync();
}

void hrSettings::setWindowScrollSpeed(quint8 speed)
{
    _windowScrollSpeed = speed;
    _settings->setValue("windowScrollSpeed", speed);
    _settings->sync();
}
