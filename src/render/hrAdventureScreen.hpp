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
#include "hrScene.hpp"
#include "hrGraphicsItem.hpp"
#include "hrTile.hpp"
#include "hrSceneObject.hpp"
#include "hrH3MReader.hpp"
#include "hrCache.hpp"
#include "hrCoord.hpp"

enum AdvCursorType_e
{
    CURSOR_POINTER = 0,
    CURSOR_WAIT    = 1,
    CURSOR_HEROES  = 2,
    CURSOR_CASTLE  = 3,
    CURSOR_MOVE    = 4,
    CURSOR_FIGHT   = 5,
    CURSOR_BOAT    = 6,
    CURSOR_ANCHOR  = 7,
    CURSOR_CHANGE  = 8,
    CURSOR_ACTION  = 9,
    CURSOR_MOVE2   = 10,
    CURSOR_FIGHT2  = 11,
    CURSOR_BOAT2   = 12,
    CURSOR_ANCHOR2 = 13,
    CURSOR_CHANGE2 = 14,
    CURSOR_ACTION2 = 15,
    CURSOR_MOVE3   = 16,
    CURSOR_FIGHT3  = 17,
    CURSOR_BOAT3   = 18,
    CURSOR_ANCHOR3 = 19,
    CURSOR_CHANGE3 = 20,
    CURSOR_ACTION3 = 21,
    CURSOR_MOVE4   = 22,
    CURSOR_FIGHT4  = 23,
    CURSOR_BOAT4   = 24,
    CURSOR_ANCHOR4 = 25,
    CURSOR_CHANGE4 = 26,
    CURSOR_ACTION4 = 27,
    CURSOR_REDBOAT = 28,
    CURSOR_REDBOAT2 = 29,
    CURSOR_REDBOAT3 = 30,
    CURSOR_REDBOAT4 = 31,
    CURSOR_SCROLL_TOP  = 32,
    CURSOR_SCROLL_TOPRIGHT = 33,
    CURSOR_SCROLL_RIGHT    = 34,
    CURSOR_SCROLL_DOWNRIGHT = 35,
    CURSOR_SCROLL_DOWN      = 36,
    CURSOR_SCROLL_DOWNLEFT  = 37,
    CURSOR_SCROLL_LEFT      = 38,
    CURSOR_SCROLL_TOPLEFT   = 39,
    CURSOR_POINTER2    = 40,
    CURSOR_TELEPORT    = 41,
    CURSOR_DOOR        = 42,
    CURSOR_BOATTOWATER = 43,
};

class hrAdventureScreen : public hrScene
{
    Q_OBJECT
public:
    hrAdventureScreen();
    ~hrAdventureScreen();
    void loadMap(hrH3MReader *reader);
    void collect();
    void onMouseEvent(QMouseEvent *event);
    void onMouseLeave();
    void setViewport(int width, int height);

private:
    QVector<hrTile> tiles;
    QVector<hrTile> tilesUnderground;
    QList<hrSceneObject> objects;
    QList<hrSceneObject> objectsUnderground;

    QMap<int, hrGraphicsItem> itemsTerrain;
    QMap<int, hrGraphicsItem> itemsRiver;
    QMap<int, hrGraphicsItem> itemsRoad;
    QMap<int, hrGraphicsItem> itemsObject;

    QRect viewport;
    QRect size;

    QTimer scrollTimer;
    QTimer animateTimer;

    void setCursor(AdvCursorType_e cursorType);

    int width() const;
    int height() const;
    int dx;
    int dy;
    bool isAnimate;
    void clearMap();
    void setVisibleRect(QRect rect);
    const hrTile& getTile(const QPoint& point) const;
    void drawRoad(const hrTile& tile, const QPoint& pos);

private slots:
    void scroll();
    void animate();
};
    
