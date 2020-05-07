#ifndef LCXMLBUILDERSLOADER_H
#define LCXMLBUILDERSLOADER_H

#include <QDomElement>
#include <functional>

class LCXmlBuildersLoader
{
private:
    const QString mTagRoot;
    const QString mTagBuilder;
    const QString mAttrFile;
    const QString mAttrName;
    const QString mAttrLib;
    const QString mAttrLibHandler;
public:
    LCXmlBuildersLoader(    const QString& _tagRoot,
                            const QString& _tagBuilder,
                            const QString& _attrFile,
                            const QString& _attrName,
                            const QString& _attrLib,
                            const QString& _attrLibHandler);
    virtual ~LCXmlBuildersLoader();
    virtual int load(const QDomElement& _element, const QString& _pathPrj, const QString& _pathLib) final;
    virtual bool add(const QString& _name, void* _builder) = 0;
private:
    int loadBuilders(const QDomNodeList& _nodeList, const QString& _libPath);
};

#endif // LCXMLBUILDERSLOADER_H
