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
#include "hrScene.hpp"

/*!
  \class hrScene
  \brief The hrScene class
*/


void hrScene::addItem(const hrGraphicsItem& item)
{
    items.append(item);
}

void hrScene::clearItems()
{
    items.clear();
}

void hrScene::sortItems()
{
    qSort(items);
}

QListIterator<hrGraphicsItem> hrScene::getItems()
{
    return QListIterator<hrGraphicsItem>(items);
}

void hrScene::loadCursor(const QString &name)
{
    QImageReader ir("vfs:/" + name);
    QImage im;
    for (int i = 0; ir.jumpToImage(i); i++)
    {
        if (ir.read(&im))
            cursor.append(QCursor(QPixmap::fromImage(im.copy(0, 0, 32, 32))
                                  , 0
                                  , 0
                                  )
                          );
    }
}

QCursor hrScene::getCursor(int type) const
{
    if (!cursor.isEmpty() && type >= 0 && type < cursor.size())
        return cursor.at(type);
    return QCursor(Qt::ArrowCursor);
}
