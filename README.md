OpenHoMM  [![Build Status](https://travis-ci.org/openhomm/openhomm.svg?branch=master)](https://travis-ci.org/openhomm/openhomm) [![Build status](https://ci.appveyor.com/api/projects/status/oa8t0l90y9jjl45v?svg=true)](https://ci.appveyor.com/project/openhomm/openhomm)
========

OpenHoMM is open source clone of 'Heroes Of Might And Magic III Shadow Of Death' game engine.

## Some demo (build  Qt 5.8 2017/06/07)
[![Alt text](https://i.ytimg.com/vi/yo2wqjiMjjo/hq720.jpg?custom=true&w=196&h=110&stc=true&jpg444=true&jpgq=90&sp=68&sigh=z_Pn0iP1GQtyqqbEPv49LN14XO4)](https://www.youtube.com/watch?v=VID)

### Requirments:
* original game data
* Qt >= 5.5.1

### Plans for near future:
* It is necessary to rewrite support of snd/lod, because QFSFileEngine, QAbstractFileEngine, QAbstractFileEngineIterator and QAbstractFileEngineHandler were removed from Qt 5 public API.
* Migrate build system from qmake to qbs or cmake.
