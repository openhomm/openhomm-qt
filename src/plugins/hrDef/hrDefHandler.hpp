#pragma once

#include <qimageiohandler.h>

class QImage;
class QByteArray;
class QIODevice;
class QVariant;
class DefReader;

class hrDefHandler : public QImageIOHandler
{
public:
    hrDefHandler(QIODevice *device);
    ~hrDefHandler();

    virtual bool canRead() const;
    virtual bool read(QImage *image);
    virtual bool write(const QImage &image);
    virtual int currentImageNumber() const;
    virtual int imageCount() const;
    virtual bool jumpToImage(int imageNumber);
    virtual bool jumpToNextImage();
    virtual int loopCount() const;
    virtual int nextImageDelay() const;
    static bool canRead(QIODevice *device);

    QVariant option(ImageOption option) const;
    void setOption(ImageOption option, const QVariant &value);
    bool supportsOption(ImageOption option) const;

    virtual QByteArray name() const;

private:
    DefReader *dr;
};

