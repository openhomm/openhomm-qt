#include "precompiled.hpp"
#include "hrApplication.hpp"
#include "hrLodEngineIterator.hpp"
#include "hrLodEngine.hpp"
#include "hrSndEngineIterator.hpp"
#include "hrSndEngine.hpp"

hrLodEngineHandler lodHandler;
hrSndEngineHandler sndHandler;

int main(int argc, char** argv)
{
    QT_REQUIRE_VERSION(argc, argv, "4.5.0");
    hrApplication app(argc, argv);

    return app.exec();
}
