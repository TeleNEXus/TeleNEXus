#ifndef LCXMLWINDOWS_H_
#define LCXMLWINDOWS_H_

#include <QSharedPointer>

class QDomElement;
class LIApplication;
class LIWindow;
class LCXmlWindows
{
private:
    LCXmlWindows();
    ~LCXmlWindows();
    LCXmlWindows(const LCXmlWindows&) = delete;
    LCXmlWindows& operator=(const LCXmlWindows&) = delete;
public:
    static LCXmlWindows& instance();
    void create(const QDomElement& _element);
    QSharedPointer<LIWindow> getWindow();
};
#endif /* LCXMLWINDOWS_H_ */
