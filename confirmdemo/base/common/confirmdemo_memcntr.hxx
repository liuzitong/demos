#ifndef CONFIRMDEMO_MEMCNTR_HXX
#define CONFIRMDEMO_MEMCNTR_HXX
 
#include "confirmdemo_def.h"

#ifdef CONFIRMDEMO_QT5_ENABLED
#include <QObject>
#endif


namespace confirmDemo {

// ////////////////////////////////////////////////////////////////////////////
//
// memory counter
//
// Tips:
// 1) enable memory trace.
//    define the CONFIRMDEMO_CFG_MEM_TRACE
//    call  MemCntr::enableMemTrace( ) to enable or disable memory trace
//
// 2) use confirmdemo_new() / confirmdemo_delete() to manage the new / delete normal object
//
// 3) use confirmdemo_new_qobj() / confirmdemo_delete_qobj() to manage qobject
//
// ////////////////////////////////////////////////////////////////////////////
class CONFIRMDEMO_API MemCntr {
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
#if defined( CONFIRMDEMO_QT5_ENABLED )
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

#if defined( CONFIRMDEMO_CFG_MEM_TRACE )

#define  confirmdemo_new( t, ... )       ( confirmDemo::MemCntr::incrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t ),  static_cast< t *>( confirmDemo::MemCntr::_assertNonNullPtr( new t ( __VA_ARGS__ ), true )) )
#define  confirmdemo_delete( o, t )      do{ confirmDemo::MemCntr::decrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t ); delete( static_cast< t *>( confirmDemo::MemCntr::_assertNonNullPtr( o, false )) ); }while(0)
#define  confirmdemo_incr_new_cntr( t )  confirmDemo::MemCntr::incrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t )
#define  confirmdemo_decr_new_cntr( t )  confirmDemo::MemCntr::decrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t )

#define  confirmdemo_alloc( sz )         ( confirmDemo::MemCntr::incrNewCntr(1,__FILE__,__FUNCTION__,__LINE__), confirmDemo::MemCntr::_assertNonNullPtr( confirmDemo::MemCntr::allocMemory( sz ), true ))
#define  confirmdemo_realloc( p, sz )    ( confirmDemo::MemCntr::incrNewCntr(( p != nullptr ? 0 : 1 ),__FILE__,__FUNCTION__,__LINE__), confirmDemo::MemCntr::_assertNonNullPtr( confirmDemo::MemCntr::reallocMemory( p, sz ), true ))
#define  confirmdemo_free( p )           do{ confirmDemo::MemCntr::decrNewCntr(1,__FILE__,__FUNCTION__,__LINE__); confirmDemo::MemCntr::freeMemory( confirmDemo::MemCntr::_assertNonNullPtr( p, false )); }while(0)


#else

#define  confirmdemo_new( t, ... )       ( confirmDemo::MemCntr::incrNewCntr(1 ), static_cast< t *>( confirmDemo::MemCntr::_assertNonNullPtr( new t ( __VA_ARGS__ ), true ) ))
#define  confirmdemo_delete( o, t )      do{ confirmDemo::MemCntr::decrNewCntr(1); delete( static_cast< t *>( confirmDemo::MemCntr::_assertNonNullPtr( o, false ) )); }while(0)
#define  confirmdemo_incr_new_cntr( t )  confirmDemo::MemCntr::incrNewCntr(1)
#define  confirmdemo_decr_new_cntr( t )  confirmDemo::MemCntr::decrNewCntr(1)

#define  confirmdemo_alloc( sz )         ( confirmDemo::MemCntr::incrNewCntr(1), confirmDemo::MemCntr::_assertNonNullPtr( confirmDemo::MemCntr::allocMemory( sz ), true ))
#define  confirmdemo_realloc( p, sz )    ( confirmDemo::MemCntr::incrNewCntr(( p != nullptr ? 0 : 1 )), confirmDemo::MemCntr::_assertNonNullPtr( confirmDemo::MemCntr::reallocMemory( p, sz ), true ))
#define  confirmdemo_free( p )           do{ confirmDemo::MemCntr::decrNewCntr(1); confirmDemo::MemCntr::freeMemory( confirmDemo::MemCntr::_assertNonNullPtr( p, false )); }while(0)

#endif

#define  confirmdemo_curr_new_cntr           ( confirmDemo::MemCntr::currNewCntr())
#define  confirmdemo_enable_mem_trace( sw )  confirmDemo::MemCntr::enableMemTrace( sw )
#define  confirmdemo_enable_mem_cntr( sw )   confirmDemo::MemCntr::enableMemCntr( sw )



#ifdef CONFIRMDEMO_QT5_ENABLED
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
 *     manual free it use confirmdemo_delete_qobj( o )
 */
// ============================================================================
#define confirmdemo_new_qobj( t, ... ) \
    ( \
     [=]( t* _t, const char *fn, const char *func, int ln, const char *type )-> t * { \
         return qobject_cast< t *>( confirmDemo::MemCntr::memCntOnQObj( _t, fn, func, ln, type )); \
     } \
    )( new t ( __VA_ARGS__ ), __FILE__, __FUNCTION__, __LINE__, #t )



#define confirmdemo_delete_qobj( o ) \
    do{ confirmDemo::MemCntr::memCntOnDelObj( qobject_cast<QObject*>(o), false ); } while(0)

#define confirmdemo_delete_qobj_later( o ) \
    do{ confirmDemo::MemCntr::memCntOnDelObj( qobject_cast<QObject*>(o), true ); } while(0)

#endif




#endif
