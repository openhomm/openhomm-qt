include(../base.pri)
CONFIG(debug, debug|release):TARGET = openhomm3d
else:TARGET = openhomm3
CONFIG += warn_on
QT += opengl \
    network \
    xml
TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += . \
    gui \
    render \
    core

PRECOMPILED_HEADER = precompiled.hpp
SOURCES += main.cpp \
    hrApplication.cpp
include(gui/gui.pri)
include(render/render.pri)
include(core/core.pri)
HEADERS += hrApplication.hpp
