#ifndef LCXMLBUILDERSLOADER_H
#define LCXMLBUILDERSLOADER_H

#include <QDomElement>
#include <functional>

class LCXmlBuildersLoader
{
private:
    const QString mTagRoot;
    const QString mAttrFile         = "file";
    const QString mAttrLib          = "lib";
    const QString mAttrLibHandler   = "libhandler";
public:
    LCXmlBuildersLoader(const QString& _tagRoot);
    virtual ~LCXmlBuildersLoader();
    virtual int load(const QDomElement& _element, const QString& _pathPrj, const QStringList& _libPaths) final;
    virtual bool add(const QString& _name, void* _builder) = 0;
private:
    int loadBuilders(const QDomElement& _element, const QStringList& _libPaths);
};

#endif // LCXMLBUILDERSLOADER_H
