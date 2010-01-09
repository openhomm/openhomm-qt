INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
contains(QT_CONFIG, zlib) { 
    INCLUDEPATH += $$(QTDIR)/src/3rdparty/zlib
    DEPENDPATH += $$(QTDIR)/src/3rdparty/zlib
}
HEADERS += hrString.hpp \
    hrLodEngine.hpp \
    hrLodEngineIterator.hpp \
    hrSndEngineIterator.hpp \
    hrSndEngine.hpp \
    hrH3MReader.hpp \
    hrTile.hpp \
    hrObject.hpp \
    hrSceneObject.hpp \
    hrPlatform.hpp \
    hrSettings.hpp \
    hrFilesystem.hpp \
    hrFileEngineHandlers.hpp
SOURCES += hrString.cpp \
    hrLodEngine.cpp \
    hrLodEngineIterator.cpp \
    hrSndEngineIterator.cpp \
    hrSndEngine.cpp \
    hrH3MReader.cpp \
    hrTile.cpp \
    hrObject.cpp \
    hrSceneObject.cpp \
    hrSettings.cpp \
    hrFilesystem.cpp \
    hrFileEngineHandlers.cpp
