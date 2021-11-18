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
#ifndef CQEVENTSFILTER_H_
#define CQEVENTSFILTER_H_

#include "lcxmlstdactionbuilder.h"
#include <QObject>
#include <QSharedPointer>
#include <QEvent>

class QTimer;
class QDomElement;
class LIApplication;

class LQEventsFilter : public QObject
{
  Q_OBJECT;
private:

  using TActions = LCXmlStdActionBuilder::TActions;
  using TActionsMap = QMap<QEvent::Type, TActions>;

  TActionsMap mActionsMap;
  QTimer* mpTimer;
  enum class EStatePress
  {
    released,
    pressed
  };
  EStatePress mStatePress;
  QEvent* mpCurrentEvent;
  bool mReturnFlag;

  LQEventsFilter() = delete;
  LQEventsFilter(const TActionsMap& _actionsMap, QObject* _parent = nullptr);
public:
  virtual ~LQEventsFilter();
  virtual bool eventFilter(QObject* _obj, QEvent* _event) override;
  static void install(QWidget* _target, const QDomElement& _element, const LIApplication& _app);
};

#endif
