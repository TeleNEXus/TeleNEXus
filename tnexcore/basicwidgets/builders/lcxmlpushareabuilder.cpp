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

#include "lcxmlpushareabuilder.h"
#include "lcxmlstdactionbuilder.h"

#include "widgets/lcqpushlabel.h"

#include "LIApplication.h"
#include "LIMovieAccess.h"
#include "lcsecurity.h"

#include <QDebug>
#include <QDomElement>
#include <QTimer>
#include <qnamespace.h>

static const struct
{
  QString picture   = "picture";
  QString movie     = "movie";
  QString delay     = "delay";
}__slAttributes;

static const struct 
{
  QString pressed  = "press";
  QString released = "release";
}__slTags;

//==============================================================================
LCXmlPushAreaBuilder::LCXmlPushAreaBuilder()
{
}

//------------------------------------------------------------------------------
LCXmlPushAreaBuilder::~LCXmlPushAreaBuilder()
{
}


//------------------------------------------------------------------------------
QWidget* LCXmlPushAreaBuilder::buildLocal(
    const QDomElement& _element, const LIApplication& _app)
{

  Q_UNUSED(_app);

  using TChangeView = LCQPushLabel::TChangeView;
  auto sp_movie_access_list = 
    QSharedPointer<QList<QSharedPointer<LIMovieAccess>>>(
        new QList<QSharedPointer<LIMovieAccess>>);

  auto get_view_setter = 
    [sp_movie_access_list](const QDomElement _element)
    {

      auto stop_movies = 
        [sp_movie_access_list]()
        {
          for(auto it = (*sp_movie_access_list).begin();
              it != (*sp_movie_access_list).end();
              it++)
          {
            (*it)->stop();
          }
        };

      TChangeView empty_setter = 
        [stop_movies](QLabel* _label)
        {
          stop_movies();
          _label->setPixmap(QPixmap());
          _label->setMovie(nullptr);
          _label->update();
        };

      if(_element.isNull()) 
      {
        return empty_setter;
      }

      auto get_picture_setter = 
        [&empty_setter, stop_movies](QString _attr)
        {
          TChangeView ret;
          QPixmap picture = LCXmlBuilderBase::parsePixmap(_attr);
          if(picture.isNull())
          {
            ret = empty_setter;
          }
          else
          {
            ret = 
              [picture, stop_movies](QLabel* _label)
              {
                stop_movies();
                _label->setMovie(nullptr);
                _label->setPixmap(picture);
                _label->update();
              };
          }
          return ret;
        };

      auto get_movie_setter = 
        [&empty_setter, sp_movie_access_list, stop_movies](QString _attr)
        {
          TChangeView ret;

          auto movie_access = LCXmlBuilderBase::getMovie(_attr);
          if(movie_access.isNull()) 
          {
            ret = empty_setter;
          }
          else
          {
            *sp_movie_access_list << movie_access;
            ret = 
              [movie_access, stop_movies](QLabel* _label)
              {
                stop_movies();
                _label->setPixmap(QPixmap());
                _label->setMovie(movie_access->getMovie());
                movie_access->start();
                _label->update();
              };
          }
          return ret;
        };

      QString attr = _element.attribute(__slAttributes.picture);
      if(!attr.isNull()) return get_picture_setter(attr);
      attr = _element.attribute(__slAttributes.movie);
      return get_movie_setter(attr);
    };

  QDomElement el_press = _element.firstChildElement(__slTags.pressed);
  QDomElement el_release = _element.firstChildElement(__slTags.released);


  TChangeView push_veiw_setter = get_view_setter(el_press);
  TChangeView release_veiw_setter = get_view_setter(el_release);

  LCQPushLabel::TActionsList push_actions;
  LCQPushLabel::TActionsList release_actions;

  if(!el_press.isNull())
  {
    push_actions = 
      LCXmlStdActionBuilder::instance().build(el_press, _app);
  }

  if(!el_release.isNull())
  {
    release_actions = 
      LCXmlStdActionBuilder::instance().build(el_release, _app);
  }

  bool flag = false;
  int push_delay = el_press.attribute(__slAttributes.delay).toInt(&flag);

  if(!flag) 
  {
    push_delay = 0;
  }

  LCQPushLabel* push_label = new LCQPushLabel(
      push_veiw_setter, 
      release_veiw_setter, 
      push_actions, 
      release_actions, 
      push_delay);

  setWidgetName(      _element, push_label);
  setWidgetStyle(     _element, push_label);
  setWidgetSize(      _element, push_label);
  setWidgetPosition(  _element, push_label);
  setWidgetFixedSize( _element, push_label);

  QSet<QEvent::Type> events;

  events 
    << QEvent::Type::MouseButtonPress
    << QEvent::Type::MouseButtonDblClick
    << QEvent::Type::TouchBegin;


  QObject* security_filter =  
    LCSecurity::instance().createEventFilter(_element, events);
  if(security_filter != nullptr)
  {
    push_label->installEventFilter(security_filter);
  }


  push_label->setAlignment(Qt::AlignmentFlag::AlignCenter);

  return push_label;
}


