CONFIG(debug, debug|release):TARGET = defviewerd
else:TARGET = defviewer
include(../base_tools.pri)
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    hrTreeView.cpp
HEADERS += mainwindow.h \
    hrTreeView.hpp
DEPENDPATH += .
FORMS += mainwindow.ui
