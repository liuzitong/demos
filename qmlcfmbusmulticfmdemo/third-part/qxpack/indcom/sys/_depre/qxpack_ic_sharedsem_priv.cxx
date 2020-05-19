#ifndef QXPACK_IC_SHAREDSEM_PRIV_CXX
#define QXPACK_IC_SHAREDSEM_PRIV_CXX

#include "qxpack_ic_sharedsem_priv.hxx"

#include <qxpack/indcom/common/qxpack_ic_memcntr.hxx>
#define QXPACK_IcPImplPrivTemp_new    qxpack_ic_new
#define QXPACK_IcPImplPrivTemp_delete qxpack_ic_delete
#include <qxpack/indcom/common/qxpack_ic_pimplprivtemp.hpp>

#include <QSemaphore>
#include <QDebug>

namespace QxPack {

// /////////////////////////////////////////////////
// private object
// /////////////////////////////////////////////////
#define T_PrivPtr( o )  T_ObjCast( IcSharedSemPrivPriv *, o )
class QXPACK_IC_HIDDEN IcSharedSemPrivPriv : public IcPImplPrivTemp< IcSharedSemPrivPriv > {
private: QSemaphore  m_sem;
public :
    IcSharedSemPrivPriv ( ) { }
    IcSharedSemPrivPriv ( const IcSharedSemPrivPriv & )
    { qFatal("IcSharedSemPrivPriv did not support PImpl Copy CTOR!"); }
    virtual ~IcSharedSemPrivPriv ( );
    inline void acquire   ( int n ) { m_sem.acquire(n); }
    inline int  available ( ) const { return m_sem.available();  }
    inline bool tryAcquire( int n ) { return m_sem.tryAcquire( n ); }
    inline bool tryAcquire( int n, int timeout ) { return m_sem.tryAcquire( n, timeout ); }
    inline void release   ( int n ) { m_sem.release(n); }
};
IcSharedSemPrivPriv :: ~IcSharedSemPrivPriv( ) { }


// //////////////////////////////////////////////////
// wrap API
// //////////////////////////////////////////////////
    IcSharedSemPriv :: IcSharedSemPriv ( )
{ m_obj = nullptr; }

    IcSharedSemPriv :: IcSharedSemPriv ( bool cr )
{ m_obj = nullptr; if ( cr ) { IcSharedSemPrivPriv::buildIfNull( & m_obj ); }}

    IcSharedSemPriv :: IcSharedSemPriv ( const IcSharedSemPriv &other )
{ m_obj = nullptr; IcSharedSemPrivPriv::attach( & m_obj, T_PtrCast( void **, & other.m_obj )); }

IcSharedSemPriv &  IcSharedSemPriv :: operator = ( const IcSharedSemPriv &other )
{ IcSharedSemPrivPriv::attach( & m_obj, T_PtrCast( void **, & other.m_obj )); return *this; }

    IcSharedSemPriv :: ~IcSharedSemPriv( )
{ if ( m_obj != nullptr ) { IcSharedSemPrivPriv::attach( & m_obj, nullptr ); }}

void  IcSharedSemPriv :: release ( int n )
{ if ( m_obj != nullptr ) { T_PrivPtr( m_obj )->release( n ); } }

void  IcSharedSemPriv :: acquire ( int n )
{ if ( m_obj != nullptr ) { T_PrivPtr( m_obj )->acquire( n ); } }

int   IcSharedSemPriv :: available ( ) const
{ return ( m_obj != nullptr ? T_PrivPtr( m_obj )->available() : 0 ); }

bool  IcSharedSemPriv :: tryAcquire( int n )
{ return ( m_obj != nullptr ? T_PrivPtr( m_obj )->tryAcquire( n ) : false ); }

bool  IcSharedSemPriv :: tryAcquire( int n, int timeout )
{ return ( m_obj != nullptr ? T_PrivPtr( m_obj )->tryAcquire( n, timeout ) : false ); }




}


#endif
