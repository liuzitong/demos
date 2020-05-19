#ifndef QMLCFMBUSMULTICFMDEMO_MEMCNTR_HXX
#define QMLCFMBUSMULTICFMDEMO_MEMCNTR_HXX
 
#include "qmlcfmbusmulticfmdemo_def.h"

#ifdef QMLCFMBUSMULTICFMDEMO_QT5_ENABLED
#include <QObject>
#endif


namespace qmlCfmBusMulticfmDemo {

// ////////////////////////////////////////////////////////////////////////////
//
// memory counter
//
// Tips:
// 1) enable memory trace.
//    define the QMLCFMBUSMULTICFMDEMO_CFG_MEM_TRACE
//    call  MemCntr::enableMemTrace( ) to enable or disable memory trace
//
// 2) use qmlcfmbusmulticfmdemo_new() / qmlcfmbusmulticfmdemo_delete() to manage the new / delete normal object
//
// 3) use qmlcfmbusmulticfmdemo_new_qobj() / qmlcfmbusmulticfmdemo_delete_qobj() to manage qobject
//
// ////////////////////////////////////////////////////////////////////////////
class QMLCFMBUSMULTICFMDEMO_API MemCntr {
public:
    static int    currNewCntr( );
    static void*  _assertNonNullPtr( void* p, bool is_alloc ); // nw: 20180807 added
    static int    incrNewCntr( int cntr, const char *file = nullptr, const char *func = nullptr, int ln = 0, const char *type = nullptr );
    static int    decrNewCntr( int cntr, const char *file = nullptr, const char *func = nullptr, int ln = 0, const char *type = nullptr );
    static void   saveTraceInfoToFile( const char *fn );
    static void   enableMemTrace( bool sw );

    // nw: 2018/11 added
    static void*  allocMemory  ( size_t size );
    static void*  reallocMemory( void *old, size_t size );
    static void   freeMemory ( void *p );

    // nw: 2019/05 added
    static void   enableMemCntr( bool sw ); // default is true

    // nw: 2019/05/23 added for QT5
#if defined( QMLCFMBUSMULTICFMDEMO_QT5_ENABLED )
    static QObject*  memCntOnQObj  ( QObject*, const char *file, const char *func, int ln, const char *type );
    static void      memCntOnDelObj( QObject *o, bool is_later = false );
#endif

protected:
    MemCntr ( );
    virtual ~MemCntr( );
private:
    void *m_obj;
};

}

#if defined( QMLCFMBUSMULTICFMDEMO_CFG_MEM_TRACE )

#define  qmlcfmbusmulticfmdemo_new( t, ... )       ( qmlCfmBusMulticfmDemo::MemCntr::incrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t ),  static_cast< t *>( qmlCfmBusMulticfmDemo::MemCntr::_assertNonNullPtr( new t ( __VA_ARGS__ ), true )) )
#define  qmlcfmbusmulticfmdemo_delete( o, t )      do{ qmlCfmBusMulticfmDemo::MemCntr::decrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t ); delete( static_cast< t *>( qmlCfmBusMulticfmDemo::MemCntr::_assertNonNullPtr( o, false )) ); }while(0)
#define  qmlcfmbusmulticfmdemo_incr_new_cntr( t )  qmlCfmBusMulticfmDemo::MemCntr::incrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t )
#define  qmlcfmbusmulticfmdemo_decr_new_cntr( t )  qmlCfmBusMulticfmDemo::MemCntr::decrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t )

#define  qmlcfmbusmulticfmdemo_alloc( sz )         ( qmlCfmBusMulticfmDemo::MemCntr::incrNewCntr(1,__FILE__,__FUNCTION__,__LINE__), qmlCfmBusMulticfmDemo::MemCntr::_assertNonNullPtr( qmlCfmBusMulticfmDemo::MemCntr::allocMemory( sz ), true ))
#define  qmlcfmbusmulticfmdemo_realloc( p, sz )    ( qmlCfmBusMulticfmDemo::MemCntr::incrNewCntr(( p != nullptr ? 0 : 1 ),__FILE__,__FUNCTION__,__LINE__), qmlCfmBusMulticfmDemo::MemCntr::_assertNonNullPtr( qmlCfmBusMulticfmDemo::MemCntr::reallocMemory( p, sz ), true ))
#define  qmlcfmbusmulticfmdemo_free( p )           do{ qmlCfmBusMulticfmDemo::MemCntr::decrNewCntr(1,__FILE__,__FUNCTION__,__LINE__); qmlCfmBusMulticfmDemo::MemCntr::freeMemory( qmlCfmBusMulticfmDemo::MemCntr::_assertNonNullPtr( p, false )); }while(0)


#else

#define  qmlcfmbusmulticfmdemo_new( t, ... )       ( qmlCfmBusMulticfmDemo::MemCntr::incrNewCntr(1 ), static_cast< t *>( qmlCfmBusMulticfmDemo::MemCntr::_assertNonNullPtr( new t ( __VA_ARGS__ ), true ) ))
#define  qmlcfmbusmulticfmdemo_delete( o, t )      do{ qmlCfmBusMulticfmDemo::MemCntr::decrNewCntr(1); delete( static_cast< t *>( qmlCfmBusMulticfmDemo::MemCntr::_assertNonNullPtr( o, false ) )); }while(0)
#define  qmlcfmbusmulticfmdemo_incr_new_cntr( t )  qmlCfmBusMulticfmDemo::MemCntr::incrNewCntr(1)
#define  qmlcfmbusmulticfmdemo_decr_new_cntr( t )  qmlCfmBusMulticfmDemo::MemCntr::decrNewCntr(1)

#define  qmlcfmbusmulticfmdemo_alloc( sz )         ( qmlCfmBusMulticfmDemo::MemCntr::incrNewCntr(1), qmlCfmBusMulticfmDemo::MemCntr::_assertNonNullPtr( qmlCfmBusMulticfmDemo::MemCntr::allocMemory( sz ), true ))
#define  qmlcfmbusmulticfmdemo_realloc( p, sz )    ( qmlCfmBusMulticfmDemo::MemCntr::incrNewCntr(( p != nullptr ? 0 : 1 )), qmlCfmBusMulticfmDemo::MemCntr::_assertNonNullPtr( qmlCfmBusMulticfmDemo::MemCntr::reallocMemory( p, sz ), true ))
#define  qmlcfmbusmulticfmdemo_free( p )           do{ qmlCfmBusMulticfmDemo::MemCntr::decrNewCntr(1); qmlCfmBusMulticfmDemo::MemCntr::freeMemory( qmlCfmBusMulticfmDemo::MemCntr::_assertNonNullPtr( p, false )); }while(0)

#endif

#define  qmlcfmbusmulticfmdemo_curr_new_cntr           ( qmlCfmBusMulticfmDemo::MemCntr::currNewCntr())
#define  qmlcfmbusmulticfmdemo_enable_mem_trace( sw )  qmlCfmBusMulticfmDemo::MemCntr::enableMemTrace( sw )
#define  qmlcfmbusmulticfmdemo_enable_mem_cntr( sw )   qmlCfmBusMulticfmDemo::MemCntr::enableMemCntr( sw )



#ifdef QMLCFMBUSMULTICFMDEMO_QT5_ENABLED
// ////////////////////////////////////////////////////////////////////////////
//
//             added for QT5
//
// ////////////////////////////////////////////////////////////////////////////
// ============================================================================
/*!
 * @brief memory counted new QObject
 * @details user can use this macro to do count on classes inherit QObject
 * @warning This mechanism used QObject::destroyed signal, do not do disconnect like \n
 *     obj::disconnect() or QObject::disconnect( obj, 0,0,0 ) \n
 *     that will discard signals. If possible, use obj::blockSignals(true) to instead
 *     block emit signals.
 * @note  For third library object, maybe inner discard all signals, so user can \n
 *     manual free it use qmlcfmbusmulticfmdemo_delete_qobj( o )
 */
// ============================================================================
#define qmlcfmbusmulticfmdemo_new_qobj( t, ... ) \
    ( \
     [=]( t* _t, const char *fn, const char *func, int ln, const char *type )-> t * { \
         return qobject_cast< t *>( qmlCfmBusMulticfmDemo::MemCntr::memCntOnQObj( _t, fn, func, ln, type )); \
     } \
    )( new t ( __VA_ARGS__ ), __FILE__, __FUNCTION__, __LINE__, #t )



#define qmlcfmbusmulticfmdemo_delete_qobj( o ) \
    do{ qmlCfmBusMulticfmDemo::MemCntr::memCntOnDelObj( qobject_cast<QObject*>(o), false ); } while(0)

#define qmlcfmbusmulticfmdemo_delete_qobj_later( o ) \
    do{ qmlCfmBusMulticfmDemo::MemCntr::memCntOnDelObj( qobject_cast<QObject*>(o), true ); } while(0)

#endif




#endif
