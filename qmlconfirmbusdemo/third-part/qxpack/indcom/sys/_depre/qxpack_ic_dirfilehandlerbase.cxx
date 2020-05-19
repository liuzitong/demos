#ifndef QXPACK_IC_DIRFILEHANDLERBASE_CXX
#define QXPACK_IC_DIRFILEHANDLERBASE_CXX

#include "qxpack_ic_dirfilehandlerbase.hxx"

namespace QxPack {

// /////////////////////////////////////////////////
//
// /////////////////////////////////////////////////
    IcDirFileHandlerBase :: IcDirFileHandlerBase ( )
{ m_obj = Q_NULLPTR; }

    IcDirFileHandlerBase :: ~IcDirFileHandlerBase( )
{ }

IcVarData  IcDirFileHandlerBase :: handle ( const QString &, const QString & )
{ return IcVarData(); }


}

#endif
