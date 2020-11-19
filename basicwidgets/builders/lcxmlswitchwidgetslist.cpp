
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
                if(el.tagName() == _element.tagName()) 
                    return this->build(el, _app);
            }
        }
    }

    QSplitter* splitter = new QSplitter(Qt::Orientation::Horizontal);
    QListWidget* listWidget = new QListWidget;
    QStackedWidget* stacked_widget  = new QStackedWidget;

    /* listWidget->setStyleSheet("font: 22pt;"); */
    /* listWidget->setStyleSheet("font: 6pt; min-width: 100pt;"); */


    QFontMetrics font_metrics(listWidget->font());

    int font_max_width = 0;

    for( QDomNode node = _element.firstChildElement(__sTags.item.name);
            !node.isNull();
            node = node.nextSiblingElement(__sTags.item.name))
    {
        QDomElement el = node.toElement();
        QString attr_item_name = el.attribute(__sTags.item.attr.name);
        if(attr_item_name.isNull()) continue;

        QWidget* widget = createWidget(el, _app);

        if(widget)
        {
            listWidget->addItem(attr_item_name);
            stacked_widget->addWidget(widget);
            int text_width = font_metrics.width(attr_item_name);
            font_max_width = (font_max_width < text_width) ? 
                (text_width):(font_max_width);
        }
    }

    if(font_max_width > 0)
    {
        listWidget->setMaximumWidth(
                font_max_width + font_metrics.width("    "));
    }

    splitter->addWidget(listWidget);
    splitter->addWidget(stacked_widget);
    

    QObject::connect(
            listWidget, 
            &QListWidget::currentRowChanged, 
            [stacked_widget](int i)
            {
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
