include(../../base.pri)
DESTDIR     = $$join(DESTDIR,,,/imageformats)
OBJECTS_DIR = $$join(OBJECTS_DIR,,,/imageformats)
MOC_DIR     = $$join(MOC_DIR,,,/imageformats)
RCC_DIR     = $$join(RCC_DIR,,,/imageformats)

CONFIG += qt plugin
TEMPLATE = lib
