#include <qimageiohandler.h>
#include <qdebug.h>

#include "hrDefHandler.hpp"

class hrDefPlugin : public QImageIOPlugin
{
public:
    Capabilities capabilities(QIODevice * device, const QByteArray & format) const;
    QImageIOHandler * create(QIODevice * device, const QByteArray & format = QByteArray()) const;
    QStringList keys() const;
};

QImageIOPlugin::Capabilities hrDefPlugin::capabilities(QIODevice *device, const QByteArray &format) const
{
    if (format == "def")
        return CanRead;
    if (!format.isEmpty())
        return 0;
    if (!device->isOpen())
        return 0;

    Capabilities cap;

    if ( device->isReadable() && hrDefHandler::canRead(device) )
        cap |= CanRead;

    return cap;
}

QImageIOHandler* hrDefPlugin::create(QIODevice *device, const QByteArray &format) const
{
    QImageIOHandler *defHandler = new hrDefHandler();
    defHandler->setDevice(device);
    defHandler->setFormat(format);
    return defHandler;
}

QStringList hrDefPlugin::keys() const
{
    return QStringList() << QLatin1String("def");
}

Q_EXPORT_STATIC_PLUGIN(hrDefPlugin)
Q_EXPORT_PLUGIN2(hrDef, hrDefPlugin)
