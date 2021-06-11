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
#include "lcformattertextstream.h"

//==============================================================================LCFormatterTextStream
LCFormatterTextStream::LCFormatterTextStream() 
{
}

//------------------------------------------------------------------------------toString
QString LCFormatterTextStream::toString(const QByteArray& _data)
{
  return QString::fromUtf8(_data);
}

//------------------------------------------------------------------------------toBytes
QByteArray LCFormatterTextStream::toBytes(const QString& _str)
{

  using QRE = QRegularExpression;

  static const QRE zero_expr    = QRE("(?<!\\\\)(\\\\0)");
  static const QRE slash_b      = QRE("(?<!\\\\)(\\\\b)");
  static const QRE slash_f      = QRE("(?<!\\\\)(\\\\f)");
  static const QRE slash_n      = QRE("(?<!\\\\)(\\\\n)");
  static const QRE slash_r      = QRE("(?<!\\\\)(\\\\r)");
  static const QRE slash_t      = QRE("(?<!\\\\)(\\\\t)");
  static const QRE slash_v      = QRE("(?<!\\\\)(\\\\v)");
  static const QRE single_slash = QRE("(?<!\\\\)(\\\\)(?!x)");
  static const QRE double_slash = QRE("(\\\\)(?!x)");

  QString format_str = _str;

  //Разделение по нулевому символу.
  {
    auto split_list = format_str.split(zero_expr);
    if(split_list.length() > 1)
    {
      format_str = split_list.first();
      format_str.append(QStringLiteral("\0"));
    }
  }

  //Замена управляющих символов.
  format_str.replace(slash_b, QStringLiteral("\b"));
  format_str.replace(slash_f, QStringLiteral("\f"));
  format_str.replace(slash_n, QStringLiteral("\n"));
  format_str.replace(slash_r, QStringLiteral("\r"));
  format_str.replace(slash_t, QStringLiteral("\t"));
  format_str.replace(slash_v, QStringLiteral("\v"));

  //Удаление одиночных обратных черт.
  format_str.replace(single_slash, QStringLiteral(""));
  //Замена двойной обратной черты на одинарную.
  format_str.replace(double_slash, QString("\\"));

  //Замена hex кодов.
  {
    static const QRE hex_code = QRE("\\\\x[0-9a-fA-F]{1,4}");
    auto match_it = hex_code.globalMatch(format_str);

    while(match_it.hasNext())
    {
      auto m = match_it.next();
      QString ms = m.captured();
      ms.replace(QRE("\\\\x"), QString(""));
      bool flag = false;
      quint16 b = ms.toUInt(&flag, 16);
      if(flag)
      {
        format_str.replace(m.captured(), QChar(b));
      }
    }
  }

  return format_str.toUtf8();

}


//------------------------------------------------------------------------------validator
QValidator* LCFormatterTextStream::validator()
{
  return nullptr;
}


