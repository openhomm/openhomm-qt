#include <qimageiohandler.h>
#include <qdebug.h>

class hrPcxPlugin : public QImageIOPlugin
{
public:
    Capabilities capabilities(QIODevice * device, const QByteArray & format) const;
    QImageIOHandler * create(QIODevice * device, const QByteArray & format = QByteArray()) const;
    QStringList keys() const;
};

QImageIOPlugin::Capabilities hrPcxPlugin::capabilities(QIODevice *device, const QByteArray &format) const
{
    Capabilities cap;
    return cap;
}

QImageIOHandler* hrPcxPlugin::create(QIODevice *device, const QByteArray &format) const
{
    return NULL;
}

QStringList hrPcxPlugin::keys() const
{
    return QStringList() << QLatin1String("pcx");
}

Q_EXPORT_STATIC_PLUGIN(hrPcxPlugin)
Q_EXPORT_PLUGIN2(hrPcx, hrPcxPlugin)
