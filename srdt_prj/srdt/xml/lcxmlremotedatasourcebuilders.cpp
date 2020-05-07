#include "lcxmlremotedatasourcebuilders.h"
#include "lcxmlapplication.h"

#include <QMap>
#include <QLibrary>
#include <QCoreApplication>
#include <QDebug>

//======================================================================================================================
using LQSourceBuildersMap = QMap<QString, QSharedPointer<LIXmlRemoteDataSourceBuilder>>;

using TLibAccessFunc = void* (*)();

//======================================================================================================================
static LQSourceBuildersMap __buildersMap;

//======================================================================================================================
LCXmlRemoteDataSourceBuilders::LCXmlRemoteDataSourceBuilders()
{

}

//----------------------------------------------------------------------------------------------------------------------
LCXmlRemoteDataSourceBuilders& LCXmlRemoteDataSourceBuilders::instance()
{
    static LCXmlRemoteDataSourceBuilders inst;
    return inst;
}

//----------------------------------------------------------------------------------------------------------------------
QSharedPointer<LIXmlRemoteDataSourceBuilder> LCXmlRemoteDataSourceBuilders::getBuilder(const QString _name)
{

    LQSourceBuildersMap::iterator it = __buildersMap.find(_name);
    if(it != __buildersMap.end())
    {
        return it.value();
    }

    return QSharedPointer<LIXmlRemoteDataSourceBuilder>();
}

//----------------------------------------------------------------------------------------------------------------------
static int loadBuilders(const QDomNodeList& nodeList);
//----------------------------------------------------------------------------------------------------------------------
int LCXmlRemoteDataSourceBuilders::load(const QDomElement& _element)
{
    QString fileName = _element.attribute(LCXmlApplication::mBaseAttributeNames.file);

    if(fileName.isNull())
    {
        return loadBuilders(_element.elementsByTagName(LCXmlApplication::mBaseTagNames.builder));
    }

    fileName = LCXmlApplication::instance().getXmlMainFileWay() + fileName;

    QFile file(fileName);
    QDomDocument domDoc;
    QString errorStr;
    int errorLine;
    int errorColumn;

    if(!domDoc.setContent(&file, true, &errorStr, &errorLine, &errorColumn))
    {
        qDebug() << "ERROR[Remote source bulder]: load parse file "<< fileName << " error at line:"
                 << errorLine << " column:" << errorColumn << " msg: " << errorStr;
        return 0;
    }

    qDebug() << "MESSAGE[Remote source bulder]: " << "parce file " << fileName;

    QDomElement rootElement = domDoc.documentElement();
    if(rootElement.tagName() != LCXmlApplication::mBaseTagNames.sourceBuilders)
    {
        qDebug() << "ERROR[Remote source bulder]: file "<< fileName << "wrong root element tag name";
    }

    return loadBuilders(rootElement.elementsByTagName(LCXmlApplication::mBaseTagNames.builder));
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

        if(__buildersMap.find(name) != __buildersMap.end())
        {
            qDebug() << "WARNING[Remote source bulder]: duplicate remode data source builder name: " << name;
            continue;
        }

        QString libhandler = el.attribute(LCXmlApplication::mBaseAttributeNames.libhandler);
        if(libhandler.isNull()) continue;


        QString libfilename  = el.attribute(LCXmlApplication::mBaseAttributeNames.lib);
        if(libfilename.isNull()) continue;


        QLibrary lib;

        lib.setFileName(libfilename);

        if(!lib.isLoaded())
        {
            if(!lib.load())
            {
                lib.setFileName(LCXmlApplication::instance().getXmlMainFileWay() + libfilename);
                if(!lib.isLoaded())
                {
                    if(!lib.load())
                    {
                        lib.setFileName(LCXmlApplication::instance().getApplicationDirPath() + libfilename);
                        if(!lib.isLoaded())
                        {
                           if(!lib.load())
                           {
                               qDebug() << "WARNING[Remote source bulder]: can't load library " << libfilename
                                        << " to load source builder " << name;

                               continue;
                           }
                           else
                           {
                               qDebug() << "MESSAGE[Remote source bulder]: library " << lib.fileName() << " is loaded";
                           }
                        }
                        else
                        {
                            qDebug() << "MESSAGE[Remote source bulder]: library " << lib.fileName() << " is already loaded";
                        }

                    }
                    else
                    {
                        qDebug() << "MESSAGE[Remote source bulder]: library " << lib.fileName() << " is loaded";
                    }
                }
                else
                {
                    qDebug() << "MESSAGE[Remote source bulder]: library " << lib.fileName() << " is already loaded";
                }
            }
            else
            {
                qDebug() << "MESSAGE[Remote source bulder]: library " << lib.fileName() << " is loaded";
            }
        }
        else
        {
            qDebug() << "MESSAGE[Remote source bulder]: library " << lib.fileName() << " is already loaded";
        }

        TLibAccessFunc func = (TLibAccessFunc)lib.resolve(libhandler.toStdString().c_str());

        if(!func)
        {
            qDebug() << "WARNING[Remote source bulder]: can't resolve access func " << libhandler << " in library "
                     << lib.fileName() << " to load remote source builder" << name;
            continue;
        }

        QSharedPointer<LIXmlRemoteDataSourceBuilder> creator((LIXmlRemoteDataSourceBuilder*)func());
        __buildersMap.insert(name, creator);
        qDebug() << "MESSAGE[Remote source bulder]: load builder name: " << name << " access func: " << libhandler;
        loadCounter++;
    }

    return loadCounter;

}

