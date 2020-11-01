#include "lcxmllayoutbuilders.h"

#include "lcxmlapplication.h"

#include <QMap>
#include <QLibrary>
#include <QCoreApplication>

//==============================================================================
static QMap<QString, QSharedPointer<LIXmlLayoutBuilder>> __slBuildersMap;

//==============================================================================
LCXmlLayoutBuilders::LCXmlLayoutBuilders() :
        LCXmlBuildersLoader( LCXmlApplication::mBaseTags.layoutBuilders )
{
}

//------------------------------------------------------------------------------
LCXmlLayoutBuilders& LCXmlLayoutBuilders::instance()
{
    static LCXmlLayoutBuilders inst;
    return inst;
}

//------------------------------------------------------------------------------
QSharedPointer<LIXmlLayoutBuilder> 
LCXmlLayoutBuilders::getBuilder(const QString _name)
{
    auto it = __slBuildersMap.find(_name);
    if(it != __slBuildersMap.end())
    {
        return it.value();
    }
    return QSharedPointer<LIXmlLayoutBuilder>();
}

//------------------------------------------------------------------------------
bool LCXmlLayoutBuilders::add(const QString &_name, void *_builder)
{
    if(__slBuildersMap.find(_name) != __slBuildersMap.end()) return false;

    __slBuildersMap.insert(_name,
                         QSharedPointer<LIXmlLayoutBuilder>
                            (reinterpret_cast<LIXmlLayoutBuilder*>(_builder)));
    return true;
}
