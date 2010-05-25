INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

include (map/map.pri)
HEADERS += hrH3MReader.hpp \
    hrTile.hpp \
    hrObject.hpp \
    hrSceneObject.hpp

SOURCES += hrH3MReader.cpp \
    hrTile.cpp \
    hrObject.cpp \
    hrSceneObject.cpp
