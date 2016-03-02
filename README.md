OpenHoMM  [![Build Status](https://travis-ci.org/openhomm/openhomm.svg?branch=master)](https://travis-ci.org/openhomm/openhomm) [![Build status](https://ci.appveyor.com/api/projects/status/oa8t0l90y9jjl45v?svg=true)](https://ci.appveyor.com/project/openhomm/openhomm)
========

OpenHoMM is open source clone of 'Heroes Of Might And Magic III Shadow Of Death' game engine.

###Requirments:
* original game data
* Qt >= 5.4

###Plans for near future:
* It is necessary to rewrite support of snd/lod, because QFSFileEngine, QAbstractFileEngine, QAbstractFileEngineIterator and QAbstractFileEngineHandler were removed from Qt 5 public API.
* Migrate build system from qmake to qbs or cmake.
