#include "hrCoord.hpp"


int coord::toPix(int x)
{
    return x * 32;
}
int coord::toCell(int x)
{
    return x / 32;
}
QPoint coord::toPix(QPoint p)
{
    return QPoint(toPix(p.x()), toPix(p.y()));
}
QPoint coord::toCell(QPoint p)
{
    return QPoint(toCell(p.x()), toCell(p.y()));
}
QRect coord::toPix(QRect r)
{
    return QRect(toPix(r.x()), toPix(r.y()), toPix(r.width()), toPix(r.height()));
}
QRect coord::toCell(QRect r)
{
    return QRect(toCell(r.x()), toCell(r.y()), toCell(r.width()), toCell(r.height()));
}
QRect coord::toPixRectSize(QRect r)
{
    return QRect(0, 0, toPix(r.width()), toPix(r.height()));
}
QRect coord::toCellRectSize(QRect r)
{
    return QRect(0, 0, toCell(r.width()), toCell(r.height()));
}
