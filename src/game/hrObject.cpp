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
#include "hrObject.hpp"

QT_BEGIN_NAMESPACE
QDataStream &operator<<(QDataStream &out, const hrObject &s)
{
    saveHString(out, s.filename);
    out.writeRawData( (char *) s.passability, sizeof(s.passability) );
    out.writeRawData( (char *) s.actions, sizeof(s.actions) );

    out << s.landscape << s.land_edit_groups << s.object_class << s.object_number << s.object_group;
    out << s.isOverlay;

    out.writeRawData( (char *) s.junk, sizeof(s.junk) );

    return out;
}
QDataStream &operator>>(QDataStream &in, hrObject &s)
{
    loadHString(in, s.filename);
    in.readRawData( (char *) s.passability, sizeof(s.passability) );
    in.readRawData( (char *) s.actions, sizeof(s.actions) );

    in >> s.landscape >> s.land_edit_groups >> s.object_class >> s.object_number >> s.object_group;
    in >> s.isOverlay;

    in.readRawData( (char *) s.junk, sizeof(s.junk) );

    return in;
}

QDataStream &operator<<(QDataStream &out, const hrObjectOptions &s)
{
    out.writeRawData( (char *) s.coord, sizeof(s.coord) );
    out << s.objectID;
    out.writeRawData( (char *) s.junk, sizeof(s.junk) );
    return out;
}
QDataStream &operator>>(QDataStream &in, hrObjectOptions &s)
{
    in.readRawData( (char *) s.coord, sizeof(s.coord) );
    in >> s.objectID;
    in.readRawData( (char *) s.junk, sizeof(s.junk) );

    return in;
}

QDataStream &operator<<(QDataStream &out, const SecondarySkill_t &s)
{
    out << s.skillID << s.skillLevel;
    return out;
}

QDataStream &operator>>(QDataStream &in, SecondarySkill_t &s)
{
    in >> s.skillID >> s.skillLevel;
    return in;
}

QDataStream &operator<<(QDataStream &out, const ObjectSign &s)
{
    saveHString(out, s.text);
    out << s.junk;
    return out;
}

QDataStream &operator>>(QDataStream &in, ObjectSign &s)
{
    loadHString(in, s.text);
    in >> s.junk;
    return in;
}

QDataStream &operator<<(QDataStream &out, const ObjectEvent &s)
{
    out << s.event << s.players << s.isAICan << s.disableAfterFirstDay << s.junk;
    return out;
}

QDataStream &operator>>(QDataStream &in, ObjectEvent &s)
{
    in >> s.event >> s.players >> s.isAICan >> s.disableAfterFirstDay >> s.junk;
    return in;
}

QDataStream &operator<<(QDataStream &out, const ObjectShrine &s)
{
    out << s.spellID;
    return out;
}

QDataStream &operator>>(QDataStream &in, ObjectShrine &s)
{
    in >> s.spellID;
    return in;
}

QDataStream &operator<<(QDataStream &out, const ObjectTown &s)
{
    out << s.identifier << s.owner << s.hasName;

    if ( s.hasName == 1 )
        saveHString(out, s.name);

    out << s.hasGuard;

    if ( s.hasGuard == 1 )
    {
        for ( quint8 i = 0; i < 7; i++ )
            out << s.guards[i];
    }

    out << s.formation << s.hasBuildings;

    if ( s.hasBuildings == 1 )
    {
        out.writeRawData( (char *) s.built, sizeof(s.built) );
        out.writeRawData( (char *) s.active, sizeof(s.active) );
    }
    else
        out << s.hasFort;

    out.writeRawData( (char *) s.mustSpells, sizeof(s.mustSpells) );
    out.writeRawData( (char *) s.canSpells, sizeof(s.canSpells) );

    out << s.eventCount;
    for (quint32 i = 0; i < s.eventCount; i++ )
    {
        out << s.events[i];
    }

    out << s.alignment;
    out.writeRawData( (char *) s.junk3, sizeof(s.junk3) );

    return out;
}

QDataStream &operator>>(QDataStream &in, ObjectTown &s)
{
    in >> s.identifier >> s.owner >> s.hasName;

    if ( s.hasName == 1 )
        loadHString(in, s.name);

    in >> s.hasGuard;

    if ( s.hasGuard == 1 )
    {
        for ( quint8 i = 0; i < 7; i++ )
            in >> s.guards[i];
    }

    in >> s.formation >> s.hasBuildings;

    if ( s.hasBuildings == 1 )
    {
        in.readRawData( (char *) s.built, sizeof(s.built) );
        in.readRawData( (char *) s.active, sizeof(s.active) );
    }
    else
        in >> s.hasFort;

    in.readRawData( (char *) s.mustSpells, sizeof(s.mustSpells) );
    in.readRawData( (char *) s.canSpells, sizeof(s.canSpells) );

    in >> s.eventCount;
    for (quint32 i = 0; i < s.eventCount; i++ )
    {
        TownEvent_t event;
        in >> event;
        s.events.push_back(event);
    }

    in >> s.alignment;
    in.readRawData( (char *) s.junk3, sizeof(s.junk3) );

    return in;
}

QDataStream &operator<<(QDataStream &out, const TownEvent_t &s)
{
    saveHString(out, s.event_name);
    saveHString(out, s.event_text);

    out << s.res << s.players_affected << s.human_affected << s.ai_affected;

    out << s.day_of_first_event << s.event_iteration;
    out.writeRawData( (char *) s.junk, sizeof(s.junk) );
    out.writeRawData( (char *) s.buildings, sizeof(s.buildings) );
    out.writeRawData( (char *) s.creatures, sizeof(s.creatures) );
    out << s.junk2;

    return out;
}

QDataStream &operator>>(QDataStream &in, TownEvent_t &s)
{
    loadHString(in, s.event_name);
    loadHString(in, s.event_text);

    in >> s.res >> s.players_affected >> s.human_affected >> s.ai_affected;

    in >> s.day_of_first_event >> s.event_iteration;
    in.readRawData( (char *) s.junk, sizeof(s.junk) );
    in.readRawData( (char *) s.buildings, sizeof(s.buildings) );
    in.readRawData( (char *) s.creatures, sizeof(s.creatures) );
    in >> s.junk2;

    return in;
}

QDataStream &operator<<(QDataStream &out, const ObjectSpell &s)
{
    out << s.spell << s.spellID;
    return out;
}

QDataStream &operator>>(QDataStream &in, ObjectSpell &s)
{
    in >> s.spell >> s.spellID;
    return in;
}

QDataStream &operator<<(QDataStream &out, const ObjectScientist &s)
{
    out << s.bonus_type << s.primaryID;
    out.writeRawData( (char *) s.junk2, sizeof(s.junk2) );

    return out;
}

QDataStream &operator>>(QDataStream &in, ObjectScientist &s)
{
    in >> s.bonus_type >> s.primaryID;
    in.readRawData( (char *) s.junk2, sizeof(s.junk2) );

    return in;
}

QDataStream &operator<<(QDataStream &out, const ObjectResource &s)
{
    out << s.res << s.quantity << s.junk;
    return out;
}

QDataStream &operator>>(QDataStream &in, ObjectResource &s)
{
    in >> s.res >> s.quantity >> s.junk;
    return in;
}

QDataStream &operator<<(QDataStream &out, const ObjectPandora &s)
{
    out << s.messageGuards;
    out << s.exp << s.spell_points << s.morals << s.luck << s.res;
    out << s.offence << s.defence << s.power << s.knowledge;
    out << s.secSkillsCount;

    if ( s.secSkillsCount > 0 )
    {
        for ( quint8 i = 0; i < s.secSkillsCount; i++)
        {
            out << s.sec_skills[i];
        }
    }

    out << s.artefactCount;

    if ( s.artefactCount > 0 )
    {
        for ( quint8 i = 0; i < s.artefactCount; i++)
        {
            out << s.artefacts[i];
        }
    }

    out << s.spellsCount;

    if ( s.spellsCount > 0 )
    {
        for ( quint8 i = 0; i < s.spellsCount; i++)
        {
            out << s.spells[i];
        }
    }

    out << s.monstreCount;

    if ( s.monstreCount > 0 )
    {
        for ( quint8 i = 0; i < s.monstreCount; i++)
        {
            out << s.monstres[i];
        }
    }
    out.writeRawData( (char *) s.junk3, sizeof(s.junk3) );
    return out;
}

QDataStream &operator>>(QDataStream &in, ObjectPandora &s)
{
    in >> s.messageGuards;
    in >> s.exp >> s.spell_points >> s.morals >> s.luck >> s.res;
    in >> s.offence >> s.defence >> s.power >> s.knowledge;
    in >> s.secSkillsCount;

    if ( s.secSkillsCount > 0 )
    {
        for ( quint8 i = 0; i < s.secSkillsCount; i++)
        {
            SecondarySkill_t skill;
            in >> skill;
            s.sec_skills.push_back(skill);
        }
    }

    in >> s.artefactCount;

    if ( s.artefactCount > 0 )
    {
        for ( quint8 i = 0; i < s.artefactCount; i++)
        {
            quint16 art;
            in >> art;
            s.artefacts.push_back(art);
        }
    }

    in >> s.spellsCount;

    if ( s.spellsCount > 0 )
    {
        for ( quint8 i = 0; i < s.spellsCount; i++)
        {
            quint8 sp;
            in >> sp;
            s.spells.push_back(sp);
        }
    }

    in >> s.monstreCount;

    if ( s.monstreCount > 0 )
    {
        for ( quint8 i = 0; i < s.monstreCount; i++)
        {
            Guard_t g;
            in >> g;
            s.monstres.push_back(g);
        }
    }
    in.readRawData( (char *) s.junk3, sizeof(s.junk3) );
    return in;
}

QDataStream &operator<<(QDataStream &out, const ObjectMonster &s)
{
    out << s.monsterID << s.monstersCount << s.mood << s.isTreasureOrText;

    if ( s.isTreasureOrText == 1 )
    {
        saveHString(out, s.text);
        out << s.res << s.artefactID;
    }
    out << s.monsterNeverRunAway << s.monsterDontGrowUp;
    out.writeRawData( (char * ) s.junk2, sizeof(s.junk2) );

    return out;
}

QDataStream &operator>>(QDataStream &in, ObjectMonster &s)
{
    in >> s.monsterID >> s.monstersCount >> s.mood >> s.isTreasureOrText;

    if ( s.isTreasureOrText == 1 )
    {
        loadHString(in, s.text);
        in >> s.res >> s.artefactID;
    }
    in >> s.monsterNeverRunAway >> s.monsterDontGrowUp;
    in.readRawData( (char * ) s.junk2, sizeof(s.junk2) );

    return in;
}

QDataStream &operator<<(QDataStream &out, const ObjectGrail &s)
{
    out << s.radius;
    return out;
}

QDataStream &operator>>(QDataStream &in, ObjectGrail &s)
{
    in >> s.radius;
    return in;
}

QDataStream &operator<<(QDataStream &out, const ObjectTownRandomDwelling &s)
{
    out << s.owner << s.minlevel << s.maxlevel;
    return out;
}
QDataStream &operator>>(QDataStream &in, ObjectTownRandomDwelling &s)
{
    in >> s.owner >> s.minlevel >> s.maxlevel;
    return in;
}


QDataStream &operator>>(QDataStream &in, ObjectLevelRandomDwelling &s)
{
    in >> s.owner >> s.castleSpecID;

    if ( s.castleSpecID == 0 )
        in >> s.towns;

    return in;
}


QDataStream &operator>>(QDataStream &in, ObjectGeneralRandomDwelling &s)
{
    in >> s.owner >> s.castleSpecID;

    if ( s.castleSpecID == 0 )
        in >> s.towns;

    in >> s.minlevel >> s.maxlevel;

    return in;
}


QDataStream &operator>>(QDataStream &in, ObjectShipyard &s)
{
    in >> s.owner;
    return in;
}


QDataStream &operator>>(QDataStream &in, ObjectDwelling &s)
{
    in >> s.owner;
    return in;
}


QDataStream &operator>>(QDataStream &in, ObjectMessageGuards &s)
{
    in >> s.hasText;

    if ( s.hasText == 1 )
    {
        loadHString(in, s.text);
        in >> s.hasGuards;

        if ( s.hasGuards == 1)
        {
            for (quint8 i = 0; i < 7; i++)
                in >> s.guards[i];
        }
        in.readRawData( (char *) s.junk, sizeof(s.junk) );
    }

    return in;
}


QDataStream &operator>>(QDataStream &in, Guard_t &s)
{
    in >> s.GuardID >> s.GuardCount;
    return in;
}


QDataStream &operator>>(QDataStream &in, Resources_t &s)
{
    in >> s.wood >> s.mercury >> s.ore >> s.sulfur >> s.crystal >> s.gem >> s.gold;
    return in;
}


QDataStream &operator>>(QDataStream &in, ObjectGarrison &s)
{
    in >> s.color;

    for (quint8 i = 0; i < 7; i++ )
        in >> s.guards[i];

    in >> s.undeleteSoldiers;
    in.readRawData( (char *) s.junk, sizeof(s.junk) );
    return in;
}


QDataStream &operator>>(QDataStream &in, ObjectAbandonedMine &s)
{
    in >> s.resources;
    in.readRawData( (char *) s.junk, sizeof(s.junk) );
    return in;
}


QDataStream &operator>>(QDataStream &in, GlobalEvent &s)
{
    loadHString(in, s.name);
    loadHString(in, s.text);

    in >> s.res >> s.players_affected >> s.human_affected >> s.ai_affected;
    in >> s.day_of_first_event;
    in >> s.event_iteration;
    in.readRawData( (char *) s.junk, sizeof(s.junk) );
    return in;
}


QDataStream &operator>>(QDataStream &in, ObjectWitchHut &s)
{
    in >> s.secskills;
    return in;
}


QDataStream &operator>>(QDataStream &in, ObjectMine &s)
{
    in >> s.color;
    return in;
}


QDataStream &operator>>(QDataStream &in, ObjectHeroPlaceholder &s)
{
    in >> s.color >> s.type;
    return in;
}


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


QDataStream &operator>>(QDataStream &in, ObjectHero &s)
{
    in >> s.heroID >> s.color >> s.hero >> s.isName;

    if ( s.isName == 1 )
        loadHString(in, s.name);

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
        loadHString(in, s.biography);

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

    loadHString(in, s.quest_begin);
    loadHString(in, s.quest_inprocess);
    loadHString(in, s.quest_end);

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

QDataStream &operator<<(QDataStream &out, const ObjectLevelRandomDwelling &s)
{
    out << s.owner << s.castleSpecID;

    if ( s.castleSpecID == 0 )
        out << s.towns;

    return out;
}


QDataStream &operator<<(QDataStream &out, const ObjectGeneralRandomDwelling &s)
{
    out << s.owner << s.castleSpecID;

    if ( s.castleSpecID == 0 )
        out << s.towns;

    out << s.minlevel << s.maxlevel;

    return out;
}


QDataStream &operator<<(QDataStream &out, const ObjectShipyard &s)
{
    out << s.owner;
    return out;
}


QDataStream &operator<<(QDataStream &out, const ObjectDwelling &s)
{
    out << s.owner;
    return out;
}


QDataStream &operator<<(QDataStream &out, const ObjectMessageGuards &s)
{
    out << s.hasText;

    if ( s.hasText == 1 )
    {
        saveHString(out, s.text);
        out << s.hasGuards;

        if ( s.hasGuards == 1)
        {
            for (quint8 i = 0; i < 7; i++)
                out << s.guards[i];
        }
        out.writeRawData( (char *) s.junk, sizeof(s.junk) );
    }

    return out;
}


QDataStream &operator<<(QDataStream &out, const Guard_t &s)
{
    out << s.GuardID << s.GuardCount;
    return out;
}


QDataStream &operator<<(QDataStream &out, const Resources_t &s)
{
    out << s.wood << s.mercury << s.ore << s.sulfur << s.crystal << s.gem << s.gold;
    return out;
}


QDataStream &operator<<(QDataStream &out, const ObjectGarrison &s)
{
    out << s.color;

    for (quint8 i = 0; i < 7; i++ )
        out << s.guards[i];

    out << s.undeleteSoldiers;
    out.writeRawData( (char *) s.junk, sizeof(s.junk) );
    return out;
}


QDataStream &operator<<(QDataStream &out, const ObjectAbandonedMine &s)
{
    out << s.resources;
    out.writeRawData( (char *) s.junk, sizeof(s.junk) );
    return out;
}


QDataStream &operator<<(QDataStream &out, const GlobalEvent &s)
{
    saveHString(out, s.name);
    saveHString(out, s.text);

    out << s.res << s.players_affected << s.human_affected << s.ai_affected;
    out << s.day_of_first_event;
    out << s.event_iteration;
    out.writeRawData( (char *) s.junk, sizeof(s.junk) );
    return out;
}


QDataStream &operator<<(QDataStream &out, const ObjectWitchHut &s)
{
    out << s.secskills;
    return out;
}


QDataStream &operator<<(QDataStream &out, const ObjectMine &s)
{
    out << s.color;
    return out;
}


QDataStream &operator<<(QDataStream &out, const ObjectHeroPlaceholder &s)
{
    out << s.color << s.type;
    return out;
}


QDataStream &operator<<(QDataStream &out, const ObjectQuestionGuard &s)
{
    out << s.quest;

    switch(s.quest)
    {
    case 0:
        break;
    case 1:
        out << s.level;
        break;
    case 2:
        out << s.offence << s.defence << s.power << s.knowledge;
        break;
    case 3:
        out << s.heroID;
        break;
    case 4:
        out << s.monsterID;
        break;
    case 5:
        out << s.artQuantity;
        for ( quint8 i = 0; i < s.artQuantity; i++ )
        {
            out << s.arts[i];
        };
        break;
    case 6:
        out << s.creaturesQuantity;
        if ( s.creaturesQuantity > 0 )
        {
            for ( quint8 i = 0; i < s.creaturesQuantity; i++ )
            {
                Guard_t g;
                out << s.creatures[i];
            }
        }
        break;
    case 7:
        out << s.res;
        break;
    case 8:
        out << s.heroID_;
        break;
    case 9:
        out << s.playerColor;
        break;
    default:
        qDebug() << "lolwut? o_O";
        break;
    };

    out << s.time_limit;
    out << s.quest_begin << s.quest_inprocess << s.quest_end;

    return out;
}


QDataStream &operator<<(QDataStream &out, const ObjectHero &s)
{
    out << s.heroID << s.color << s.hero << s.isName;

    if ( s.isName == 1 )
        saveHString(out, s.name);

    out << s.isExp;

    if ( s.isExp == 1 )
        out << s.exp;

    out << s.isPortrait;

    if ( s.isPortrait == 1)
        out << s.portrait;

    out << s.isSecondary;

    if ( s.isSecondary == 1 )
    {
        out << s.skills_quantity;
        if ( s.skills_quantity > 0 )
        {
            for (quint8 i = 0; i < s.skills_quantity; i++ )
            {
                out << s.skills[i];
            }
        }
    }

    out << s.isCreature;

    if ( s.isCreature == 1 )
    {
        for (quint8 i = 0; i < 7; i++)
            out << s.creatures[i];
    }

    out << s.creaturesFormation << s.isArtefacts;

    if ( s.isArtefacts == 1 )
    {
        out << s.headID << s.shouldersID << s.neckID << s.rightHandID << s.leftHandID;
        out << s.trunkID << s.rightRingID << s.leftRingID << s.legsID;
        out << s.misc1ID << s.misc2ID << s.misc3ID << s.misc4ID;
        out << s.machine1ID << s.machine2ID << s.machine3ID << s.machine4ID;
        out << s.magicbook << s.misc5ID;

        out << s.knapsack_count;

        if ( s.knapsack_count > 0 )
        {
            for ( quint16 i = 0; i < s.knapsack_count; i++ )
            {
                out << s.knapsack[i];
            }
        }
    }

    out << s.zoneRadius << s.isBiography;

    if ( s.isBiography == 1 )
        saveHString(out, s.biography);

    out << s.gender << s.isSpells;

    if ( s.isSpells == 1 )
        out.writeRawData( (char *) s.spells, sizeof(s.spells) );

    out << s.isPrimary;

    if ( s.isPrimary == 1 )
    {
        out << s.offence << s.defence << s.power << s.knowledge;
    }

    out.writeRawData( (char *) s.unknown, sizeof(s.unknown) );

    return out;
}


QDataStream &operator<<(QDataStream &out, const ObjectProphet &s)
{
    out << s.quest;
    switch(s.quest)
    {
    case 0:

        break;
    case 1:
        out << s.level;
        break;
    case 2:
        out << s.offence << s.defence << s.power << s.knowledge;
        break;
    case 3:
        out << s.heroID;
        break;
    case 4:
        out << s.monsterID;
        break;
    case 5:
        out << s.artQuantity;
        for ( quint8 i = 0; i < s.artQuantity; i++ )
        {
            quint16 art;
            out << s.arts[i];
        };
        break;
    case 6:
        out << s.creaturesQuantity;
        if ( s.creaturesQuantity > 0 )
        {
            for ( quint8 i = 0; i < s.creaturesQuantity; i++ )
            {
                Guard_t g;
                out << s.creatures[i];
            }
        }
        break;
    case 7:
        out << s.res;
        break;
    case 8:
        out << s.heroID_;
        break;
    case 9:
        out << s.playerColor;
        break;
    default:
        qDebug() << "lolwut? o_O";
        break;
    };

    out << s.time_limit;

    saveHString(out, s.quest_begin);
    saveHString(out, s.quest_inprocess);
    saveHString(out, s.quest_end);

    out << s.reward;

    switch(s.reward)
    {
    case 0:
        break;
    case 1:
        out << s.exp;
        break;
    case 2:
        out << s.spellPoints;
        break;
    case 3:
        out << s.morale;
        break;
    case 4:
        out << s.lucky;
        break;
    case 5:
        out << s.resID << s.resQ;
        break;
    case 6:
        out << s.priSkillID << s.priSkillBonus;
        break;
    case 7:
        out << s.secSkillID << s.secSkillLevel;
        break;
    case 8:
        out << s.artId;
        break;
    case 9:
        out << s.spell;
        break;
    case 10:
        out << s.creatureID << s.creatureQuantity;
        break;
    default:
        qDebug() << "lolwut?";
    };
    out << s.junk;

    return out;
}
QT_END_NAMESPACE
