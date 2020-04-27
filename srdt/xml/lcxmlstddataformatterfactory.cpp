#include "lcxmlstddataformatterfactory.h"

#include "lcdatastrformathex.h"
#include "lcdatastrformatbool.h"
#include "lcdatastrformatu8.h"
#include "lcdatastrformats8.h"
#include "lcdatastrformatu16.h"
#include "lcdatastrformats16.h"
#include "lcdatastrformatu32.h"
#include "lcdatastrformats32.h"
#include "lcdatastrformatf32.h"

#include <QSharedPointer>

#include <functional>
#include <QMap>

using namespace remgui;

static QSharedPointer<LCDataStrFormatBase> __formatterHex;
static QSharedPointer<LCDataStrFormatBase> __formatterBool;
static QSharedPointer<LCDataStrFormatBase> __formatterUint8;
static QSharedPointer<LCDataStrFormatBase> __formatterInt8;
static QSharedPointer<LCDataStrFormatBase> __formatterUint16;
static QSharedPointer<LCDataStrFormatBase> __formatterInt16;
static QSharedPointer<LCDataStrFormatBase> __formatterUint32;
static QSharedPointer<LCDataStrFormatBase> __formatterInt32;
static QSharedPointer<LCDataStrFormatBase> __formatterFloat32;

//======================================================================================================================

using TFormatterCreators =
    QMap<QString, std::function<QSharedPointer<LCDataStrFormatBase>(const QDomNamedNodeMap& _attr)>>;

TFormatterCreators __formatterCreators;

LCXmlStdDataFormatterFactory::LCXmlStdDataFormatterFactory()
{
    __formatterHex.reset(     new LCDataStrFormatHex());
    __formatterBool.reset(    new LCDataStrFormatBool());
    __formatterUint8.reset(   new LCDataStrFormatU8());
    __formatterInt8.reset(    new LCDataStrFormatS8());
    __formatterUint16.reset(  new LCDataStrFormatU16());
    __formatterInt16.reset(   new LCDataStrFormatS16());
    __formatterUint32.reset(  new LCDataStrFormatU32());
    __formatterInt32.reset(   new LCDataStrFormatS32());
    __formatterFloat32.reset( new LCDataStrFormatF32());

    __formatterCreators.insert("hex",
                               [](const QDomNamedNodeMap& _attr){Q_UNUSED(_attr); return __formatterHex;});
    __formatterCreators.insert("bool",
                               [](const QDomNamedNodeMap& _attr){Q_UNUSED(_attr); return __formatterBool;});
    __formatterCreators.insert("uint8",
                               [](const QDomNamedNodeMap& _attr){Q_UNUSED(_attr); return __formatterUint8;});
    __formatterCreators.insert("int8",
                               [](const QDomNamedNodeMap& _attr){Q_UNUSED(_attr); return __formatterInt8;});
    __formatterCreators.insert("uint16",
                               [](const QDomNamedNodeMap& _attr){Q_UNUSED(_attr); return __formatterUint16;});
    __formatterCreators.insert("int16",
                               [](const QDomNamedNodeMap& _attr){Q_UNUSED(_attr); return __formatterInt16;});
    __formatterCreators.insert("uint32",
                               [](const QDomNamedNodeMap& _attr){Q_UNUSED(_attr); return __formatterUint32;});
    __formatterCreators.insert("int32",
                               [](const QDomNamedNodeMap& _attr){Q_UNUSED(_attr); return __formatterInt32;});
    __formatterCreators.insert("float32",
                               [](const QDomNamedNodeMap& _attr){Q_UNUSED(_attr); return __formatterFloat32;});

}

//----------------------------------------------------------------------------------------------------------------------
LCXmlStdDataFormatterFactory& LCXmlStdDataFormatterFactory::instance()
{
    static LCXmlStdDataFormatterFactory ins;
    return ins;
}

QSharedPointer<LCStringDataFormatter> LCXmlStdDataFormatterFactory::create(const QDomNamedNodeMap& _attr)
{
    QDomNode node = _attr.namedItem("format");
    if(node.isNull()) return nullptr;

    QString format = node.toAttr().value();

    TFormatterCreators::const_iterator it = __formatterCreators.find(format);
    if(it == __formatterCreators.end())
    {
        return __formatterHex;
    }
    return it.value()(_attr);
}
