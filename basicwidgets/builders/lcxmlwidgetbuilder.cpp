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
  this->setObjectName(QString("LCWidget_%1").arg(mObjectCounter));
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
static QWidget* buildLocal(
        const QDomElement& _element, 
        const LIApplication& _app);

//------------------------------------------------------------------------------
QWidget* LCXmlWidgetBuilder::build( const QDomElement& _element, 
                                    const LIApplication& _app)
{

    QString attr;

    attr = _element.attribute(__attrName.file);

    if(!attr.isNull())
    {
        QDomElement el = _app.getDomDocument(attr).documentElement();
        if(!el.isNull())
        {
            if(el.tagName() == _element.tagName()) return build(el, _app);
        } 
    }

    return buildLocal(_element, _app);
}

//------------------------------------------------------------------------------
static void buildFromLayout(LCWidget* _widget, const QDomElement& _element, 
        const LIApplication& _app);
//------------------------------------------------------------------------------
static void buildFromWidgets(LCWidget* _widget, const QDomElement& _element, 
        const LIApplication& _app);
//------------------------------------------------------------------------------
static QWidget* buildLocal(
    const QDomElement& _element, 
    const LIApplication& _app)
{

  QDomNode  node = _element.firstChildElement(__slTags.layout);
  LCWidget* widget = new LCWidget;

  QString style = LCBuildersCommon::getBaseStyleSheet(_element, _app);

  style = QString("QWidget#%1{  %2 }").arg(widget->objectName()).arg(style);

  widget->setStyleSheet(style);

  if(!node.isNull()) 
  {
    buildFromLayout(widget, node.toElement(), _app);
  }
  else
  {
    node = _element.firstChildElement(__slTags.widgets);

    if(!node.isNull()) 
    {
      buildFromWidgets(widget, node.toElement(), _app);
    }
  }

  LCBuildersCommon::initPosition(_element, *widget);

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
         QWidget* addWidget = builder->build(el, _app);
         if(addWidget)
         {
             _widget->addWidget(addWidget);
         }
    }
}


