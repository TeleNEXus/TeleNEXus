#include "lcxmlwidgetcreatorsmap.h"

//======================================================================================================================
LCXmlWidgetCreatorsMap LCXmlWidgetCreatorsMap::mInstance;

LCXmlWidgetCreatorsMap& LCXmlWidgetCreatorsMap::instace()
{
    return mInstance;
}

LCXmlWidgetCreatorsMap::LCXmlWidgetCreatorsMap()
{

}

//----------------------------------------------------------------------------------------------------------------------
bool LCXmlWidgetCreatorsMap::addCreator(const QString& _name, LCXmlWidgetCreatorBase* _p_creator)
{
    if(mMap.contains(_name)) return false;
    mMap.insert(_name, _p_creator);
    return true;
}

//----------------------------------------------------------------------------------------------------------------------
LCXmlWidgetCreatorBase* LCXmlWidgetCreatorsMap::getCreator(const QString& _name)
{
    QMap<QString, LCXmlWidgetCreatorBase*>::iterator it;
    it = mMap.find(_name);
    if(it == mMap.end()) return nullptr;
    return it.value();
}
