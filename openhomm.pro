#version check qt
#TOO_OLD_LIST=$$find(QT_VERSION, ^4\\.[0-4])
#count(TOO_OLD_LIST, 1) {
#    message("Cannot build the Openhomm with a Qt version that old:" $$QT_VERSION)
#    error("Use at least Qt 4.5.")
#}

OPENHOMM_MAJOR = 1
OPENHOMM_MINOR = 0
OPENHOMM_PATCH = 0

!exists(src/version.hpp) {
    GIT = $$system(git)
    isEmpty(GIT) {
        error("You must installed GIT to make from sources")
    }

    VERSION_HEADER_GUARD = "pragma once"
    VERSION_HEADER_GUARD = $$join(VERSION_HEADER_GUARD,,$$LITERAL_HASH)

    VERSION_STR = git log -n 1 --pretty=format:\"$$LITERAL_HASH define VERSION_STR OPENHOMM_MAJOR,OPENHOMM_MINOR,OPENHOMM_PATCH,%h%n\"
    VERSION_STR = $$replace(VERSION_STR, OPENHOMM_MAJOR, $$OPENHOMM_MAJOR)
    VERSION_STR = $$replace(VERSION_STR, OPENHOMM_MINOR, $$OPENHOMM_MINOR)
    VERSION_STR = $$replace(VERSION_STR, OPENHOMM_PATCH, $$OPENHOMM_PATCH)

    VERSION_RELEASE_STR = git log -n 1 --pretty=format:\"$$LITERAL_HASH define VERSION_RELEASE_STR \\\"OpenHoMM 1.0.0.{%h} Date: %ad\\\"%n\"
    system(echo $$VERSION_HEADER_GUARD > src/version.hpp)
    system($$VERSION_RELEASE_STR >> src/version.hpp)
    system($$VERSION_STR  >> src/version.hpp)
}
OPENHOMMDIR = $$PWD
include(doc/doc.pri)
CONFIG += debug_and_release debug_and_release_target
TEMPLATE = subdirs
SUBDIRS = src src/plugins# tools
