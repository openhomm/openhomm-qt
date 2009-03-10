CONFIG(debug, debug|release) {
    TARGET = hrDefd
}
else {
    TARGET = hrDef
}

include(../base_plugins.pri)

SOURCES = main.cpp
