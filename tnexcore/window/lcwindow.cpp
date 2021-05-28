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
#include "lcwindow.h"
#include "tnexcommon.h"
#include <QWidget>
#include <QEvent>
#include <QMap>
#include <functional>
#include <qnamespace.h>
#include <QDebug>

#define Debug(_msg) TNEXDEBUG("LCWindow", _msg)

class CQEventFilter : public QObject
{
private:

  class CStateBase
  {
  protected:
    using TAction = std::function<void(void)>;
  public:
    CStateBase(){}
    virtual CStateBase* operator()(QObject* _obj, QEvent* _event) = 0;
  };

  //--------------------------------------------------------------------
  class CStateHidden : public CStateBase
  {
  private:
    TAction mActionShow;
    CStateBase* mpStateShow;
  public:
    CStateHidden(){}

    void init(CStateBase* _stateShow, const TAction& _showAction)
    {
      mActionShow = _showAction;
      mpStateShow = _stateShow;
    }

    virtual CStateBase* operator()(QObject* _obj, QEvent* _event) override
    {
      Q_UNUSED(_obj);
      if(_event->type() != QEvent::Show) return this;
      mActionShow();
      return mpStateShow;
    }
  };

  class CStateMinimized: public CStateBase
  {
  private:
    CStateBase* mpStateShow;
    CStateBase* mpStateHidden;
    TAction mHideAction;
  public:
    CStateMinimized(){}
    void init(CStateBase* _stateShow, CStateBase* _stateHidden, const TAction& _hideAction)
    {
      mpStateShow   = _stateShow;
      mpStateHidden = _stateHidden;
      mHideAction   = _hideAction;
    }

    virtual CStateBase* operator()(QObject* _obj, QEvent* _event) override
    {
      Q_UNUSED(_obj);
      CStateBase* ret = this;

      QWidget* w = dynamic_cast<QWidget*>(_obj);
      if(!w) return ret;

      switch(_event->type())
      {
      case QEvent::Type::Show:
        ret = mpStateShow;
        break;
      case QEvent::Type::Hide:
        mHideAction();
        ret = mpStateHidden;
        break;
      default:
        break;
      }

      return ret;
    }
  };

  class CStateShow: public CStateBase
  {
  private:
    TAction mActionHide;
    CStateBase* mpStateHidden;
    CStateBase* mpStateMinimized;

  public:
    CStateShow(){}
    void init(
        CStateBase*     _stateHidden, 
        CStateBase*     _stateMinimized, 
        const TAction&  _hideAction) 
    { 
      mActionHide       = _hideAction;
      mpStateHidden     = _stateHidden;
      mpStateMinimized  = _stateMinimized;
    }

    virtual CStateBase* operator()(QObject* _obj, QEvent* _event) override
    {
      Q_UNUSED(_obj);
      CStateBase* ret = this;

      QWidget* w = dynamic_cast<QWidget*>(_obj);

      if(!w) return ret;

      switch(_event->type())
      {
      case QEvent::Type::Close:
        mActionHide();
        ret = mpStateHidden;
        break;

      case QEvent::Type::Hide:
        if(w->windowState() == Qt::WindowState::WindowMinimized)
        {
          ret = mpStateMinimized;
        }
        else
        {
          mActionHide();
          ret = mpStateHidden;
        }
        break;

      default:
        break;
      }
      return ret;
    }
  };

  using TActionList = QList<LIWindow::TAction>;

  TActionList mShowActions;
  TActionList mHideActions;

  CStateHidden    mStateHidden;
  CStateMinimized mStateMinimized;
  CStateShow      mStateShow;
  CStateBase*     mpCurrentState = &mStateHidden;

public:

  CQEventFilter() : QObject(nullptr)
  {
    auto actions_show = 
      [this]()
      {
        for(auto it = mShowActions.begin();
            it != mShowActions.end();
            it++)
        {
          (*it)();
        }
      }; 

    auto actions_hide = 
      [this]()
      {
        for(auto it = mHideActions.begin();
            it != mHideActions.end();
            it++)
        {
          (*it)();
        }
      }; 
    mStateHidden.init(&mStateShow, actions_show);
    mStateMinimized.init(&mStateShow, &mStateHidden, actions_hide);
    mStateShow.init(&mStateHidden, &mStateMinimized, actions_hide);
  }

  virtual bool eventFilter(QObject* _obj, QEvent* _event) override
  {
    auto ret_state = (*mpCurrentState)(_obj, _event);
    if(ret_state == mpCurrentState) return false;
    mpCurrentState = ret_state;
    return true;
  }

  void addActionShow(LIWindow::TAction _action)
  {
    mShowActions << _action; 
  }

  void addActionHide(LIWindow::TAction _action)
  {
    mHideActions << _action; 
  }
};

//==============================================================================
class CShowParameterValues
{
private:
  QMap<QString, LIWindow::EShowMode> mParametersMap;
public:
  static const QString normal;
  static const QString fullScreen;
  static const QString minimized;
  static const QString maximized;
private:
  CShowParameterValues()
  {
    mParametersMap.insert(normal,     LIWindow::EShowMode::normal);
    mParametersMap.insert(fullScreen, LIWindow::EShowMode::fullScreen);
    mParametersMap.insert(minimized,  LIWindow::EShowMode::minimized);
    mParametersMap.insert(maximized,  LIWindow::EShowMode::maximized);
  }
  CShowParameterValues(const CShowParameterValues&) = delete;
  CShowParameterValues& operator=(const CShowParameterValues&) = delete;
public:
  static CShowParameterValues& instance()
  {
    static CShowParameterValues instance;
    return instance;
  }

  LIWindow::EShowMode stringToShowMode(const QString& _stringMode, bool* _flag)
  {
    auto ret = [&_flag](LIWindow::EShowMode _retMode, bool _retFlag)
    {
      if(_flag) *_flag = _retFlag;
      return _retMode;
    };

    auto it = mParametersMap.find(_stringMode);

    if(it == mParametersMap.end()) 
    {
      return ret(LIWindow::EShowMode::normal, false);
    }
    return ret(it.value(), true);
  }
};

const QString CShowParameterValues::normal      = "normal";
const QString CShowParameterValues::fullScreen  = "fullScreen";
const QString CShowParameterValues::minimized   = "minimized";
const QString CShowParameterValues::maximized   = "maximized";

//==============================================================================SLocalData
struct SLocalData
{
  QMap<QString, std::function<void(const QStringList&)>> actions;
  QWidget* pWidget;
  CQEventFilter eventFilter;
  LIWindow::EShowMode defaultShowMode = LIWindow::EShowMode::normal;
};

#define toLocalData(p) (reinterpret_cast<SLocalData*>(p))
#define ld (*(toLocalData(mpLocal)))

//==============================================================================LCWindow
LCWindow::LCWindow(QWidget* _widget)
{
  mpLocal = new SLocalData;
  ld.pWidget = _widget;
  ld.pWidget->installEventFilter(&(ld.eventFilter));

  ld.actions.insert(QStringLiteral("show"),
      [this](const QStringList& _param)
      {
        QString mode_string;
        if(_param.size() != 0) mode_string = _param[0];
        bool flag = false;
        EShowMode mode = stringToShowMode(mode_string, &flag);
        if(flag)
        {
          show(mode);
        }
        else
        {
          show(ld.defaultShowMode);
        };
      });

  ld.actions.insert(QStringLiteral("hide"),
      [this](const QStringList&)
      {
        hide();
      });
}

LCWindow::~LCWindow()
{
  delete toLocalData(mpLocal);
}

//--------------------------------------------------------------------------
void LCWindow::show(EShowMode _mode)
{
  switch(_mode)
  {
  case EShowMode::normal:
    ld.pWidget->showNormal();
    break;

  case EShowMode::fullScreen:
    ld.pWidget->showFullScreen();
    break;

  case EShowMode::minimized:
    ld.pWidget->showMinimized();
    break;

  case EShowMode::maximized:
    Debug("show maximized");
    ld.pWidget->showMaximized();
    break;
  }
}

//--------------------------------------------------------------------------
void LCWindow::hide()
{
  if(ld.pWidget->windowState() == Qt::WindowState::WindowFullScreen)
  {
    ld.pWidget->close();
  }
  else
  {
    ld.pWidget->hide();
  }
}

//--------------------------------------------------------------------------
void LCWindow::action(const QString& _action)
{
  tnexcommon::performParamActions(_action, ld.actions);
}

//--------------------------------------------------------------------------
bool LCWindow::validateAction(
    const QString& _action, QString* msg)
{
  auto it = ld.actions.find(_action);
  if(it != ld.actions.end()) { return true; }
  if(msg)
  {
    *msg = QStringLiteral(
        "Window: can't recognize action \"%1\"").arg(_action);
  }
  return false;
}

//------------------------------------------------------------------------------
void LCWindow::addActionShow(TAction _action)
{
  ld.eventFilter.addActionShow(_action);
}

//------------------------------------------------------------------------------
void LCWindow::addActionHide(TAction _action)
{
  ld.eventFilter.addActionHide(_action);
}

//------------------------------------------------------------------------------
void LCWindow::setTitle(const QString& _title)
{
  ld.pWidget->setWindowTitle(_title);
}

//------------------------------------------------------------------------------
void LCWindow::setShowMode(EShowMode _showMode)
{
  ld.defaultShowMode = _showMode;
}

//------------------------------------------------------------------------------
void LCWindow::setModality(bool _flag)
{
  if(_flag)
  {
    ld.pWidget->setWindowModality(Qt::WindowModality::ApplicationModal);
  }
  else
  {
    ld.pWidget->setWindowModality(Qt::WindowModality::NonModal);
  }
}

//------------------------------------------------------------------------------
void LCWindow::setFlags(Qt::WindowFlags _flags)
{
  ld.pWidget->setWindowFlags(_flags);
}

//------------------------------------------------------------------------------
void LCWindow::setSize(const QSize& _size)
{
  ld.pWidget->resize(_size);
}

//------------------------------------------------------------------------------
void LCWindow::setPosition(const QPoint& _pos)
{
  ld.pWidget->move(_pos);
}

QSize LCWindow::getSize()
{
  return ld.pWidget->size();
}

//------------------------------------------------------------------------------
LIWindow::EShowMode LCWindow::stringToShowMode(const QString& _modeString,
    bool* _flag)
{
  return 
    CShowParameterValues::instance().stringToShowMode(_modeString, _flag);
}


