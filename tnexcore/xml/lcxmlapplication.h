#ifndef LCXMLAPPLICATION_H
#define LCXMLAPPLICATION_H

#include <QString>
#include <QDir>

class LCXmlApplication
{
public:
    //Базовые метки приложения.
    struct SBaseTags
    {
        QString rootTag         = "APPLICATION";
        QString sourceBuilders  = "SOURCEBUILDERS";
        QString widgetBuilders  = "WIDGETBUILDERS";
        QString layoutBuilders  = "LAYOUTBUILDERS";

        struct
        {
            QString tag = "SOURCES";
            struct
            {
                QString file = "file";
            }attrs;
        }sources;

        struct 
        {
            QString tag = "PLAGINPATHS";

            struct
            {
                struct
                {
                    QString tag = "item";
                    struct
                    {
                        QString path = "path";
                    }attrs;
                }item;
            }tags;
        }plaginPaths;

        struct 
        {
            QString tag = "WINDOW";

            struct
            {
                QString file    = "file";
                QString id      = "id";
                QString title   = "title";
                QString widht   = "width";
                QString height  = "height";
                QString posx    = "posx";
                QString posy    = "posy";

                struct
                {
                    QString tag = "show";
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
};

#endif // LCXMLAPPLICATION_H
