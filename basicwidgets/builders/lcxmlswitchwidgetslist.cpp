
#include "lcxmlswitchwidgetslist.h"
#include "LIApplication.h"
#include <QDomElement>
#include <QDebug>
#include <QListWidget>
#include <QSplitter>
#include <QStackedWidget>
#include <qnamespace.h>

//------------------------------------------------------------------------------
static const struct
{
    struct 
    {
        QString name = "item";
        struct
        {
            QString name = "name";
        }attr;
    }item;
} __sTags;

//------------------------------------------------------------------------------
static const struct
{
    QString file  = "file";
}__sAttrs;

//==============================================================================
LCXmlSwitchWidgetsListBuilder::LCXmlSwitchWidgetsListBuilder()
{
}

//------------------------------------------------------------------------------
LCXmlSwitchWidgetsListBuilder::~LCXmlSwitchWidgetsListBuilder()
{
}

//------------------------------------------------------------------------------
static QWidget* createWidget(
        const QDomElement& _element, 
        const LIApplication& _app);

QWidget* LCXmlSwitchWidgetsListBuilder::build(
        const QDomElement& _element, 
        const LIApplication& _app)
{

    QString file = _element.attribute(__sAttrs.file);
    //Рекурсивная загрузка виджета из файлов.
    if(!file.isNull()) 
    {
        QDomDocument doc = _app.getDomDocument(file);
        if(!doc.isNull()) 
        {
            QDomElement el = doc.documentElement();
            if(!el.isNull())
            {
                if(el.tagName() == _element.tagName()) return this->build(el, _app);
            }
        }
    }

    QSplitter* splitter = new QSplitter(Qt::Orientation::Horizontal);
    QListWidget* listWidget = new QListWidget;
    QStackedWidget* stacked_widget = new QStackedWidget;

    qDebug() << "switch widgets list 0";
    for( QDomNode node = _element.firstChildElement(__sTags.item.name);
            !node.isNull();
            node = node.nextSiblingElement(__sTags.item.name))
    {
        qDebug() << "switch widgets list 1";
        QDomElement el = node.toElement();
        QString attr_item_name = el.attribute(__sTags.item.attr.name);
        if(attr_item_name.isNull()) continue;

        qDebug() << "switch widgets list 2";
        QWidget* widget = createWidget(el, _app);

        if(widget)
        {
            qDebug() << "switch widgets list 3 item name = "<<attr_item_name;
            listWidget->addItem(attr_item_name);
            stacked_widget->addWidget(widget);
        }
    }

    splitter->addWidget(listWidget);
    splitter->addWidget(stacked_widget);

    QObject::connect(listWidget, &QListWidget::currentRowChanged, [=](int i){
            stacked_widget->setCurrentIndex(i);
            });
    return splitter;
}

//------------------------------------------------------------------------------
static QWidget* createWidget(
        const QDomElement& _element, 
        const LIApplication& _app)
{
    QWidget* widget = nullptr;
    for(QDomNode node = _element.firstChild();
            !node.isNull();
            node = node.nextSibling())
    {
        QDomElement el = node.toElement();
        if(el.isNull()) continue;

        auto builder = _app.getWidgetBuilder(el.tagName());
        if(builder.isNull()) continue;

        widget = builder->build(el, _app);
    }

    return widget;
}
