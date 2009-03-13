#pragma once

#include <QtGui/qimageiohandler.h>
#include <QtCore/QSize>
#include <QtGlobal>
#include <QColor>

class hrPcxHandler : public QImageIOHandler
{
public:
    hrPcxHandler();

    bool canRead() const;
    bool read(QImage *image);
    bool write(const QImage &image);

    QByteArray name() const;

    static bool canRead(QIODevice *device);

    QVariant option(ImageOption option) const;
    void setOption(ImageOption option, const QVariant &value);
    bool supportsOption(ImageOption option) const;
private:
    void readPallete();
private:
    QVector<QRgb> pallete;
};
