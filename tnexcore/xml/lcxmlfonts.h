#ifndef LCXMLFONTS_H_
#define LCXMLFONTS_H_

#include <QSharedPointer>

class QDomElement;
class LIApplication;

class LCXmlFonts
{
private:
  LCXmlFonts();
  ~LCXmlFonts();
  LCXmlFonts(const LCXmlFonts&) = delete;
  LCXmlFonts& operator=(const LCXmlFonts&) = delete;
public:
  static LCXmlFonts& instance();
  void create(
      const QDomElement &_element, 
      const LIApplication& _app);

  const QFont& getFont(const QString& _fontId, bool* flag = nullptr);
};

#endif /* LCXMLFONTS_H_ */
