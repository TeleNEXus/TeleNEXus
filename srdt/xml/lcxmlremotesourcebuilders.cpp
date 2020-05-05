#include "lcxmlremotesourcebuilders.h"
#include "lcxmlapplication.h"

#include <QMap>
#include <QLibrary>
#include <QCoreApplication>
#include <QDebug>

//======================================================================================================================
using LQSourceBuildersMap = QMap<QString, QSharedPointer<LIXmlRemoteDataSourceCreator>>;

using TLibAccessFunc = void* (*)();

//======================================================================================================================
static LQSourceBuildersMap __buildersMap;

//======================================================================================================================
LCXmlRemoteSourceBuilders::LCXmlRemoteSourceBuilders()
{

}

//----------------------------------------------------------------------------------------------------------------------
LCXmlRemoteSourceBuilders& LCXmlRemoteSourceBuilders::instance()
{
    static LCXmlRemoteSourceBuilders inst;
    return inst;
}

//----------------------------------------------------------------------------------------------------------------------
QSharedPointer<LIXmlRemoteDataSourceCreator> LCXmlRemoteSourceBuilders::getBuilder(const QString _name)
{

    LQSourceBuildersMap::iterator it = __buildersMap.find(_name);
    if(it != __buildersMap.end())
    {
        return it.value();
    }

    return QSharedPointer<LIXmlRemoteDataSourceCreator>();
}

//----------------------------------------------------------------------------------------------------------------------
static int loadBuilders(const QDomNodeList& nodeList);
//----------------------------------------------------------------------------------------------------------------------
int LCXmlRemoteSourceBuilders::load(const QDomElement& _element)
{
    QString fileName = _element.attribute(LCXmlApplication::mBaseAttributeNames.file);
    if(fileName.isNull())
    {
        return loadBuilders(_element.elementsByTagName(LCXmlApplication::mBaseTagNames.builder));
    }

    fileName = LCXmlApplication::instance().getXmlMainFileWay() + "/" + fileName;

    QFile file(fileName);
    QDomDocument domDoc;
    QString errorStr;
    int errorLine;
    int errorColumn;

    if(!domDoc.setContent(&file, true, &errorStr, &errorLine, &errorColumn))
    {
        qDebug() << "Remote source bulderd load parse file "<< fileName << " error at line:"
                 << errorLine << " column:" << errorColumn << " msg: " << errorStr;
        return 0;
    }

    QDomElement rootElement = domDoc.documentElement();


}

//----------------------------------------------------------------------------------------------------------------------
static int loadBuilders(const QDomNodeList& _nodeList)
{
    int loadCounter = 0;

    for(int i = 0; i < _nodeList.size(); i++)
    {

        QDomElement el = _nodeList.at(i).toElement();

        QString name = el.attribute(LCXmlApplication::mBaseAttributeNames.name);
        if(name.isNull()) continue;
        if(__buildersMap.find(name) != __buildersMap.end()) continue;

        QString libhandler = el.attribute(LCXmlApplication::mBaseAttributeNames.libhandler);
        if(libhandler.isNull()) continue;


        QString libfilename  = el.attribute(LCXmlApplication::mBaseAttributeNames.lib);
        if(libfilename.isNull()) continue;

        QString libfile = libfilename;

        if(!QLibrary::isLibrary(libfile))
        {
            libfile = LCXmlApplication::instance().getXmlMainFileWay() + "/" + libfilename;
            if(!QLibrary::isLibrary(libfile))
            {
                libfile = LCXmlApplication::instance().getApplicationDirPath() + "/" + libfilename;
                if(!QLibrary::isLibrary(libfile))
                {
                    continue;
                }
            }
        }

        QLibrary lib(libfile);

        if(!lib.isLoaded())
        {
            if(!lib.load())
            {
                continue;
            }
        }

        TLibAccessFunc func = (TLibAccessFunc)lib.resolve(libhandler.toStdString().c_str());

        if(!func) continue;

        QSharedPointer<LIXmlRemoteDataSourceCreator> creator((LIXmlRemoteDataSourceCreator*)func());
        __buildersMap.insert(name, creator);

        loadCounter++;
    }

    return loadCounter;

}
