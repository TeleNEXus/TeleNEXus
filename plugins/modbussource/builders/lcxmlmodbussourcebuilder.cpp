/* 
 * TeleNEXus is a simple SCADA programm
 *
 * Copyright (C) 2020 Sergey S. Kuzmenko
 *
 * This file is part of TeleNEXus.
 *
 * TeleNEXus is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * TeleNEXus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with TeleNEXus.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "lcxmlmodbussourcebuilder.h"
#include "lqmodbusmasterbase.h"
#include "lqmodbusmasterrtu.h"
#include "lqmodbusmastertcp.h"
#include "lqmodbusdatasource.h"

#include "LIApplication.h"

#include <QDebug>
#include <QDomElement>

static const struct
{
  QString memorymap = "memorymap";
  QString modbussources = "modbussources";
  QString master = "master";
  QString source = "source";
}__slTags;

using LTMastersMap = QMap<QString, QSharedPointer<LQModbusMasterBase>>;

//==============================================================================
LCXmlModbusSourceBuilder::LCXmlModbusSourceBuilder()
{

}

//------------------------------------------------------------------------------
static LTMastersMap createMasters(const QDomNodeList& nodes);

//------------------------------------------------------------------------------
static void createSources(const QDomNodeList& nodes,
    LTMastersMap _masters,
    LQDataSources& _sourcesmap, const LIApplication& _app);

//------------------------------------------------------------------------------
LQDataSources LCXmlModbusSourceBuilder::build(
    const QDomElement &_element, 
    const LIApplication& _app)
{
  Q_UNUSED(_app);
  LQDataSources map;

  auto builder = 
    [&_app, &map](const QDomElement& _element)
    {
      QDomNodeList nodes = _element.elementsByTagName(__slTags.master);
      if(nodes.isEmpty())
      {
        _app.message("LCXmlModbusSources: no elements modbus masters");
        return;
      }
      LTMastersMap masters = createMasters(nodes);
      if(masters.isEmpty())
      {
        _app.message("LCXmlModbusSources: no valid modbus masters");
        return;
      }
      nodes = _element.elementsByTagName(__slTags.source);
      if(nodes.isEmpty())
      {
        _app.message("LCXmlModbusSources: no elements modbus source");
        return;
      }
      createSources(nodes, masters, map, _app);
    };

  _app.buildFromFile(_element, builder);

  return map;
}

//------------------------------------------------------------------------------
static int loadMemoryMap(LQModbusDataSource* _p_source, 
    const QString& _filename,
    const LIApplication& _app);

//------------------------------------------------------------------------------
static const struct
{
  QString name            = "name";
  QString master          = "master";
  QString deviceid        = "devid";
  QString updatetime      = "updatetime";
  QString memorymapfile   = "mapfile";
  QString maxbytes        = "maxbytes";

}__sourceAttributes;
//------------------------------------------------------------------------------
static void createSources(const QDomNodeList& nodes,
    LTMastersMap _masters,
    LQDataSources& _sourcesmap,
    const LIApplication& _app)
{
  QString attr;
  QString attrName;

  LTMastersMap::iterator itm;

  quint32 devid = 0;
  quint16 maxbytes = 0;
  bool boolBuff = false;

  for(int i = 0; i < nodes.size(); i++)
  {
    QDomElement el;
    el = nodes.at(i).toElement();
    if(el.isNull()) continue;

    attrName = el.attribute(__sourceAttributes.name);
    if(attrName.isNull()) continue;
    if(_sourcesmap.contains(attrName)) continue;

    attr = el.attribute(__sourceAttributes.master);
    if(attr.isNull()) continue;

    itm = _masters.find(attr);

    if(itm == _masters.end()) continue;

    attr = el.attribute(__sourceAttributes.deviceid);
    if(attr.isNull()) continue;

    devid = attr.toUInt(&boolBuff);

    if(!boolBuff) continue;
    if(devid > 255) continue;

    attr = el.attribute(__sourceAttributes.maxbytes);

    maxbytes = attr.toUInt(&boolBuff);
    if(!boolBuff)
    {
      maxbytes = 0;
    }

    attr = el.attribute(__sourceAttributes.memorymapfile);
    if(attr.isNull()) continue;

    QSharedPointer<LQModbusDataSource> source = 
      LQModbusDataSource::create(devid, itm.value(), maxbytes);

    if(loadMemoryMap(source.data(), attr, _app) != 0)
    {
      int updatetime;
      bool flag;

      _sourcesmap.insert(attrName, source);

      attr = el.attribute(__sourceAttributes.updatetime);

      updatetime = attr.toInt(&flag);

      if(flag)
      {
        source->start(updatetime);
      }
      else
      {
        source->start();
      }
    }
  }
}

//------------------------------------------------------------------------------
static const struct
{
  QString name        = "name";
  QString type        = "type";
  QString timeout     = "timeout";
  QString retries     = "retries";
}__mastersCommonAttributes;

//------------------------------------------------------------------------------
static QSharedPointer<LQModbusMasterBase> createMasterRtu(
    const QDomElement _element);
//------------------------------------------------------------------------------
static QSharedPointer<LQModbusMasterBase> createMasterTcp(
    const QDomElement _element);

//------------------------------------------------------------------------------
static LTMastersMap createMasters(const QDomNodeList& nodes)
{
  LTMastersMap map;
  QString attrType;
  QString attrName;
  for(int i = 0; i < nodes.size(); i++)
  {
    QDomElement el;
    el = nodes.at(i).toElement();

    attrName = el.attribute(__mastersCommonAttributes.name);
    if(attrName.isNull()) continue;
    if(map.contains(attrName))continue;

    attrType = el.attribute(__mastersCommonAttributes.type);
    if(attrType.isNull()) continue;

    QSharedPointer<LQModbusMasterBase> master;
    if(attrType == "rtu")
    {
      master = createMasterRtu(el);
      if(!master.isNull()) map.insert(attrName, master);
    }
    else if(attrType == "tcp")
    {
      master = createMasterTcp(el);
      if(!master.isNull()) map.insert(attrName, master);
    }
  }

  return map;
}

//------------------------------------------------------------------------------
static const struct
{
  QString port        = "portname";
  QString baudrate    = "baud";
  QString parity      = "parity";
  QString databits    = "databits";
  QString stopbits    = "stopbits";

  static QSerialPort::Parity getParity(const QString& _name)
  {
    static struct
    {
      QString noparity    = "no";
      QString evenparity  = "even";
      QString oddparity   = "odd";
      QString spaceparity = "space";
      QString markparity  = "mark";
    }parytyAttr;

    if(_name == parytyAttr.noparity)    return QSerialPort::Parity::NoParity;
    if(_name == parytyAttr.evenparity)  return QSerialPort::Parity::EvenParity;
    if(_name == parytyAttr.oddparity)   return QSerialPort::Parity::OddParity;
    if(_name == parytyAttr.spaceparity) return QSerialPort::Parity::SpaceParity;
    if(_name == parytyAttr.markparity)  return QSerialPort::Parity::MarkParity;
    return QSerialPort::Parity::UnknownParity;
  }

}__mastersRtuAttributes;


//------------------------------------------------------------------------------
static QSharedPointer<LQModbusMasterBase> createMasterRtu(
    const QDomElement _element)
{
  QString attr;

  QString portName;
  int baud;
  QSerialPort::Parity parity;
  int dataBits;
  int stopBits;
  int timeout;
  int retries;

  bool boolBuff = false;

  QSharedPointer<LQModbusMasterRtu> master;

  portName = _element.attribute(__mastersRtuAttributes.port);
  if(portName.isNull()) goto LABELRET;

  attr = _element.attribute(__mastersRtuAttributes.baudrate);
  baud = attr.toInt(&boolBuff);
  if(!boolBuff) goto LABELRET;

  attr = _element.attribute(__mastersRtuAttributes.parity);
  parity = __mastersRtuAttributes.getParity(attr);
  if(parity == QSerialPort::Parity::UnknownParity) goto LABELRET;


  attr = _element.attribute(__mastersRtuAttributes.databits);
  dataBits = attr.toInt(&boolBuff);
  if(!boolBuff || (dataBits < 0)) goto LABELRET;

  attr = _element.attribute(__mastersRtuAttributes.stopbits);
  if(attr.isNull()) goto LABELRET;

  stopBits = attr.toInt(&boolBuff);
  if(!boolBuff || (stopBits < 0)) goto LABELRET;

  master = LQModbusMasterRtu::create();

  attr = _element.attribute(__mastersCommonAttributes.timeout);
  timeout = attr.toInt(&boolBuff);
  if(!boolBuff)
  {
    timeout = 500;
  }

  boolBuff = false;

  attr = _element.attribute(__mastersCommonAttributes.retries);
  retries = attr.toInt(&boolBuff);
  if(!boolBuff)
  {
    retries = 3;
  }

  master->connectToPort(  portName,
      static_cast<QSerialPort::BaudRate>(baud),
      static_cast<QSerialPort::Parity>(parity),
      static_cast<QSerialPort::DataBits>(dataBits),
      static_cast<QSerialPort::StopBits>(stopBits),
      timeout,
      retries);

LABELRET:
  return master;
}

//------------------------------------------------------------------------------
static const struct
{
  QString url = "url";

}__mastersTcpAttributes;

//------------------------------------------------------------------------------
static QSharedPointer<LQModbusMasterBase> createMasterTcp(
    const QDomElement _element)
{
  QSharedPointer<LQModbusMasterTcp> master;
  QString attr = _element.attribute(__mastersTcpAttributes.url);
  int timeout;
  int retries;
  bool bool_buff = false;

  if(attr.isNull()) return master;

  QUrl url = QUrl::fromUserInput(attr);

  attr = _element.attribute(__mastersCommonAttributes.timeout);
  timeout = attr.toInt(&bool_buff);
  if(!bool_buff)
  {
    timeout = 500;
  }
  bool_buff= false;
  attr = _element.attribute(__mastersCommonAttributes.retries);
  retries = attr.toInt(&bool_buff);
  if(!bool_buff)
  {
    retries = 3;
  }

  if(url.isValid())
  {
    master = LQModbusMasterTcp::create();
    master->connectToHost(url, timeout, retries);
  }
  return master;
}

//------------------------------------------------------------------------------
enum class EItemType
{
  COILS,
  DISCRETEINPUTS,
  INPUTREGISTERS,
  HOLDINGREGISTERS
};

//------------------------------------------------------------------------------
static int addSourceDataItems(
    LQModbusDataSource* _p_source, 
    const QDomNodeList& _nodes, 
    EItemType _type,
    int _offset);


//------------------------------------------------------------------------------
static const struct
{
  QString bitsoffset        = "bitsoffset";
  QString coilsoffset       = "coilsoffset";
  QString discinsoffset     = "discinsoffset";
  QString regsoffset        = "regsoffset";
  QString inregsregsoffset  = "inregsoffset";
  QString holdregsoffset    = "holdregsoffset";
}__slMemoryMapAttributes;

static const struct
{
  QString coils               = "coil";
  QString discreteinputs      = "discin";
  QString inputregisters      = "inreg";
  QString holdingregisters    = "holdreg";
}__slMemoryMapTags;

//------------------------------------------------------------------------------
static int loadMemoryMap(LQModbusDataSource* _p_source,  
    const QString& _filename, 
    const LIApplication& _app)
{

  int items_counter = 0;

  auto loader = 
    [&items_counter, _p_source](const QDomElement& _element)
    {
      //-------------------------------------
      //get offsets
      auto getOffset = [&_element](const QString& _attr)
      {
        QString offsetattr;
        offsetattr = _element.attribute(_attr);
        if(offsetattr.isNull()) return 0;
        bool flag = false;
        int offset = offsetattr.toInt(&flag);
        if(!flag) return 0;
        return offset;
      };

      QDomNodeList itemnodes;

      int coilsoffset = 0;
      int discinsoffset = 0;

      int inregsoffset = 0;
      int holdregsoffset = 0;

      {
        int offset;
        int bitsoffset = getOffset(__slMemoryMapAttributes.bitsoffset);
        int regsoffset = getOffset(__slMemoryMapAttributes.regsoffset);

        offset = getOffset(__slMemoryMapAttributes.coilsoffset);
        coilsoffset = (offset == 0) ? (bitsoffset) : (offset);

        offset = getOffset(__slMemoryMapAttributes.discinsoffset);
        discinsoffset = (offset == 0) ? (bitsoffset) : (offset);

        offset = getOffset(__slMemoryMapAttributes.inregsregsoffset);
        inregsoffset = (offset == 0) ? (regsoffset) : (offset);

        offset = getOffset(__slMemoryMapAttributes.holdregsoffset);
        holdregsoffset = (offset == 0) ? (regsoffset) : (offset);
      }

      //-------------------------------------
      itemnodes = _element.elementsByTagName(__slMemoryMapTags.coils);
      if(!itemnodes.isEmpty())
      {
        items_counter += addSourceDataItems(
            _p_source, itemnodes, EItemType::COILS, coilsoffset);
      }

      itemnodes = _element.elementsByTagName(
          __slMemoryMapTags.discreteinputs);
      if(!itemnodes.isEmpty())
      {
        items_counter += addSourceDataItems(
            _p_source, itemnodes, EItemType::DISCRETEINPUTS, discinsoffset);
      }

      itemnodes = _element.elementsByTagName(
          __slMemoryMapTags.inputregisters);
      if(!itemnodes.isEmpty())
      {
        items_counter += addSourceDataItems(
            _p_source, itemnodes, EItemType::INPUTREGISTERS, inregsoffset);
      }

      itemnodes = _element.elementsByTagName(
          __slMemoryMapTags.holdingregisters);
      if(!itemnodes.isEmpty())
      {
        items_counter += addSourceDataItems(
            _p_source, itemnodes, EItemType::HOLDINGREGISTERS, holdregsoffset);
      }
    };

  QDomDocument domDoc = _app.loadDomDocument(_filename);

  if(domDoc.isNull()) 
  {
    return items_counter;
  }

  QDomElement rootElement = domDoc.documentElement();

  if(rootElement.tagName() != __slTags.memorymap)
  {
    qDebug() << 
      "LCXmlModbusSources: memory map file " <<  
      _filename << " wrong root element";
    return items_counter;
  }

  _app.buildFromFile(rootElement, loader);

  return items_counter;
}

//------------------------------------------------------------------------------
static const struct
{
  QString name = "name";
  QString addr = "addr";
  QString size = "size";
}__memoryMapItemAttributes;

//------------------------------------------------------------------------------
/*
 * Возвращает количество добавленных сущностей карты памяти.
 */
static int addSourceDataItems(
    LQModbusDataSource* _p_source, 
    const QDomNodeList& _nodes, 
    EItemType _type, 
    int _offset)
{
  QDomElement element;
  QString attr;
  QString name;
  int  addr;
  int  size;
  bool boolBuff;
  int  itemCounter = 0;

  for(int i = 0; i < _nodes.size(); i++)
  {
    element = _nodes.at(i).toElement();

    name = element.attribute(__memoryMapItemAttributes.name);
    if(name.isNull()) continue;

    attr = element.attribute(__memoryMapItemAttributes.addr);
    if(attr.isNull()) continue;

    addr = attr.toInt(&boolBuff);
    if( (!boolBuff) ||
        (addr < 0)  ||
        (addr > 0x0000ffff))
    {
      continue;
    }

    addr += _offset;

    attr = element.attribute(__memoryMapItemAttributes.size);
    if(attr.isNull())
    {
      size = 1;
    }
    else
    {
      size = attr.toInt(&boolBuff);
      if( (!boolBuff) ||
          (addr < 0)  ||
          (addr > 0x0000ffff))
      {
        continue;
      }
    }

    switch(_type)
    {
    case EItemType::COILS:
      _p_source->addDataItemCoils(name, addr, size);
      break;

    case EItemType::DISCRETEINPUTS:
      _p_source->addDataItemDiscreteInputs(name, addr, size);
      break;

    case EItemType::INPUTREGISTERS:
      _p_source->addDataItemInputRegs(name, addr, size);
      break;

    case EItemType::HOLDINGREGISTERS:
      _p_source->addDataItemHoldingRegs(name, addr, size);
      break;
    }
    itemCounter++;
  }
  return itemCounter;
}
