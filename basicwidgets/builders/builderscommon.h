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
    QString fixwidth    = "fixwidth";
    QString fixheght    = "fixheight";
    QString colorbg     = "bgcolor";
    QString colortext   = "textcolor";
    QString fontname    = "fontname";
    QString fontsize    = "fontsize";
    QString fontId      = "fontId";
    //Атрибута выравнивания.
    struct
    {
      const QString attrName = "align";
      //Возможные значения выравнивания объеденяемые по "ИЛИ".
      struct
      {
        QString Left    = "Left";
        QString Right   = "Right";
        QString Center  = "Center";
        QString HCenter = "HCenter";
        QString Top     = "Top";
        QString Bottom  = "Bottom";
        QString VCenter = "VCenter";
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
   * Преозразует атрибут в значение цвета.
   * --------------------------------------
   *  Параметры:
   *  _attr_color - атрибут содержащий значение либо стандартного цвена
   *  либо ARGB значение.
   */
  static QColor attributeToColor(const QString& _attr_color);
};

#endif /* BUILDERSCOMMON_H_ */
