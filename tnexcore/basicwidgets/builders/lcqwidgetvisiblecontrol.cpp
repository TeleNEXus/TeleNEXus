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
#include "lcqwidgetvisiblecontrol.h"
#include "LIRemoteDataReader.h"
#include "LIRemoteDataSource.h"
#include "LIApplication.h"
#include "LIDataFormatter.h"
#include "xmlcommon.h"
#include <QDomElement>
#include <QWidget>
#include <QByteArray>
#include <QDebug>
#include <QEvent>

using EReadStatus = LIRemoteDataReader::EReadStatus;

static const struct
{
  QString visibility = "visibility";
}__slXmlAttributes;

static const struct
{
  QString sourceId    = "source";
  QString dataId      = "data";
  QString format      = "format";
  QString showValue   = "showValue";
  QString hideValue   = "hideValue";
  QString undefState  = "undefState";
}__slVisibleAttributes;


static const struct
{
  QString show = "show";
  QString hide = "hide";
}__slXmlAttributesVals;





//==============================================================================
class CLocalData
{
private:

public:
  QWidget* mpWidget = nullptr;
  enum class EVisibleStatus
  {
    show,
    hide,
    undef
  };

  enum class EUndefMode
  {
    show,
    hide
  };

  EUndefMode mUndefMode = EUndefMode::hide;
  EVisibleStatus mVisibleStatus = EVisibleStatus::undef;
  QSharedPointer<LIRemoteDataReader>  mDataReader;
  QSharedPointer<LIDataFormatter>     mFormatter;
  QByteArray mCompareData;
  std::function<void(QSharedPointer<QByteArray>, EReadStatus)> mfAction;

  CLocalData(){}

  void showWidget()
  {
    if( mVisibleStatus == EVisibleStatus::show) return;
    mVisibleStatus = EVisibleStatus::show;
    mpWidget->show();
  }

  void hideWidget()
  {
    if( mVisibleStatus == EVisibleStatus::hide) return;
    mVisibleStatus = EVisibleStatus::hide;
    mpWidget->hide();
  }

  void setWidget(QWidget* _widget)
  {
    mpWidget = _widget;
  }
};

//==============================================================================
#define toLocalData(p) (reinterpret_cast<CLocalData*>(p))
#define ld (*(toLocalData(mpLocal)))

//==============================================================================
LCQWidgetVisibleControl::LCQWidgetVisibleControl(QWidget* _widget) :
  QObject(_widget)
{
  mpLocal = new CLocalData;
  ld.setWidget(_widget);
}

LCQWidgetVisibleControl::~LCQWidgetVisibleControl()
{
  delete reinterpret_cast<CLocalData*>(mpLocal);
}

//------------------------------------------------------------------------------
bool LCQWidgetVisibleControl::build(const QDomElement& _element,
    QWidget* _widget,
    const LIApplication& _app
    )
{
  LCQWidgetVisibleControl* ctrl = nullptr;

  auto ret_ok = []()
  {
    return true;
  };

  auto ret_wrong = [ctrl]()
  {
    if(ctrl != nullptr) ctrl->deleteLater();
    return false;
  };

  if(_widget == nullptr) return ret_wrong();

  QString attr = _element.attribute(__slXmlAttributes.visibility);
  if(attr.isNull()) return ret_wrong();


  auto attr_values = xmlcommon::parseAttributes(attr);

  QString attr_source = attr_values.value(__slVisibleAttributes.sourceId,   QString());
  QString attr_data   = attr_values.value(__slVisibleAttributes.dataId,     QString());
  QString attr_format = attr_values.value(__slVisibleAttributes.format,     QString());
  QString attr_show_value   = attr_values.value(__slVisibleAttributes.showValue,  QString());
  QString attr_hide_value   = attr_values.value(__slVisibleAttributes.hideValue,  QString());


  if( attr_source.isNull() ||
      attr_data.isNull() ||
      attr_format.isNull() ||
      (attr_show_value.isNull() && attr_hide_value.isNull()))
    return ret_wrong();

  auto source = _app.getDataSource(attr_source);

  if(source.isNull()) return ret_wrong();

  auto format = _app.getDataFormatter(attr_format);

  if(format.isNull()) return ret_wrong();


  ctrl = new LCQWidgetVisibleControl(_widget);



  QString attr_undef   = attr_values.value(__slVisibleAttributes.undefState, QString());
  if(!attr_undef.isNull())
  {
    if(attr_undef == __slXmlAttributesVals.show)
      toLocalData(ctrl->mpLocal)->mUndefMode = CLocalData::EUndefMode::show;
    else if(attr_undef == __slXmlAttributesVals.hide)
      toLocalData(ctrl->mpLocal)->mUndefMode = CLocalData::EUndefMode::hide;
  }

  //--------------------------------------------
  auto read_status_ctrl = [ctrl](EReadStatus _status)
  {
    if(_status == EReadStatus::Valid) return true;
    switch(toLocalData(ctrl->mpLocal)->mUndefMode)
    {
    case CLocalData::EUndefMode::show:
      toLocalData(ctrl->mpLocal)->showWidget();
      break;

    case CLocalData::EUndefMode::hide:
      toLocalData(ctrl->mpLocal)->hideWidget();
    default:
      break;
    }
    return false;
  };

  //--------------------------------------------action_show
  auto action_show = [ctrl, read_status_ctrl](
      QSharedPointer<QByteArray> _data,
      EReadStatus _status)
  {

    if(!read_status_ctrl(_status)) return;

    if(toLocalData(ctrl->mpLocal)->mCompareData == *_data)
    {
      toLocalData(ctrl->mpLocal)->showWidget();
    }
    else
    {
      toLocalData(ctrl->mpLocal)->hideWidget();
    }
  };


  //--------------------------------------------action_hide
  auto action_hide = [ctrl, read_status_ctrl](
      QSharedPointer<QByteArray> _data,
      EReadStatus _status)
  {

    if(!read_status_ctrl(_status)) return;

    if(toLocalData(ctrl->mpLocal)->mCompareData == *_data)
      toLocalData(ctrl->mpLocal)->hideWidget();
    else
      toLocalData(ctrl->mpLocal)->showWidget();
  };


  if(!attr_show_value.isNull())
  {
    toLocalData(ctrl->mpLocal)->mCompareData =
      format->toBytes(attr_show_value);
    toLocalData(ctrl->mpLocal)->mfAction = action_show;
  }
  else
  {
    toLocalData(ctrl->mpLocal)->mCompareData =
      format->toBytes(attr_hide_value);
    toLocalData(ctrl->mpLocal)->mfAction = action_hide;
  }


  //--------------------------------------------read_handler
  auto read_handler = [ctrl](
      QSharedPointer<QByteArray> _data,
      EReadStatus _status)
  {
    toLocalData(ctrl->mpLocal)->mfAction(_data, _status);
  };

  toLocalData(ctrl->mpLocal)->mDataReader = source->createReader(attr_data);

  if((!toLocalData(ctrl->mpLocal)->mDataReader.isNull())&&(!format.isNull()))
  {
    toLocalData(ctrl->mpLocal)->mDataReader->setHandler(read_handler);
    toLocalData(ctrl->mpLocal)->mFormatter = format;
    toLocalData(ctrl->mpLocal)->mDataReader->connectToSource();
  }

  return ret_ok();
}




