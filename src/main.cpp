#include "precompiled.hpp"
#include "hrApplication.hpp"
#include "hrLodEngineIterator.hpp"
#include "hrLodEngine.hpp"

hrLodEngineHandler handler;

int main(int argc, char** argv)
{
    hrApplication app(argc, argv);

    QFile aa("lod:/my/1.png");
    aa.open(QIODevice::ReadOnly);

    return app.exec();
}
