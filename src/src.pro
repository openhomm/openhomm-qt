win32-* { 
    win32-msvc2005 { 
        DESTDIR = ../bin/win32-msvc2005/
        OBJECTS_DIR = ../obj/win32-msvc2005/
        MOC_DIR = ../moc/win32-msvc2005/
        RCC_DIR = ../res/win32-msvc2005/
        DEFINES += "_CRT_SECURE_NO_WARNINGS" \
            "_CRT_NON_CONFORMING_SWPRINTFS"
        #QMAKE_CXXFLAGS_DEBUG += /Fd"../bin/win32-msvc2005/"
    }
    win32-msvc2008 { 
        DESTDIR = ../bin/win32-msvc2008/
        OBJECTS_DIR = ../obj/win32-msvc2008/
        MOC_DIR = ../moc/win32-msvc2008/
        RCC_DIR = ../res/win32-msvc2008/
        DEFINES += "_CRT_SECURE_NO_WARNINGS" \
            "_CRT_NON_CONFORMING_SWPRINTFS"
        #QMAKE_CXXFLAGS_DEBUG += /Fd"../bin/win32-msvc2008/"
    }
    win32-gcc { 
        DESTDIR = ../bin/win32-gcc/
        OBJECTS_DIR = ../obj/win32-gcc/
        MOC_DIR = ../moc/win32-gcc/
        RCC_DIR = ../res/win32-gcc/
    }
}
CONFIG(debug,debug|release) { 
    message(Debug build!)
    CONFIG += console
    DESTDIR = $$join(DESTDIR,,,debug)
    OBJECTS_DIR = $$join(OBJECTS_DIR,,,debug)
    MOC_DIR = $$join(MOC_DIR,,,debug)
    RCC_DIR = $$join(RCC_DIR,,,debug)
    TARGET = openhomm3d
}

# VERSION = 0.1.0ad
# RC_FILE = openhomm3d.rc
else { 
    message(Release build!)
    DESTDIR = $$join(DESTDIR,,,release)
    OBJECTS_DIR = $$join(OBJECTS_DIR,,,release)
    MOC_DIR = $$join(MOC_DIR,,,release)
    RCC_DIR = $$join(RCC_DIR,,,release)
    TARGET = openhomm3
}

# VERSION = 0.1.0a
# RC_FILE = openhomm3d.rc
CONFIG += warn_on
QT += opengl \
    network \
    xml
TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += . \
    gui \
    core
PRECOMPILED_HEADER = precompiled.hpp
SOURCES += main.cpp \
    hrApplication.cpp
include(gui/gui.pri)
include(core/core.pri)
HEADERS += hrApplication.hpp
