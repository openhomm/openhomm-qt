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

linux-* {
    !contains(QT_ARCH, x86_64) {
    SOURCES +=  src/client/linux/handler/exception_handler.cc \
                src/client/linux/handler/minidump_generator.cc \
                src/client/linux/handler/linux_thread.cc \
                src/client/minidump_file_writer.cc \
                src/common/linux/guid_creator.cc \
                src/common/linux/file_id.cc \
                src/common/md5.c \
                src/common/convert_UTF.c \
                src/common/string_conversion.cc
	}
}
