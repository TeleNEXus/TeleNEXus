#ifndef LCXMLWIDGETCREATORSMAP_H
#define LCXMLWIDGETCREATORSMAP_H

#include "lcxmlwidgetcreatorbase.h"

#include <QMap>

using LTXmlWidgetsCreators = QMap<QString, LCXmlWidgetCreatorBase*>;

class LCXmlWidgetCreatorsMap
{
private:
    LTXmlWidgetsCreators mMap;
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
