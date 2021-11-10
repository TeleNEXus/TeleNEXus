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

#include "cqeventsfilter.h"

#include "LIApplication.h"
#include "LIMovieAccess.h"

#include <QDebug>
#include <QDomElement>
#include <QTimer>
#include <qnamespace.h>

static const struct
{
  QString picture   = "picture";
  QString movie     = "movie";
}__slAttributes;

static const struct 
{
  QString pressed  = "pressed";
  QString released = "released";
}__slTags;




/* //============================================================================== */
/* using TActions = LCXmlStdActionBuilder::TActions; */

/* //============================================================================== */
/* static void performActions(TActions _actions); */

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
  /* TActions actions_press; */
  /* TActions actions_release; */

  struct SCommonData
  {
    QSharedPointer<LIMovieAccess> spCurrentMovieAccess;
    SCommonData(){}
  };

  LCQPushLabel* push_label = new LCQPushLabel();

  CQEventsFilter::install(push_label, _element, _app);

  auto common_data = QSharedPointer<SCommonData>(new SCommonData());

  /* auto create_handler = */ 
  /*   [&_element, common_data, push_label](const QString& _tag, const TActions& _actions) */
  /*   { */
  /*     using TRet = std::function<void(QLabel*)>; */

  /*     auto ret_show_void = */
  /*       [&_actions, common_data, push_label]() */
  /*       { */
  /*         push_label->setPixmap(QPixmap()); */
  /*         push_label->setMovie(nullptr); */

  /*         return */ 
  /*           (TRet)[_actions, common_data](QLabel* _label) */
  /*           { */
  /*             if(!common_data->spCurrentMovieAccess.isNull()) */
  /*             { */
  /*               common_data->spCurrentMovieAccess->stop(); */
  /*               common_data->spCurrentMovieAccess.clear(); */
  /*             } */

  /*             _label->setPixmap(QPixmap()); */
  /*             _label->setMovie(nullptr); */
  /*             _label->update(); */

  /*             performActions(_actions); */
  /*           }; */
  /*       }; */

  /*     auto state_element = _element.firstChildElement(_tag); */

  /*     if(state_element.isNull()) */ 
  /*     { */
  /*       return ret_show_void(); */
  /*     } */

  /*     QString attr = state_element.attribute(__slAttributes.picture); */
  /*     if(!attr.isNull()) */
  /*     { */
  /*       auto picture = LCXmlBuilderBase::parsePixmap(attr); */
  /*       if(picture.isNull()) */ 
  /*       { */
  /*         return ret_show_void(); */
  /*       } */
  /*       push_label->setPixmap(picture); */ 
  /*       if(!common_data->spCurrentMovieAccess.isNull()) */
  /*       { */
  /*         common_data->spCurrentMovieAccess->stop(); */
  /*         common_data->spCurrentMovieAccess.clear(); */
  /*       } */
  /*       return */ 
  /*         (TRet)[picture, _actions, common_data](QLabel* _label) */
  /*         { */
  /*           if(!common_data->spCurrentMovieAccess.isNull()) */
  /*           { */
  /*             common_data->spCurrentMovieAccess->stop(); */
  /*             common_data->spCurrentMovieAccess.clear(); */
  /*           } */
  /*           _label->setPixmap(picture); */
  /*           _label->update(); */
  /*           performActions(_actions); */
  /*         }; */
  /*     } */

  /*     attr = state_element.attribute(__slAttributes.movie); */

  /*     if(!attr.isNull()) */
  /*     { */
  /*       auto movie = LCXmlBuilderBase::getMovie(attr); */
  /*       if(movie.isNull()) return ret_show_void(); */
  /*       push_label->setMovie(movie->getMovie()); */
  /*       movie->start(); */
  /*       common_data->spCurrentMovieAccess = movie; */
  /*       return */ 
  /*         (TRet)[movie, _actions, common_data](QLabel* _label) */
  /*         { */
  /*           if(!common_data->spCurrentMovieAccess.isNull()) */
  /*           { */
  /*             common_data->spCurrentMovieAccess->stop(); */
  /*           } */
  /*           common_data->spCurrentMovieAccess = movie; */
  /*           _label->setPixmap(QPixmap()); */
  /*           _label->setMovie(movie->getMovie()); */
  /*           movie->start(); */
  /*           _label->update(); */
  /*           performActions(_actions); */
  /*         }; */
  /*     } */

  /*     return ret_show_void(); */
  /*   }; */

  auto create_handler = 
    [&_element, common_data, push_label](const QString& _tag)
    {
      using TRet = std::function<void(QLabel*)>;

      auto ret_show_void =
        [common_data, push_label]()
        {
          push_label->setPixmap(QPixmap());
          push_label->setMovie(nullptr);

          return 
            (TRet)[common_data](QLabel* _label)
            {
              if(!common_data->spCurrentMovieAccess.isNull())
              {
                common_data->spCurrentMovieAccess->stop();
                common_data->spCurrentMovieAccess.clear();
              }

              _label->setPixmap(QPixmap());
              _label->setMovie(nullptr);
              _label->update();
            };
        };

      auto state_element = _element.firstChildElement(_tag);

      if(state_element.isNull()) 
      {
        return ret_show_void();
      }

      QString attr = state_element.attribute(__slAttributes.picture);
      if(!attr.isNull())
      {
        auto picture = LCXmlBuilderBase::parsePixmap(attr);
        if(picture.isNull()) 
        {
          return ret_show_void();
        }
        push_label->setPixmap(picture); 
        if(!common_data->spCurrentMovieAccess.isNull())
        {
          common_data->spCurrentMovieAccess->stop();
          common_data->spCurrentMovieAccess.clear();
        }
        return 
          (TRet)[picture, common_data](QLabel* _label)
          {
            if(!common_data->spCurrentMovieAccess.isNull())
            {
              common_data->spCurrentMovieAccess->stop();
              common_data->spCurrentMovieAccess.clear();
            }
            _label->setPixmap(picture);
            _label->update();
          };
      }

      attr = state_element.attribute(__slAttributes.movie);

      if(!attr.isNull())
      {
        auto movie = LCXmlBuilderBase::getMovie(attr);
        if(movie.isNull()) return ret_show_void();
        push_label->setMovie(movie->getMovie());
        movie->start();
        common_data->spCurrentMovieAccess = movie;
        return 
          (TRet)[movie, common_data](QLabel* _label)
          {
            if(!common_data->spCurrentMovieAccess.isNull())
            {
              common_data->spCurrentMovieAccess->stop();
            }
            common_data->spCurrentMovieAccess = movie;
            _label->setPixmap(QPixmap());
            _label->setMovie(movie->getMovie());
            movie->start();
            _label->update();
          };
      }

      return ret_show_void();
    };

  QObject::connect(push_label, &LCQPushLabel::shown, 
      [common_data](QLabel*)
      {
        if(!common_data->spCurrentMovieAccess.isNull())
        {
          common_data->spCurrentMovieAccess->start();
        }
      });

  QObject::connect(push_label, &LCQPushLabel::hidden, 
      [common_data](QLabel*)
      {
        if(!common_data->spCurrentMovieAccess.isNull())
        {
          common_data->spCurrentMovieAccess->stop();
        }
      });

  auto handler_press = create_handler(__slTags.pressed);
  auto handler_release = create_handler(__slTags.released);

  QObject::connect(push_label, &LCQPushLabel::press, handler_press);
  QObject::connect(push_label, &LCQPushLabel::release, handler_release);

  setWidgetName(      _element, push_label);
  setWidgetStyle(     _element, push_label);
  setWidgetSize(      _element, push_label);
  setWidgetPosition(  _element, push_label);
  setWidgetFixedSize( _element, push_label);

  push_label->setAlignment(Qt::AlignmentFlag::AlignCenter);

  return push_label;
}

/* //============================================================================== */
/* static void performActions(TActions _actions) */
/* { */
/*   for(auto it = _actions.begin(); it != _actions.end(); it++) */
/*   { */
/*     (*it)(); */
/*   } */
/* } */


