#include "lcxmllayoutbuilders.h"

#include "lcxmlapplication.h"

#include <QMap>
#include <QLibrary>
#include <QCoreApplication>

//======================================================================================================================
using LQLayoutBuildersMap = QMap<QString, QSharedPointer<LIXmlLayoutBuilder>>;

//======================================================================================================================
static LQLayoutBuildersMap __buildersMap;

//======================================================================================================================
LCXmlLayoutBuilders::LCXmlLayoutBuilders() :
        LCXmlBuildersLoader(    LCXmlApplication::mBaseTagNames.sourceBuilders,
                                LCXmlApplication::mBaseTagNames.builder,
                                LCXmlApplication::mBaseAttributeNames.file,
                                LCXmlApplication::mBaseAttributeNames.name,
                                LCXmlApplication::mBaseAttributeNames.lib,
                                LCXmlApplication::mBaseAttributeNames.libhandler)
{
}

//----------------------------------------------------------------------------------------------------------------------
LCXmlLayoutBuilders& LCXmlLayoutBuilders::instance()
{
    static LCXmlLayoutBuilders inst;
    return inst;
}

//----------------------------------------------------------------------------------------------------------------------
QSharedPointer<LIXmlLayoutBuilder> LCXmlLayoutBuilders::getBuilder(const QString _name)
{
    LQLayoutBuildersMap::iterator it = __buildersMap.find(_name);
    if(it != __buildersMap.end())
    {
        return it.value();
    }
    return QSharedPointer<LIXmlLayoutBuilder>();
}

//----------------------------------------------------------------------------------------------------------------------
bool LCXmlLayoutBuilders::add(const QString &_name, void *_builder)
{
    if(__buildersMap.find(_name) != __buildersMap.end()) return false;

    __buildersMap.insert(_name,
                         QSharedPointer<LIXmlLayoutBuilder>
                            (reinterpret_cast<LIXmlLayoutBuilder*>(_builder)));
    return true;
}
