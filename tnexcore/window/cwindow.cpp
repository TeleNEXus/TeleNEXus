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
#include "cwindow.h"
#include <QWidget>
#include <QEvent>

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

//==============================================================================SLocalData
struct SLocalData
{
  QWidget* pWidget;
  CQEventFilter eventFilter;
};

#define toLocalData(p) (reinterpret_cast<SLocalData*>(p))
#define ld (*(toLocalData(mpLocal)))

//==============================================================================LCXmlWindow
LCXmlWindow::LCXmlWindow(QWidget* _widget)
{
  ld.pWidget = _widget;
  ld.pWidget->installEventFilter(&(ld.eventFilter));
}

LCXmlWindow::~LCXmlWindow()
{
  delete toLocalData(mpLocal);
}

//--------------------------------------------------------------------------
void LCXmlWindow::show()
{
  ld.pWidget->show();
}

//--------------------------------------------------------------------------
void LCXmlWindow::hide()
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
void LCXmlWindow::addActionShow(TAction _action)
{
  ld.eventFilter.addActionShow(_action);
}

//--------------------------------------------------------------------------
void LCXmlWindow::addActionHide(TAction _action)
{
  ld.eventFilter.addActionHide(_action);
}


