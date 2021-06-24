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
#include "lcxmllabelbuilder.h"
#include "LIApplication.h"
#include "lcqdatalabel.h"
#include "xmlcommon.h"
#include "LIMovieAccess.h"

#include <QLabel>
#include <QDomElement>
#include <QDebug>
#include <QMovie>

static const struct
{
  QString data = "data";
  QString text = "text";
  QString movie = "movie";
  QString picture = "picture";
}__slAttributes;

//==============================================================================CQMovieLabel
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

//------------------------------------------------------------------------------
QWidget* LCXmlLabelBuilder::buildLocal(
   const QDomElement& _element, const LIApplication& _app) 
{
  auto ret = 
    [&_element](QLabel* _label)
    {
      setWidgetName(      _element, _label);
      setWidgetStyle(     _element, _label);
      setWidgetSize(      _element, _label);
      setWidgetPosition(  _element, _label);
      setWidgetFixedSize( _element, _label);
      return _label;
    };

  auto ret_wrong = 
    [&_element]()
    {
      return new QLabel(_element.tagName());
    };



  auto build_data = 
    [&_element, &_app, &ret]()
    {
      QSharedPointer<LIRemoteDataSource> source;
      QSharedPointer<LIDataFormatter> format;

      bool err_flag = false;

      auto dataSpec = xmlcommon::parseDataSpecification(
          _element.attribute(__slAttributes.data), 
          [&err_flag](const QString&)
          {
            err_flag = true;
          });

      if(err_flag) 
      {
        return (QLabel*)nullptr;
      }

      source = _app.getDataSource(dataSpec.sourceId);

      if(source.isNull())
      {
        return (QLabel*)nullptr;
      }

      format = _app.getDataFormatter(dataSpec.formatterId);

      if(format.isNull())
      {
        return (QLabel*)nullptr;
      }
      return ret(new LCQDataLabel(dataSpec.dataId, source, format));
    };

  auto build_text = 
    [&_element, &ret]()
    {
      QString attr_text = _element.attribute(__slAttributes.text);
      if(attr_text.isNull()) return (QLabel*)(nullptr);
      return ret(new QLabel(attr_text));
    };

  auto build_movie =
    [&_element, &ret]()
    {
      QString attr_movie = _element.attribute(__slAttributes.movie);
      if(attr_movie.isNull()) return (QLabel*)nullptr;
      auto movie = getMovie(attr_movie);
      if(movie.isNull()) return (QLabel*)nullptr;
      return ret(new CQMovieLabel(movie));
    };

  auto build_picture = 
    [&_element, &ret]()
    {
      QString attr_picture = _element.attribute(__slAttributes.picture);
      if(attr_picture.isNull()) return (QLabel*)nullptr;
      QPixmap pixmap = parsePixmap(attr_picture);
      if(pixmap.isNull()) return (QLabel*)nullptr;
      QLabel* label = new QLabel();
      label->setPixmap(pixmap);
      return ret(label);
    };
  QWidget* ret_widget = build_data();
  if(ret_widget != nullptr) return ret_widget;
  ret_widget = build_text();
  if(ret_widget != nullptr) return ret_widget;
  ret_widget = build_movie();
  if(ret_widget != nullptr) return ret_widget;
  ret_widget = build_picture();
  if(ret_widget != nullptr) return ret_widget;
  return ret_wrong();
}

