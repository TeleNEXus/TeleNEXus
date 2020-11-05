#include "builderscommon.h"
#include <QDomElement>
#include <QWidget>
#include <QDebug>

const LCWidgetBuildersCommon::SAttributes LCWidgetBuildersCommon::mAttributes;

void LCWidgetBuildersCommon::initPosition(const QDomElement& _element, QWidget* _widget)
{
    QString attr_posxy = _element.attribute(mAttributes.posx);
    bool flag = false;

    QPoint pos = _widget->pos();

    int posxy = attr_posxy.toInt(&flag);
    if(flag)
    {
        pos.setX(posxy);
    }

    attr_posxy = _element.attribute(mAttributes.posy);
    flag = false;
    posxy = attr_posxy.toInt(&flag);
    if(flag)
    {
        pos.setY(posxy);
    }
    qDebug() << "pos = " << pos;
    
    _widget->move(pos);

}

