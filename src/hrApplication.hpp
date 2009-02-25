#pragma once

#include <QApplication>
#include <QObject>

class hrApplication : public QApplication
{
    Q_OBJECT
public:
    hrApplication(int argc, char **argv);
};
