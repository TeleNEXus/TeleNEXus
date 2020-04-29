#include "lcxmlwidgetcreatorsmap.h"

//======================================================================================================================
LCXmlWidgetCreatorsMap LCXmlWidgetCreatorsMap::mInstance;

LCXmlWidgetCreatorsMap& LCXmlWidgetCreatorsMap::instace()
{
    return mInstance;
}
//----------------------------------------------------------------------------------------------------------------------
LCXmlWidgetCreatorsMap::LCXmlWidgetCreatorsMap()
{

}
//----------------------------------------------------------------------------------------------------------------------
LCXmlWidgetCreatorsMap::~LCXmlWidgetCreatorsMap()
{

}

//----------------------------------------------------------------------------------------------------------------------
bool LCXmlWidgetCreatorsMap::addCreator(const QString& _name, LCXmlWidgetCreatorInterface* _p_creator)
{
    if(mMap.contains(_name)) return false;
    mMap.insert(_name, _p_creator);
    return true;
}

//----------------------------------------------------------------------------------------------------------------------
LCXmlWidgetCreatorInterface* LCXmlWidgetCreatorsMap::getCreator(const QString& _name)
{
    QMap<QString, LCXmlWidgetCreatorInterface*>::iterator it;
    it = mMap.find(_name);
    if(it == mMap.end()) return nullptr;
    return it.value();
}
