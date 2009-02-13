#pragma once

class hrLodEngineIterator : public QAbstractFileEngineIterator
{
public:
    hrLodEngineIterator(QDir::Filters filters, const QStringList &filterNames);
    ~hrLodEngineIterator();

    QString next();
    bool hasNext() const;
    QString currentFileName() const;

private:
    QStringList entries;
    int index;
};
