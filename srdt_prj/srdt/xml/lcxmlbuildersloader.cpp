#include "lcxmlbuildersloader.h"
#include <QLibrary>
#include <QDebug>
#include <QFile>

//======================================================================================================================
using TLibAccessFunc = void* (*)();

//======================================================================================================================
LCXmlBuildersLoader::LCXmlBuildersLoader(const QString& _tagRoot,
                                         const QString& _attrFile,
                                         const QString& _attrLib,
                                         const QString& _attrLibHandler) :  mTagRoot(_tagRoot),
                                                                            mAttrFile(_attrFile),
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
        return loadBuilders(_element, _pathLib);
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

    return loadBuilders(rootElement, _pathLib);
}

//----------------------------------------------------------------------------------------------------------------------
int LCXmlBuildersLoader::loadBuilders(const QDomElement& _element, const QString& _libPath)
{
    int loadCounter = 0;

    QDomNode node = _element.firstChild();

    while(!node.isNull())
    {
        if(!node.isElement())
        {
            node = node.nextSibling();
            continue;
        }

        QDomElement el = node.toElement();

        QString libhandler = el.attribute(mAttrLibHandler);
        QString libfilename  = el.attribute(mAttrLib);

        if(libhandler.isNull() || libfilename.isNull())
        {
            node = node.nextSibling();
            continue;
        }

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
                        qDebug() << "WARNING[builder loader]: tag=" << mTagRoot << " can't load library "
                                 << _libPath + libfilename << " to load source builder " << el.tagName();
                        node = node.nextSibling();
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
                     << lib.fileName() << " to load remote source builder" << el.tagName();
            node = node.nextSibling();
            continue;
        }

        if(add(el.tagName(), func()))
        {
            loadCounter++;
            qDebug() << "MESSAGE[builder loader]: tag=" << mTagRoot << " load builder name: "
                     << el.tagName() << " access func: " << libhandler;
        }

        loadCounter++;
        node = node.nextSibling();
    }

    return loadCounter;
}
