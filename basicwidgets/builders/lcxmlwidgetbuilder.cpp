#include "lcxmlwidgetbuilder.h"
#include "LIApplication.h"
#include "LIXmlLayoutBuilder.h"
#include <QWidget>
#include <QDomElement>
#include <QFile>
#include <QDebug>
#include "builderscommon.h"

class CWidget : public QWidget
{
private:
    QSize mSizeHint;
public:
    explicit CWidget(QWidget* _parent=nullptr) : QWidget(_parent){}
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
        qDebug() << "mSizeHint = " << mSizeHint;
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
static QWidget* buildFromLayout(const QDomElement& _element, 
        const LIApplication& _app);
//------------------------------------------------------------------------------
static QWidget* buildFromWidgets(const QDomElement& _element, 
        const LIApplication& _app);
//------------------------------------------------------------------------------
static QWidget* buildLocal(
        const QDomElement& _element, 
        const LIApplication& _app)
{

    QDomNode  node = _element.firstChildElement(__slTags.layout);
    QWidget* widget = nullptr;

    if(!node.isNull()) 
    {
        widget = buildFromLayout(node.toElement(), _app);
    }
    else
    {
        node = _element.firstChildElement(__slTags.widgets);

        if(!node.isNull()) 
        {
            widget = buildFromWidgets(node.toElement(), _app);
        }
        else
        {
            widget = new QWidget;
        }
    }

    LCWidgetBuildersCommon::initPosition(_element, *widget);
    LCWidgetBuildersCommon::initSize(_element, *widget);
    LCWidgetBuildersCommon::initFixedSize(_element, *widget);

    return widget;
}

//------------------------------------------------------------------------------
static QWidget* buildFromLayout(const QDomElement& _element, 
        const LIApplication& _app)
{
    QWidget* widget = new QWidget;

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
                    widget->setLayout(layout);
                    break;
                }
             }                 
         }
    }
    return widget;
}

//------------------------------------------------------------------------------
static QWidget* buildFromWidgets(const QDomElement& _element, 
        const LIApplication& _app)
{
    /* QWidget* widget = new QWidget; */
    CWidget* widget = new CWidget;

    int width = 0;
    int height = 0;

    for(QDomNode node = _element.firstChild();
            !node.isNull();
            node = node.nextSibling())
    {
         QDomElement el = node.toElement();
         if(el.isNull()) continue;
         auto builder = _app.getWidgetBuilder(el.tagName());
         QWidget* addWidget = builder->build(el, _app);
         if(addWidget)
         {
             widget->addWidget(addWidget);
             /* addwidget->setParent(widget); */
             /* addwidget->show(); */

             /* int wh = addwidget->geometry().bottomRight().x(); */

             /* width = (wh > width) ? */ 
             /*     (wh) : (width); */

             /* wh = addwidget->geometry().bottomRight().y(); */

             /* height = (wh > height) ? */ 
             /*     (wh) : (height); */
         }
    }

    /* if((width != 0) && (height != 0)) */
    /* { */
    /*     widget->resize(width, height); */
        /* widget->resize(widget->sizeHint().width(), widget->sizeHint().height()); */
    /* } */
    /* widget->adjustSize(); */
    qDebug() << "widget size =" << widget->size();
    
    return widget;
}

