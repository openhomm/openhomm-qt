#include "precompiled.hpp"
#include "hrApplication.hpp"
#include "hrPushButton.hpp"


hrApplication::hrApplication(int argc, char **argv): QApplication(argc, argv)
{
    QFile f("lod:/blabla/blabla.txt");
    f.open(QIODevice::ReadOnly);
}
