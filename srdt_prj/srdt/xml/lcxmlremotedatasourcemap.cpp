#include "lcxmlremotedatasourcemap.h"

//======================================================================================================================
LCXmlRemoteDataSourceMap LCXmlRemoteDataSourceMap::mInstance;

LCXmlRemoteDataSourceMap& LCXmlRemoteDataSourceMap::instace()
{
    return mInstance;
}

LCXmlRemoteDataSourceMap::LCXmlRemoteDataSourceMap()
{

}

//----------------------------------------------------------------------------------------------------------------------
void LCXmlRemoteDataSourceMap::
    addRemoteDataSorce(const QString& _name, QSharedPointer<LIRemoteDataSource> _source)
{
    if(mMap.contains(_name)) return;
    mMap.insertMulti(_name, _source);
}

void LCXmlRemoteDataSourceMap::
    addRemoteDataSorce(const LQDataSources& sources)
{
    LQDataSources::const_iterator it;
    it = sources.begin();
    while(it != sources.end() )
    {
        if(!mMap.contains(it.key()))
        {
            mMap[it.key()] = it.value();
        }
        it++;
    }
}

//----------------------------------------------------------------------------------------------------------------------
QSharedPointer<LIRemoteDataSource> LCXmlRemoteDataSourceMap::getRemoteDataSorce(const QString& _name)
{
    LQDataSources::iterator it;
    it = mMap.find(_name);
    if(it == mMap.end()) return nullptr;
    return it.value();
}

