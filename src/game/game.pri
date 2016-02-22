INCLUDEPATH += $$PWD
VPATH += $$PWD

include (map/map.pri)
HEADERS += hrH3Map.hpp \
    hrTile.hpp \
    hrObject.hpp \
    hrSceneObject.hpp

SOURCES += hrH3Map.cpp \
    hrTile.cpp \
    hrObject.cpp \
    hrSceneObject.cpp
