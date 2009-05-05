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
#include "hrObject.hpp"

//QDataStream &operator<<(QDataStream &out, const hrObject &)
//{
//    qWarning("%s is not yet implemented", Q_FUNC_INFO);
//    return out;
//}
QDataStream &operator>>(QDataStream &in, hrObject &s)
{
    in >> s.filename;
    in.readRawData( (char *) s.passability, sizeof(s.passability) );
    in.readRawData( (char *) s.actions, sizeof(s.actions) );

    in >> s.landscape >> s.land_edit_groups >> s.object_class >> s.object_number >> s.object_group;
    in >> s.isOverlay;

    in.readRawData( (char *) s.junk, sizeof(s.junk) );

    return in;
}

//QDataStream &operator<<(QDataStream &out, const hrObjectOptions &)
//{
//    qWarning("%s is not yet implemented", Q_FUNC_INFO);
//    return out;
//}
QDataStream &operator>>(QDataStream &in, hrObjectOptions &s)
{
    in.readRawData( (char *) s.coord, sizeof(s.coord) );
    in >> s.objectID;
    in.readRawData( (char *) s.junk, sizeof(s.junk) );

    return in;
}

void hrObject::dump()
{
    qDebug() << filename;
//    for ( quint32 i = 0; i < sizeof(junk); i++ )
//        qDebug() << '\t' << junk[i];
}

void hrObjectOptions::dump()
{
    qDebug("%d - {%d,%d,%d}\n", objectID, coord[0], coord[1], coord[2]);
//    for ( quint32 i = 0; i < sizeof(junk); i++ )
//        qDebug() << '\t' << junk[i];
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

//QDataStream &operator<<(QDataStream &, const ObjectSign &);
QDataStream &operator>>(QDataStream &in, ObjectSign &s)
{
    in >> s.text >> s.junk;
    return in;
}

//QDataStream &operator<<(QDataStream &, const ObjectEvent &);
QDataStream &operator>>(QDataStream &in, ObjectEvent &s)
{
    in >> s.event >> s.players >> s.isAICan >> s.disableAfterFirstDay >> s.junk;
    return in;
}

//QDataStream &operator<<(QDataStream &, const ObjectShrine &);
QDataStream &operator>>(QDataStream &in, ObjectShrine &s)
{
    in >> s.spellID;
    return in;
}

//QDataStream &operator<<(QDataStream &, const ObjectTown &);
QDataStream &operator>>(QDataStream &in, ObjectTown &s)
{
    in >> s.junk >> s.owner >> s.isName;

    if ( s.isName == 1 )
        in >> s.name;

    in >> s.isGuard;

    if ( s.isGuard == 1 )
    {
        for ( quint8 i = 0; i < 7; i++ )
            in >> s.guards[i];
    }

    in >> s.formation >> s.isBuildings;

    if ( s.isBuildings == 1 )
    {
        in.readRawData( (char *) s.built, sizeof(s.built) );
        in.readRawData( (char *) s.active, sizeof(s.active) );
    }
    else
        in >> s.isFort;

    in.readRawData( (char *) s.mustSpells, sizeof(s.mustSpells) );
    in.readRawData( (char *) s.canSpells, sizeof(s.canSpells) );

    in >> s.event_quantity;

    if ( s.event_quantity > 0 )
    {
        for (quint32 i = 0; i < s.event_quantity; i++ )
        {
            TownEvent_t event;
            in >> event;
            s.events.push_back(event);
        }
    }

    in >> s.junk3;

    return in;
}

//QDataStream &operator<<(QDataStream &, const TownEvent_t &);
QDataStream &operator>>(QDataStream &in, TownEvent_t &s)
{
    in >> s.event_name >> s.event_text >> s.res >> s.players_affected >> s.human_affected >> s.ai_affected;

    in >> s.day_of_first_event >> s.event_iteration;
    in.readRawData( (char *) s.junk, sizeof(s.junk) );
    in.readRawData( (char *) s.buildings, sizeof(s.buildings) );
    in.readRawData( (char *) s.creatures, sizeof(s.creatures) );
    in >> s.junk2;

    return in;
}

//QDataStream &operator<<(QDataStream &, const ObjectSpell &);
QDataStream &operator>>(QDataStream &in, ObjectSpell &s)
{
    in >> s.spell >> s.spellID;
    return in;
}

//QDataStream &operator<<(QDataStream &, const ObjectScientist &);
QDataStream &operator>>(QDataStream &in, ObjectScientist &s)
{
    in >> s.bonus_type >> s.primaryID;
    in.readRawData( (char *) s.junk2, sizeof(s.junk2) );

    return in;
}

//QDataStream &operator<<(QDataStream &, const ObjectResource &);
QDataStream &operator>>(QDataStream &in, ObjectResource &s)
{
    in >> s.res >> s.quantity >> s.junk;
    return in;
}

//QDataStream &operator<<(QDataStream &, const ObjectPandora &);
QDataStream &operator>>(QDataStream &in, ObjectPandora &s)
{
    in >> s.art;
    in >> s.exp >> s.spell_points >> s.morals >> s.luck >> s.res;
    in >> s.offence >> s.defence >> s.power >> s.knowledge;
    in >> s.isSecSkills;

    if ( s.isSecSkills > 0 )
    {
        for ( quint8 i = 0; i < s.isSecSkills; i++)
        {
            SecondarySkill_t skill;
            in >> skill;
            s.sec_skills.push_back(skill);
        }
    }

    in >> s.isArtefacts;

    if ( s.isArtefacts > 0 )
    {
        for ( quint8 i = 0; i < s.isArtefacts; i++)
        {
            quint16 art;
            in >> art;
            s.artefacts.push_back(art);
        }
    }

    in >> s.isSpells;

    if ( s.isSpells > 0 )
    {
        for ( quint8 i = 0; i < s.isSpells; i++)
        {
            quint8 sp;
            in >> sp;
            s.spells.push_back(sp);
        }
    }

    in >> s.monstres_count;

    if ( s.monstres_count > 0 )
    {
        for ( quint8 i = 0; i < s.monstres_count; i++)
        {
            Guard_t g;
            in >> g;
            s.monstres.push_back(g);
        }
    }
    in.readRawData( (char *) s.junk3, sizeof(s.junk3) );
    return in;
}

//QDataStream &operator<<(QDataStream &, const ObjectMonster &);
QDataStream &operator>>(QDataStream &in, ObjectMonster &s)
{
    in >> s.monsterID >> s.monsters_count >> s.mood >> s.isTreasureOrText;

    if ( s.isTreasureOrText == 1 )
    {
        in >> s.text >> s.res >> s.artefactID;
    }
    in >> s.monsterNeverRunAway >> s.monsterDontGrowUp;
    in.readRawData( (char * ) s.junk2, sizeof(s.junk2) );

    return in;
}

//QDataStream &operator<<(QDataStream &, const ObjectGrail &);
QDataStream &operator>>(QDataStream &in, ObjectGrail &s)
{
    in >> s.radius;
    in.readRawData( (char *) s.junk2, sizeof(s.junk2) );
    return in;
}

//QDataStream &operator<<(QDataStream &, const ObjectTownRandomDwelling &);
QDataStream &operator>>(QDataStream &in, ObjectTownRandomDwelling &s)
{
    in >> s.owner >> s.minlevel >> s.maxlevel;
    return in;
}

//QDataStream &operator<<(QDataStream &, const ObjectLevelRandomDwelling &);
QDataStream &operator>>(QDataStream &in, ObjectLevelRandomDwelling &s)
{
    in >> s.owner >> s.junk;

    if ( s.junk == 0 )
        in >> s.towns;

    return in;
}

//QDataStream &operator<<(QDataStream &, const ObjectGeneralRandomDwelling &);
QDataStream &operator>>(QDataStream &in, ObjectGeneralRandomDwelling &s)
{
    in >> s.owner >> s.junk;

    if ( s.junk == 0 )
        in >> s.towns;

    in >> s.minlevel >> s.maxlevel;

    return in;
}

//QDataStream &operator<<(QDataStream &, const ObjectShipyard &);
QDataStream &operator>>(QDataStream &in, ObjectShipyard &s)
{
    in >> s.owner;
    return in;
}

//QDataStream &operator<<(QDataStream &, const ObjectDwelling &);
QDataStream &operator>>(QDataStream &in, ObjectDwelling &s)
{
    in >> s.owner;
    return in;
}

//QDataStream &operator<<(QDataStream &, const ObjectArtefact &);
QDataStream &operator>>(QDataStream &in, ObjectArtefact &s)
{
    in >> s.isText;

    if ( s.isText == 1 )
    {
        in >> s.text >> s.isGuards;

        if ( s.isGuards == 1)
        {
            for (quint8 i = 0; i < 7; i++)
                in >> s.guards[i];
        }
        in.readRawData( (char *) s.junk, sizeof(s.junk) );
    }

    return in;
}

//QDataStream &operator<<(QDataStream &, const Guard_t &);
QDataStream &operator>>(QDataStream &in, Guard_t &s)
{
    in >> s.GuardID >> s.GuardCount;
    return in;
}

//QDataStream &operator<<(QDataStream &, const Resources_t &);
QDataStream &operator>>(QDataStream &in, Resources_t &s)
{
    in >> s.wood >> s.mercury >> s.ore >> s.sulfur >> s.crystal >> s.gem >> s.gold;
    return in;
}

//QDataStream &operator<<(QDataStream &, const ObjectGarrison &);
QDataStream &operator>>(QDataStream &in, ObjectGarrison &s)
{
    in >> s.color;

    for (quint8 i = 0; i < 7; i++ )
        in >> s.guards[i];

    in >> s.undeleteSoldiers;
    in.readRawData( (char *) s.junk, sizeof(s.junk) );
    return in;
}

//QDataStream &operator<<(QDataStream &, const ObjectAbandonedMine &);
QDataStream &operator>>(QDataStream &in, ObjectAbandonedMine &s)
{
    in >> s.resources;
    in.readRawData( (char *) s.junk, sizeof(s.junk) );
    return in;
}

//QDataStream &operator<<(QDataStream &, const globalEvent &);
QDataStream &operator>>(QDataStream &in, globalEvent &s)
{
    in >> s.name >> s.text >> s.res >> s.players_affected >> s.human_affected >> s.ai_affected;
    in >> s.day_of_first_event;
    in >> s.event_iteration;
    in.readRawData( (char *) s.junk, sizeof(s.junk) );
    return in;
}

//QDataStream &operator<<(QDataStream &, const ObjectWitchHut &);
QDataStream &operator>>(QDataStream &in, ObjectWitchHut &s)
{
    in >> s.secskills;
    return in;
}

//QDataStream &operator<<(QDataStream &, const ObjectMine &);
QDataStream &operator>>(QDataStream &in, ObjectMine &s)
{
    in >> s.color;
    return in;
}

//QDataStream &operator<<(QDataStream &, const ObjectHeroPlaceholder &);
QDataStream &operator>>(QDataStream &in, ObjectHeroPlaceholder &s)
{
    in >> s.color >> s.type;
    return in;
}

//QDataStream &operator<<(QDataStream &, const ObjectQuestionGuard &);
QDataStream &operator>>(QDataStream &in, ObjectQuestionGuard &s)
{
    in >> s.quest;

    switch(s.quest)
    {
    case 0:
        break;
    case 1:
        in >> s.level;
        break;
    case 2:
        in >> s.offence >> s.defence >> s.power >> s.knowledge;
        break;
    case 3:
        in >> s.heroID;
        break;
    case 4:
        in >> s.monsterID;
        break;
    case 5:
        in >> s.artQuantity;
        for ( quint8 i = 0; i < s.artQuantity; i++ )
        {
            quint16 art;
            in >> art;
            s.arts.push_back(art);
        };
        break;
    case 6:
        in >> s.creaturesQuantity;
        if ( s.creaturesQuantity > 0 )
        {
            for ( quint8 i = 0; i < s.creaturesQuantity; i++ )
            {
                Guard_t g;
                in >> g;
                s.creatures.push_back(g);
            }
        }
        break;
    case 7:
        in >> s.res;
        break;
    case 8:
        in >> s.heroID_;
        break;
    case 9:
        in >> s.playerColor;
        break;
    default:
        qDebug() << "lolwut? o_O";
        break;
    };

    in >> s.time_limit;
    in >> s.quest_begin >> s.quest_inprocess >> s.quest_end;

    return in;
}

//QDataStream &operator<<(QDataStream &, const ObjectHero &);
QDataStream &operator>>(QDataStream &in, ObjectHero &s)
{
    in >> s.heroID >> s.color >> s.hero >> s.isName;

    if ( s.isName == 1 )
        in >> s.name;

    in >> s.isExp;

    if ( s.isExp == 1 )
        in >> s.exp;

    in >> s.isPortrait;

    if ( s.isPortrait == 1)
        in >> s.portrait;

    in >> s.isSecondary;

    if ( s.isSecondary == 1 )
    {
        in >> s.skills_quantity;
        if ( s.skills_quantity > 0 )
        {
            for (quint8 i = 0; i < s.skills_quantity; i++ )
            {
                SecondarySkill_t sk;
                in >> sk;
                s.skills.push_back(sk);
            }
        }
    }

    in >> s.isCreature;

    if ( s.isCreature == 1 )
    {
        for (quint8 i = 0; i < 7; i++)
            in >> s.creatures[i];
    }

    in >> s.creaturesFormation >> s.isArtefacts;

    if ( s.isArtefacts == 1 )
    {
        in >> s.headID >> s.shouldersID >> s.neckID >> s.rightHandID >> s.leftHandID;
        in >> s.trunkID >> s.rightRingID >> s.leftRingID >> s.legsID;
        in >> s.misc1ID >> s.misc2ID >> s.misc3ID >> s.misc4ID;
        in >> s.machine1ID >> s.machine2ID >> s.machine3ID >> s.machine4ID;
        in >> s.magicbook >> s.misc5ID;

        in >> s.knapsack_count;

        if ( s.knapsack_count > 0 )
        {
            for ( quint16 i = 0; i < s.knapsack_count; i++ )
            {
                quint16 knap;
                in >> knap;
                s.knapsack.push_back(knap);
            }
        }
    }

    in >> s.zoneRadius >> s.isBiography;

    if ( s.isBiography == 1 )
        in >> s.biography;

    in >> s.gender >> s.isSpells;

    if ( s.isSpells == 1 )
        in.readRawData( (char *) s.spells, sizeof(s.spells) );

    in >> s.isPrimary;

    if ( s.isPrimary == 1 )
    {
        in >> s.offence >> s.defence >> s.power >> s.knowledge;
    }

    in.readRawData( (char *) s.unknown, sizeof(s.unknown) );

    return in;
}

//QDataStream &operator<<(QDataStream &, const ObjectProphet &);
QDataStream &operator>>(QDataStream &in, ObjectProphet &s)
{
    in >> s.quest;
    switch(s.quest)
    {
    case 0:

        break;
    case 1:
        in >> s.level;
        break;
    case 2:
        in >> s.offence >> s.defence >> s.power >> s.knowledge;
        break;
    case 3:
        in >> s.heroID;
        break;
    case 4:
        in >> s.monsterID;
        break;
    case 5:
        in >> s.artQuantity;
        for ( quint8 i = 0; i < s.artQuantity; i++ )
        {
            quint16 art;
            in >> art;
            s.arts.push_back(art);
        };
        break;
    case 6:
        in >> s.creaturesQuantity;
        if ( s.creaturesQuantity > 0 )
        {
            for ( quint8 i = 0; i < s.creaturesQuantity; i++ )
            {
                Guard_t g;
                in >> g;
                s.creatures.push_back(g);
            }
        }
        break;
    case 7:
        in >> s.res;
        break;
    case 8:
        in >> s.heroID_;
        break;
    case 9:
        in >> s.playerColor;
        break;
    default:
        qDebug() << "lolwut? o_O";
        break;
    };

    in >> s.time_limit;
    qDebug("s.time_limit = %d", s.time_limit);
    in >> s.quest_begin >> s.quest_inprocess >> s.quest_end;

    in >> s.reward;

    switch(s.reward)
    {
    case 0:
        break;
    case 1:
        in >> s.exp;
        break;
    case 2:
        in >> s.spellPoints;
        break;
    case 3:
        in >> s.morale;
        break;
    case 4:
        in >> s.lucky;
        break;
    case 5:
        in >> s.resID >> s.resQ;
        break;
    case 6:
        in >> s.priSkillID >> s.priSkillBonus;
        break;
    case 7:
        in >> s.secSkillID >> s.secSkillLevel;
        break;
    case 8:
        in >> s.artId;
        break;
    case 9:
        in >> s.spell;
        break;
    case 10:
        in >> s.creatureID >> s.creatureQuantity;
        break;
    default:
        qDebug() << "lolwut?";
    };
    in >> s.junk;

    return in;
}
