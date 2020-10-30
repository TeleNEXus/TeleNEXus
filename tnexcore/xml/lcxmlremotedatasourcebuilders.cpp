#include "lcxmlremotedatasourcebuilders.h"
#include "lcxmlapplication.h"

#include <QMap>
#include <QLibrary>
#include <QCoreApplication>

//======================================================================================================================
using LQWidgetBuildersMap = QMap<QString, QSharedPointer<LIXmlRemoteDataSourceBuilder>>;

//======================================================================================================================
static LQWidgetBuildersMap sl_BuildersMap;

//======================================================================================================================
LCXmlRemoteDataSourceBuilders::LCXmlRemoteDataSourceBuilders() :
        LCXmlBuildersLoader(    LCXmlApplication::mBaseTags.sourceBuilders)
{
}

//----------------------------------------------------------------------------------------------------------------------
LCXmlRemoteDataSourceBuilders& LCXmlRemoteDataSourceBuilders::instance()
{
    static LCXmlRemoteDataSourceBuilders inst;
    return inst;
}

//----------------------------------------------------------------------------------------------------------------------
QSharedPointer<LIXmlRemoteDataSourceBuilder> LCXmlRemoteDataSourceBuilders::getBuilder(const QString _name)
{
    auto it = sl_BuildersMap.find(_name);
    if(it != sl_BuildersMap.end())
    {
        return it.value();
    }
    return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------
bool LCXmlRemoteDataSourceBuilders::add(const QString &_name, void *_builder)
{
    if(sl_BuildersMap.find(_name) != sl_BuildersMap.end()) return false;

    sl_BuildersMap.insert(_name,
                         QSharedPointer<LIXmlRemoteDataSourceBuilder>
                            (reinterpret_cast<LIXmlRemoteDataSourceBuilder*>(_builder)));
    return true;
}

//----------------------------------------------------------------------------------------------------------------------
bool LCXmlRemoteDataSourceBuilders::noItems()
{
    return sl_BuildersMap.isEmpty();
}
