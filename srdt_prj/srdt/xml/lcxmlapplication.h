#ifndef LCXMLAPPLICATION_H
#define LCXMLAPPLICATION_H

#include <QString>
#include <QDir>

class LCXmlApplication
{
public:

    struct SBaseTagsNames
    {
        QString rootTag         = "APPLICATION";
        QString sourceBuilders  = "SOURCEBUILDERS";
        QString widgetBuilders  = "WIDGETBUILDERS";
        QString layoutBuilders  = "LAYOUTBUILDERS";
        QString builder         = "BUILDER";
        QString sources         = "SOURCES";
        QString widgets         = "WIDGETS";
    };

    static const SBaseTagsNames mBaseTagNames;

    struct SBaseAttributeNames
    {
        QString file        = "file";
        QString lib         = "lib";
        QString name        = "name";
        QString libhandler  = "libhandler";
        QString builder     = "builder";
    };

    static const SBaseAttributeNames mBaseAttributeNames;

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

    /*
     * Возвращает папку расположения исполняемого файла приложения.
     */
    const QString& getApplicationDirPath();

private:

};

#endif // LCXMLAPPLICATION_H
