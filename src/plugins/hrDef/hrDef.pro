CONFIG(debug, debug|release) {
    TARGET = hrDefd
}
else {
    TARGET = hrDef
}
PLUGIN_TYPE = imageformats
PLUGIN_CLASS_NAME = hrDefPlugin
include(../base_plugins.pri)

SOURCES =   main.cpp \
            hrDefHandler.cpp
HEADERS =   hrDefHandler.hpp \
    main.hpp
OTHER_FILES = hrdef.json
