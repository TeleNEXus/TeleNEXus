#ifndef LCQREMWRITEBUTTON_H_
#define LCQREMWRITEBUTTON_H_

#include "LIRemoteDataSource.h"
#include "LIRemoteDataWriteListner.h"
#include <QPushButton>
#include <QList>

class LCQRemWriteButton : public QPushButton
{
private:
    class CWriteListener : public LIRemoteDataWriteListner 
    {
    private:
        QString mDataName;
    public:
        CWriteListener(){};
        CWriteListener(const QString& _dataName);
        ~CWriteListener(){};
        virtual void dataIsWrite(LERemoteDataStatus _status) override;
    };

    struct SWriteSet
    {
        QByteArray mData;
        QString mDataName;
        QSharedPointer<LIRemoteDataWriter> mWriter;
        QSharedPointer<CWriteListener> mListener;
        void write();
    };

    QList<SWriteSet> mListWriteSets;

public:
    explicit LCQRemWriteButton(QPushButton* _parent = nullptr);
    explicit LCQRemWriteButton(const QString& _text, QPushButton* _parent = nullptr);

    void addDataWrite(
            QSharedPointer<LIRemoteDataSource> _source, 
            const QString _dataName, 
            const QByteArray _data);
};

#endif /* LCQREMWRITEBUTTON_H_ */
