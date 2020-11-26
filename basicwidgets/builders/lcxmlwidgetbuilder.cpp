#include "lcxmlwidgetbuilder.h"
#include "lcbuilderscommon.h"
#include "LIApplication.h"
#include "LIXmlLayoutBuilder.h"
#include <QFrame>
#include <QDomElement>
#include <QFile>
#include <QDebug>

//==============================================================================
class LCWidget : public QFrame
{
private:
  static int mObjectCounter;
  QSize mSizeHint;
public:
  explicit LCWidget(QWidget* _parent=nullptr); 
  virtual QSize sizeHint() const override;
  void addWidget(QWidget* _widget);
};

//------------------------------------------------------------------------------
int LCWidget::mObjectCounter = 0;
//------------------------------------------------------------------------------
LCWidget::LCWidget(QWidget* _parent) : 
  QFrame(_parent)
{
  setObjectName(QString("LCWidget_%1").arg(mObjectCounter));
  mObjectCounter++;
}

//------------------------------------------------------------------------------
QSize LCWidget::sizeHint() const 
{
  return QSize(mSizeHint.width()+5, mSizeHint.height()+5);
}

//------------------------------------------------------------------------------
void LCWidget::addWidget(QWidget* _widget)
{
  int width   = mSizeHint.width();
  int height  = mSizeHint.height();
  _widget->setParent(this);
  _widget->show();

  if( _widget->geometry().bottomRight().x() > width)
  {
    width = _widget->geometry().bottomRight().x();
  }

  if( _widget->geometry().bottomRight().y() > height)
  {
    height = _widget->geometry().bottomRight().y();
  }
  mSizeHint = QSize(width, height);
}

//------------------------------------------------------------------------------
static const struct
{
  QString layout = "layout";
  QString widgets = "widgets";
} __slTags;

//------------------------------------------------------------------------------
static const struct
{
  QString file = "file";
} __attrName;

//==============================================================================
LCXmlWidgetBuilder::LCXmlWidgetBuilder()
{

}

//------------------------------------------------------------------------------
LCXmlWidgetBuilder::~LCXmlWidgetBuilder()
{

}

//------------------------------------------------------------------------------
static void buildFromLayout(LCWidget* _widget, const QDomElement& _element, 
    const LIApplication& _app);
//------------------------------------------------------------------------------
static void buildFromWidgets(LCWidget* _widget, const QDomElement& _element, 
    const LIApplication& _app);

//------------------------------------------------------------------------------
/* QWidget* LCXmlWidgetBuilder::buildLocal( */ 
/*     const QDomElement& _element, */ 
/*     const LIApplication& _app){ return nullptr; } */
//------------------------------------------------------------------------------
QWidget* LCXmlWidgetBuilder::buildLocal(QSharedPointer<SBuildData> _buildData)
{
  const QDomElement& element = _buildData->element;
  const LIApplication& app = _buildData->application;

  QDomNode  node = element.firstChildElement(__slTags.layout);
  LCWidget* widget = new LCWidget;

  QString style = LCBuildersCommon::getBaseStyleSheet(element, app);

  style = QString("QWidget#%1{  %2 }").arg(widget->objectName()).arg(style);

  widget->setStyleSheet(style);

  if(!node.isNull()) 
  {
    buildFromLayout(widget, node.toElement(), app);
  }
  else
  {
    node = element.firstChildElement(__slTags.widgets);

    if(!node.isNull()) 
    {
      buildFromWidgets(widget, node.toElement(), app);
    }
  }

  LCBuildersCommon::initPosition(element, *widget);

  return widget;
}


//------------------------------------------------------------------------------
static void buildFromLayout(LCWidget* _widget, const QDomElement& _element, 
    const LIApplication& _app)
{
  for(QDomNode node = _element.firstChild();
      !node.isNull();
      node = node.nextSibling())
  {
    QDomElement el = node.toElement();
    if(!el.isNull())
    {
      auto builder = _app.getLayoutBuilder(el.tagName());
      if(!builder.isNull())
      {
        QLayout* layout = (*builder).build(el, _app);
        if(layout)
        {
          _widget->setLayout(layout);
          break;
        }
      }                 
    }
  }
}

//------------------------------------------------------------------------------
static void buildFromWidgets(LCWidget* _widget, const QDomElement& _element, 
    const LIApplication& _app)
{

  for(QDomNode node = _element.firstChild();
      !node.isNull();
      node = node.nextSibling())
  {
    QDomElement el = node.toElement();
    if(el.isNull()) continue;
    auto builder = _app.getWidgetBuilder(el.tagName());
    if(builder.isNull()) continue;
    QWidget* build_widget = builder->build(el, _app);
    if(build_widget)
    {
      _widget->addWidget(build_widget);
    }
  }
}

