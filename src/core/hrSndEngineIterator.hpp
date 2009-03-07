#pragma once

class hrSndEngineIterator : public QAbstractFileEngineIterator
{
public:
    hrSndEngineIterator(QDir::Filters filters, const QStringList &filterNames);
    ~hrSndEngineIterator();

    QString next();
    bool hasNext() const;
    QString currentFileName() const;

private:
    QStringList entries;
    int index;
};
