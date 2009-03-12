#include <qimageiohandler.h>
#include <qdebug.h>

#include "hrPcxHandler.hpp"

class hrPcxPlugin : public QImageIOPlugin
{
public:
    Capabilities capabilities(QIODevice * device, const QByteArray & format) const;
    QImageIOHandler * create(QIODevice * device, const QByteArray & format = QByteArray()) const;
    QStringList keys() const;
};

QImageIOPlugin::Capabilities hrPcxPlugin::capabilities(QIODevice *device, const QByteArray &format) const
{
    if (format == "pcx")
        return Capabilities(CanRead | CanWrite);
    if (!format.isEmpty())
        return 0;
    if (!device->isOpen())
        return 0;

    Capabilities cap;

    if ( device->isReadable() && hrPcxHandler::canRead(device) )
        cap |= CanRead;

    return cap;
}

QImageIOHandler* hrPcxPlugin::create(QIODevice *device, const QByteArray &format) const
{
    QImageIOHandler *pcxHandler = new hrPcxHandler();
    pcxHandler->setDevice(device);
    pcxHandler->setFormat(format);
    return pcxHandler;
}

QStringList hrPcxPlugin::keys() const
{
    return QStringList() << QLatin1String("pcx");
}

Q_EXPORT_STATIC_PLUGIN(hrPcxPlugin)
Q_EXPORT_PLUGIN2(hrPcx, hrPcxPlugin)
