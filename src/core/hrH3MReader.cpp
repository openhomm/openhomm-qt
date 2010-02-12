// openhomm - open source clone of Heroes of Might and Magic III
// Copyright (C) 2009-2010 openhomm developers team (see AUTHORS)
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
#include <zlib.h>

QByteArray unpack(const QString &filename)
{
    QFile map(filename);
    QByteArray array;
    int len = 0;
    if ( map.open(QIODevice::ReadOnly) )
    {
        map.seek(map.size()-(quint64)4);

        map.read((char*)&len, 4);
        map.close();
    }

    gzFile file = gzopen(filename.toLocal8Bit().data(), "rb");

    char * a = new char[len];
    gzread(file, a, len);

    gzclose(file);

    array.append(a, len);

    delete [] a;

    return array;
}
/*!
  \class hrH3MReader
  \brief The hrH3MReader class
*/
hrH3MReader::hrH3MReader() : ground(NULL), underground(NULL), objects(NULL), obj(NULL)
{
}

hrH3MReader::~hrH3MReader()
{
    if (ground != NULL)
        delete [] ground;
    ground = NULL;
    if (underground != NULL)
        delete [] underground;
    underground = NULL;
    if (objects != NULL)
        delete [] objects;
    objects = NULL;
    if (obj != NULL)
        delete [] obj;
    obj = NULL;
}

bool hrH3MReader::load(const QString &name)
{
    QByteArray data;

    data = unpack(name);

    QBuffer map(&data);
    if ( !map.open(QIODevice::ReadOnly) )
    {
        qWarning() << map.errorString();
        return false;
    }

    QDataStream m(&map);
    m.setByteOrder(QDataStream::LittleEndian);

    m >> basic;

    if (basic.version != 0x0000001C)
       return false;

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
    m.readRawData( (char *) ground, sizeof(hrTile)*basic.size*basic.size );

    if ( basic.under == 1 )
    {
        underground = new hrTile[basic.size * basic.size];
        m.readRawData( (char *) underground, sizeof(hrTile)*basic.size*basic.size );
    }

    m >> objectQuantity;

    objects = NULL;
    objects = new hrObject[objectQuantity];

    for ( quint32 i = 0; i < objectQuantity; i++ )
    {
        m >> objects[i];
    }

    m >> objectOptions;


    obj = new hrObjectOptions[objectOptions];
    for ( quint32 i = 0; i < objectOptions; i++ )
    {
        m >> obj[i];

        switch(objects[obj[i].objectID].object_class)
        {
        case 5:
        case 65:
        case 66:
        case 67:
        case 68:
        case 69:
            {
                ObjectArtefact artefact;
                m >> artefact;
            }
             break;

        case 6:
            {
                ObjectPandora pandora;
                m >> pandora;
            }
            break;

        case 17:
        case 20:
        case 42: //lighthouse
            {
                ObjectDwelling dwelling;
                m >> dwelling;
            }
            break;

        case 26:
            {
                ObjectEvent localevent;
                m >> localevent;
            }
            break;

        case 33:
        case 219:
            {
                ObjectGarrison garrison;
                m >> garrison;
            }
            break;

        case 34:
        case 70:
            {
                ObjectHero hero;
                m >> hero;
            }
            break;
        case 62:
            {
                ObjectHero hero;
                m >> hero;
            }
            break;

        case 36:
            {
                ObjectGrail grail;
                m >> grail;
            };
            break;

        case 53:
            {
                switch(objects[obj[i].objectID].object_number) {
                case 7:
                    {
                        ObjectAbandonedMine abandoned;  // bit0 - mercury, 1 - ore, 2 - sulfur,
                        m >> abandoned;                 // bit3 - crystal, 4 - gem, 5 - gold
                    }
                    break;
                default:
                    {
                        ObjectMine mine;
                        m >> mine;
                    }
                    break;
                }
            }
            break;

        case 54:
        case 71:
        case 72:
        case 73:
        case 74:
        case 75:
        case 162:
        case 163:
        case 164:
            {
                ObjectMonster monster;
                m >> monster;
            }
            break;

        case 76:
        case 79:
            {
                ObjectResource res;
                m >> res;
            }
            break;

        case 81:
            {
                ObjectScientist scientist;
                m >> scientist;
            }
            break;

         case 83:
            {
                ObjectProphet prophet;
                m >> prophet;
            }
            break;

         case 87:
            {
                ObjectShipyard shipyard;
                m >> shipyard;
            }
            break;

         case 88:
         case 89:
         case 90:
            {
                ObjectShrine shrine;
                m >> shrine;
            }
            break;

         case 91:
         case 59:
            {
                ObjectSign sign;
                m >> sign;
            }
            break;

         case 93:
            {
                ObjectSpell spell;
                m >> spell;
            }
            break;

         case 98:
         case 77:
            {
                ObjectTown town;
                m >> town;
            }
            break;

         case 113:
            {
                ObjectWitchHut whut;
                m >> whut;
            }
            break;

         case 215:
            {
                ObjectQuestionGuard qguard;
                m >> qguard;
            }
            break;

         case 216:
            {
                ObjectGeneralRandomDwelling dwelling;
                m >> dwelling;
            }
            break;
         case 217:
            {
                ObjectLevelRandomDwelling dwelling;
                m >> dwelling;
            }
            break;
         case 218:
             {
                 ObjectTownRandomDwelling dwelling;
                 m >> dwelling;
             }
             break;
         case 220:
            {
                 ObjectAbandonedMine abandoned;
                 m >> abandoned;
            }
            break;
        };
    }
    return true;
}

hrTile hrH3MReader::getTile(quint32 index, bool isUnderground)
{
    Q_ASSERT(index >= 0 && index <= basic.size * basic.size);
    if (isUnderground && basic.under == 1)
      return underground[index];

    return ground[index];
}

bool hrH3MReader::hasUndergrund() const
{
    if (basic.under == 1)
        return true;
    return false;
}

int hrH3MReader::getSize() const
{
    return basic.size;
}

int hrH3MReader::getObjectsCount() const
{
    return objectOptions;
}

hrSceneObject hrH3MReader::getObject(quint32 index) const
{
    Q_ASSERT(index >= 0 && index < objectOptions);
    return hrSceneObject(obj[index].objectID
                         , objects[obj[index].objectID].actions
                         , objects[obj[index].objectID].isOverlay
                         , obj[index].coord[2] == 0 ? false : true
                         , QPoint(obj[index].coord[0], obj[index].coord[1])
                         );
}
const QString& hrH3MReader::getObjectName(quint32 id) const
{
    Q_ASSERT(id >= 0 && id < objectQuantity);
    return objects[id].filename;
}

QT_BEGIN_NAMESPACE
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
    in >> h.portret >> h.name;
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

    in >> p.heroPortret >> p.heroName;
    if ( p.heroType != 0xFF )
    {
        in >> p.junk >> p.heroesCount;

        if ( p.heroesCount > 0 )
        {
            for ( quint32 i = 0; i < p.heroesCount; i++ )
            {
                Hero_t hero;
                in >> hero;
                p.heroes.push_back(hero);
            }
        }
    }
    return in;
}
QT_END_NAMESPACE
PlayerAttributes_t::PlayerAttributes_t()
{
}

PlayerAttributes_t::~PlayerAttributes_t()
{
    heroes.clear();
}
QT_BEGIN_NAMESPACE
QDataStream &operator<<(QDataStream &out, const SpecialVictoryCondition_t &)
{
    qWarning("%s is not yet implemented", Q_FUNC_INFO);
    return out;
}
QDataStream &operator>>(QDataStream &in, SpecialVictoryCondition_t &s)
{
    in >> s.id;

    if ( s.id != 0xFF )
    {
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
            in >> s.townCoord[0] >> s.townCoord[1] >> s.townCoord[2];
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
    }
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

    if ( s.id != 0xFF )
    {

        if ( s.id == 0x00 || s.id == 0x01 )
            in >> s.coord[0] >> s.coord[1] >> s.coord[2];
        else if ( s.id == 0x02 )
            in >> s.days;
        else
            qWarning("SpecialLossConditions ID: %d is unkown", s.id);
    }
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
        in.readRawData((char*)t.commands, sizeof(t.commands));

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

    if ( f.heroesQuantity > 0 )
    {
        for ( quint8 i = 0; i < f.heroesQuantity; i++ )
        {
            TunedHero_t tuned;
            in >> tuned;
            f.tunedHeroes.push_back(tuned);
        }
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

    if ( r.quantity > 0 )
    {
        for ( quint32 i = 0; i < r.quantity; i++ )
        {
            Rumor_t rumor;
            in >> rumor;
            r.rumors.push_back(rumor);
        }
    }
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
        if ( h.secSkillsQuantity > 0 )
        {
            for ( quint32 i = 0; i < h.secSkillsQuantity; i++ )
            {
                SecondarySkill_t skill;
                in >> skill;
                h.secSkills.push_back(skill);
            }
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

        if ( h.knapsack_count > 0 )
        {
            for ( quint16 i = 0; i < h.knapsack_count; i++ )
            {
                quint16 knap;
                in >> knap;
                h.knapsackID.push_back(knap);
            }
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
QT_END_NAMESPACE
