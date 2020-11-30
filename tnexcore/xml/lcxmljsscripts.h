#ifndef LCXMLJSCRIPTS_H_ 
#define LCXMLJSCRIPTS_H_

#include <QSharedPointer>

class QDomElement;
class LIApplication;
class LIJScript;

class LCXmlJScripts
{
private:
    LCXmlJScripts();
    ~LCXmlJScripts();
    LCXmlJScripts(const LCXmlJScripts&) = delete;
    LCXmlJScripts& operator=(const LCXmlJScripts&) = delete;
public:
    static LCXmlJScripts& instance();
    void load(
        const QDomElement &_element, 
        const LIApplication& _app);

    QSharedPointer<LIJScript> getScript(const QString& _scriptId);
};
#endif /* LCXMLWINDOWS_H_ */
