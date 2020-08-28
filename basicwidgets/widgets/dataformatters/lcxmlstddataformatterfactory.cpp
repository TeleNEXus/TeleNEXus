#include "lcxmlstddataformatterfactory.h"

#include "lcstringdataformatterhex.h"
#include "lcstringdataformatterbool.h"
#include "lcstringdataformatteru8.h"
#include "lcstringdataformatters8.h"
#include "lcstringdataformatteru16.h"
#include "lcstringdataformatters16.h"
#include "lcstringdataformatteru32.h"
#include "lcstringdataformatters32.h"
#include "lcstringdataformatterf32.h"

#include <QSharedPointer>

#include <functional>
#include <QMap>

static QSharedPointer<LCStringDataFormatterBase> __formatterHex;
static QSharedPointer<LCStringDataFormatterBase> __formatterBool;
static QSharedPointer<LCStringDataFormatterBase> __formatterUint8;
static QSharedPointer<LCStringDataFormatterBase> __formatterInt8;
static QSharedPointer<LCStringDataFormatterBase> __formatterUint16;
static QSharedPointer<LCStringDataFormatterBase> __formatterInt16;
static QSharedPointer<LCStringDataFormatterBase> __formatterUint32;
static QSharedPointer<LCStringDataFormatterBase> __formatterInt32;
static QSharedPointer<LCStringDataFormatterBase> __formatterFloat32;

//======================================================================================================================

using TFormatterCreators =
    QMap<QString, std::function<QSharedPointer<LCStringDataFormatterBase>(const QDomNamedNodeMap& _attr)>>;

TFormatterCreators __formatterCreators;

LCXmlStdDataFormatterFactory::LCXmlStdDataFormatterFactory()
{
    __formatterHex.reset(     new LCStringDataFormatterHex());
    __formatterBool.reset(    new LCStringDataFormatterBool());
    __formatterUint8.reset(   new LCStringDataFormatterU8());
    __formatterInt8.reset(    new LCStringDataFormatterS8());
    __formatterUint16.reset(  new LCStringDataFormatterU16());
    __formatterInt16.reset(   new LCStringDataFormatterS16());
    __formatterUint32.reset(  new LCStringDataFormatterU32());
    __formatterInt32.reset(   new LCStringDataFormatterS32());
    __formatterFloat32.reset( new LCStringDataFormatterF32());

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

//----------------------------------------------------------------------------------------------------------------------
QSharedPointer<LCStringDataFormatter> LCXmlStdDataFormatterFactory::createStringFormatter(const QDomNamedNodeMap& _attr)
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
