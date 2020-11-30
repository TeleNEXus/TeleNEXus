#ifndef LCXMLCOMMON_H_
#define LCXMLCOMMON_H_

#include <QString>

class LCXmlCommon
{
private:
  LCXmlCommon() = delete;
  LCXmlCommon(const LCXmlCommon&) = delete;
  LCXmlCommon& operator=(const LCXmlCommon&) = delete;

public:

  struct SBaseTags
  {
    QString rootTag         = "APPLICATION";
    QString window          = "WINDOW";
    QString sourceBuilders  = "SOURCEBUILDERS";
    QString widgetBuilders  = "WIDGETBUILDERS";
    QString layoutBuilders  = "LAYOUTBUILDERS";
    QString fonts           = "FONTS";
    QString sources         = "SOURCES";
    QString plaginpath      = "PLAGINPATHS";
    QString scripts         = "SCRIPTS";
  };

  struct SCommonTags
  {
    QString item  = "item";
    QString script = "script";
  };

  struct SCommonAttributes
  {
    QString file      = "file";
    QString path      = "path";
    QString id        = "id";
    QString title     = "title";
    QString widht     = "width";
    QString height    = "height";
    QString posx      = "posx";
    QString posy      = "posy";
    QString interval  = "interval";

    struct
    {
      QString tag = "show";
      struct
      {
        QString yes = "yes";
        QString no  = "no";
      }vals;
    }show;
  };

  static const SBaseTags          mBaseTags;
  static const SCommonTags        mCommonTags;
  static const SCommonAttributes  mCommonAttributes;

};
#endif

