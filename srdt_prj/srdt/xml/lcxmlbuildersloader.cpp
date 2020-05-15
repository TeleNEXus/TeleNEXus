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
int LCXmlBuildersLoader::load(const QDomElement& _element, const QString& _pathPrj, const QStringList& _libPaths)
{
    QString fileName = _element.attribute(mAttrFile);

    if(fileName.isNull())
    {
        return loadBuilders(_element, _libPaths);
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

    return loadBuilders(rootElement, _libPaths);
}

//----------------------------------------------------------------------------------------------------------------------
int LCXmlBuildersLoader::loadBuilders(const QDomElement& _element, const QStringList& _libPaths)
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

        //Поиск библиотеки по списку путей.
        for(int i = 0; i < _libPaths.size(); i++)
        {
            qDebug() << "lib file: " + _libPaths[i] + "/" + libfilename;
            lib.setFileName(_libPaths[i] + "/" + libfilename);

            if(lib.isLoaded())
            {
                qDebug() << "MESSAGE[builder loader]: tag=" << mTagRoot << " library "
                         << lib.fileName() << " is already loaded";
                break;
            }

            if(lib.load())
            {
                qDebug() << "MESSAGE[builder loader]: tag=" << mTagRoot << " library "
                         << lib.fileName() << " is loaded";
                break;
            }
        }

        if(!lib.isLoaded())
        {
            qDebug() << "WARNING[builder loader]: tag=" << mTagRoot << " can't load library "
                 << lib.fileName() << " to load source builder " << el.tagName();
            node = node.nextSibling();
            continue;
        }

        //Загрузка построителей объектов.
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
