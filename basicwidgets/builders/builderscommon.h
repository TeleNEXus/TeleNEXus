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


class LCWidgetBuildersCommon
{
public:
  //Общие атрибуты построителей виджетов.
  struct SAttributes
  {

    QString posx        = "posx";
    QString posy        = "posy";
    QString width       = "width";
    QString height      = "height";
    QString maxwidth    = "maxWidth";
    QString maxheight   = "maxHeight";
    QString minwidth    = "minWidth";
    QString minheight   = "minHeight";
    QString fixwidth    = "fixWidth";
    QString fixheight   = "fixHeight";
    QString bgcolor     = "bgColor";
    QString bgimage     = "bgImage";
    QString bgpos       = "bgPos";
    QString textcolor   = "textColor";
    QString font        = "font";
    QString fontid      = "fontId";

    QString icon        = "icon";
    QString iconwidth   = "iconWidth";
    QString iconheight  = "iconHeight";
    QString iconscale   = "iconScale";

    QString image        = "image";
    QString imagewidth   = "imageWidth";
    QString imageheight  = "imageHeight";
    QString imagescale   = "imageScale";


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
  static LCWidgetBuildersCommon instance;
  explicit LCWidgetBuildersCommon();
  explicit LCWidgetBuildersCommon(const LCWidgetBuildersCommon&) = delete;
  LCWidgetBuildersCommon& operator=(const LCWidgetBuildersCommon&) = delete;

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
   *  _pixmap - отнисительный путь к файлу фрагметна.
   *  _app - интерфейс доступа к функциям приложения.
   */
  static QPixmap getPixmap(const QString& _pixmap, const LIApplication& _app);
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
};

#endif /* BUILDERSCOMMON_H_ */
