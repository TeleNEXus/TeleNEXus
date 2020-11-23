
#include <QApplication>
#include <QtWidgets>
#include <qglobal.h>
#include <qnamespace.h>
/* class MyProxyStyle : public QProxyStyle */
/* { */
/* public: */
/*     explicit MyProxyStyle(QStyle* _parent = nullptr): QProxyStyle(_parent){} */
/*     void drawItemText( QPainter* painter, */
/*             const QRect& rect, */
/*             int alignment, */
/*             const QPalette& palette, */
/*             bool enabled, */
/*             const QString& text, */
/*             QPalette::ColorRole textRole ) const */
/*     { */
/*         painter->save(); */
/*         painter->translate(160,50); */
/*         painter->rotate(-180); */

/*         QCommonStyle::drawItemText( painter, */
/*                 rect, */
/*                 alignment, */
/*                 palette, */
/*                 enabled, */
/*                 text, */
/*                 textRole ); */
/* qDebug() << "drawItemText"; */
/*         painter->restore(); */
/*     } */
/* }; */

/* class TabBar : public QTabBar */
/* { */
/* private: */
/*     QSize mSizeHint; */
/* public: */
 
/*     explicit TabBar(QWidget* parent=0) : QTabBar(parent), mSizeHint(50,25) */
/*     { */
/*         /1* setIconSize(QSize(18, 18)); *1/ */
/*         /1* this->setStyleSheet("QTabBar::tab { Alignment: left; color: red;}"); *1/ */
/*         /1* this->setStyleSheet("QTabBar::tab { color: red; }"); *1/ */
/*         /1* this->setElideMode(Qt::TextElideMode::ElideLeft); *1/ */
/*         this->setStyle(new MyProxyStyle(this->style())); */
/*         /1* this->setStyleSheet("QTabBar::tab { Alignment:horizontal; }"); *1/ */
/*     } */
 
/* protected: */
/*     virtual void tabInserted(int _index) override */
/*     { */
/*         Q_UNUSED(_index); */

/*         /1* QSize new_tab_size = *1/ */ 
/*         /1*     fontMetrics().size( *1/ */
/*         /1*             Qt::TextFlag::TextExpandTabs | *1/ */
/*         /1*             Qt::TextFlag::TextWordWrap, *1/ */ 
/*         /1*             tabText(_index)); *1/ */

/*         /1* if( (new_tab_size.height()  > mSizeHint.height() * 1.5)  || *1/ */
/*         /1*     (new_tab_size.width()   > mSizeHint.width()) ) *1/ */
/*         /1* { *1/ */
/*         /1*     mSizeHint = new_tab_size; *1/ */
/*         /1*     mSizeHint.setHeight(mSizeHint.height() * 1.5); *1/ */
/*         /1* } *1/ */
/*         /1* qDebug() << "Tab Insert size" << new_tab_size; *1/ */ 
        









/*         /1* if( (button->sizeHint().height() > mSizeHint.height()) || *1/ */
/*         /1*         (button->sizeHint().width() > mSizeHint.width()) ) *1/ */
/*         /1* { *1/ */
/*         /1*     mSizeHint = button->sizeHint(); *1/ */
/*         /1* } *1/ */



/*         /1* QLabel* button  = new QLabel(this->tabText(_index)); *1/ */
/*         /1* button->setStyleSheet("QLabel { text-align: left; }"); *1/ */
/*         /1* this->setTabText(_index, ""); *1/ */
/*         /1* this->setTabButton(_index, QTabBar::ButtonPosition::RightSide, button); *1/ */
/*     } */

/*     /1* virtual QSize tabSizeHint(int _index) const override *1/ */
/*     /1* { *1/ */
/*     /1*     Q_UNUSED(_index); *1/ */
/*     /1*     return mSizeHint; *1/ */
/*     /1* } *1/ */
/*     /1* virtual QSize tabSizeHint(int _index) const override *1/ */
/*     /1* { *1/ */
/*     /1*     /2* QSize new_tab_size = *2/ *1/ */ 
/*     /1*     /2*     fontMetrics().size( *2/ *1/ */
/*     /1*     /2*             Qt::TextFlag::TextExpandTabs | *2/ *1/ */
/*     /1*     /2*             Qt::TextFlag::TextWordWrap, *2/ *1/ */ 
/*     /1*     /2*             tabText(_index)); *2/ *1/ */

/*     /1*     /2*     new_tab_size.setHeight(new_tab_size.height() * 1.5); *2/ *1/ */

/*     /1*     /2* qDebug() << this->fontMetrics().size( *2/ *1/ */
/*     /1*     /2*         Qt::TextFlag::TextSingleLine | *2/ *1/ */ 
/*     /1*     /2*         Qt::TextFlag::TextWordWrap | *2/ *1/ */ 
/*     /1*     /2*         Qt::TextFlag::TextExpandTabs, *2/ *1/ */ 
/*     /1*     /2*         "asdfasdf___________________"); *2/ *1/ */
/*     /1*     /2* qDebug() << "Tab Bar count = " << this->count(); *2/ *1/ */
/*     /1*     /2* return QSize(150, 30); *2/ *1/ */
/*     /1*     /2* return QTabBar::tabSizeHint(_i); *2/ *1/ */
/*     /1*     /2* return mSizeHint; *2/ *1/ */
/*     /1*     /2* return new_tab_size; *2/ *1/ */
/*     /1*     qDebug() << " sizeHint = " << tabButton(_index, QTabBar::LeftSide)->sizeHint(); *1/ */
/*     /1*     return QSize( *1/ */
/*     /1*             tabButton(_index, QTabBar::LeftSide)->size().width(), *1/ */ 
/*     /1*             tabButton(_index, QTabBar::LeftSide)->size().height()); *1/ */
/*     /1* } *1/ */

/*     /1* virtual QSize tabSizeHint(int _index) const override *1/ */
/*     /1* { *1/ */
/*     /1*     return QSize(QTabBar::tabSizeHint(_index).height(), QTabBar::tabSizeHint(_index).width()); *1/ */
/*     /1* } *1/ */

/*     /1* virtual void paintEvent(QPaintEvent *) override *1/ */
/*     /1* { *1/ */
/*     /1*     QStylePainter p(this); *1/ */
/*     /1*     for (int index = 0; index < count(); index++) *1/ */
/*     /1*     { *1/ */
/*     /1*         QStyleOptionTab tab; *1/ */
/*     /1*         initStyleOption(&tab, index); *1/ */
/*     /1*         QIcon tempIcon = tab.icon; *1/ */
/*     /1*         QString tempText = tab.text; *1/ */
/*     /1*         tab.icon = QIcon(); *1/ */
/*     /1*         tab.text = QString(); *1/ */
/*     /1*         p.drawControl(QStyle::CE_TabBarTab, tab); *1/ */
/*     /1*         QPainter painter; *1/ */
/*     /1*         painter.begin(this); *1/ */
/*     /1*         QRect tabrect = tabRect(index); *1/ */
/*     /1*         painter.drawText(tabrect, Qt::AlignBottom | Qt::AlignHCenter, tempText); *1/ */
/*     /1*         tempIcon.paint(&painter, 0, tabrect.top(), tab.iconSize.width(), tab.iconSize.height(), Qt::AlignTop | Qt::AlignHCenter); *1/ */
/*     /1*         painter.end(); *1/ */
/*     /1*     } *1/ */
/*     /1*     /2* QStylePainter style_painter(this); *2/ *1/ */

/*     /1*     /2* for (int index = 0; index < count(); index++) *2/ *1/ */
/*     /1*     /2* { *2/ *1/ */

/*     /1*     /2*     QStyleOptionTab tab; *2/ *1/ */
/*     /1*     /2*     /3* initStyleOption(&tab, index); *3/ *2/ *1/ */
/*     /1*     /2*     /3* QPainter painter(this); *3/ *2/ *1/ */
/*     /1*     /2*     QRect tabrect = tabRect(index); *2/ *1/ */
/*     /1*     /2*     style_painter.drawControl(QStyle::CE_TabBarTab, tab); *2/ *1/ */

/*     /1*     /2*     style_painter.drawItemText( *2/ *1/ */
/*     /1*     /2*             tabrect, *2/ *1/ */ 
/*     /1*     /2*             Qt::AlignHCenter | Qt::AlignCenter, *2/ *1/ */  
/*     /1*     /2*             tab.palette, *2/ *1/ */ 
/*     /1*     /2*             true, *2/ *1/ */ 
/*     /1*     /2*             this->tabText(index), *2/ *1/ */ 
/*     /1*     /2*             QPalette::ColorRole::ButtonText); *2/ *1/ */ 




/*     /1*         /2* QStyleOptionTab tab; *2/ *1/ */

/*     /1*         /2* initStyleOption(&tab, index); *2/ *1/ */

/*     /1*         /2* /3* QIcon tempIcon = tab.icon; *3/ *2/ *1/ */
/*     /1*         /2* /3* QString tempText = tab.text; *3/ *2/ *1/ */
/*     /1*         /2* /3* tab.icon = QIcon(); *3/ *2/ *1/ */
/*     /1*         /2* /3* tab.text = QString(); *3/ *2/ *1/ */

/*     /1*         /2* QPainter painter(this); *2/ *1/ */
/*     /1*         /2* /3* painter.begin(this); *3/ *2/ *1/ */
/*     /1*         /2* QRect tabrect = tabRect(index); *2/ *1/ */

/*     /1*         /2* style_painter.drawControl(QStyle::CE_TabBarTab, tab); *2/ *1/ */
/*     /1*         /2* /3* painter.setPen(QPen(QColor(Qt::GlobalColor::blue))); *3/ *2/ *1/ */ 
/*     /1*         /2* /3* this->style()->drawItemText(&painter, tabrect, Qt::AlignHCenter | Qt::AlignCenter,  tab.palette, true, this->tabText(index)); *3/ *2/ *1/ */ 

/*     /1*         /2* style_painter.drawItemText(tabrect, Qt::AlignHCenter | Qt::AlignCenter,  tab.palette, false, this->tabText(index)); *2/ *1/ */ 


/*     /1*         /2* /3* this->style()->drawControl(QStyle::CE_TabBarTabLabel, &tab, &painter, this); *3/ *2/ *1/ */ 

/*     /1*         /2* /3* painter.drawText(tabrect, Qt::AlignHCenter | Qt::AlignCenter, this->tabText(index)); *3/ *2/ *1/ */
            
/*     /1*         /2* /3* painter.drawText( *3/ *2/ *1/ */
/*     /1*         /2* /3*         QPoint(tabrect.bottomLeft().x() +10, tabrect.bottomLeft().y()), *3/ *2/ *1/ */
/*     /1*         /2* /3*         this->tabText(index)); *3/ *2/ *1/ */
/*     /1*         /2* /3* tempIcon.paint(&painter, 0, tabrect.top(), tab.iconSize.width(), tab.iconSize.height(), Qt::AlignTop | Qt::AlignHCenter); *3/ *2/ *1/ */
/*     /1*         /2* /3* tempIcon.paint( *3/ *2/ *1/ */
/*     /1*         /2* /3*         &painter, *3/ *2/ *1/ */ 
/*     /1*         /2* /3*         0, *3/ *2/ *1/ */ 
/*     /1*         /2* /3*         tabrect.top(), *3/ *2/ *1/ */ 
/*     /1*         /2* /3*         tab.iconSize.width(), *3/ *2/ *1/ */ 
/*     /1*         /2* /3*         tab.iconSize.height(), *3/ *2/ *1/ */ 
/*     /1*         /2* /3*         Qt::AlignTop | Qt::AlignHCenter); *3/ *2/ *1/ */
/*     /1*         /2* painter.end(); *2/ *1/ */
/*     /1*     /2* } *2/ *1/ */
/*     /1* } *1/ */
/* }; */
 class TabBar: public QTabBar{
public:
    TabBar()
    {
        this->setStyleSheet("QTabBar::tab { color: red; }");
        /* this->setStyleSheet("QTabBar::tab { Alignment: bottom; }"); */
    }
    virtual QSize tabSizeHint(int index) const override
    {
        return QTabBar::tabSizeHint(index).transposed();
    }

protected:

    virtual void paintEvent(QPaintEvent * ) override
    {
        QStylePainter painter(this);
        QStyleOptionTab opt;

        for(int i = 0;i < count();i++)
        {
            initStyleOption(&opt,i);
            painter.drawControl(QStyle::CE_TabBarTabShape, opt);
            painter.save();

            QSize s = opt.rect.size();
            s.transpose();
            QRect r(QPoint(), s);
            r.moveCenter(opt.rect.center());
            opt.rect = r;

            QPoint c = tabRect(i).center();
            painter.translate(c);
            painter.rotate(90);
            painter.translate(-c);
            painter.drawControl(QStyle::CE_TabBarTabLabel, opt);
            opt.rect.size().transpose();
            painter.restore();
        }
    }
};

class LQTabWidget : public QTabWidget
{
public:
    explicit LQTabWidget(QWidget *parent = 0) : QTabWidget(parent)
    {
        setTabBar(new TabBar());
    }
};


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    /* LQTabWidget* tab = new LQTabWidget; */
    QTabWidget* tab = new QTabWidget;
    tab->setTabPosition(QTabWidget::TabPosition::North);
    tab->setIconSize(QSize(60,30));
    /* tab->addTab(new QWidget(), "tab0_______________________"); */
    /* tab->addTab(new QWidget(), "TAB1"); */
    QPixmap pixmap1 = QPixmap("/home/serg/pprj/tnex/xmltestprj/linux/prj1/picture/picture_1.png");
    QPixmap pixmap2 = QPixmap("/home/serg/pprj/tnex/xmltestprj/linux/prj1/picture/icon1.png");
    pixmap2 = pixmap2.scaled(60, 30);
    /* tab->addTab(new QWidget(), QIcon(pixmap1), ""); */
    tab->addTab(new QWidget(), QIcon(pixmap2), "");
    tab->addTab(new QWidget(), "tab 3");
    tab->addTab(new QWidget(), "tab 4");
    tab->addTab(new QWidget(), "tab 5");
    tab->addTab(new QWidget(), "tab 6");
    QString style;
    style += "color: red;";
    style += "background-color: green;";
    style += "font: 22pt;";
    /* style += "min-width: 150px;"; */
    /* style += "min-height: 50px;"; */
/* style = "background: green;" */
/*         " border: 2px solid #C4C4C3;" */
/*         " border-bottom-color: #C2C7CB; " */
/*         " border-top-left-radius: 4px;" */
/*         " border-top-right-radius: 4px;" */
/*         " min-width: 8ex;" */
/*         " padding: 2px;"; */
/* style = "background: green;" */
/*         " min-width: 8ex;" */
/*         " padding: 2px;"; */
    /* tab->setStyleSheet("QTabBar { " + style + " }" + "QTabBar:tab:selected { background: blue}"); */
    /* tab->setStyleSheet("QTabWidget::tab-bar { " + style + " }"); */
    /* style = "background: blue"; */
    /* tab->setStyleSheet("QTabBar:tab{ " + style + " }"); */
    /* style = "QTabBar{ " + style + " }" + "QTabBar:tab:selected { background: yellow}"; */
    style = "QTabBar{ " + style + " }" + "QTabBar:tab:selected { background: yellow}";
    /* style = "QTabBar{ " + style + " }"; */
    /* style = "QTabWidget QTabBar{ " + style + " }"; */
    tab->setStyleSheet(style);
    tab->show();
    return app.exec();
}
