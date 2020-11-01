#include "lcxmlwindows.h"
#include "LIWindow.h"
#include "LIApplication.h"
#include <QDomElement>
#include <QMap>
#include <QWidget>

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
        mpWidget->hide();
    }
};

QMap<QString, QSharedPointer<LIWindow>> LCXmlWindow::smWindowsMap;
//==============================================================================
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
#include "lcxmlapplication.h"
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
            LCXmlApplication::mBaseTags.window.attrs.id);
    QString attr_title = _element.attribute(
            LCXmlApplication::mBaseTags.window.attrs.id);
    QString attr_show = _element.attribute(
            LCXmlApplication::mBaseTags.window.attrs.show.tag);

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
            LCXmlApplication::mBaseTags.window.attrs.file);
    if(!attr_file.isNull())
    {
        QDomElement el = _app.getDomDocument(attr_file).documentElement();
        if(!el.isNull())
        {
            return createLocal(el, _app);
        }
        return nullptr;
    }

    QWidget* widget = buildWidget(_element, _app);
    if(widget == nullptr) return nullptr;

    LCXmlWindow* window = new LCXmlWindow();
    window->mpWidget = widget;

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
