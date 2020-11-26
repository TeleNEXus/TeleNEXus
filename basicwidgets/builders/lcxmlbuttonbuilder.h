#ifndef LCXMLBUTTONBUILDER_H__
#define LCXMLBUTTONBUILDER_H__

#include "lcxmlbuilderbase.h"

class LCXmlButtonBuilder : public LCXmlBuilderBase
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

protected:
  virtual QWidget* buildLocal(
      QSharedPointer<SBuildData> _buildData) override final;
};

#endif 
