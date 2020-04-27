#include "lcqremlabel.h"
#include "ltypedef.h"
#include <QCoreApplication>
#include <QDebug>

#define __WRONG_DATA_STRING "####"

namespace remgui
{

//===========================================================================================================LCQRemLabel
LCQRemLabel::LCQRemLabel(QWidget* _parent) : QLabel(_parent)
{
    setText(LCDataStrFormatBase::getUndefStateString());
}

LCQRemLabel::LCQRemLabel(QString _text, QWidget* _parent) : QLabel(_text, _parent)
{
}

LCQRemLabel::LCQRemLabel(const QString& _dataName,
                         QWeakPointer<LCQRemoteDataSourceBase> _dataSource,
                         QSharedPointer<LCDataStrFormatBase> _formatter,
                         QWidget* _parent) :    QLabel(_parent),
                                                mDataName(_dataName),
                                                mDataReader(mDataName, _dataSource),
                                                mFormatter(_formatter)
{
    setText(mFormatter.data()->undefStateString());
    connect(&mDataReader, &LCQRemoteDataReader::dataIsRead,
            [=](    QSharedPointer<QByteArray> _data,
                    LCQRemoteDataSourceBase::EDataStatus _status){
                if(_status != LCQRemoteDataSourceBase::EDataStatus::DS_OK)
                {
                    setText(mFormatter.data()->undefStateString());
                    return;
                }
                setText(mFormatter.data()->toString(*_data));
            });
}

LCQRemLabel::~LCQRemLabel()
{
    qDebug() << "LCQRemLabel::~LCQRemLabel()";
}

//-------------------------------------------------------------------------------------------------------------setActive
void LCQRemLabel::setActive(bool _flag)
{
    if(_flag)
    {
        mDataReader.connectToSource();
    }
    else
    {
        mDataReader.disconnectFromSource();
    }
}

//-----------------------------------------------------------------------------------------------------------------event
bool LCQRemLabel::event(QEvent *_event)
{
    bool ret = false;
    switch(_event->type())
    {
    case QEvent::Type::Show:
        mDataReader.connectToSource();
        ret = true;
        break;
    case QEvent::Type::Hide:
        mDataReader.disconnectFromSource();
        ret = true;
        break;
    default:
        break;
    }
    QLabel::event(_event);
    return ret;
}

}//namespcae remgui
