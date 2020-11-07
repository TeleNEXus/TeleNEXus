#include "builderscommon.h"
#include <QDomElement>
#include <QWidget>
#include <QDebug>

//==============================================================================
const LCWidgetBuildersCommon::SAttributes LCWidgetBuildersCommon::mAttributes;

//------------------------------------------------------------------------------
void LCWidgetBuildersCommon::initPosition(const QDomElement& _element, 
        QWidget& _widget)
{
    QString attr_pos = _element.attribute(mAttributes.posx);

    bool flag = false;

    int posx;
    int posy;

    posx = attr_pos.toInt(&flag);
    
    if(!flag)
    {
        return;
    }

    flag = false;

    attr_pos = _element.attribute(mAttributes.posy);
    posy = attr_pos.toInt(&flag);

    if(!flag)
    {
        return;
    }
    
    _widget.move(posx, posy);
}

//------------------------------------------------------------------------------
void LCWidgetBuildersCommon::initSize(const QDomElement& _element, 
        QWidget& _widget)
{
    QString attr_width = _element.attribute(mAttributes.width);
    QString attr_height = _element.attribute(mAttributes.height);
    
    int width;
    int height;

    bool flag;
    width = attr_width.toInt(&flag);
    if(!flag)
    {
        return;
    }

    height = attr_height.toInt(&flag);
    if(!flag)
    {
        return;
    }
    _widget.resize(width, height);
}

//------------------------------------------------------------------------------
void LCWidgetBuildersCommon::initFixedSize(const QDomElement& _element, 
        QWidget& _widget)
{
    QString attr_width = _element.attribute(mAttributes.fixwidth);
    QString attr_height = _element.attribute(mAttributes.fixheght);

    int width;
    int height;

    bool flag;
    width = attr_width.toInt(&flag);

    if(!flag)
    {
        return;
    }

    height = attr_height.toInt(&flag);

    if(!flag)
    {
        return;
    }
    _widget.setFixedSize(width, height);
}
