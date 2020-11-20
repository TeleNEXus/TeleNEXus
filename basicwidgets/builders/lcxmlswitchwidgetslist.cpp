
#include "lcxmlswitchwidgetslist.h"
#include "LIApplication.h"
#include <QDomElement>
#include <QDebug>
#include <QListWidget>
#include <QSplitter>
#include <QStackedWidget>
#include <qnamespace.h>
#include <QPicture>
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

    QString style = "background: green;";
    style += "color: yellow;" ;
    style += "font-size: 15pt;";
    /* style += "max-width: 25pt;"; */
    /* style += "max-width: 350pt;"; */
    style += "min-width: 300px;";
    style += "max-width: 300px;";
    /* style += "icon-size: 40px;"; */
    QString back_pimap_file("/home/serg/pprj/tnex/xmltestprj/linux/prj1/picture/fon3.png");
    style += QString("background-image:url(\"%1\"); background-position: center; " ).arg(back_pimap_file);
    style = QString(".QListWidget { %1 }").arg(style);
    /* style = QString(".QListView { %1 }").arg(style); */
    /* qDebug() << "List Widget style = " << style; */

    /* listWidget->setStyleSheet("font: 22pt;"); */
    /* listWidget->setStyleSheet("font: 6pt; min-width: 100pt;"); */

    listWidget->setStyleSheet(style);
    listWidget->style()->unpolish(listWidget);
    listWidget->style()->polish(listWidget);
    listWidget->update();

    
    /* qDebug() << "List Widget font = " << listWidget->font(); */

    int font_max_width = 0;
    QFont font;
    int icon_max_width = 0;

        int i = 0;
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
          QListWidgetItem *item = new QListWidgetItem;
          item->setText(attr_item_name);

            QPixmap pixmap(
                QString("/home/serg/pprj/tnex/xmltestprj/linux/prj1/picture/icon1.png"));
            pixmap = pixmap.scaled(60, 30);
            listWidget->setIconSize(QSize(60, 30));

            item->setIcon(QIcon(pixmap));
            
            if(i == 1) 
            {
              item->setText("Test item____________________________");
              QColor color("red");
              item->setTextColor(color);
              QFont item_font = item->font();
              item_font.setPointSize(15);
              item->setFont(item_font);
            }

            listWidget->addItem(item);
            stacked_widget->addWidget(widget);

            QFontMetrics font_metrics(item->font());

            qDebug() << "List Widget Item font = " << item->font();

            int text_width = font_metrics.width(item->text());
            font_max_width = (font_max_width < text_width) ? 
                (text_width):(font_max_width);
            icon_max_width = (icon_max_width < pixmap.width()) ? (pixmap.width()) : (icon_max_width);

            /* int text_width = font_metrics.width(attr_item_name); */
            /* qDebug() << "List Widget font metric = " << text_width; */
            /* font_max_width = (font_max_width < text_width) ? */ 
            /*     (text_width):(font_max_width); */
            i++;
        }
    }

    qDebug() << "List Widget font max width = " << font_max_width;

    if(font_max_width > 0)
    {
        /* listWidget->setMaximumWidth( */
        /*         font_max_width + font_metrics.width("    ")); */
        /* listWidget->setMinimumWidth( */
        /*         font_max_width + font_metrics.width("    ")); */
        /* listWidget->setMaximumWidth( */
        /*         font_max_width+icon_max_width + 5); */



        /* listWidget->setMinimumWidth( */
        /*         font_max_width+icon_max_width + 25); */
        /* listWidget->setMaximumWidth( */
        /*         font_max_width+icon_max_width + 25); */
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
