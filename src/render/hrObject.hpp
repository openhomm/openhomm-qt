#ifndef HROBJECT_H
#define HROBJECT_H

#include "precompiled.hpp"

class hrObject
{
public:
    hrObject(int x, int y, QString name);
    int x;
    int y;
    QPoint point;
    QString name;
};

#endif // HROBJECT_H
