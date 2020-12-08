#include "lcqlocalsourcehiden.h"
#include "lcqlocaldatareader.h"
#include "lcqlocaldatawriter.h"

#include <QSharedPointer>


//==============================================================================
LCQLocalSourceHiden::LCQLocalSourceHiden()
{
}

//------------------------------------------------------------------------------
LCQLocalSourceHiden::~LCQLocalSourceHiden()
{
}

//------------------------------------------------------------------------------
void LCQLocalSourceHiden::connectReader(LCQLocalDataReader* _p_reader)
{
}

//------------------------------------------------------------------------------
void LCQLocalSourceHiden::disconnectReader(LCQLocalDataReader* _p_reader)
{
}

//------------------------------------------------------------------------------
void LCQLocalSourceHiden::read(LCQLocalDataReader* _p_reader)
{
}

//------------------------------------------------------------------------------
void LCQLocalSourceHiden::write( const QByteArray& _data, 
    LCQLocalDataWriter* _writer)
{
}
