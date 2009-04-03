#include "hrCoord.hpp"


/*int coord::toPix(int x)
{
    return x * 32;
}
int coord::toCell(int x)
{
    return x / 32;
}*/
QPoint coord::toPixPoint(const QPoint &p)
{
    return QPoint(toPix(p.x()), toPix(p.y()));
}
QPoint coord::toCellPoint(const QPoint &p)
{
    return QPoint(toCell(p.x()), toCell(p.y()));
}
QRect coord::toPixRect(const QRect &r)
{
    return QRect(toPix(r.x()), toPix(r.y()), toPix(r.width()), toPix(r.height()));
}
QRect coord::toCellRect(const QRect &r)
{
    return QRect(toCell(r.x()), toCell(r.y()), toCell(r.width()), toCell(r.height()));
}

