#ifndef LCXMLBUTTONBUILDER_H__
#define LCXMLBUTTONBUILDER_H__

#include "LIXmlWidgetBuilder.h"

#include <QString>

class LCXmlButtonBuilder : public LIXmlWidgetBuilder
{
public:

  //Тип кнопки.
  enum class EType
  {
    writeData,      //Кнопка записи данных.
    controlWindows  //Кнопка управления окнами.
  };
private:
  EType mType;

public:
  LCXmlButtonBuilder() = delete;
  explicit LCXmlButtonBuilder(EType _type = EType::writeData);
  ~LCXmlButtonBuilder();
  virtual QWidget* build( const QDomElement& _element, 
      const LIApplication& _app) override;
};

#endif 
