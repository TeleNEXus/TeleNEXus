#ifndef LCXMLWIDGETCREATORSMAP_H
#define LCXMLWIDGETCREATORSMAP_H

#include "lcxmlwidgetcreatorinterface.h"

#include <QMap>

using LTXmlWidgetsCreators = QMap<QString, LCXmlWidgetCreatorInterface*>;

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
    bool addCreator(const QString& _name, LCXmlWidgetCreatorInterface* _p_creator);
    LCXmlWidgetCreatorInterface* getCreator(const QString& _name);
};

#endif // LCXMLWIDGETCREATORSMAP_H
