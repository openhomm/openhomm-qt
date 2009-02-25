include(../base_plugins.pri)

CONFIG(debug, debug|release) {
    TARGET = hrDefd
}
else {
    TARGET = hrDef
}

SOURCES = main.cpp
