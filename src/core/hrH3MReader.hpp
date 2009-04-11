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

#include "hrString.hpp"
#include "hrTile.hpp"
#include "hrObject.hpp"

struct BasicParametres_t {
    quint32 version;
    quint8  junk;
    quint32 size;
    quint8  under;

    QString name;
    QString description;

    quint8  difficult;
    quint8  levelLimit;
};
QDataStream &operator<<(QDataStream &, const BasicParametres_t &);
QDataStream &operator>>(QDataStream &, BasicParametres_t &);

struct Hero_t {
    quint8 portret;
    hrString name;
};
QDataStream &operator<<(QDataStream &, const Hero_t &);
QDataStream &operator>>(QDataStream &, Hero_t &);

struct PlayerAttributes_t {
    PlayerAttributes_t();
    ~PlayerAttributes_t();

    quint8 isHuman;
    quint8 isComputer;
    quint8 behavior;	// 0-Random
                        // 1-Warrior
                        // 2-Builder
                        // 3-Explorer
    quint8 isCityTypesOpt;
    quint16 cityTypes;  // Bit0 Castle
                        // Bit1 Rampart
                        // Bit2 Tower
                        // Bit3 Inferno
                        // Bit4 Necropolis
                        // Bit5 Dungeon
                        // Bit6 Stronghold
                        // Bit7 Fortress
                        // Bit8 Conflux
    quint8 randomCity;
    quint8 mainCity;

//  if mainCity == 1 {
    quint8 generateHero;
    quint8 city[4]; // 0 - city type, 0xFF - random city
                    // 1-3 coords (x,y,z)
// }

    quint8 randomHero;
    quint8 heroType;


//  if ( heroType != 0xFF ) {
    quint8 heroPortret;
    hrString heroName;
//  }

    quint8 junk; // ??? get more info
    quint32 heroesCount;

//  if ( heroesCount > 0 ) {
    QVector<Hero_t> heroes;
//  }
};
QDataStream &operator<<(QDataStream &, const PlayerAttributes_t &);
QDataStream &operator>>(QDataStream &, PlayerAttributes_t &);

struct SpecialVictoryCondition_t
{
    quint8 id;

    quint8 canStandardEnd;
    quint8 canComputer;

    union
    {
        // id == 0x00
        struct {
            quint8 artId;
        };
        // id == 0x01
        struct {
            quint16 creatureId;
            quint32 creatureCount;
        };
        // id == 0x02
        struct {
            quint8 resId;
            quint32 resCount;
        };
        // id == 0x03
        struct {
            quint8 townCoord[3];
            quint8 hallLevel;
            quint8 castleLevel;
        };
        // id == 0x04, 0x05, 0x06, 0x07
        struct {
            quint8 coord[3];
        };
        // id == 0x08, 0x09 - no data
        // id == 0x0A
        struct {
            quint8 artType;
            quint8 artCoord[3];
        };
    };
};
QDataStream &operator<<(QDataStream &, const SpecialVictoryCondition_t &);
QDataStream &operator>>(QDataStream &, SpecialVictoryCondition_t &);

struct SpecialLossCondition_t
{
    quint8 id;
    union
    {
        // id == 0x00, 0x01
        quint8 coord[3];
        // id == 0x02
        quint16 days;
    };
};
QDataStream &operator<<(QDataStream &, const SpecialLossCondition_t &);
QDataStream &operator>>(QDataStream &, SpecialLossCondition_t &);

struct Teams_t
{
    quint8 quantity;

    // if quantity > 0
    quint8 commands[8];
};
QDataStream &operator<<(QDataStream &, const Teams_t &);
QDataStream &operator>>(QDataStream &, Teams_t &);

struct TunedHero_t
{
    quint8 id;
    quint8 portrait;
    hrString name;
    quint8 players;
};
QDataStream &operator<<(QDataStream &, const TunedHero_t &);
QDataStream &operator>>(QDataStream &, TunedHero_t &);

struct FreeHeroes_t
{
    quint8 heroes[20];  // bitfield, bit == 1 - heros is free
    quint8 junk[4];     // need more info
    quint8 heroesQuantity;

    QVector<TunedHero_t> tunedHeroes;

    quint8 junk2[31];   // need more info
};
QDataStream &operator<<(QDataStream &, const FreeHeroes_t &);
QDataStream &operator>>(QDataStream &, FreeHeroes_t &);

struct Artefacts_t
{
    quint8 artefacts[18];
};
QDataStream &operator<<(QDataStream &, const Artefacts_t &);
QDataStream &operator>>(QDataStream &, Artefacts_t &);

struct Spells_t
{
    quint8 spells[9];
};
QDataStream &operator<<(QDataStream &, const Spells_t &);
QDataStream &operator>>(QDataStream &, Spells_t &);

struct SecSkills_t
{
    quint8 skills[4];
};
QDataStream &operator<<(QDataStream &, const SecSkills_t &);
QDataStream &operator>>(QDataStream &, SecSkills_t &);

struct Rumor_t
{
    hrString rumor_name;
    hrString rumor_text;
};
QDataStream &operator<<(QDataStream &, const Rumor_t &);
QDataStream &operator>>(QDataStream &, Rumor_t &);

struct Rumors_t
{
    quint32 quantity;
    QVector<Rumor_t> rumors;
};
QDataStream &operator<<(QDataStream &, const Rumors_t &);
QDataStream &operator>>(QDataStream &, Rumors_t &);

struct HeroOptions_enabled {
    quint8 isExp;

//if ( isExp == 1 )
    quint32 exp;

    quint8 isSecSkill;

//if ( isSecSkill == 1 ) {
    quint32 secSkillsQuantity;
    QVector<SecondarySkill_t> secSkills;
//}

    quint8 isArtefacts;

//if ( isArtefacts == 1 ) {
    quint16 headID;
    quint16 shouldersID;
    quint16 neckID;
    quint16 rightHandID;
    quint16 leftHandID;
    quint16 trunkID;
    quint16 rightRingID;
    quint16 leftRingID;
    quint16 legsID;
    quint16 misc1ID;
    quint16 misc2ID;
    quint16 misc3ID;
    quint16 misc4ID;
    quint16 machine1ID;
    quint16 machine2ID;
    quint16 machine3ID;
    quint16 machine4ID;
    quint16 magicbook;
    quint16 misc5ID;

    quint16 knapsack_count;

//if ( knapsack_count > 0 )
    QVector<quint16> knapsackID;
//}
    quint8 isBiography;

//if (isBiography == 1)
    hrString biography;

    quint8 gender;

    quint8 isSpells;

//if ( isSpells == 1 )
    quint8 spells[9];

    quint8 isPrimarySkills;

//if ( isPrimarySkills == 1 ) {
    quint8 attack;
    quint8 defence;
    quint8 power;
    quint8 knowledge;
//}
};
QDataStream &operator<<(QDataStream &, const HeroOptions_enabled &);
QDataStream &operator>>(QDataStream &, HeroOptions_enabled &);

//struct HeroOptions_t {
//    int i;
//
//    for (i = 0; i < 156; i++) {
//        quint8 enable;
//
//        if ( enable == 1 )
//            HeroOptions_enabled options;
//    }
//    quint8 enable[156];
//    HeroOptions_enabled options[156];
//};

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
    SpecialVictoryCondition_t svc;
    SpecialLossCondition_t slc;
    Teams_t teams;
    FreeHeroes_t fh;
    Artefacts_t artefacts;
    Spells_t spells;
    SecSkills_t secSkills;
    Rumors_t rumors;
    quint8 enable[156];
    HeroOptions_enabled heroOptions[156];

    hrTile *ground;
    hrTile *underground;
    hrObject * objects;
    hrObjectOptions * obj;
    quint32 objectQuantity;
    quint32 objectOptions;
};
