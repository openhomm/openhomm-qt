#ifndef HRCOORD_H
#define HRCOORD_H

#include "precompiled.hpp"

namespace coord
{
    int toPix(int x);
    int toCell(int x);
    QPoint toPix(QPoint p);
    QPoint toCell(QPoint p);
    QRect toPix(QRect r);
    QRect toCell(QRect r);
}

#endif // HRCOORD_H
