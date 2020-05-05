#ifndef LCXMLSOURCEBUILDERLOADER_H
#define LCXMLSOURCEBUILDERLOADER_H

#include <QSharedPointer>
#include <QDomElement>

class LIXmlRemoteDataSourceCreator;

class LCXmlRemoteSourceBuilders
{
public:

private:
    LCXmlRemoteSourceBuilders();
    LCXmlRemoteSourceBuilders(const LCXmlRemoteSourceBuilders&) = delete;
    LCXmlRemoteSourceBuilders& operator=(const LCXmlRemoteSourceBuilders&) = delete;
public:
    static LCXmlRemoteSourceBuilders& instance();
    QSharedPointer<LIXmlRemoteDataSourceCreator> getBuilder(const QString _name);
    int load(const QDomElement& _element);
};

#endif // LCXMLSOURCEBUILDERLOADER_H
