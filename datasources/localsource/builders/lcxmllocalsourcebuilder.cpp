#include "lcxmllocalsourcebuilder.h"
#include "lclocalsource.h"
#include "LIApplication.h"

#include <QBitArray>
#include <QDomElement>
#include <QDebug>

static const struct
{
  QString id      = "id";
  QString file    = "file";
  QString mapfile = "mapfile";
  QString source  = "source";
  QString type    = "type";
  QString defval  = "defval";
  QString size    = "size";
}__slAttributes;

static const struct
{
  QString localsourcemap  = "localsourcemap";
  QString bits            = "bits";
  QString bytes           = "bytes";
}__slTags;

//==============================================================================
LCXmlLocalSourceBuilder::LCXmlLocalSourceBuilder()
{
}

//------------------------------------------------------------------------------
static LQDataSources buildLocal(
    const QDomElement& _element, const LIApplication& _app);
//------------------------------------------------------------------------------
LQDataSources LCXmlLocalSourceBuilder::build(
    const QDomElement &_element, 
    const LIApplication& _app)
{
  Q_UNUSED(_element);
  Q_UNUSED(_app);

  QString attr_file = _element.attribute(__slAttributes.file);
  if(!attr_file.isNull())
  {
    QDomElement el = _app.getDomDocument(attr_file).documentElement();
    if(el.isNull()) return LQDataSources();
    if(el.tagName() != _element.tagName()) return LQDataSources();
    return build(el, _app);
  }

  return buildLocal( _element, _app);
}

//==============================================================================buildLocal
static void buildSource(
    const QDomElement _element, 
    LQDataSources& _sources, 
    const LIApplication& _app);

//------------------------------------------------------------------------------
static LQDataSources buildLocal(
    const QDomElement& _element, const LIApplication& _app)
{

  LQDataSources sources;

  for(auto node  = _element.firstChildElement(__slAttributes.source);
      !node.isNull();
      node = node.nextSiblingElement(__slAttributes.source))
  {
    buildSource(node.toElement(), sources, _app);
  }
  return sources;
}

//==============================================================================buildSource
static void loadMap(
    LCLocalDataSource& _source, 
    const QDomElement& _map_element, 
    const LIApplication& _app);

static void buildSource(
    const QDomElement _element, 
    LQDataSources& _sources, 
    const LIApplication& _app)
{
  QString attr_id = _element.attribute(__slAttributes.id);
  if(attr_id.isNull()) return;
  QString attr_mapfile = _element.attribute(__slAttributes.mapfile);
  if(attr_mapfile.isNull()) return;
  QDomElement map_element = _app.getDomDocument(attr_mapfile).documentElement();
  if(map_element.isNull()) return;
  if(map_element.tagName() != __slTags.localsourcemap) return;

  auto source = LCLocalDataSource::create();
  loadMap(*source.data(), map_element, _app);

  _sources.insert(attr_id, source);
}

//==============================================================================loadMap
static void addItemBits(
    LCLocalDataSource& _source, 
    const QDomElement& _item_element, 
    const LIApplication& _app);

static void addItemBytes(
    LCLocalDataSource& _source, 
    const QDomElement& _item_element, 
    const LIApplication& _app);
//------------------------------------------------------------------------------
static void loadMap(
    LCLocalDataSource& _source, 
    const QDomElement& _map_element, 
    const LIApplication& _app)
{
  for(auto node =_map_element.firstChild();
     !node.isNull();
    node = node.nextSibling())
  {
    if(!node.isElement()) continue;
    QDomElement el = node.toElement();

    if(el.tagName() == __slTags.bits)
    {
      addItemBits(_source, el, _app);
    }
    else if(el.tagName() == __slTags.bytes)
    {
      addItemBytes(_source, el, _app);
    }
  } 
}


//==============================================================================addItemBits
static void addItemBits(
    LCLocalDataSource& _source, 
    const QDomElement& _item_element, 
    const LIApplication& _app)
{

  Q_UNUSED(_app);
  
  QString attr_id= _item_element.attribute(__slAttributes.id);
  if(attr_id.isNull()) return;

  QString attr = _item_element.attribute(__slAttributes.size);

  if(!attr.isNull())
  {
    int size = 0;
    size = attr.toInt();
    if(size <= 0) return;
    _source.addBitItem(attr_id, QBitArray(size, false));
    return;
  }

  attr = _item_element.attribute(__slAttributes.defval);
  if(attr.isNull()) return;

  //Удаление символов форматирования строки.
  attr.remove(QRegExp(  QStringLiteral("[ ]{1,}|[_]{1,}")));
  if(attr.size() == 0) return;
  //Проверка на посторонние символы.
  if( attr.contains( QRegExp( QStringLiteral("[^0-1]{1,}")))) return;

  //Формирование массива.
  QBitArray bits(attr.size(), false);
  for(int i = 0; i < attr.size(); i++)
  {
    if(attr.at(attr.size() - i) == QStringLiteral("1"))
    {
      bits[i] = true;
    }
  } 

  _source.addBitItem(attr_id, bits);
}

//==============================================================================addItemBytes
static void addItemBytes(
    LCLocalDataSource& _source, 
    const QDomElement& _item_element, 
    const LIApplication& _app)
{
  Q_UNUSED(_source);
  Q_UNUSED(_item_element);
  Q_UNUSED(_app);
}


