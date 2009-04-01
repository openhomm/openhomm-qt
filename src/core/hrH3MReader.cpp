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

hrH3MReader::hrH3MReader()
{
}

hrH3MReader::~hrH3MReader()
{
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

    qDebug() << basic.version;
    qDebug() << basic.under;
    qDebug() << basic.size;
    qDebug() << basic.under;
    qDebug() << basic.name;
    qDebug() << basic.description;
    qDebug() << basic.difficult;
    qDebug() << basic.levelLimit;

    for ( int i = 0; i < 8; i++ )
    {
        qDebug() << "===" << i << "===";
        m >> players[i];
        players[i].dump();
    }

    return true;
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

void PlayerAttributes_t::dump()
{
    qDebug("isHuman: %d", isHuman);
    qDebug("isComputer: %d", isComputer);
    qDebug("behavior: %d", behavior);
    qDebug("isCityTypesOpt: %d", isCityTypesOpt);
    qDebug("cityTypes: %d", cityTypes);
    qDebug("randomCity: %d", randomCity);
    qDebug("mainCity: %d", mainCity);
    if ( mainCity == 1 )
    {
        qDebug("\tgenerateHero: %d", generateHero);
        qDebug("\tcity %d {%d.%d.%d}", city[0], city[1], city[2], city[3]);
    }
    qDebug("randomHero: %d", randomHero);
    qDebug("heroType: %d", heroType);

    if ( heroType != 0xFF )
    {
        qDebug("\theroPortret: %d", heroPortret);
        qDebug() << "\theroName: " << heroName;
    }
    qDebug("junk: %d", junk);
    qDebug("heroesCount: %d", heroesCount);

    for ( quint32 i = 0; i < heroesCount; i++ )
    {
        qDebug("\tHero::portret : %d", heroes[i].portret);
        qDebug() << "\tHero::name : " << heroes[i].name;
    }
    qDebug() << "===================";
}
