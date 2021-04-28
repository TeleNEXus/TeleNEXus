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
#include <QDomElement>
#include <QWidget>
#include <QByteArray>
#include <QDebug>

static const struct
{
    QString vcSource  = "vcSource";
    QString vcData    = "vcData";
    QString vcFormat  = "vcFormat";
    QString vcShow    = "vcShow";
    QString vcHide    = "vcHide";
}__slAttributes;

//==============================================================================
struct SLocalData
{
  QWidget* mpWidget = nullptr;
  QSharedPointer<LIRemoteDataReader>  mDataReader;
  QSharedPointer<LIDataFormatter>     mFormatter;
  QString mCompareData;
  std::function<void(QSharedPointer<QByteArray>, LERemoteDataStatus)> mfAction;
};

//==============================================================================
#define toLocalData(p) (reinterpret_cast<SLocalData*>(p))

LCQWidgetVisibleControl::LCQWidgetVisibleControl(QWidget* _widget) : 
  QObject(_widget)
{
  mpLocal = new SLocalData;
  toLocalData(mpLocal)->mpWidget = _widget;
}

LCQWidgetVisibleControl::~LCQWidgetVisibleControl()
{
  delete reinterpret_cast<SLocalData*>(mpLocal);
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

  QString attr_source = _element.attribute(__slAttributes.vcSource);
  QString attr_data   = _element.attribute(__slAttributes.vcData);
  QString attr_format = _element.attribute(__slAttributes.vcFormat);
  QString attr_show   = _element.attribute(__slAttributes.vcShow);
  QString attr_hide   = _element.attribute(__slAttributes.vcHide);

  if( attr_source.isNull() || 
      attr_data.isNull() || 
      attr_format.isNull() || 
      (attr_show.isNull() && attr_hide.isNull())) 
    return ret_wrong();

  auto source = _app.getDataSource(attr_source);

  if(source.isNull()) return ret_wrong();

  auto format = [&_app, &attr_format]() {
    auto ret = _app.getStdDataFormatter(attr_format);
    if(ret.isNull())
    {
      ret = _app.getDataFormatter(attr_format);
    }
    return ret;
  }();

  if(format.isNull()) return ret_wrong();

  ctrl = new LCQWidgetVisibleControl(_widget);

  //--------------------------------------------
  auto action_show = [ctrl](
      QSharedPointer<QByteArray> _data, 
      LERemoteDataStatus _status)
  {
    if(_status != LERemoteDataStatus::DS_OK)
      toLocalData(ctrl->mpLocal)->mpWidget->hide();

    if(toLocalData(ctrl->mpLocal)->mCompareData ==
        toLocalData(ctrl->mpLocal)->mFormatter->toString(*_data))
      toLocalData(ctrl->mpLocal)->mpWidget->show();
    else
      toLocalData(ctrl->mpLocal)->mpWidget->hide();
  };

  //--------------------------------------------
  auto action_hide = [ctrl](
      QSharedPointer<QByteArray> _data, 
      LERemoteDataStatus _status)
  {
    if(_status != LERemoteDataStatus::DS_OK)
      toLocalData(ctrl->mpLocal)->mpWidget->hide();

    if(toLocalData(ctrl->mpLocal)->mCompareData == 
        toLocalData(ctrl->mpLocal)->mFormatter->toString(*_data))
      toLocalData(ctrl->mpLocal)->mpWidget->hide();
    else
      toLocalData(ctrl->mpLocal)->mpWidget->show();
  };
  
  if(!attr_show.isNull())
  {
    toLocalData(ctrl->mpLocal)->mCompareData = 
      format->fitting(attr_show);
    toLocalData(ctrl->mpLocal)->mfAction = action_show;
  }
  else
  {
    toLocalData(ctrl->mpLocal)->mCompareData = 
      format->fitting(attr_hide);
    toLocalData(ctrl->mpLocal)->mfAction = action_hide;
  }

  auto action = [ctrl](
      QSharedPointer<QByteArray> _data, 
      LERemoteDataStatus _status)
  {
    toLocalData(ctrl->mpLocal)->mfAction(_data, _status);
  };

  toLocalData(ctrl->mpLocal)->mDataReader = 
    source->createReader(attr_data, action);

  toLocalData(ctrl->mpLocal)->mFormatter = format;
  toLocalData(ctrl->mpLocal)->mDataReader->connectToSource();

  return ret_ok();
}
