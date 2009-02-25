DESTDIR     = $$join(PWD,,,/bin/$$(QMAKESPEC))
OBJECTS_DIR = $$join(PWD,,,/obj/$$(QMAKESPEC))
MOC_DIR     = $$join(PWD,,,/moc/$$(QMAKESPEC))
RCC_DIR     = $$join(PWD,,,/res/$$(QMAKESPEC))

CONFIG(debug,debug|release) { 
    CONFIG += console
    DESTDIR = $$join(DESTDIR,,,/debug)
    OBJECTS_DIR = $$join(OBJECTS_DIR,,,/debug)
    MOC_DIR = $$join(MOC_DIR,,,/debug)
    RCC_DIR = $$join(RCC_DIR,,,/debug)
}

CONFIG(release, debug|release) {
    DESTDIR = $$join(DESTDIR,,,/release)
    OBJECTS_DIR = $$join(OBJECTS_DIR,,,/release)
    MOC_DIR = $$join(MOC_DIR,,,/release)
    RCC_DIR = $$join(RCC_DIR,,,/release)
}

win32-msvc2*:DEFINES += "_CRT_SECURE_NO_WARNINGS" "_CRT_NON_CONFORMING_SWPRINTFS"
