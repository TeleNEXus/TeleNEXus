#include "lcxmlwindows.h"
#include "lcxmlcommon.h"
#include "LIWindow.h"
#include "LIApplication.h"

#include <QDomElement>
#include <QMap>
#include <QWidget>
#include <QDebug>

//==============================================================================LCXmlWindow
class LCXmlWindow : public LIWindow
{
public:
    static QMap<QString, QSharedPointer<LIWindow>> smWindowsMap;
    QWidget* mpWidget;
public:
    LCXmlWindow(){}
    virtual ~LCXmlWindow()
    {
    }
    //--------------------------------------------------------------------------
    virtual void show() override
    {
        mpWidget->show();
    }

    //--------------------------------------------------------------------------
    virtual void hide() override
    {
        mpWidget->close();
    }
};

//------------------------------------------------------------------------------
QMap<QString, QSharedPointer<LIWindow>> LCXmlWindow::smWindowsMap;

//==============================================================================LCXmlWindows
LCXmlWindows::LCXmlWindows()
{
}

//------------------------------------------------------------------------------
LCXmlWindows::~LCXmlWindows()
{
}

//------------------------------------------------------------------------------
LCXmlWindows& LCXmlWindows::instance()
{
    static LCXmlWindows instance;
    return instance;
}

//------------------------------------------------------------------------------
QSharedPointer<LIWindow> LCXmlWindows::getWindow(const QString& _windowId)
{
    return LCXmlWindow::smWindowsMap.find(_windowId).value();
}

//------------------------------------------------------------------------------
#include "LIXmlWidgetBuilder.h"

static QWidget* buildWidget(const QDomElement& _element, 
        const LIApplication& _app);

static LCXmlWindow* createLocal(const QDomElement& _element, 
        const LIApplication& _app);

void LCXmlWindows::create(
        const QDomElement &_element, 
        const LIApplication& _app)
{
    auto window = createLocal(_element, _app); 
    if(window == nullptr) return;

    QString attr_id = _element.attribute(
            LCXmlCommon::mCommonAttributes.id);
    QString attr_title = _element.attribute(
            LCXmlCommon::mCommonAttributes.title);
    QString attr_show = _element.attribute(
            LCXmlCommon::mCommonAttributes.show.tag);


    if(!attr_id.isNull()) 
    {
        LCXmlWindow::smWindowsMap.insert(attr_id, 
                QSharedPointer<LIWindow>(window));
        if(attr_title.isNull()) attr_title = attr_id;
    }

    if(!attr_title.isNull())
    {
        window->mpWidget->setWindowTitle(attr_title);
    }


    if(!attr_show.isNull())
        window->mpWidget->show();
}

//------------------------------------------------------------------------------
static LCXmlWindow* createLocal(const QDomElement& _element, 
        const LIApplication& _app)
{
    QString attr_file =  _element.attribute(
            LCXmlCommon::mCommonAttributes.file);
    if(!attr_file.isNull())
    {
        QDomElement el = _app.getDomDocument(attr_file).documentElement();
        if(!el.isNull())
        {
            return createLocal(el, _app);
        }
        return nullptr;
    }

    LCXmlWindow* window = new LCXmlWindow();
    QWidget* widget = buildWidget(_element, _app);
    if(widget == nullptr) 
    {
        delete window;
        return nullptr;
    }

    window->mpWidget = widget;

    //Получение атрибутов.
    QString attr_width = _element.attribute(
            LCXmlCommon::mCommonAttributes.widht);

    QString attr_height = _element.attribute(
            LCXmlCommon::mCommonAttributes.height);

    //Переопределение размеров окна.
    QSize s = window->mpWidget->geometry().size();

    if(!attr_width.isNull())
    {
        bool flag = false;
        int width = attr_width.toInt(&flag);

        if(flag) 
        {
            s.setWidth(width);
        }
    }

    if(!attr_height.isNull())
    {
        bool flag = false;
        int height = attr_height.toInt(&flag);
        if(flag) 
        {
            s.setHeight(height);
        }
    }

    bool flag_posx = false;
    bool flag_posy = false;
    int posx;
    int posy;

    QString attr = _element.attribute(
            LCXmlCommon::mCommonAttributes.posx);
    posx = attr.toInt(&flag_posx);

    attr = _element.attribute(
            LCXmlCommon::mCommonAttributes.posy);
    posy = attr.toInt(&flag_posy);

    if(flag_posx && flag_posy)
    {
        window->mpWidget->move(posx, posy);
    }
    window->mpWidget->resize(s);
    return window;
}

//------------------------------------------------------------------------------
static QWidget* buildWidget(const QDomElement& _element, 
        const LIApplication& _app)
{
    for(auto node = _element.firstChild(); 
            !node.isNull(); 
            node = node.nextSibling())
    {
        if(node.isElement())
        {
            auto el = node.toElement();
            auto builder = _app.getWidgetBuilder(el.tagName());
            if(!builder.isNull())
            {
                auto widget = builder->build(el, _app);
                if(widget) return widget;
            }
        }
    }
    return nullptr;
}
