#ifndef LCXMLWIDGETCREATORSMAPINTERFACE_H
#define LCXMLWIDGETCREATORSMAPINTERFACE_H

class QString;
class LCXmlWidgetCreatorInterface;

class LCXmlWidgetsCreatorsMapInterface
{
public:
    LCXmlWidgetsCreatorsMapInterface(){}
    virtual ~LCXmlWidgetsCreatorsMapInterface(){}
    virtual bool addCreator(const QString& _name, LCXmlWidgetCreatorInterface* _p_creator) = 0;
    virtual LCXmlWidgetCreatorInterface* getCreator(const QString& _name) = 0;
};

#endif // LCXMLWIDGETCREATORSMAPINTERFACE_H
