#ifndef LCXMLSOURCEBUILDERLOADER_H
#define LCXMLSOURCEBUILDERLOADER_H

#include <QSharedPointer>
#include <QDomElement>

class LIXmlRemoteDataSourceBuilder;

class LCXmlRemoteDataSourceBuilders
{
public:

private:
    LCXmlRemoteDataSourceBuilders();
    LCXmlRemoteDataSourceBuilders(const LCXmlRemoteDataSourceBuilders&) = delete;
    LCXmlRemoteDataSourceBuilders& operator=(const LCXmlRemoteDataSourceBuilders&) = delete;
public:
    static LCXmlRemoteDataSourceBuilders& instance();
    QSharedPointer<LIXmlRemoteDataSourceBuilder> getBuilder(const QString _name);
    int load(const QDomElement& _element);
};

#endif // LCXMLSOURCEBUILDERLOADER_H
