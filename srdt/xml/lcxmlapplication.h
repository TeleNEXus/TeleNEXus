#ifndef LCXMLAPPLICATION_H
#define LCXMLAPPLICATION_H

#include <QString>
#include <QDir>

class LCXmlApplication
{
private:
    LCXmlApplication();
    LCXmlApplication(const LCXmlApplication&) = delete;
    LCXmlApplication& operator=(const LCXmlApplication&) = delete;
public:
    static LCXmlApplication& instance();
    int exec(int argc, char *argv[]);
    /*
     * Возвращает наименование главного файла Xml.
     */
    const QString& getXmlMainFileName();
    /*
     * Возвращает путь к главному файлу Xml.
     */
    const QString& getXmlMainFileWay();
    /*
     * Возвращает папку главного файла Xml.
     */
    const QDir&    getXmlMainFileDir();


private:

};

#endif // LCXMLAPPLICATION_H
