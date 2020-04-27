#include "lcqremotedatasourcebase.h"

//=======================================================================================================CQEventDataRead
__LQ_EXTENDED_QEVENT_IMPLEMENTATION(LCQRemoteDataSourceBase::CQEventDataIsRead);

LCQRemoteDataSourceBase::
        CQEventDataIsRead::
            CQEventDataIsRead(const QByteArray& _data,
                              EDataStatus       _status) : QEvent(__LQ_EXTENDED_QEVENT_REGISTERED),
                                                           mData(new QByteArray(_data)),
                                                           mStatus(_status)
{
}

LCQRemoteDataSourceBase::
        CQEventDataIsRead::
            CQEventDataIsRead(EDataStatus _status) : QEvent(__LQ_EXTENDED_QEVENT_REGISTERED),
                                                     mData(new QByteArray),
                                                     mStatus(_status)
{
}

//======================================================================================================CQEventDataWrite
__LQ_EXTENDED_QEVENT_IMPLEMENTATION(LCQRemoteDataSourceBase::CQEventDataIsWrite);

LCQRemoteDataSourceBase::
        CQEventDataIsWrite::
            CQEventDataIsWrite(EDataStatus _status) :   QEvent(__LQ_EXTENDED_QEVENT_REGISTERED),
                                                        mStatus(_status)
{
}
