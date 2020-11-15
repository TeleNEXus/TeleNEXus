#ifndef BUILDERSCOMMON_H_
#define BUILDERSCOMMON_H_

#include <QString>
#include <QSharedPointer>
#include <qnamespace.h>

class QDomElement;
class QWidget;
class QMovie;
class LIApplication;
class LIMovieAccess;


class LCWidgetBuildersCommon
{
public:

    struct SAttributes
    {
        QString posx        = "posx";
        QString posy        = "posy";
        QString width       = "width";
        QString height      = "height";
        QString fixwidth    = "fixwidth";
        QString fixheght    = "fixheight";
        QString colorbg     = "bgcolor";
        QString colortext   = "textcolor";
        QString fontname    = "fontname";
        QString fontsize    = "fontsize";
        QString fontId      = "fontId";

        struct
        {
            const QString attrName = "align";
            struct
            {
                QString Left    = "Left";
                QString Right   = "Right";
                QString Center  = "Center";
                QString HCenter = "HCenter";
                QString Top     = "Top";
                QString Bottom  = "Bottom";
                QString VCenter = "VCenter";
            }vals;
        }aligns;
    };
    static const SAttributes mAttributes;

private:
    static LCWidgetBuildersCommon instance; 
    explicit LCWidgetBuildersCommon();
    explicit LCWidgetBuildersCommon(const LCWidgetBuildersCommon&) = delete;
    LCWidgetBuildersCommon& operator=(const LCWidgetBuildersCommon&) = delete;

public:
   static void initPosition(const QDomElement& _element, QWidget& _widget);
   static void initSize(const QDomElement& _element, QWidget& _widget);
   static void initFixedSize(const QDomElement& _element, QWidget& _widget);

   static QSharedPointer<LIMovieAccess> getMovie(const QString& _movie, 
           const LIApplication& _app);
   static QPixmap getPixmap(const QString& _pixmap, const LIApplication& _app);
   static bool toAlignFlags(const QString& _attributes, Qt::Alignment& _flags);
   static bool toAlignFlags(const QDomElement& _element, Qt::Alignment& _flags);
   static QColor attributeToColor(const QString& _attr_color);
};

#endif /* BUILDERSCOMMON_H_ */
