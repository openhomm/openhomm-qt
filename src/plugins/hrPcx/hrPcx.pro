include(../base_plugins.pri)

CONFIG(debug, debug|release) {
    TARGET = hrPcxd
}
else {
    TARGET = hrPcx
}

SOURCES = main.cpp
