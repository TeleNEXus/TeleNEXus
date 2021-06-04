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

#ifndef BUILDERSCOMMON_H_
#define BUILDERSCOMMON_H_

#include <QString>
#include <QSharedPointer>
#include <qnamespace.h>

class QDomElement;
class QWidget;
class QMovie;
class LIApplication;
class LIMovieAccess;


class LCBuildersCommon
{
public:
  struct SDataSpec
  {
    QString sourceId;
    QString dataId;
    QString formagId;
  };
  //Общие атрибуты построителей виджетов.
  struct SAttributes
  {
    QString data        = "data";
    QString source      = "source";
    QString label       = "label";
    QString file        = "file";
    QString posx        = "posx";
    QString posy        = "posy";
    QString width       = "fixWidth";
    QString height      = "fixHeight";
    QString maxwidth    = "maxWidth";
    QString maxheight   = "maxHeight";
    QString minwidth    = "minWidth";
    QString minheight   = "minHeight";
    QString fixwidth    = "fixWidth";
    QString fixheight   = "fixHeight";
    QString bgcolor     = "bgColor";
    QString bgimage     = "bgImage";
    QString bgpos       = "bgPos";
    QString text        = "text";
    QString textcolor   = "textColor";

    QString font        = "font";
    QString fontid      = "fontId";

    QString dataformatter = "format";

    QString icon        = "icon";
    QString iconsize    = "iconSize";
    QString iconwidth   = "iconWidth";
    QString iconheight  = "iconHeight";
    QString iconscale   = "iconScale";

    QString image        = "image";
    QString imagewidth   = "imageWidth";
    QString imageheight  = "imageHeight";
    QString imagescale   = "imageScale";

    QString keyboard     = "keyboard";

    //Атрибута выравнивания.
    struct
    {
      const QString attrName = "align";
      //Возможные значения выравнивания объеденяемые по "ИЛИ".
      struct
      {
        QString Left    = "left";
        QString Right   = "right";
        QString Center  = "center";
        QString HCenter = "hcenter";
        QString Top     = "top";
        QString Bottom  = "bottom";
        QString VCenter = "vcenter";
      }vals;
    }aligns;
  };
  static const SAttributes mAttributes;

private:
  static LCBuildersCommon instance;
  explicit LCBuildersCommon();
  explicit LCBuildersCommon(const LCBuildersCommon&) = delete;
  LCBuildersCommon& operator=(const LCBuildersCommon&) = delete;

public:

  /*
   * Устанавливает положение виджета.
   */
  static void initPosition(const QDomElement& _element, QWidget& _widget);
  /*
   * Устанавливает размер виджета.
   */
  static void initSize(const QDomElement& _element, QWidget& _widget);
  /*
   * Устанавливает фиксированный размер виджета.
   */
  static void initFixedSize(const QDomElement& _element, QWidget& _widget);

  /*
   * Возвращает указатель на интерфейс доступа к видеофрагменту.
   * --------------------------
   *  _movie - отнисительный путь к файлу фрагметна.
   *  _app - интерфейс доступа к функциям приложения.
   */
  static QSharedPointer<LIMovieAccess> getMovie(const QString& _movie,
      const LIApplication& _app);
  /*
   * Возвращает пиксельное изображение.
   * --------------------------
   *  _file - отнисительный путь к файлу фрагметна.
   *  _app - интерфейс доступа к функциям приложения.
   */
  static QPixmap getPixmap(const QString& _file, const LIApplication& _app);

  /*
   * Преозразует атрибут в флаг выравнивания.
   * Возвращает true при успешном преобразовании, и false при неуспешном.
   * --------------------------------------
   *  Параметры:
   *  _attributes - атрибут.
   *  _flags - ссылка на возвращаемое значение флага.
   */
  static bool toAlignFlags(const QString& _attributes, Qt::Alignment& _flags);
  /*
   * Производит поиск и преобразования атрибута
   * DOM елемента вo флаг выравнивания.
   * Возвращает true при успешном преобразовании, и false при неуспешном.
   * --------------------------------------
   *  Параметры:
   *  _element- атрибут.
   *  _flags - ссылка на возвращаемое значение флага.
   */
  static bool toAlignFlags(const QDomElement& _element, Qt::Alignment& _flags);

  /*
   * Преозразует атрибут в строку вида "AlignTop | ... | AlignVCenter"
   * для использования при задании стилей выравниавния чурез механизм
   * свойств qproperty-alignment.
   */
  static QString toAlignString(const QString& _attributes);
  /*
   * Преозразует атрибут в значение цвета.
   * --------------------------------------
   *  Параметры:
   *  _attr_color - атрибут содержащий значение либо стандартного цвена
   *  либо ARGB значение.
   */
  static QColor attributeToColor(const QString& _attr_color);
  /*
   * Получение строки определения базового стиля из DOM элемента.
   */
  static QString getBaseStyleSheet(const QDomElement& _element, const LIApplication& _app);



  //=============================================================================================


  /* Parses a string of an expression like 
   * "file='pixmapfile'; size=(width, height)" 
   * and returns a QPixmap of the specified size.
   */
  static QPixmap parsePixmap(const QString& _expr, const LIApplication& _app);
};

#endif /* BUILDERSCOMMON_H_ */
