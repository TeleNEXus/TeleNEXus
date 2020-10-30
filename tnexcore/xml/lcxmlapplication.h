#ifndef LCXMLAPPLICATION_H
#define LCXMLAPPLICATION_H

#include <QString>
#include <QDir>

class LCXmlApplication
{
public:

    struct SBaseTags
    {
        QString rootTag         = "APPLICATION";
        QString sourceBuilders  = "SOURCEBUILDERS";
        QString widgetBuilders  = "WIDGETBUILDERS";
        QString layoutBuilders  = "LAYOUTBUILDERS";
        QString builder         = "BUILDER";
        QString add             = "ADD";

        /* QString sources         = "SOURCES"; */
        struct
        {
            QString name = "SOURCES";
            struct
            {
                QString file = "file";
            }attrs;
        }sources;

        struct 
        {
            QString name = "PLAGINPATHS";

            struct
            {
                QString path = "path";
            }attrs;

        }plaginPaths;

        struct 
        {
            QString name = "WINDOW";

            struct
            {
                QString file    = "file";
                QString id      = "id";
                QString title   = "title";

                struct
                {
                    QString name = "show";
                    struct
                    {
                        QString yes = "yes";
                        QString no  = "no";
                    }vals;
                }show;
            }attrs;
        } window;

    };

    static const SBaseTags mBaseTags;

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
