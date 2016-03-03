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
#include "hrMessageOutput.hpp"

#if defined(Q_OS_UNIX) || defined(Q_OS_OSX)
    #define C_FRONTBLACK    30
    #define C_FRONTRED      31
    #define C_FRONTGREEN    32
    #define C_FRONTYELLOW   33
    #define C_FRONTBLUE     34
    #define C_FRONTPURPLE   35
    #define C_FRONTCYAN     36
    #define C_FRONTWHITE    37

    #define C_BACKBLACK     40
    #define C_BACKRED       41
    #define C_BACKGREEN     42
    #define C_BACKYELLOW    43
    #define C_BACKBLUE      44
    #define C_BACKPURPLE    45
    #define C_BACKCYAN      46
    #define C_BACKWHITE     47

    #define C_DEFAULT       0
    #define C_BRIGHT        1
    #define C_BLINK         5

#endif

#if defined(Q_OS_WIN32)
    #define C_FRONTBLACK    0
    #define C_FRONTRED      FOREGROUND_RED
    #define C_FRONTGREEN    FOREGROUND_GREEN
    #define C_FRONTBLUE     FOREGROUND_BLUE

    #define C_FRONTYELLOW   (FOREGROUND_RED | FOREGROUND_GREEN)
    #define C_FRONTPURPLE   (FOREGROUND_RED | FOREGROUND_BLUE)
    #define C_FRONTCYAN     (FOREGROUND_GREEN | FOREGROUND_BLUE)
    #define C_FRONTWHITE    (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)

    #define C_BACKBLACK     0
    #define C_BACKRED       BACKGROUND_RED
    #define C_BACKGREEN     BACKGROUND_GREEN
    #define C_BACKBLUE      BACKGROUND_BLUE

    #define C_BACKYELLOW    (BACKGROUND_RED | BACKGROUND_GREEN)
    #define C_BACKPURPLE    (BACKGROUND_RED | BACKGROUND_BLUE)
    #define C_BACKCYAN      (BACKGROUND_GREEN | BACKGROUND_BLUE)
    #define C_BACKWHITE     (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE)

    #define C_DEFAULT       0
    #define C_BRIGHT        FOREGROUND_INTENSITY
    #define C_BLINK         6
#endif

static void COLOR_set(short color)
{
#if defined(Q_OS_UNIX) || defined(Q_OS_OSX)
    fprintf(stdout, "\e[%dm", color);
#endif
#if defined(Q_OS_WIN32)
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color);
#endif
}

static void COLOR_reset()
{
#if defined(Q_OS_UNIX) || defined(Q_OS_OSX)
    fprintf(stdout, "\e[0m");
#endif
#if defined(Q_OS_WIN32)
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0);
#endif
}

void logColoredConsole (QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        COLOR_set(C_FRONTCYAN);
        fprintf(stdout, "(II): %s\n", localMsg.constData());
        COLOR_reset();
        break;
    case QtWarningMsg:
        COLOR_set(C_FRONTGREEN);
        fprintf(stdout, "(WW): %s\n", localMsg.constData());
        COLOR_reset();
        break;
    case QtCriticalMsg:
        COLOR_set(C_FRONTRED);
        fprintf(stdout, "(EE): %s\n", localMsg.constData());
        COLOR_reset();
        break;
    case QtFatalMsg:
        COLOR_set(C_BACKRED);
        fprintf(stdout, "(FATAL): %s\n", localMsg.constData());
        COLOR_reset();
        abort();
    }
}

void logConsole(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stdout, "(II): %s\n", localMsg.constData());
        break;
    case QtWarningMsg:
        fprintf(stdout, "(WW): %s\n", localMsg.constData());
        break;
    case QtCriticalMsg:
        fprintf(stdout, "(EE): %s\n", localMsg.constData());
        break;
    case QtFatalMsg:
        fprintf(stdout, "(FATAL): %s\n", localMsg.constData());
        abort();
    }
    fflush(stdout);
}

void logNull(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context);
    Q_UNUSED(type);
    Q_UNUSED(msg);
}
