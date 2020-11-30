#include "lcxmlwidgetbuilders.h"
#include "lcxmlcommon.h"

#include <QMap>
#include <QLibrary>
#include <QCoreApplication>

//======================================================================================================================
using LQLayoutBuildersMap = QMap<QString, QSharedPointer<LIXmlWidgetBuilder>>;

//======================================================================================================================
static LQLayoutBuildersMap __buildersMap;

//======================================================================================================================
LCXmlWidgetBuilders::LCXmlWidgetBuilders() :
        LCXmlBuildersLoader(    LCXmlCommon::mBaseTags.widgetBuilders)
{
}

//----------------------------------------------------------------------------------------------------------------------
LCXmlWidgetBuilders& LCXmlWidgetBuilders::instance()
{
    static LCXmlWidgetBuilders inst;
    return inst;
}

//----------------------------------------------------------------------------------------------------------------------
QSharedPointer<LIXmlWidgetBuilder> LCXmlWidgetBuilders::getBuilder(const QString _name)
{
    LQLayoutBuildersMap::iterator it = __buildersMap.find(_name);
    if(it != __buildersMap.end())
    {
        return it.value();
    }
    return QSharedPointer<LIXmlWidgetBuilder>();
}

//----------------------------------------------------------------------------------------------------------------------
bool LCXmlWidgetBuilders::add(const QString &_name, void *_builder)
{
    if(__buildersMap.find(_name) != __buildersMap.end()) return false;

    __buildersMap.insert(_name,
                         QSharedPointer<LIXmlWidgetBuilder>
                            (reinterpret_cast<LIXmlWidgetBuilder*>(_builder)));
    return true;
}
