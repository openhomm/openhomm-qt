#ifndef HRCOORD_H
#define HRCOORD_H

#include "precompiled.hpp"

#define toPix(x) x * 32
#define toCell(x) x / 32

namespace coord
{
    //int toPix(int x);
    //int toCell(int x);
    QPoint toPixPoint(const QPoint &p);
    QPoint toCellPoint(const QPoint &p);
    QRect toPixRect(const QRect &r);
    QRect toCellRect(const QRect &r);
}

#endif // HRCOORD_H
