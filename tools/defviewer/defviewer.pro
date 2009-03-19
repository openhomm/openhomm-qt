CONFIG(debug, debug|release):TARGET = defviewerd
else:TARGET = defviewer

QT += opengl

include(../base_tools.pri)
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp
HEADERS += mainwindow.h
FORMS += mainwindow.ui
