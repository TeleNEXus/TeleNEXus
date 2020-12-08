#include "lcqlocalsourcehiden.h"


//==============================================================================
LCQLocalSourceHiden::LCQLocalSourceHiden() :
  QObject(nullptr)
{
}

//------------------------------------------------------------------------------
LCQLocalSourceHiden::~LCQLocalSourceHiden()
{
}

//------------------------------------------------------------------------------
void LCQLocalSourceHiden::addItem(const QString& _id, const QByteArray& _data)
{
  mDataMap.addItem(_id, _data);
}

//------------------------------------------------------------------------------
void LCQLocalSourceHiden::addItem(const QString& _id, const QBitArray& _data)
{
  mDataMap.addItem(_id, _data);
}

