#include "precompiled.hpp"
#include "hrSndEngineIterator.hpp"

hrSndEngineIterator::hrSndEngineIterator (QDir::Filters filters, const QStringList &filterNames)
        : QAbstractFileEngineIterator(filters, filterNames), index(-1)
{
    qDebug() << Q_FUNC_INFO;
}

hrSndEngineIterator::~hrSndEngineIterator()
{
    qDebug() << Q_FUNC_INFO;
}

QString hrSndEngineIterator::next()
{
    qDebug() << Q_FUNC_INFO;
    return "";
}

bool hrSndEngineIterator::hasNext() const
{
    qDebug() << Q_FUNC_INFO;
    return false;
}

QString hrSndEngineIterator::currentFileName() const
{
    qDebug() << Q_FUNC_INFO;
    return "";
}
