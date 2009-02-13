#include "precompiled.hpp"
#include "hrLodEngineIterator.hpp"

hrLodEngineIterator::hrLodEngineIterator (QDir::Filters filters, const QStringList &filterNames)
        : QAbstractFileEngineIterator(filters, filterNames), index(-1)
{
    qDebug() << __FUNCTION__;
}

hrLodEngineIterator::~hrLodEngineIterator()
{
    qDebug() << __FUNCTION__;
}

QString hrLodEngineIterator::next()
{
    qDebug() << __FUNCTION__;
    return "";
}

bool hrLodEngineIterator::hasNext() const
{
    qDebug() << __FUNCTION__;
    return false;
}

QString hrLodEngineIterator::currentFileName() const
{
    qDebug() << __FUNCTION__;
    return "";
}
