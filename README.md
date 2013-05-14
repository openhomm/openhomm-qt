openhomm
========

OpenHoMM is open source clone of 'Heroes Of Might And Magic III Shadow Of Death' game engine.

###Requirments:
* original game data
* Qt >= 4.5 and Qt < 5

###Plans for near future:
* Port to Qt 5. It is necessary to rewrite support of snd/lod, because QFSFileEngine, QAbstractFileEngine, QAbstractFileEngineIterator and QAbstractFileEngineHandler were removed from public API.
* Migrate build system from qmake to qbs or cmake.

Written in Qt.