#ifndef LCXMLWIDGETCREATORSMAP_H
#define LCXMLWIDGETCREATORSMAP_H

#include "lcxmlwidgetcreatorbase.h"

#include <QMap>

class LCXmlWidgetCreatorsMap
{
private:
    QMap<QString, LCXmlWidgetCreatorBase*> mMap;
private:
    static LCXmlWidgetCreatorsMap mInstance;
    LCXmlWidgetCreatorsMap();
    LCXmlWidgetCreatorsMap(const LCXmlWidgetCreatorsMap&) = delete;
    LCXmlWidgetCreatorsMap& operator=(const LCXmlWidgetCreatorsMap&) = delete;
public:
    static LCXmlWidgetCreatorsMap& instace();
    bool addCreator(const QString& _name, LCXmlWidgetCreatorBase* _p_creator);
    LCXmlWidgetCreatorBase* getCreator(const QString& _name);
};

#endif // LCXMLWIDGETCREATORSMAP_H
