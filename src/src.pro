include(../base.pri)
include(3rdparty/3rdparty.pri)
CONFIG(debug, debug|release):TARGET = openhomm3d
else:TARGET = openhomm3
CONFIG += warn_on
QT += opengl
TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += . \
    gui \
    render \
    core

include(core/core.pri)
include(game/game.pri)
include(gui/gui.pri)
include(render/render.pri)


PRECOMPILED_HEADER = precompiled.hpp

SOURCES += main.cpp \
    hrApplication.cpp \

HEADERS += hrApplication.hpp \
    precompiled.hpp

QMAKE_DISTCLEAN += version.hpp
