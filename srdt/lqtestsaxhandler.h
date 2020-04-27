#ifndef LQTESTSAXHANDLER_H
#define LQTESTSAXHANDLER_H

#include <QXmlDefaultHandler>

class LQTestSaxHandler : public QXmlDefaultHandler
{
private:
    QString mText;
public:
    LQTestSaxHandler();
    virtual bool startElement(const QString &namespaceURI,
                              const QString &localName,
                              const QString &qName,
                              const QXmlAttributes &atts) override;
    virtual bool characters(const QString &ch) override;
    virtual bool endElement(const QString &namespaceURI, const QString &localName, const QString &qName) override;
    virtual bool fatalError(const QXmlParseException &exception) override;
//    virtual bool internalEntityDecl(const QString &name, const QString &value) override;
//    virtual bool externalEntityDecl(const QString &name, const QString &publicId, const QString &systemId) override;
//    bool startEntity(const QString &name) override;
//    bool resolveEntity(const QString &publicId, const QString &systemId, QXmlInputSource *&ret) override;
};

#endif // LQTESTSAXHANDLER_H
