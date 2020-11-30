#ifndef LCXMLAPPLICATION_H
#define LCXMLAPPLICATION_H

#include <QString>
#include <QDir>

class LIApplication;

class LCXmlMain
{
private:

  LCXmlMain();
  LCXmlMain(const LCXmlMain&) = delete;
  LCXmlMain& operator=(const LCXmlMain&) = delete;

public:
  static LCXmlMain& instance();
  static const LIApplication& getApplicationInterface();
  int exec(int argc, char *argv[]);
};

#endif // LCXMLAPPLICATION_H
