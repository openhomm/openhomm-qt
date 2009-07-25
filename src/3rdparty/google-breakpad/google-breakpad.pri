INCLUDEPATH += $$PWD/src
DEPENDPATH += $$PWD

win32-msvc2* {
    SOURCES +=  src/client/windows/handler/exception_handler.cc \
                src/common/windows/guid_string.cc \
                src/client/windows/crash_generation/minidump_generator.cc \
                src/client/windows/crash_generation/crash_generation_client.cc \
                src/client/windows/crash_generation/crash_generation_server.cc \
                src/client/windows/crash_generation/client_info.cc
}
