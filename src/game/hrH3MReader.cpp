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
bool pack(const QByteArray& array, const QString &filename)
{
    QFile map(filename);
    if ( !map.open(QIODevice::WriteOnly) ) {
        return false;
    }

    gzFile file = gzopen(filename.toLocal8Bit().data(), "wb");
    gzwrite(file,array.data(),array.length());
    gzclose(file);

    return true;
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

    m >> _version;

    qWarning("Map version is %d", _version);

    _header.load(m,_version);

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

    quint32 sqMapSize = _header.mapSize()*_header.mapSize();
    ground = new hrTile[sqMapSize];
    m.readRawData( (char *) ground, sizeof(hrTile)* sqMapSize);

    if ( _header.isUnderground() )
    {
        underground = new hrTile[sqMapSize];
        m.readRawData( (char *) underground, sizeof(hrTile)*sqMapSize );
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

        quint32 objID=obj[i].objectID;

        switch(objects[obj[i].objectID].object_class)
        {
        case 5:
        case 65:
        case 66:
        case 67:
        case 68:
        case 69:
             m >> artefactObjs[i];
             break;

        case 6:
             m >> pandoras[i];
            break;

        case 17:
        case 20:
        case 42: //lighthouse
            m >> dwellings[i];
            break;

        case 26:
            m >> localevents[i];
            break;

        case 33:
        case 219:
            m >> garrisons[i];
            break;

        case 34:
        case 70:
            m >> heroes[i];
            break;
        case 62:
            m >> heroes[i];
            break;

        case 36:
            m >> grails[i];
            break;

        case 53:
            switch(objects[obj[i].objectID].object_number) {
            case 7:
                // bit0 - mercury, 1 - ore, 2 - sulfur,
                // bit3 - crystal, 4 - gem, 5 - gold
                m >> aMines[i];
                break;
            default:
                m >> mines[i];
                break;
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
            m >> monsters[i];
            break;

        case 76:
        case 79:
            m >> resources[i];
            break;

        case 81:
            m >> scientists[i];
            break;

         case 83:
            m >> prophets[i];
            break;

         case 87:
            m >> shipyards[i];
            break;

         case 88:
         case 89:
         case 90:
            m >> shrines[i];
            break;

         case 91:
         case 59:
            m >> signs[i];
            break;

         case 93:
            m >> spellObjs[i];
            break;

         case 98:
         case 77:
            m >> towns[i];
            break;

         case 113:
            m >> whuts[i];
            break;

         case 215:
            m >> qguards[i];
            break;

         case 216:
            m >> grDwellings[i];
            break;
         case 217:
            m >> lrDwellings[i];
            break;
         case 218:
            m >> trDwellings[i];
            break;
         case 220:
            m >> aMines[i];
            break;
        };
    }
    return true;
}

bool hrH3MReader::save(const QString &name)
{
    QByteArray data;

    QBuffer map(&data);
    if ( !map.open(QIODevice::WriteOnly) ) {
        qWarning() << map.errorString();
        return false;
    }

    QDataStream m(&map);
    m.setByteOrder(QDataStream::LittleEndian);

    m << _version;

    _header.save(m,_version);

    for ( int i = 0; i < 8; i++ ) {
        m << players[i];
    }

    m << svc << slc << teams;
    m << fh;
    m << artefacts << spells << secSkills << rumors;

    for ( int i = 0; i < 156; i++ ) {
        m << enable[i];
        if ( enable[i] == 1 ) {
            m << heroOptions[i];
        }
    }

    quint32 sqMapSize = _header.mapSize()*_header.mapSize();
    m.writeRawData( (char *) ground, sizeof(hrTile)* sqMapSize);

    if ( _header.isUnderground() ) {
        m.writeRawData( (char *) underground, sizeof(hrTile)*sqMapSize );
    }

    m << objectQuantity;

    for ( quint32 i = 0; i < objectQuantity; i++ ) {
        m << objects[i];
    }

    m << objectOptions;

    for ( quint32 i = 0; i < objectOptions; i++ )
    {
        m << obj[i];

        quint32 objID=obj[i].objectID;
        switch(objects[obj[i].objectID].object_class)
        {
        case 5:
        case 65:
        case 66:
        case 67:
        case 68:
        case 69:
             m << artefactObjs[i];
             break;

        case 6:
             m << pandoras[i];
            break;

        case 17:
        case 20:
        case 42: //lighthouse
            m << dwellings[i];
            break;

        case 26:
            m << localevents[i];
            break;

        case 33:
        case 219:
            m << garrisons[i];
            break;

        case 34:
        case 70:
            m << heroes[i];
            break;
        case 62:
            m << heroes[i];
            break;

        case 36:
            m << grails[i];
            break;

        case 53:
            switch(objects[obj[i].objectID].object_number) {
            case 7:
                // bit0 - mercury, 1 - ore, 2 - sulfur,
                // bit3 - crystal, 4 - gem, 5 - gold
                m << aMines[i];
                break;
            default:
                m << mines[i];
                break;
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
            m << monsters[i];
            break;

        case 76:
        case 79:
            m << resources[i];
            break;

        case 81:
            m << scientists[i];
            break;

         case 83:
            m << prophets[i];
            break;

         case 87:
            m << shipyards[i];
            break;

         case 88:
         case 89:
         case 90:
            m << shrines[i];
            break;

         case 91:
         case 59:
            m << signs[i];
            break;

         case 93:
            m << spellObjs[i];
            break;

         case 98:
         case 77:
            m << towns[i];
            break;

         case 113:
            m << whuts[i];
            break;

         case 215:
            m << qguards[i];
            break;

         case 216:
            m << grDwellings[i];
            break;
         case 217:
            m << lrDwellings[i];
            break;
         case 218:
            m << trDwellings[i];
            break;
         case 220:
            m << aMines[i];
            break;
        };
    }

    return pack(data,name);
}

hrTile hrH3MReader::getTile(quint32 index, bool isUnderground)
{
    Q_ASSERT(index >= 0 && index <= _header.mapSize() * _header.mapSize());
    if ( isUnderground && _header.isUnderground() )
      return underground[index];

    return ground[index];
}

bool hrH3MReader::hasUnderground() const
{
    return _header.isUnderground();
}

int hrH3MReader::getSize() const
{
    return _header.mapSize();
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

bool hrMapHeader::load(QDataStream &in, quint32 mapVersion)
{
    if ( mapVersion != MAP_HOMM3_AB && mapVersion != MAP_HOMM3_ROE && mapVersion != MAP_HOMM3_SOD )
        return false;

    in >> _areAnyPlayers;//True if there are any playable players on the map.
    in >> _mapSize;
    in >> _underground;
    loadHString(in,_name);
    loadHString(in, _description);
    in >> _difficult;

    if ( mapVersion == MAP_HOMM3_SOD )
        in >> _levelLimit;

    return true;
}

bool hrMapHeader::save(QDataStream &in, quint32 mapVersion)
{
    if ( mapVersion != MAP_HOMM3_SOD )
        return false;

    in << _areAnyPlayers;//True if there are any playable players on the map.
    in << _mapSize;
    in << _underground;
    saveHString(in,_name);
    saveHString(in, _description);
    in << _difficult;

    if ( mapVersion == MAP_HOMM3_SOD )
        in << _levelLimit;

    return true;
}

QT_BEGIN_NAMESPACE
QDataStream &operator<<(QDataStream &out, const Hero_t &h)
{
    out << h.portret;
    saveHString(out, h.name);
    return out;
}
QDataStream &operator>>(QDataStream &in, Hero_t &h)
{
    in >> h.portret;
    loadHString(in, h.name);
    return in;
}
QDataStream &operator<<(QDataStream &out, const PlayerAttributes_t &p)
{
    out << p.isHuman << p.isComputer << p.behavior << p.isCityTypesOpt;
    out << p.cityTypes << p.randomCity << p.mainCity;

    if ( p.mainCity == 1 )
        out << p.generateHeroAtMainTown << p.generateHero
                << p.city[0] << p.city[1] << p.city[2] ;

    out << p.randomHero << p.heroType;

    if(p.heroType != 0xFF)
    {
        out << p.heroPortret;
        saveHString(out, p.heroName);
    }

    out << p.junk << p.heroesCount;
    for ( quint32 i = 0; i < p.heroesCount; i++ )
    {
        out << p.heroes[i];
    }

    return out;
}
QDataStream &operator>>(QDataStream &in, PlayerAttributes_t &p)
{
    in >> p.isHuman >> p.isComputer >> p.behavior >> p.isCityTypesOpt;
    in >> p.cityTypes >> p.randomCity >> p.mainCity;

    if ( p.mainCity == 1 )
        in >> p.generateHeroAtMainTown >> p.generateHero
                >> p.city[0] >> p.city[1] >> p.city[2] ;

    in >> p.randomHero >> p.heroType;

    if(p.heroType != 0xFF)
    {
        in >> p.heroPortret;
        loadHString(in, p.heroName);
    }

    in >> p.junk >> p.heroesCount;
    for ( quint32 i = 0; i < p.heroesCount; i++ )
    {
        Hero_t hero;
        in >> hero;
        p.heroes.push_back(hero);
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
QDataStream &operator<<(QDataStream &out, const SpecialVictoryCondition_t &s)
{
    out << s.id;

    if ( s.id != 0xFF )
    {
        out << s.canStandardEnd << s.canComputer;

        switch(s.id)
        {
        case 0x00:
            out << s.artId;
            break;
        case 0x01:
            out << s.creatureId << s.creatureCount;
            break;
        case 0x02:
            out << s.resId << s.resCount;
            break;
        case 0x03:
            out << s.townCoord[0] << s.townCoord[1] << s.townCoord[2];
            out << s.hallLevel << s.castleLevel;
            break;
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
            out << s.coord[0] << s.coord[1] << s.coord[2];
            break;
        case 0x08:
        case 0x09:
            break;
        case 0x0A:
            out << s.artType << s.artCoord[0] << s.artCoord[1] << s.artCoord[2];
            break;
        default:
            qWarning("SpecialVictoryCondition ID: %d is unkonwn", s.id);
            break;
        };
    }

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

QDataStream &operator<<(QDataStream &out, const SpecialLossCondition_t &s)
{
     out <<  s.id;

    if ( s.id != 0xFF )
    {

        if ( s.id == 0x00 || s.id == 0x01 )
             out <<  s.coord[0] << s.coord[1] << s.coord[2];
        else if ( s.id == 0x02 )
             out <<  s.days;
        else
            qWarning("SpecialLossConditions ID: %d is unkown", s.id);
    }

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

QDataStream &operator<<(QDataStream &out, const Teams_t &t)
{
    out << t.quantity;

    if ( t.quantity > 0 )
        out.writeRawData((char*)t.commands, sizeof(t.commands));

    return out;
}
QDataStream &operator>>(QDataStream &in, Teams_t &t)
{
    in >> t.quantity;

    if ( t.quantity > 0 )
        in.readRawData((char*)t.commands, sizeof(t.commands));

    return in;
}

QDataStream &operator<<(QDataStream &out, const TunedHero_t &t)
{
    out << t.id << t.portrait;
    saveHString(out,t.name);
    out << t.players;
    return out;
}
QDataStream &operator>>(QDataStream &in, TunedHero_t &t)
{
    in >> t.id >> t.portrait;
    loadHString(in,t.name);
    in >> t.players;
    return in;
}

QDataStream &operator<<(QDataStream &out, const FreeHeroes_t &f)
{
    out.writeRawData( (char *)f.heroes, sizeof(f.heroes) );
    out.writeRawData( (char *)f.junk, sizeof(f.junk) );

    out << f.heroesQuantity;

    if ( f.heroesQuantity > 0 )
    {
        for ( quint8 i = 0; i < f.heroesQuantity; i++ )
        {
            out << f.tunedHeroes[i];
        }
    }

    out.writeRawData( (char *) f.junk2, sizeof(f.junk2) );
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

QDataStream &operator<<(QDataStream &out, const Artefacts_t &a)
{
    out.writeRawData( (char *) a.artefacts, sizeof(a.artefacts) );
    return out;
}
QDataStream &operator>>(QDataStream &in, Artefacts_t &a)
{
    in.readRawData( (char *) a.artefacts, sizeof(a.artefacts) );
    return in;
}

QDataStream &operator<<(QDataStream &out, const Spells_t &s)
{
    out.writeRawData( (char *) s.spells, sizeof(s.spells) );
    return out;
}
QDataStream &operator>>(QDataStream &in, Spells_t &s)
{
    in.readRawData( (char *) s.spells, sizeof(s.spells) );
    return in;
}

QDataStream &operator<<(QDataStream &out, const SecSkills_t &s)
{
    out.writeRawData( (char *) s.skills, sizeof(s.skills) );
    return out;
}
QDataStream &operator>>(QDataStream &in, SecSkills_t &s)
{
    in.readRawData( (char *) s.skills, sizeof(s.skills) );
    return in;
}

QDataStream &operator<<(QDataStream &out, const Rumor_t &r)
{
    saveHString(out, r.rumor_name);
    saveHString(out,r.rumor_text);
    return out;
}
QDataStream &operator>>(QDataStream &in, Rumor_t &r)
{
    loadHString(in, r.rumor_name);
    loadHString(in,r.rumor_text);

    return in;
}

QDataStream &operator<<(QDataStream &out, const Rumors_t &r)
{
    out << r.quantity;

    if ( r.quantity > 0 )
    {
        for ( quint32 i = 0; i < r.quantity; i++ )
        {
            out << r.rumors[i];
        }
    }
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

QDataStream &operator<<(QDataStream &out, const HeroOptions_enabled &h)
{
    out <<h.isExp;

    if ( h.isExp == 1 )
        out <<h.exp;

    out <<h.isSecSkill;

    if ( h.isSecSkill == 1 )
    {
        out <<h.secSkillsQuantity;
        if ( h.secSkillsQuantity > 0 )
        {
            for ( quint32 i = 0; i < h.secSkillsQuantity; i++ )
            {
                out <<h.secSkills[i];
            }
        }
    }

    out <<h.isArtefacts;

    if ( h.isArtefacts == 1 )
    {
        out <<h.headID << h.shouldersID << h.neckID << h.rightHandID << h.leftHandID;
        out <<h.trunkID << h.rightRingID << h.leftRingID << h.legsID;
        out <<h.misc1ID << h.misc2ID << h.misc3ID << h.misc4ID;
        out <<h.machine1ID << h.machine2ID << h.machine3ID << h.machine4ID;
        out <<h.magicbook << h.misc5ID;

        out <<h.knapsack_count;

        if ( h.knapsack_count > 0 )
        {
            for ( quint16 i = 0; i < h.knapsack_count; i++ )
            {
                out <<h.knapsackID[i];
            }
        }
    }

    out <<h.isBiography;

    if ( h.isBiography == 1 )
        saveHString(out, h.biography);

    out <<h.gender << h.isSpells;

    if ( h.isSpells == 1 )
    {
        out.writeRawData( (char *) h.spells, sizeof(h.spells) );
    }

    out <<h.isPrimarySkills;

    if ( h.isPrimarySkills == 1 )
    {
        out <<h.attack << h.defence << h.power << h.knowledge;
    }

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
        loadHString(in, h.biography);

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
