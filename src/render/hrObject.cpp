#include "hrObject.hpp"

hrObject::hrObject(int x, int y, QString name) : x(x), y(y), name(name)
{
    point.setX(x);
    point.setY(y);
}
