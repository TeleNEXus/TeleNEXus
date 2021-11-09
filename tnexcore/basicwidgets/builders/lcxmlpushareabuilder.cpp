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
  QString pushDelay = "pushDelay";
}__slAttributes;

static const struct 
{
  QString actions = "actions";
  QString press   = "press";
  QString release = "release";
  QString pressed = "pressed";
  QString released = "released";
}__slTags;

static const struct
{
  QString actions = "events";
  QString press   = "press";
  QString release = "release";
}__slEventsTags;



//==============================================================================
using TActions = LCXmlStdActionBuilder::TActions;

//==============================================================================
static void performActions(TActions _actions);

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
  TActions actions_press;
  TActions actions_release;

  qDebug() << "++++++++++++++++++++++++++++++++++++++++++++k build local 0";

  struct SCommonData
  {
    QSharedPointer<LIMovieAccess> spCurrentMovieAccess;
    SCommonData(){}
  };

  qDebug() << "++++++++++++++++++++++++++++++++++++++++++++k build local 0";

  LCQPushLabel* push_label = new LCQPushLabel();


  CQEventsFilter::install(push_label, _element, _app);


  auto common_data = QSharedPointer<SCommonData>(new SCommonData());


  auto element_actions = _element.firstChildElement(__slTags.actions);
  if(!element_actions.isNull())
  {
    QString attr_push_delay = element_actions.attribute(__slAttributes.pushDelay);

    if(!attr_push_delay.isNull())
    {
      bool flag = false;
      int delay = attr_push_delay.toInt(&flag);
      if(flag)
      {
        push_label->setPushDelay(delay);
      }
    }

    actions_press = LCXmlStdActionBuilder::instance().
      build(element_actions.firstChildElement(__slTags.press), _app);

    actions_release = LCXmlStdActionBuilder::instance().
      build(element_actions.firstChildElement(__slTags.release), _app);
  }


  auto create_handler = 
    [&_element, common_data, push_label](const QString& _tag, const TActions& _actions)
    {
      using TRet = std::function<void(QLabel*)>;

      auto ret_show_void =
        [&_actions, common_data, push_label]()
        {
          push_label->setPixmap(QPixmap());
          push_label->setMovie(nullptr);

          return 
            (TRet)[_actions, common_data](QLabel* _label)
            {
              if(!common_data->spCurrentMovieAccess.isNull())
              {
                common_data->spCurrentMovieAccess->stop();
                common_data->spCurrentMovieAccess.clear();
              }

              _label->setPixmap(QPixmap());
              _label->setMovie(nullptr);
              _label->update();

              performActions(_actions);
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
          (TRet)[picture, _actions, common_data](QLabel* _label)
          {
            if(!common_data->spCurrentMovieAccess.isNull())
            {
              common_data->spCurrentMovieAccess->stop();
              common_data->spCurrentMovieAccess.clear();
            }
            _label->setPixmap(picture);
            _label->update();
            performActions(_actions);
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
          (TRet)[movie, _actions, common_data](QLabel* _label)
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
            performActions(_actions);
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

  auto handler_press = create_handler(__slTags.pressed, actions_press);
  auto handler_release = create_handler(__slTags.released, actions_release);

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

//==============================================================================
static void performActions(TActions _actions)
{
  for(auto it = _actions.begin(); it != _actions.end(); it++)
  {
    (*it)();
  }
}


