#ifndef LCQREMWRITEBUTTON_H_
#define LCQREMWRITEBUTTON_H_

#include "LIRemoteDataWriteListner.h"
#include <QPushButton>

class LCQRemWriteButton : public QPushButton
{
private:
    class CWriteListener : public LIRemoteDataWriteListner 
    {
    public:
        CWriteListener();
        ~CWriteListener(){};
        virtual void dataIsWrite(LERemoteDataStatus _status) override {Q_UNUSED(_status);}
    };
    QByteArray mData;
private:
    explicit LCQRemWriteButton(QPushButton* _parent = nullptr);
    explicit LCQRemWriteButton(const QString& _text, QPushButton* _parent = nullptr);
public:
    explicit LCQRemWriteButton(
                const QString& _dataNameWrite, 
                const QByteArray _data, 
                const QString& _text, 
                QPushButton* _parent = nullptr);
};

#endif /* LCQREMWRITEBUTTON_H_ */
