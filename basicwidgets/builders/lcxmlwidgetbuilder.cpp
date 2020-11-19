#include "lcxmlwidgetbuilder.h"
#include "LIApplication.h"
#include "LIXmlLayoutBuilder.h"
#include <QWidget>
#include <QDomElement>
#include <QFile>
#include <QDebug>
#include <qnamespace.h>
#include "builderscommon.h"
#include <QFrame>

class CWidget : public QFrame
{
private:
    QSize mSizeHint;
public:
    explicit CWidget(QFrame* _parent=nullptr) : QFrame(_parent){this->setObjectName("MyObject");}
    virtual QSize sizeHint() const override
    {
        return QSize(mSizeHint.width()+5, mSizeHint.height()+5);
    }

    void addWidget(QWidget* _widget)
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
};
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
static void buildFromLayout(CWidget* _widget, const QDomElement& _element, 
        const LIApplication& _app);
//------------------------------------------------------------------------------
static void buildFromWidgets(CWidget* _widget, const QDomElement& _element, 
        const LIApplication& _app);
//------------------------------------------------------------------------------
static void setColor(QWidget* _widget, const QDomElement& _element,
        const LIApplication& _app);
//------------------------------------------------------------------------------
static QWidget* buildLocal(
        const QDomElement& _element, 
        const LIApplication& _app)
{

    QDomNode  node = _element.firstChildElement(__slTags.layout);
    CWidget* widget = new CWidget;

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

    LCWidgetBuildersCommon::initPosition(_element, *widget);
    LCWidgetBuildersCommon::initSize(_element, *widget);
    LCWidgetBuildersCommon::initFixedSize(_element, *widget);
    setColor(widget, _element, _app);

    return widget;
}

//------------------------------------------------------------------------------
static void buildFromLayout(CWidget* _widget, const QDomElement& _element, 
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
static void buildFromWidgets(CWidget* _widget, const QDomElement& _element, 
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

//------------------------------------------------------------------------------
static void setColor(QWidget* _widget, const QDomElement& _element,
        const LIApplication& _app)
{
  QString style = LCWidgetBuildersCommon::getBaseStyleSheet(_element, _app);

  QString attr = _element.attribute(
      LCWidgetBuildersCommon::mAttributes.picturebg);
  if(!attr.isNull())
  {
    attr = _app.getProjectPath() + attr;
    attr = QString("background-image:url(\"%1\"); background-position: center; " ).arg(attr);
    style += attr;
  }

   style = ".QFrame{ " + style + "}";
   qDebug() << "Widget builder style = " << style;
   _widget->setStyleSheet(style);
}


