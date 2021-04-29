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
    QString vcUndef   = "vcUndef";
}__slAttributes;

static const struct
{
  //vcUndef
  QString show = "show";
  QString hide = "hide";
}__slAttributesVals;

//==============================================================================
class CLocalData
{
private:
  QWidget* mpWidget = nullptr;
public:
  enum class EUndefMode
  {
    show,
    hide
  };
  EUndefMode mUndefMode = EUndefMode::hide;
  bool mShowAllowFlag = false;
  QSharedPointer<LIRemoteDataReader>  mDataReader;
  QSharedPointer<LIDataFormatter>     mFormatter;
  QString mCompareData;
  std::function<void(QSharedPointer<QByteArray>, LERemoteDataStatus)> mfAction;

  CLocalData(){}

  void showWidget()
  {
    mpWidget->show();
    mShowAllowFlag = true;
  }
  void hideWidget()
  {
    mpWidget->hide();
    mShowAllowFlag = false;
  }

  void setWidget(QWidget* _widget)
  {
    mpWidget = _widget;
  }
};


//==============================================================================
#define toLocalData(p) (reinterpret_cast<CLocalData*>(p))

LCQWidgetVisibleControl::LCQWidgetVisibleControl(QWidget* _widget) : 
  QObject(_widget)
{
  mpLocal = new CLocalData;
  toLocalData(mpLocal)->setWidget(_widget);
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


  QString attr_undef = _element.attribute(__slAttributes.vcUndef);
  if(!attr_undef.isNull())
  {
    if(attr_undef == __slAttributesVals.show) 
      toLocalData(ctrl->mpLocal)->mUndefMode = CLocalData::EUndefMode::show;
    else if(attr_undef == __slAttributesVals.hide) 
      toLocalData(ctrl->mpLocal)->mUndefMode = CLocalData::EUndefMode::hide;
  }

  auto status_string = [](LERemoteDataStatus _status)
  {
    QString str;
    switch(_status)
    {
    case LERemoteDataStatus::DS_OK:
      str = QStringLiteral("DS_OK");
      break;
    case LERemoteDataStatus::DS_WRONG:
      str = QStringLiteral("DS_WRONG");
      break;
    case LERemoteDataStatus::DS_UNDEF:
    default:
      str = QStringLiteral("DS_UNDEF");
      break;
    }
    return str;
  };

  //--------------------------------------------
  auto read_status_ctrl = [ctrl](LERemoteDataStatus _status)
  {
    if(_status == LERemoteDataStatus::DS_OK) return true;
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
  auto action_show = [ctrl, status_string, read_status_ctrl](
      QSharedPointer<QByteArray> _data, 
      LERemoteDataStatus _status)
  {
    qDebug() << "action hide data status = " << status_string(_status);

    if(!read_status_ctrl(_status)) return;

    if(toLocalData(ctrl->mpLocal)->mCompareData ==
        toLocalData(ctrl->mpLocal)->mFormatter->toString(*_data))
    {
      toLocalData(ctrl->mpLocal)->showWidget();
    }
    else
    {
      toLocalData(ctrl->mpLocal)->hideWidget();
    }
  };


  //--------------------------------------------action_hide
  auto action_hide = [ctrl, status_string, read_status_ctrl](
      QSharedPointer<QByteArray> _data, 
      LERemoteDataStatus _status)
  {
    qDebug() << "action hide data status = " << status_string(_status);

    if(!read_status_ctrl(_status)) return;

    if(toLocalData(ctrl->mpLocal)->mCompareData == 
        toLocalData(ctrl->mpLocal)->mFormatter->toString(*_data))
      toLocalData(ctrl->mpLocal)->hideWidget();
    else
      toLocalData(ctrl->mpLocal)->showWidget();
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

  //--------------------------------------------action_read_data
  auto action_read_data = [ctrl](
      QSharedPointer<QByteArray> _data, 
      LERemoteDataStatus _status)
  {
    toLocalData(ctrl->mpLocal)->mfAction(_data, _status);
  };


  toLocalData(ctrl->mpLocal)->mDataReader = 
    source->createReader(attr_data, action_read_data);

  toLocalData(ctrl->mpLocal)->mFormatter = format;
  toLocalData(ctrl->mpLocal)->mDataReader->connectToSource();

  return ret_ok();
}

bool LCQWidgetVisibleControl::eventFilter(QObject* _opbj, QEvent* _event)
{

}





