#ifndef QMLMSGBUSMULTIWINDEMO_MEMCNTR_HXX
#define QMLMSGBUSMULTIWINDEMO_MEMCNTR_HXX
 
#include "qmlmsgbusmultiwindemo_def.h"

#ifdef QMLMSGBUSMULTIWINDEMO_QT5_ENABLED
#include <QObject>
#endif


namespace qmlMsgbusMultiwinDemo {

// ////////////////////////////////////////////////////////////////////////////
//
// memory counter
//
// Tips:
// 1) enable memory trace.
//    define the QMLMSGBUSMULTIWINDEMO_CFG_MEM_TRACE
//    call  MemCntr::enableMemTrace( ) to enable or disable memory trace
//
// 2) use qmlmsgbusmultiwindemo_new() / qmlmsgbusmultiwindemo_delete() to manage the new / delete normal object
//
// 3) use qmlmsgbusmultiwindemo_new_qobj() / qmlmsgbusmultiwindemo_delete_qobj() to manage qobject
//
// ////////////////////////////////////////////////////////////////////////////
class QMLMSGBUSMULTIWINDEMO_API MemCntr {
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
#if defined( QMLMSGBUSMULTIWINDEMO_QT5_ENABLED )
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

#if defined( QMLMSGBUSMULTIWINDEMO_CFG_MEM_TRACE )

#define  qmlmsgbusmultiwindemo_new( t, ... )       ( qmlMsgbusMultiwinDemo::MemCntr::incrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t ),  static_cast< t *>( qmlMsgbusMultiwinDemo::MemCntr::_assertNonNullPtr( new t ( __VA_ARGS__ ), true )) )
#define  qmlmsgbusmultiwindemo_delete( o, t )      do{ qmlMsgbusMultiwinDemo::MemCntr::decrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t ); delete( static_cast< t *>( qmlMsgbusMultiwinDemo::MemCntr::_assertNonNullPtr( o, false )) ); }while(0)
#define  qmlmsgbusmultiwindemo_incr_new_cntr( t )  qmlMsgbusMultiwinDemo::MemCntr::incrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t )
#define  qmlmsgbusmultiwindemo_decr_new_cntr( t )  qmlMsgbusMultiwinDemo::MemCntr::decrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t )

#define  qmlmsgbusmultiwindemo_alloc( sz )         ( qmlMsgbusMultiwinDemo::MemCntr::incrNewCntr(1,__FILE__,__FUNCTION__,__LINE__), qmlMsgbusMultiwinDemo::MemCntr::_assertNonNullPtr( qmlMsgbusMultiwinDemo::MemCntr::allocMemory( sz ), true ))
#define  qmlmsgbusmultiwindemo_realloc( p, sz )    ( qmlMsgbusMultiwinDemo::MemCntr::incrNewCntr(( p != nullptr ? 0 : 1 ),__FILE__,__FUNCTION__,__LINE__), qmlMsgbusMultiwinDemo::MemCntr::_assertNonNullPtr( qmlMsgbusMultiwinDemo::MemCntr::reallocMemory( p, sz ), true ))
#define  qmlmsgbusmultiwindemo_free( p )           do{ qmlMsgbusMultiwinDemo::MemCntr::decrNewCntr(1,__FILE__,__FUNCTION__,__LINE__); qmlMsgbusMultiwinDemo::MemCntr::freeMemory( qmlMsgbusMultiwinDemo::MemCntr::_assertNonNullPtr( p, false )); }while(0)


#else

#define  qmlmsgbusmultiwindemo_new( t, ... )       ( qmlMsgbusMultiwinDemo::MemCntr::incrNewCntr(1 ), static_cast< t *>( qmlMsgbusMultiwinDemo::MemCntr::_assertNonNullPtr( new t ( __VA_ARGS__ ), true ) ))
#define  qmlmsgbusmultiwindemo_delete( o, t )      do{ qmlMsgbusMultiwinDemo::MemCntr::decrNewCntr(1); delete( static_cast< t *>( qmlMsgbusMultiwinDemo::MemCntr::_assertNonNullPtr( o, false ) )); }while(0)
#define  qmlmsgbusmultiwindemo_incr_new_cntr( t )  qmlMsgbusMultiwinDemo::MemCntr::incrNewCntr(1)
#define  qmlmsgbusmultiwindemo_decr_new_cntr( t )  qmlMsgbusMultiwinDemo::MemCntr::decrNewCntr(1)

#define  qmlmsgbusmultiwindemo_alloc( sz )         ( qmlMsgbusMultiwinDemo::MemCntr::incrNewCntr(1), qmlMsgbusMultiwinDemo::MemCntr::_assertNonNullPtr( qmlMsgbusMultiwinDemo::MemCntr::allocMemory( sz ), true ))
#define  qmlmsgbusmultiwindemo_realloc( p, sz )    ( qmlMsgbusMultiwinDemo::MemCntr::incrNewCntr(( p != nullptr ? 0 : 1 )), qmlMsgbusMultiwinDemo::MemCntr::_assertNonNullPtr( qmlMsgbusMultiwinDemo::MemCntr::reallocMemory( p, sz ), true ))
#define  qmlmsgbusmultiwindemo_free( p )           do{ qmlMsgbusMultiwinDemo::MemCntr::decrNewCntr(1); qmlMsgbusMultiwinDemo::MemCntr::freeMemory( qmlMsgbusMultiwinDemo::MemCntr::_assertNonNullPtr( p, false )); }while(0)

#endif

#define  qmlmsgbusmultiwindemo_curr_new_cntr           ( qmlMsgbusMultiwinDemo::MemCntr::currNewCntr())
#define  qmlmsgbusmultiwindemo_enable_mem_trace( sw )  qmlMsgbusMultiwinDemo::MemCntr::enableMemTrace( sw )
#define  qmlmsgbusmultiwindemo_enable_mem_cntr( sw )   qmlMsgbusMultiwinDemo::MemCntr::enableMemCntr( sw )



#ifdef QMLMSGBUSMULTIWINDEMO_QT5_ENABLED
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
 *     manual free it use qmlmsgbusmultiwindemo_delete_qobj( o )
 */
// ============================================================================
#define qmlmsgbusmultiwindemo_new_qobj( t, ... ) \
    ( \
     [=]( t* _t, const char *fn, const char *func, int ln, const char *type )-> t * { \
         return qobject_cast< t *>( qmlMsgbusMultiwinDemo::MemCntr::memCntOnQObj( _t, fn, func, ln, type )); \
     } \
    )( new t ( __VA_ARGS__ ), __FILE__, __FUNCTION__, __LINE__, #t )



#define qmlmsgbusmultiwindemo_delete_qobj( o ) \
    do{ qmlMsgbusMultiwinDemo::MemCntr::memCntOnDelObj( qobject_cast<QObject*>(o), false ); } while(0)

#define qmlmsgbusmultiwindemo_delete_qobj_later( o ) \
    do{ qmlMsgbusMultiwinDemo::MemCntr::memCntOnDelObj( qobject_cast<QObject*>(o), true ); } while(0)

#endif




#endif
