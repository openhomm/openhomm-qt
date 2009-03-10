#include <qimageiohandler.h>
#include <qdebug.h>

class hrDefPlugin : public QImageIOPlugin
{
public:
    Capabilities capabilities(QIODevice * device, const QByteArray & format) const;
    QImageIOHandler * create(QIODevice * device, const QByteArray & format = QByteArray()) const;
    QStringList keys() const;
};

QImageIOPlugin::Capabilities hrDefPlugin::capabilities(QIODevice *device, const QByteArray &format) const
{
    Capabilities cap;
    return cap;
}

QImageIOHandler* hrDefPlugin::create(QIODevice *device, const QByteArray &format) const
{
    return NULL;
}

QStringList hrDefPlugin::keys() const
{
    return QStringList() << QLatin1String("def");
}

Q_EXPORT_STATIC_PLUGIN(hrDefPlugin)
Q_EXPORT_PLUGIN2(hrDef, hrDefPlugin)
