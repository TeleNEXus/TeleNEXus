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
#include "lcjscriptservice.h"
#include "xmlcommon.h"
#include "lcqjscriptservicehiden.h"
#include <QJSEngine>
#include <QTimer>
#include <QDebug>
#include <QThread>
#include <QWaitCondition>

//==============================================================================
class CLocalData
{
private:
  LCQJScriptHiden* mpScriptHiden;
  QMap<QString, std::function<void(const QStringList&)>> mActions;
public:
  CLocalData() = delete;
  CLocalData(
      const QString& _script, 
      const QMap<QString, QString> _attributes,
      const QString& _fileName,
      const QString& _scriptId):
    mpScriptHiden(new LCQJScriptHiden(_script, _attributes, _fileName, _scriptId))
  {
    mActions.insert(QStringLiteral("launch"),
        [this](const QStringList& _param)
        {
          if(_param.isEmpty()) return;
          bool flag = false;
          int interval = _param[0].toInt(&flag);
          if(!flag) return;
          launch(interval);
        });

    mActions.insert(QStringLiteral("execute"),
        [this](const QStringList& _param)
        {
          Q_UNUSED(_param);
          execute();
        });

    mActions.insert(QStringLiteral("stop"),
        [this](const QStringList& _param)
        {
          Q_UNUSED(_param);
          stop();
        });
  }

  ~CLocalData()
  {
    mpScriptHiden->deleteLater();
  }

  void launch(int _interval)
  {
    mpScriptHiden->launch(_interval);
  }

  void execute()
  {
    mpScriptHiden->execute();
  }

  void stop()
  {
    mpScriptHiden->stop();
  }

  void action(QString _expression)
  {
    if(_expression.isNull()) return;
    QString error;
    auto action = xmlcommon::parseAction(_expression, 
        [&error](const QString& _str){error = _str;});
    if(!error.isNull()) 
    {
      return;
    }

    auto it = mActions.find(action.name);
    if(it == mActions.end()) return;
    if(!action.parameters.isEmpty())
    {
      it.value()(action.parameters);
    }
    else
    {
      it.value()(QStringList());
    }
  }
};

//==============================================================================
#define mpLocalData(p) (static_cast<CLocalData*>(p))
#define ld (*(mpLocalData(mpLocal)))

//==============================================================================
LCJScriptService::LCJScriptService(
    const QString& _script, 
    const QMap<QString, QString>& _attributes,
    const QString& _fileName,
    const QString& _scriptId)  :
  mpLocal( new CLocalData(_script, _attributes, _fileName, _scriptId) )
{
}

//------------------------------------------------------------------------------
LCJScriptService::~LCJScriptService()
{
  delete &ld;
}

//------------------------------------------------------------------------------
QSharedPointer<LIJScriptService> LCJScriptService::create(
    const QString& _script, 
    const QMap<QString, QString>& _attributes,
    const QString& _fileName,
    const QString& _scriptId)
{
  return QSharedPointer<LCJScriptService>( 
      new LCJScriptService(_script, _attributes, _fileName, _scriptId) );
}

//------------------------------------------------------------------------------
void LCJScriptService::launch(int _interval) 
{
  ld.launch(_interval);
}

//------------------------------------------------------------------------------
void LCJScriptService::stop() 
{
  ld.stop();
}

//------------------------------------------------------------------------------
void LCJScriptService::execute() 
{
  ld.execute();
}

//------------------------------------------------------------------------------
void LCJScriptService::action(const QString& _action)
{
  ld.action(_action);
}
