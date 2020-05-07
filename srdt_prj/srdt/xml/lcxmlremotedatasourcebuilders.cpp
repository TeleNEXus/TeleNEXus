#include "lcxmlremotedatasourcebuilders.h"
#include "lcxmlapplication.h"

#include <QMap>
#include <QLibrary>
#include <QCoreApplication>

//======================================================================================================================
using LQWidgetBuildersMap = QMap<QString, QSharedPointer<LIXmlRemoteDataSourceBuilder>>;

//======================================================================================================================
static LQWidgetBuildersMap __buildersMap;

//======================================================================================================================
LCXmlRemoteDataSourceBuilders::LCXmlRemoteDataSourceBuilders() :
        LCXmlBuildersLoader(    LCXmlApplication::mBaseTagNames.sourceBuilders,
                                LCXmlApplication::mBaseTagNames.builder,
                                LCXmlApplication::mBaseAttributeNames.file,
                                LCXmlApplication::mBaseAttributeNames.name,
                                LCXmlApplication::mBaseAttributeNames.lib,
                                LCXmlApplication::mBaseAttributeNames.libhandler)
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
    LQWidgetBuildersMap::iterator it = __buildersMap.find(_name);
    if(it != __buildersMap.end())
    {
        return it.value();
    }
    return QSharedPointer<LIXmlRemoteDataSourceBuilder>();
}

//----------------------------------------------------------------------------------------------------------------------
bool LCXmlRemoteDataSourceBuilders::add(const QString &_name, void *_builder)
{
    if(__buildersMap.find(_name) != __buildersMap.end()) return false;

    __buildersMap.insert(_name,
                         QSharedPointer<LIXmlRemoteDataSourceBuilder>
                            (reinterpret_cast<LIXmlRemoteDataSourceBuilder*>(_builder)));
    return true;
}
