#include "LIXmlWidgetBuilder.h"

#include <QString>

class LCXmlRemWriteButtonBuilder : public LIXmlWidgetBuilder
{
private:

  struct SAttributes
  {
    QString text = "text";
  };

  struct STags
  {
    struct
    {
      QString tagsName = "data";
      struct
      {
        QString source = "sourceName";
        QString dataName = "dataName";
        QString value   = "value";
      }attributes;
    }data;
  };

  static const SAttributes mAttributes;
  static const STags mTags;

public:
  LCXmlRemWriteButtonBuilder();
  ~LCXmlRemWriteButtonBuilder();
  virtual QWidget* build( const QDomElement& _element, 
      const LIApplication& _app) override;

};
