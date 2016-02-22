include(../base.pri)
include(3rdparty/3rdparty.pri)
include(core/core.pri)
include(game/game.pri)
include(gui/gui.pri)
include(render/render.pri)
CONFIG(debug, debug|release):TARGET = openhomm3d
else:TARGET = openhomm3
CONFIG += warn_on c++11
QT += opengl core-private widgets
win32-* {
    LIBS += -lopengl32 -lglu32
}
TEMPLATE = app
VPATH += .
INCLUDEPATH += . \
    game \
    gui \
    render \
    core

DEFINES += QT_DEPRECATED_WARNINGS

PRECOMPILED_HEADER = precompiled.hpp

SOURCES += main.cpp \
    hrApplication.cpp \

HEADERS += hrApplication.hpp \
    precompiled.hpp

QMAKE_DISTCLEAN += version.hpp
