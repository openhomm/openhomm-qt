// openhomm - open source clone of Heroes of Might and Magic III
// Copyright (C) 2009 openhomm developers team (see AUTHORS)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#pragma once

struct BasicParametres_t {
    quint32 version;
    quint8  junk;
    quint32 size;
    quint8  under;

    QString name;
    QString description;

    quint8  difficult;
    quint8  levelLimit;

    bool load(QIODevice *device);
};

struct Hero {
    quint8 portret;
    QString name;
};

struct PlayerAttributes_t {
    PlayerAttributes_t();
    ~PlayerAttributes_t();

    quint8 isHuman;
    quint8 isComputer;
    quint8 behavior;	// 0-Random, 1-Warrior, 2-Builder, 3-Explorer
    quint8 isCityTypesOpt;
    quint16 cityTypes;
    quint8 randomCity;
    quint8 mainCity;

//  if mainCity == 1 {
    quint8 generateHero;
    quint8 city[4]; // 0 - city type, 1-3 coords
// }

    quint8 randomHero;
    quint8 heroType;


//  if ( heroType != 0xFF ) {
    quint8 heroPortret;
    QString heroName;
//  }

    quint8 junk; // ??? get more info
    quint32 heroesCount;

//  if ( heroesCount > 0 ) {
    QVector<Hero> heroes;
//  }

    bool load(QIODevice *device);
    void dump();
};

class hrH3MReader
{
public:
    hrH3MReader();
    ~hrH3MReader();

    bool load(const QString &name);
protected:

// data
    BasicParametres_t basic;
    PlayerAttributes_t players[8];
};
