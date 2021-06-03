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
#ifndef LCXMLCOMMON_H_
#define LCXMLCOMMON_H_

#include <QString>

class QDomDocument;
class LIApplication;

namespace xmlcommon
{
struct SBaseTags
{
  /* QString rootTag         = "APPLICATION"; */
  QString fonts           = "FONTS";
  QString plaginpath      = "PLAGINPATHS";
  /* QString scripts         = "SCRIPTS"; */
  QString formatters      = "FORMATTERS";
};

struct SCommonTags
{
  QString item  = "item";
  /* QString script = "script"; */
};

struct SCommonAttributes
{
  QString file      = "file";
  QString path      = "path";
  /* QString id        = "id"; */
  /* QString title     = "title"; */
  /* QString widht     = "width"; */
  /* QString height    = "height"; */
  /* QString posx      = "posx"; */
  /* QString posy      = "posy"; */
  /* QString interval  = "interval"; */

  /* struct */
  /* { */
  /*   QString tag = "show"; */
  /*   struct */
  /*   { */
  /*     QString yes = "yes"; */
  /*     QString no  = "no"; */
  /*   }vals; */
  /* }show; */
};

extern const SBaseTags          mBaseTags;
extern const SCommonTags        mCommonTags;
extern const SCommonAttributes  mCommonAttributes;


QDomDocument loadDomDocument(const QString& _fileName);
}

#endif

