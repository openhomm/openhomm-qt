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
#include "hrAdventureScreen.hpp"

hrAdventureScreen::hrAdventureScreen()
    : dx(0), dy(0)
    , isAnimate(false)
    , isUnderground(false)
    , hasUnderground(false)
{
    connect(&scrollTimer, SIGNAL(timeout()), this, SLOT(scroll()));
    connect(&animateTimer, SIGNAL(timeout()), this, SLOT(animate()));
}

hrAdventureScreen::~hrAdventureScreen()
{
}

void hrAdventureScreen::setViewport(int width, int height)
{
    setVisibleRect(QRect(0, 0, width, height));
}

void hrAdventureScreen::setVisibleRect(QRect rect)
{
    rect.width() % coord::dim != 0 ?
                viewport.setWidth(rect.width() / coord::dim + 1) :
                viewport.setWidth(coord::toCell(rect.width()));
    rect.height() % coord::dim != 0 ?
                viewport.setHeight(rect.height() / coord::dim + 1) :
                viewport.setHeight(coord::toCell(rect.height()));

    if (!size.isEmpty() && !size.contains(viewport))
    {
        QRect rect = size.intersected(viewport);
        viewport.moveBottomRight(rect.bottomRight());
    }
}

void hrAdventureScreen::collect()
{
    clearItems();

    const QVector<hrTile> &tiles = isUnderground ? tilesUnderground : tilesGround;
    QList<hrSceneObject> &objects = isUnderground ? objectsUnderground : objectsGround;

    if (tiles.isEmpty())
        return;

    for (int i = viewport.width() - 1; i >= 0; i--)
        for (int j = viewport.height() - 1; j >= 0; j--)
        {
            QPoint pos = coord::toPix(QPoint(i,j));
            const hrTile &tile = getTile(viewport.topLeft() + QPoint(i, j));

            hrGraphicsItem item = itemsTerrain[tile.terrainId];
            item.setCurFrame(tile.terrainFrame);
            item.setMirror(tile.isTerrainHorizontal(), tile.isTerrainVertical());
            item.setPoint(pos);
            addItem(item);

            if (tile.hasRiver())
            {
                hrGraphicsItem item = itemsRiver[tile.riverId];
                item.setCurFrame(tile.riverFrame);
                item.setMirror(tile.isRiverHorizontal(), tile.isRiverVertical());
                item.setPoint(pos);
                addItem(item);
            }
            if (tile.hasRoad())
            {
                drawRoad(tile, pos);
            }
        }

    if (viewport.y() > 0)
    {
        for (int i = 0; i < viewport.width(); i++)
        {
            QPoint pos = coord::toPix(QPoint(i, -1));
            const hrTile &tile = getTile(viewport.topLeft() + QPoint(i, -1));

            if (tile.hasRoad())
            {
                drawRoad(tile, pos);
            }
        }
    }

    QMutableListIterator<hrSceneObject> it(objects);

    while (it.hasNext())
    {
        hrSceneObject &obj = it.next();

        if (viewport.intersects(obj.getRect()))
        {
            QPoint pos = coord::toPix(obj.getPoint() - viewport.topLeft());
            hrGraphicsItem item = itemsObject[obj.getId()];
            item.setCurFrame(isAnimate ? obj.getNextFrame() : obj.getCurFrame());
            item.setPoint(pos);
            addItem(item);
        }
    }

    isAnimate = false;
}

void hrAdventureScreen::drawRoad(const hrTile &tile, const QPoint &pos)
{
    hrGraphicsItem item = itemsRoad[tile.roadId];
    item.setCurFrame(tile.roadFrame);
    item.setMirror(tile.isRoadHorizontal(), tile.isRoadVertical());
    item.setPoint(pos.x(), pos.y() + coord::dim / 2);
    addItem(item);
}

void hrAdventureScreen::loadMap(hrH3MReader *reader)
{
    Q_ASSERT(reader);
    clearMap();
    size.setSize(QSize(reader->getSize(), reader->getSize()));

    hrCache& cache = hrCache::Get();
    hasUnderground = reader->hasUnderground();
    for (int i = 0; i < size.width() * size.height(); i++)
    {
        hrTile tile = reader->getTile(i);
        tilesGround.append(tile);
        loadTile(tile);
        if (hasUnderground)
        {
            tile = reader->getTile(i, true);
            tilesUnderground.append(tile);
            loadTile(tile);
        }
    }
    for (int i = 0; i < reader->getObjectsCount(); i++)
    {
        hrSceneObject object = reader->getObject(i);

        hrGraphicsItem item;
        if (!itemsObject.contains(object.getId()))
        {
            QString name = reader->getObjectName(object.getId());
            item = cache.loadItem(name);
            itemsObject[object.getId()] = item;
        }
        else
        {
            item = itemsObject[object.getId()];
        }
        object.setFrames(item.countFrames);
        object.setSize(coord::toCell(item.getSize()));

        if (!object.isUnderground())
            objectsGround.append(object);
        else
            objectsUnderground.append(object);
    }
    qSort(objectsGround);
    qSort(objectsUnderground);

    loadCursor("cradvntr.def");
    setCursor(CURSOR_POINTER);

    animateTimer.start(200);
}

void hrAdventureScreen::loadTile(const hrTile &tile)
{
    hrCache& cache = hrCache::Get();
    if (!itemsTerrain.contains(tile.terrainId))
    {
        itemsTerrain[tile.terrainId] = cache.loadItem(tile.getTerrainName(), true);
    }
    if (tile.hasRiver() && !itemsRiver.contains(tile.riverId))
    {
        itemsRiver[tile.riverId] = cache.loadItem(tile.getRiverName(), true);
    }
    if (tile.hasRoad() && !itemsRoad.contains(tile.roadId))
    {
        itemsRoad[tile.roadId] = cache.loadItem(tile.getRoadName(), true);
    }
}

void hrAdventureScreen::switchGround(bool isUnderground)
{
    if (hasUnderground)
    {
        this->isUnderground = isUnderground;
        emit sceneChanged();
    }
}

void hrAdventureScreen::clearMap()
{
    tilesGround.clear();
    tilesUnderground.clear();
    objectsGround.clear();
    objectsUnderground.clear();
}

const hrTile& hrAdventureScreen::getTile(const QPoint &p) const
{
    Q_ASSERT(size.contains(p));

    const QVector<hrTile> &tiles = isUnderground ? tilesUnderground : tilesGround;

    return tiles.at(p.y() * size.width() + p.x());
}

int hrAdventureScreen::width() const
{
    return coord::toPix(viewport.width());
}

int hrAdventureScreen::height() const
{
    return coord::toPix(viewport.height());
}

void hrAdventureScreen::scroll()
{
    bool isScroll = false;
    if (size.contains(viewport.translated(dx, dy)))
    {
        isScroll = true;
    }
    else
    {
        if (dx)
        {
            if (size.contains(viewport.translated(dx, 0)))
            {
                dy = 0;
                isScroll = true;
            }
        }
        if (dy)
        {
            if (size.contains(viewport.translated(0, dy)))
            {
                dx = 0;
                isScroll = true;
            }
        }
    }

    if (isScroll)
    {
        viewport.translate(dx, dy);
        emit sceneChanged();
    }
}

void hrAdventureScreen::animate()
{
    isAnimate = true;
    emit sceneChanged();
}

void hrAdventureScreen::setCursor(AdvCursorType_e cursorType)
{
    static int oldType = -1;
    if (oldType != cursorType)
    {
        oldType = cursorType;
        emit cursorChaged(cursorType);
    }
}

void hrAdventureScreen::onMouseEvent(QMouseEvent *event)
{
    //qWarning("on mouse event");
    QPoint pos = event->pos();
    const int border = 50;
    bool startScrollTimer = true;

    if (pos.x() < border && pos.y() < border)
    {
        // top left
        dx = -1; dy = -1;
        setCursor(CURSOR_SCROLL_TOPLEFT);
    }
    else if (pos.x() > width() - border && pos.y() < border)
    {
        // top right
        dx = 1; dy = -1;
        setCursor(CURSOR_SCROLL_TOPRIGHT);
    }
    else if (pos.x() > width() - border && pos.y() > height() - border)
    {
        // bottom right
        dx = 1; dy = 1;
        setCursor(CURSOR_SCROLL_DOWNRIGHT);
    }
    else if (pos.x() < border && pos.y() > height() - border)
    {
        // bottom left
        dx = -1; dy = 1;
        setCursor(CURSOR_SCROLL_DOWNLEFT);
    }
    else if (pos.x() < border)
    {
        // left
        dx = -1; dy = 0;
        setCursor(CURSOR_SCROLL_LEFT);
    }
    else if (pos.x() > width() - border)
    {
        // right
        dx = 1; dy = 0;
        setCursor(CURSOR_SCROLL_RIGHT);
    }
    else if (pos.y() < border)
    {
        // up
        dx = 0; dy = -1;
        setCursor(CURSOR_SCROLL_TOP);
    }
    else if (pos.y() > height() - border)
    {
        // down
        dx = 0; dy = 1;
        setCursor(CURSOR_SCROLL_DOWN);
    }
    else
    {
        // stop
        dx = 0; dy = 0;
        startScrollTimer = false;
        setCursor(CURSOR_POINTER);
    }

    const int delay = 20;

    if (startScrollTimer)
    {
        if (!scrollTimer.isActive())
            scrollTimer.start(delay);
    }
    else
    {
        scrollTimer.stop();
    }
}

void hrAdventureScreen::onMouseLeave()
{
    scrollTimer.stop();
}

