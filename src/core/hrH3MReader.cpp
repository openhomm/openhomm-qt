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

    if ( basic.load(&map) == false )
    {
        map.close();
        return false;
    }

    for ( int i = 0; i < 8; ++i )
    {
        players[i].load(&map);
        players[i].dump();
    }
    return true;
}

bool BasicParametres_t::load(QIODevice *device)
{
    Q_ASSERT(device != NULL);

    device->read( (char *) &version, 4);

    if ( version != 0x0000001C )
    {
        qWarning("File is not a HoMM III : SoD Map.");
        return false;
    }

    device->read( (char *) &junk, 1);
    device->read( (char *) &size, 4);
    device->read( (char *) &under, 1);

    name        = hrString::deserialize(device);
    description = hrString::deserialize(device);

    device->read( (char *) &difficult,  1);
    device->read( (char *) &levelLimit, 1);

    return true;
}

PlayerAttributes_t::PlayerAttributes_t()
{
}

PlayerAttributes_t::~PlayerAttributes_t()
{
    heroes.clear();
}

bool PlayerAttributes_t::load(QIODevice *device)
{
    Q_ASSERT(device != NULL);

    device->read( (char *) &isHuman,    sizeof(isHuman)     );
    device->read( (char *) &isComputer, sizeof(isComputer)  );
    device->read( (char *) &behavior,   sizeof(behavior)    );
    device->read( (char *) &isCityTypesOpt, sizeof(isCityTypesOpt) );
    device->read( (char *) &cityTypes,  sizeof(cityTypes)   );
    device->read( (char *) &randomCity, sizeof(randomCity)  );
    device->read( (char *) &mainCity,   sizeof(mainCity)    );

    if ( mainCity == 1 )
    {
        device->read( (char *) &generateHero,   sizeof(generateHero) );
        device->read( (char *) city,            sizeof(city)         );
    }

    device->read( (char *) &randomHero, sizeof(randomHero)  );
    device->read( (char *) &heroType,   sizeof(heroType)    );

    if ( heroType != 0xFF )
    {
        device->read( (char *) &heroPortret, sizeof(heroPortret) );
        heroName = hrString::deserialize(device);
    }

    device->read( (char *) &junk, sizeof(junk) );
    device->read( (char *) &heroesCount, sizeof(heroesCount) );

    for ( quint32 i = 0; i < heroesCount; ++i )
    {
        Hero hero;
        device->read( (char *) &hero.portret, sizeof(hero.portret) );
        hero.name = hrString::deserialize(device);
        heroes.push_back(hero);
    }

    return true;
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
