CONFIG(debug, debug|release) {
    TARGET = hrPcxd
}
else {
    TARGET = hrPcx
}

include(../base_plugins.pri)

SOURCES =   main.cpp \
            hrPcxHandler.cpp

HEADERS = hrPcxHandler.hpp
