#ifndef BUILDERSCOMMON_H_
#define BUILDERSCOMMON_H_

#include <QString>

class QDomElement;
class QWidget;

class LCWidgetBuildersCommon
{
public:

    struct SAttributes
    {
        QString posx = "posx";
        QString posy = "posy";
        QString width = "width";
        QString height = "height";
        QString fixwidth = "fixwidth";
        QString fixheght = "fixheight";
    };
    static const SAttributes mAttributes;

private:
    explicit LCWidgetBuildersCommon() = delete;
    explicit LCWidgetBuildersCommon(const LCWidgetBuildersCommon&) = delete;
    LCWidgetBuildersCommon& operator=(const LCWidgetBuildersCommon&) = delete;

public:
   static void initPosition(const QDomElement& _element, QWidget& _widget);
   static void initSize(const QDomElement& _element, QWidget& _widget);
   static void initFixedSize(const QDomElement& _element, QWidget& _widget);
};

#endif /* BUILDERSCOMMON_H_ */
