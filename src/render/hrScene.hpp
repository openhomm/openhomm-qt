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
#include "hrGraphicsItem.hpp"

class hrScene : public QObject
{
    Q_OBJECT
private:
    QList<hrGraphicsItem> items;
    QVector<QCursor> cursor;
public:
    void addItem(const hrGraphicsItem& item);
    QListIterator<hrGraphicsItem> getItems();
    void clearItems();
    void sortItems();

    void loadCursor(const QString &name);
    const QCursor& getCursor(int type) const;

    virtual void setViewport(int width, int height) = 0;
    virtual void collect() = 0;
    virtual void onMouseEvent(QMouseEvent *event) = 0;
    virtual void onMouseLeave() = 0;

signals:
    void sceneChanged();
    void cursorChaged(int type);
};
