CONFIG(debug, debug|release) {
    TARGET = hrPcxd
}
else {
    TARGET = hrPcx
}
PLUGIN_TYPE = imageformats
PLUGIN_CLASS_NAME = hrDefPlugin

include(../base_plugins.pri)

SOURCES =   main.cpp \
            hrPcxHandler.cpp

HEADERS = hrPcxHandler.hpp \
    main.hpp
OTHER_FILES = hrpcx.json
