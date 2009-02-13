#pragma once

class hrString3
{
public:
    hrString3();
    hrString3(const char* str);
    hrString3(const hrString3& str);
    //hrString3(const QString& str);
    ~hrString3();

    static void serialize(QIODevice *device);
    static void desrialize(QIODevice *device);

// data
protected:
    char * _str;
    size_t size;
};
