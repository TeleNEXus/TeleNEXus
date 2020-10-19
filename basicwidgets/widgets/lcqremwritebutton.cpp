#include "lcqremwritebutton.h"

//==============================================================================
LCQRemWriteButton::LCQRemWriteButton(
                const QString& _dataNameWrite, 
                const QByteArray _data, 
                const QString& _text, 
                QPushButton* _parent) : QPushButton(_text, _parent)
{

    connect(this, 
            static_cast<void(LCQRemWriteButton::*)(void)>(
                &LCQRemWriteButton::pressed), 
            [&]()
            {
                 
            });
}

//------------------------------------------------------------------------------
