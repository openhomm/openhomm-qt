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
#include "hrFullscreenWrapper.hpp"

#ifdef Q_WS_WIN32
DEVMODE hrDevMode;

bool hrFullscreenWrapper::enableFullscreen(const QSize& resolution)
{

    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &hrDevMode);
    hrDevMode.dmPelsWidth = (unsigned long) resolution.width();
    hrDevMode.dmPelsHeight = (unsigned long) resolution.height();
    if(ChangeDisplaySettings(&hrDevMode, CDS_TEST) != DISP_CHANGE_SUCCESSFUL)
        return false;
    if(ChangeDisplaySettings(&hrDevMode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
        return false;
    return true;
}

bool hrFullscreenWrapper::disableFullscreen()
{
    if(ChangeDisplaySettings(NULL, 0) == DISP_CHANGE_SUCCESSFUL)
	    return true;
    return false;
}
#else

bool hrFullscreenWrapper::enableFullscreen(const QSize& resolution)
{
    Q_UNUSED(resolution);
    return false;
}

bool hrFullscreenWrapper::disableFullscreen()
{
    return false;
}

#endif
