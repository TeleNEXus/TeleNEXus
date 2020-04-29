#ifndef LCXMLWIDGETCREATORSMAP_H
#define LCXMLWIDGETCREATORSMAP_H

#include "LCXmlWidgetCreatorsMapInterface.h"

#include <QMap>

class LCXmlWidgetCreatorsMap : public LCXmlWidgetsCreatorsMapInterface
{
private:
    QMap<QString, LCXmlWidgetCreatorInterface*> mMap;

private:
    static LCXmlWidgetCreatorsMap mInstance;

    LCXmlWidgetCreatorsMap();
    LCXmlWidgetCreatorsMap(const LCXmlWidgetCreatorsMap&) = delete;
    LCXmlWidgetCreatorsMap& operator=(const LCXmlWidgetCreatorsMap&) = delete;

public:
    ~LCXmlWidgetCreatorsMap();

    static LCXmlWidgetCreatorsMap& instace();

    bool addCreator(const QString& _name, LCXmlWidgetCreatorInterface* _p_creator) override;
    LCXmlWidgetCreatorInterface* getCreator(const QString& _name) override;
};

#endif // LCXMLWIDGETCREATORSMAP_H
