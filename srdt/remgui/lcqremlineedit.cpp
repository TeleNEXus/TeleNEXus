#include "lcqremlineedit.h"
#include <QDebug>
#include <QKeyEvent>
namespace remgui
{

LCQRemLineEdit::LCQRemLineEdit(const QString& _dataName,
                               QWeakPointer<LCQRemoteDataSourceBase> _dataSource,
                               QSharedPointer<LCDataStrFormatBase> _formatter,
                               QWidget* _parent) :  QLineEdit(_parent),
                                                    mDataReader(_dataName, _dataSource),
                                                    mDataWriter(_dataName, _dataSource),
                                                    mFormatter(_formatter)
{
    setText(mFormatter.data()->undefStateString());
    setValidator(_formatter->validator());
    setEnabled(false);
}

LCQRemLineEdit::LCQRemLineEdit(const QString& _dataNameRead,
                               const QString& _dataNameWrite,
                               QWeakPointer<LCQRemoteDataSourceBase> _dataSource,
                               QSharedPointer<LCDataStrFormatBase> _formatter,
                               QWidget* _parent) :  QLineEdit(_parent),
                                                    mDataReader(_dataNameRead,  _dataSource),
                                                    mDataWriter(_dataNameWrite, _dataSource),
                                                    mFormatter(_formatter)
{
    setText(mFormatter.data()->undefStateString());
    setValidator(_formatter->validator());
    setEnabled(false);
}

void LCQRemLineEdit::setActive(bool _flag)
{
    if(_flag)
    {
        if(!mConnection)
        {
            mConnection = connect( &mDataReader,
                                                &LCQRemoteDataReader::dataIsRead,
                                                this,
                                                &LCQRemLineEdit::updateText);
            mDataReader.connectToSource();
        }
    }else
    {
        if(mConnection)
        {
            setEnabled(false);
            if(mConnection) disconnect(mConnection);
            mDataReader.disconnectFromSource();
        }
    }
}

void LCQRemLineEdit::updateText(QSharedPointer<QByteArray> _data, LCQRemoteDataSourceBase::EDataStatus _status)
{
    if(_status != LCQRemoteDataSourceBase::EDataStatus::DS_OK)
                    {
                        setText(mFormatter.data()->undefStateString());
                        return;
                    }
                    setEnabled(true);
                    setText(mFormatter.data()->toString(*_data));
};

void LCQRemLineEdit::keyPressEvent(QKeyEvent *_ev)
{
    if((_ev->key() == Qt::Key_Enter) ||(_ev->key() == Qt::Key_Return))
    {
        mDataWriter.writeRequest(mFormatter->toBytes(text()));
        if(!mConnection) mConnection = connect( &mDataReader,
                                  &LCQRemoteDataReader::dataIsRead,
                                  this,
                                  &LCQRemLineEdit::updateText);
    }
    else
    {
        if(mConnection) disconnect(mConnection);
    }
    QLineEdit::keyPressEvent(_ev);
}

void LCQRemLineEdit::focusInEvent(QFocusEvent *_event)
{
    if(mConnection) disconnect(mConnection);
    QLineEdit::focusInEvent(_event);
}

void LCQRemLineEdit::focusOutEvent(QFocusEvent *_event)
{
    if(!mConnection) mConnection = connect(&mDataReader, &LCQRemoteDataReader::dataIsRead,
                              this, &LCQRemLineEdit::updateText);
    QLineEdit::focusOutEvent(_event);
}

//-----------------------------------------------------------------------------------------------------------------event
bool LCQRemLineEdit::event(QEvent *_event)
{
    bool ret = false;
    switch(_event->type())
    {
    case QEvent::Type::Show:
        setActive(true);
        ret = true;
        break;
    case QEvent::Type::Hide:
        setActive(false);
        ret = true;
        break;
    default:
        break;
    }
    QLineEdit::event(_event);
    return ret;
}

}
