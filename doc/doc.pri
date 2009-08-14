DOCUMENTATION = (set OPENHOMMDIR=$$OPENHOMMDIR) && (qdoc3 $$OPENHOMMDIR/doc/src/openhomm.qdocconf) && (qhelpgenerator $$OPENHOMMDIR/doc/html/openhomm.qhp -o $$OPENHOMMDIR/doc/qch/openhomm.qch)

docs.commands = $$DOCUMENTATION
docs.path = $$PWD

QMAKE_EXTRA_TARGETS += docs
