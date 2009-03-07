#include "precompiled.hpp"
#include "hrLodEngineIterator.hpp"

hrLodEngineIterator::hrLodEngineIterator (QDir::Filters filters, const QStringList &filterNames)
        : QAbstractFileEngineIterator(filters, filterNames), index(-1)
{
    qDebug() << Q_FUNC_INFO;
}

hrLodEngineIterator::~hrLodEngineIterator()
{
    qDebug() << Q_FUNC_INFO;
}

QString hrLodEngineIterator::next()
{
    qDebug() << Q_FUNC_INFO;
    return "";
}

bool hrLodEngineIterator::hasNext() const
{
    qDebug() << Q_FUNC_INFO;
    return false;
}

QString hrLodEngineIterator::currentFileName() const
{
    qDebug() << Q_FUNC_INFO;
    return "";
}
