#include "lcxmlbuildersloader.h"
#include <QLibrary>
#include <QDebug>
#include <QFile>

//======================================================================================================================
using TLibAccessFunc = void* (*)();

//======================================================================================================================
LCXmlBuildersLoader::LCXmlBuildersLoader(const QString& _tagRoot,
                                         const QString& _tagBuilder,
                                         const QString& _attrFile,
                                         const QString& _attrName,
                                         const QString& _attrLib,
                                         const QString& _attrLibHandler) :  mTagRoot(_tagRoot),
                                                                            mTagBuilder(_tagBuilder),
                                                                            mAttrFile(_attrFile),
                                                                            mAttrName(_attrName),
                                                                            mAttrLib(_attrLib),
                                                                            mAttrLibHandler(_attrLibHandler)
{

}
//----------------------------------------------------------------------------------------------------------------------
LCXmlBuildersLoader::~LCXmlBuildersLoader(){}
//----------------------------------------------------------------------------------------------------------------------
int LCXmlBuildersLoader::load(const QDomElement& _element, const QString& _pathPrj, const QString& _pathLib)
{
    QString fileName = _element.attribute(mAttrFile);

    if(fileName.isNull())
    {
        return loadBuilders(_element.elementsByTagName(mTagBuilder), _pathLib);
    }

    fileName = _pathPrj + "/" + fileName;

    QFile file(fileName);
    QDomDocument domDoc;
    QString errorStr;
    int errorLine;
    int errorColumn;

    if(!domDoc.setContent(&file, true, &errorStr, &errorLine, &errorColumn))
    {
        qDebug() << "ERROR[builder loader]: tag=" << mTagRoot << " load parse file "<< fileName << " error at line:"
                 << errorLine << " column:" << errorColumn << " msg: " << errorStr;
        return 0;
    }

    qDebug() << "MESSAGE[builder loader]: tag=" << mTagRoot << " parce file " << fileName;

    QDomElement rootElement = domDoc.documentElement();

    if(rootElement.tagName() != mTagRoot)
    {
        qDebug() << "ERROR[builder loader "+ mTagRoot + "]:file "<< fileName << " wrong root element tag name";
        return 0;
    }

    return loadBuilders(rootElement.elementsByTagName(mTagBuilder), _pathLib);
}

//----------------------------------------------------------------------------------------------------------------------
int LCXmlBuildersLoader::loadBuilders(const QDomNodeList& _nodeList, const QString& _libPath)
{
    int loadCounter = 0;

    for(int i = 0; i < _nodeList.size(); i++)
    {

        QDomElement el = _nodeList.at(i).toElement();

        QString name = el.attribute(mAttrName);
        if(name.isNull()) continue;

        QString libhandler = el.attribute(mAttrLibHandler);
        if(libhandler.isNull()) continue;


        QString libfilename  = el.attribute(mAttrLib);
        if(libfilename.isNull()) continue;


        QLibrary lib;

        lib.setFileName(libfilename);

        if(!lib.isLoaded())
        {
            if(!lib.load())
            {
                lib.setFileName(_libPath + libfilename);
                if(!lib.isLoaded())
                {
                    if(!lib.load())
                    {
                        qDebug() << "WARNING[builder loader]: tag=" << mTagRoot << " can't load library " << libfilename
                        << " to load source builder " << name;
                        continue;
                    }
                    else
                    {
                        qDebug() << "MESSAGE[builder loader]: tag=" << mTagRoot << " library "
                                 << lib.fileName() << " is loaded";
                    }
                 }
                 else
                 {
                     qDebug() << "MESSAGE[builder loader]:library "
                              << lib.fileName() << " is already loaded";
                 }
            }
            else
            {
                qDebug() << "MESSAGE[builder loader]: tag=" << mTagRoot << " library "
                         << lib.fileName() << " is loaded";
            }
        }
        else
        {
            qDebug() << "MESSAGE[builder loader]: tag=" << mTagRoot << " library "
                     << lib.fileName() << " is already loaded";
        }

        TLibAccessFunc func = (TLibAccessFunc)lib.resolve(libhandler.toStdString().c_str());

        if(!func)
        {
            qDebug() << "WARNING[builder loader]: tag=" << mTagRoot << " can't resolve access func "
                     << libhandler << " in library "
                     << lib.fileName() << " to load remote source builder" << name;
            continue;
        }

        if(add(name, func()))
        {
            loadCounter++;
            qDebug() << "MESSAGE[builder loader]: tag=" << mTagRoot << " load builder name: "
                     << name << " access func: " << libhandler;
        }
    }

    return loadCounter;
}
