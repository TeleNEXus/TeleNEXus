/* 
 * TeleNEXus is a simple SCADA programm
 *
 * Copyright (C) 2020 Sergey S. Kuzmenko
 *
 * This file is part of TeleNEXus.
 *
 * TeleNEXus is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * TeleNEXus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with TeleNEXus.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "lcxmldatacombolabelbuilder.h"
#include "widgets/lcqdatacombolabel.h"
#include "LIApplication.h"
#include "LIMovieAccess.h"
#include "xmlcommon.h"

#include <QDomElement>
#include <qdom.h>
#include <qnamespace.h>
#include <qwidget.h>
#include <QLabel>
#include <QDebug>

//==============================================================================
class CQMovieLabel : public QLabel
{
private:
  QSharedPointer<LIMovieAccess> mspMovieAccess;
public:
  explicit CQMovieLabel(
      QSharedPointer<LIMovieAccess> _movieAccess, 
      QWidget* _parent = nullptr) : 
    QLabel(_parent),
    mspMovieAccess(_movieAccess)
  {
    setMovie(mspMovieAccess->getMovie());
  }

  virtual void showEvent(QShowEvent* _event) override
  {
    Q_UNUSED(_event);
    mspMovieAccess->start();
  }

  virtual void hideEvent(QHideEvent* _event) override
  {
    Q_UNUSED(_event);
    mspMovieAccess->stop();
  }
};

//==============================================================================
LCXmlDataComboLabelBuilder::LCXmlDataComboLabelBuilder()
{

}

//------------------------------------------------------------------------------
LCXmlDataComboLabelBuilder::~LCXmlDataComboLabelBuilder()
{

}

//------------------------------------------------------------------------------
const struct
{
  QString data  = "data";
  QString value = "value";
  QString text = "text";

  struct
  {
    QString attr_name = "mode";
    struct
    {
      QString normal = "normal";
      QString undef  = "undef";
      QString wrong  = "wrong";
    }vals;
  }mode;

  /* QString movie       = "movie"; */
  QString picture     = "picture";
} __slAttributes;

//------------------------------------------------------------------------------
const struct
{
  QString item    = "item";
} __slTags;

//------------------------------------------------------------------------------
static void buildComboLabel( const QDomElement& _element, 
    LCQDataComboLabel* _label,
    QSharedPointer<LIDataFormatter> _format);

//------------------------------------------------------------------------------
QWidget* LCXmlDataComboLabelBuilder::buildLocal(
    const QDomElement& _element, const LIApplication& _app)
{

  LCQDataComboLabel *remlabel= nullptr;

  auto ret_wrong = 
   [&_element]()
   {
     return new QLabel(_element.tagName());
   }; 



  bool err_flag = false;

  auto data_spec = xmlcommon::parseDataSpecification(
      _element.attribute(__slAttributes.data),
      [&err_flag](const QString)
      {
        err_flag = true;
      });

  if(err_flag) return ret_wrong();


  auto source = _app.getDataSource(data_spec.sourceId);
  if(source.isNull()) return ret_wrong();

  auto format = _app.getDataFormatter(data_spec.formatterId);
  if(format.isNull()) return ret_wrong();


  remlabel = new LCQDataComboLabel(data_spec.dataId, source, format);

  buildComboLabel(_element, remlabel, format);

  return remlabel;
}

//------------------------------------------------------------------------------
enum class EItemMode
{
  normal,
  undef,
  wrong
};

//------------------------------------------------------------------------------
static void buildComboLabel( const QDomElement& _element, 
    LCQDataComboLabel* _cl,
    QSharedPointer<LIDataFormatter> _format)
{
  for( QDomNode node = _element.firstChildElement(__slTags.item);
      !node.isNull();
      node = node.nextSiblingElement(__slTags.item))
  {
    QDomElement el = node.toElement();

    QLabel* label = nullptr;


    QString attr = el.attribute(__slAttributes.mode.attr_name);

    EItemMode  mode = EItemMode::normal;
    if(!attr.isNull())
    {
      if(attr == __slAttributes.mode.vals.undef)
      {
        mode = EItemMode::undef;
      }
      else if(attr == __slAttributes.mode.vals.wrong)
      {
        mode = EItemMode::wrong;
      }
    }

    QString attr_data= el.attribute(__slAttributes.text);
    if(!attr_data.isNull())
    {

      switch(mode)
      {
      case EItemMode::undef:
        {
          label = new QLabel(attr_data);
          _cl->addItemUndef(label);
        }
        break;

      case EItemMode::wrong:
        {
          label = new QLabel(attr_data);
          _cl->addItemWrong(label);
        }
        break;

      default: //normal
        {
          QString attr_value = el.attribute(__slAttributes.value);

          if(!attr_value.isNull())
          {
            attr_value = _format->toString(_format->toBytes(attr_value));
            if(attr_value.isNull()) continue;
          }
          label = new QLabel(attr_data);
          _cl->addItem(label, attr_value);
        }
      }

      continue;
    }

    //-------------------------------------picture
    attr_data = el.attribute(__slAttributes.picture);

    if(attr_data.isNull()) continue;


    auto movie_access = LCXmlBuilderBase::getMovie(attr_data);  

    if(movie_access.isNull()) continue;

    label = nullptr;
    
    switch(mode)
    {
    case EItemMode::undef:
      {
        label = new CQMovieLabel(movie_access);
        _cl->addItemUndef(label);
      }
      break;

    case EItemMode::wrong:
      {
        label = new CQMovieLabel(movie_access);
        _cl->addItemWrong(label);
      }
      break;

    default: //normal
      {
        QString attr_value = el.attribute(__slAttributes.value);

        if(!attr_value.isNull())
        {

          attr_value = _format->toString(_format->toBytes(attr_value));
          if(attr_value.isNull()) continue;
        }
        label = new CQMovieLabel(movie_access);
        _cl->addItem(label, attr_value);

      }
      break;
    }

  }
}
