#include "lqtestsaxhandler.h"
#include <QDebug>

/*
 * elements names
 * MainWindow QWid
 */

LQTestSaxHandler::LQTestSaxHandler()
{

}

bool LQTestSaxHandler::startElement(const QString &namespaceURI,
                          const QString &localName,
                          const QString &qName,
                          const QXmlAttributes &attrs)
{
    Q_UNUSED(namespaceURI);
    Q_UNUSED(localName);
    Q_UNUSED(qName);

    for(int i = 0; i < attrs.count(); i++) {
        if(attrs.localName(i) == "number") {
            qDebug() << "Attr:" << attrs.value(i);
        }
    }
    return true;

}

bool LQTestSaxHandler::characters(const QString &ch)
{
    mText = ch;
    return true;
}

bool LQTestSaxHandler::endElement(const QString &namespaceURI, const QString &localName, const QString &qName)
{
    if (qName != "contact" && qName != "addressbook")
    qDebug () << "TagName:" << qName<< "\tText:" << mText;
    return true;
}

bool LQTestSaxHandler::fatalError(const QXmlParseException &exception)
{
    qDebug()    << "Line:"      << exception.lineNumber()
                << ", Column:"  << exception.columnNumber()
                << ", Message:" << exception.message();
    return false;
}

//bool LQTestSaxHandler::internalEntityDecl(const QString &name, const QString &value)
//{
//    qDebug() << "internalEntityDecl";
//    return true;
//}

//bool LQTestSaxHandler::externalEntityDecl(const QString &name, const QString &publicId, const QString &systemId)
//{
//    qDebug() << "externalEntityDecl";
//    return true;
//}

//bool LQTestSaxHandler::startEntity(const QString &name)
//{
//    qDebug() << "startEntity";
//    return true;
//}

//bool LQTestSaxHandler::resolveEntity(const QString &publicId, const QString &systemId, QXmlInputSource *&ret)
//{
//    qDebug() << "resolveEntity";
//    return true;
//}
