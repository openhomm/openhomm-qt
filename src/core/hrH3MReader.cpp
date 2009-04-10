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
#include "precompiled.hpp"
#include "hrH3MReader.hpp"
#include "hrString.hpp"

hrH3MReader::hrH3MReader() : ground(NULL), underground(NULL)
{
}

hrH3MReader::~hrH3MReader()
{
    delete [] ground;
    ground = NULL;
    delete [] underground;
    underground = NULL;
}

bool hrH3MReader::load(const QString &name)
{
    QByteArray data;

    {
        QFile map(name);
        if ( ! map.open(QIODevice::ReadOnly) )
        {
            qWarning("Can't open file: %s", qPrintable(name));
            return false;
        }

        data = map.readAll();
        map.close();
    }

    QBuffer map(&data);
    if ( !map.open(QIODevice::ReadOnly) )
    {
        qWarning() << map.errorString();
        return false;
    }

    QDataStream m(&map);
    m.setByteOrder(QDataStream::LittleEndian);

    m >> basic;

    for ( int i = 0; i < 8; i++ )
    {
        m >> players[i];
    }

    m >> svc >> slc >> teams;
    m >> fh;
    m >> artefacts >> spells >> secSkills >> rumors;

    for ( int i = 0; i < 156; i++ )
    {
        m >> enable[i];
        if ( enable[i] == 1 )
        {
            m >> heroOptions[i];
        }
    }

    ground = new hrTile[basic.size*basic.size];
    m.read( (char *) ground, sizeof(hrTile)*basic.size*basic.size );

    if ( basic.under == 1 )
    {
        underground = new hrTile[basic.size * basic.size];
        m.readRawData( (char *) underground, sizeof(hrTile)*basic.size*basic.size );
    }


    return true;
}

hrTile hrH3MReader::getTile(quint32 index, bool isUnderground)
{
    Q_ASSERT( index >= 0 && index <= basic.size * basic.size);
    if ( isUnderground )
      return underground[index];

    return ground[index];
}

int hrH3MReader::getSize()
{
    return basic.size;
}

QDataStream &operator<<(QDataStream &out, const BasicParametres_t &)
{
    qWarning("%s is not yet implemented", Q_FUNC_INFO);
    return out;
}
QDataStream &operator>>(QDataStream &in, BasicParametres_t &b)
{
    in >> b.version;

    if ( b.version == 0x0000001C )
        in >> b.junk >> b.size >> b.under >> b.name >> b.description >> b.difficult >> b.levelLimit;
    else
        qWarning("File is not a HoMM III : SoD Map.");

    return in;
}
QDataStream &operator<<(QDataStream &out, const Hero_t &)
{
    qWarning("%s is not yet implemented", Q_FUNC_INFO);
    return out;
}
QDataStream &operator>>(QDataStream &in, Hero_t &h)
{
    in >> h.portret >> h.portret;
    return in;
}
QDataStream &operator<<(QDataStream &out, const PlayerAttributes_t &)
{
    qWarning("%s is not yet implemented", Q_FUNC_INFO);
    return out;
}
QDataStream &operator>>(QDataStream &in, PlayerAttributes_t &p)
{
    in >> p.isHuman >> p.isComputer >> p.behavior >> p.isCityTypesOpt;
    in >> p.cityTypes >> p.randomCity >> p.mainCity;

    if ( p.mainCity == 1 )
        in >> p.generateHero >> p.city[0] >> p.city[1] >> p.city[2] >> p.city[3];

    in >> p.randomHero >> p.heroType;

    if ( p.heroType != 0xFF )
        in >> p.heroPortret >> p.heroName;

    in >> p.junk >> p.heroesCount;

    for ( quint32 i = 0; i < p.heroesCount; ++i )
    {
        Hero_t hero;
        in >> hero;
        p.heroes.push_back(hero);
    }
    return in;
}
PlayerAttributes_t::PlayerAttributes_t()
{
}

PlayerAttributes_t::~PlayerAttributes_t()
{
    heroes.clear();
}

QDataStream &operator<<(QDataStream &out, const SpecialVictoryCondition_t &)
{
    qWarning("%s is not yet implemented", Q_FUNC_INFO);
    return out;
}
QDataStream &operator>>(QDataStream &in, SpecialVictoryCondition_t &s)
{
    in >> s.id;

    if ( s.id == 0xFF )
        return in;

    in >> s.canStandardEnd >> s.canComputer;
    switch(s.id)
    {
    case 0x00:
        in >> s.artId;
        break;
    case 0x01:
        in >> s.creatureId >> s.creatureCount;
        break;
    case 0x02:
        in >> s.resId >> s.resCount;
        break;
    case 0x03:
        in >> s.townCoord[0] >> s.townCoord[1] >> s.townCoord[2] >> s.townCoord[3];
        in >> s.hallLevel >> s.castleLevel;
        break;
    case 0x04:
    case 0x05:
    case 0x06:
    case 0x07:
        in >> s.coord[0] >> s.coord[1] >> s.coord[2];
        break;
    case 0x08:
    case 0x09:
        break;
    case 0x0A:
        in >> s.artType >> s.artCoord[0] >> s.artCoord[1] >> s.artCoord[2];
        break;
    default:
        qWarning("SpecialVictoryCondition ID: %d is unkonwn", s.id);
        break;
    };
    return in;
}

QDataStream &operator<<(QDataStream &out, const SpecialLossCondition_t &)
{
    qWarning("%s is not yet implemented", Q_FUNC_INFO);
    return out;
}
QDataStream &operator>>(QDataStream &in, SpecialLossCondition_t &s)
{
    in >> s.id;

    if ( s.id == 0xFF )
        return in;

    if ( s.id == 0x00 || s.id == 0x01 )
        in >> s.coord[0] >> s.coord[1] >> s.coord[2];
    else if ( s.id == 0x02 )
        in >> s.days;
    else
        qWarning("SpecialLossConditions ID: %d is unkown", s.id);

    return in;
}

QDataStream &operator<<(QDataStream &out, const Teams_t &)
{
    qWarning("%s is not yet implemented", Q_FUNC_INFO);
    return out;
}
QDataStream &operator>>(QDataStream &in, Teams_t &t)
{
    in >> t.quantity;

    if ( t.quantity > 0 )
        in.readRawData((char*)t.commands, 8);

    return in;
}

QDataStream &operator<<(QDataStream &out, const TunedHero_t &)
{
    qWarning("%s is not yet implemented", Q_FUNC_INFO);
    return out;
}
QDataStream &operator>>(QDataStream &in, TunedHero_t &t)
{
    in >> t.id >> t.portrait >> t.name >> t.players;
    return in;
}

QDataStream &operator<<(QDataStream &out, const FreeHeroes_t &)
{
    qWarning("%s is not yet implemented", Q_FUNC_INFO);
    return out;
}
QDataStream &operator>>(QDataStream &in, FreeHeroes_t &f)
{
    in.readRawData( (char *)f.heroes, sizeof(f.heroes) );
    in.readRawData( (char *)f.junk, sizeof(f.junk) );

    in >> f.heroesQuantity;

    for ( quint8 i = 0; i < f.heroesQuantity; ++i )
    {
        TunedHero_t tuned;
        in >> tuned;
        f.tunedHeroes.push_back(tuned);
    }

    in.readRawData( (char *) f.junk2, sizeof(f.junk2) );

    return in;
}

QDataStream &operator<<(QDataStream &out, const Artefacts_t &)
{
    qWarning("%s is not yet implemented", Q_FUNC_INFO);
    return out;
}
QDataStream &operator>>(QDataStream &in, Artefacts_t &a)
{
    in.readRawData( (char *) a.artefacts, sizeof(a.artefacts) );
    return in;
}

QDataStream &operator<<(QDataStream &out, const Spells_t &)
{
    qWarning("%s is not yet implemented", Q_FUNC_INFO);
    return out;
}
QDataStream &operator>>(QDataStream &in, Spells_t &s)
{
    in.readRawData( (char *) s.spells, sizeof(s.spells) );
    return in;
}

QDataStream &operator<<(QDataStream &out, const SecSkills_t &)
{
    qWarning("%s is not yet implemented", Q_FUNC_INFO);
    return out;
}
QDataStream &operator>>(QDataStream &in, SecSkills_t &s)
{
    in.readRawData( (char *) s.skills, sizeof(s.skills) );
    return in;
}

QDataStream &operator<<(QDataStream &out, const Rumor_t &)
{
    qWarning("%s is not yet implemented", Q_FUNC_INFO);
    return out;
}
QDataStream &operator>>(QDataStream &in, Rumor_t &r)
{
    in >> r.rumor_name >> r.rumor_text;
    return in;
}

QDataStream &operator<<(QDataStream &out, const Rumors_t &)
{
    qWarning("%s is not yet implemented", Q_FUNC_INFO);
    return out;
}
QDataStream &operator>>(QDataStream &in, Rumors_t &r)
{
    in >> r.quantity;
    for ( quint32 i = 0; i < r.quantity; i++ )
    {
        Rumor_t rumor;
        in >> rumor;
        r.rumors.push_back(rumor);
    }
    return in;
}

QDataStream &operator<<(QDataStream &out, const SecondarySkill_t &)
{
    qWarning("%s is not yet implemented", Q_FUNC_INFO);
    return out;
}
QDataStream &operator>>(QDataStream &in, SecondarySkill_t &s)
{
    in >> s.skillID >> s.skillLevel;
    return in;
}

QDataStream &operator<<(QDataStream &out, const HeroOptions_enabled &)
{
    qWarning("%s is not yet implemented", Q_FUNC_INFO);
    return out;
}
QDataStream &operator>>(QDataStream &in, HeroOptions_enabled &h)
{
    in >> h.isExp;

    if ( h.isExp == 1 )
        in >> h.exp;

    in >> h.isSecSkill;

    if ( h.isSecSkill == 1 )
    {
        in >> h.secSkillsQuantity;
        for ( quint32 i = 0; i < h.secSkillsQuantity; i++ )
        {
            SecondarySkill_t skill;
            in >> skill;
            h.secSkills.push_back(skill);
        }
    }

    in >> h.isArtefacts;

    if ( h.isArtefacts == 1 )
    {
        in >> h.headID >> h.shouldersID >> h.neckID >> h.rightHandID >> h.leftHandID;
        in >> h.trunkID >> h.rightRingID >> h.leftRingID >> h.legsID;
        in >> h.misc1ID >> h.misc2ID >> h.misc3ID >> h.misc4ID;
        in >> h.machine1ID >> h.machine2ID >> h.machine3ID >> h.machine4ID;
        in >> h.magicbook >> h.misc5ID;

        in >> h.knapsack_count;

        for ( quint16 i = 0; i << h.knapsack_count; i++ )
        {
            quint16 knap;
            in >> knap;
            h.knapsackID.push_back(knap);
        }
    }

    in >> h.isBiography;

    if ( h.isBiography == 1 )
        in >> h.biography;

    in >> h.gender >> h.isSpells;

    if ( h.isSpells == 1 )
    {
        in.readRawData( (char *) h.spells, sizeof(h.spells) );
    }

    in >> h.isPrimarySkills;

    if ( h.isPrimarySkills == 1 )
    {
        in >> h.attack >> h.defence >> h.power >> h.knowledge;
    }

    return in;
}
