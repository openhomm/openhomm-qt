include(../../base.pri)

DESTDIR     = $$DESTDIR/imageformats
OBJECTS_DIR = $$OBJECTS_DIR/imageformats/$$TARGET
MOC_DIR     = $$MOC_DIR/imageformats/$$TARGET
RCC_DIR     = $$RCC_DIR/imageformats

CONFIG += qt plugin
TEMPLATE = lib
