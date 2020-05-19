#ifndef $D{Key2_ProjNameU}_MEMCNTR_HXX
#define $D{Key2_ProjNameU}_MEMCNTR_HXX
 
#include "$D{Key2_ProjNameL}_def.h"

#ifdef $D{Key2_ProjNameU}_QT5_ENABLED
#include <QObject>
#endif


namespace $D{Key2_ProjNameN} {

// ////////////////////////////////////////////////////////////////////////////
//
// memory counter
//
// Tips:
// 1) enable memory trace.
//    define the $D{Key2_ProjNameU}_CFG_MEM_TRACE
//    call  MemCntr::enableMemTrace( ) to enable or disable memory trace
//
// 2) use $D{Key2_ProjNameL}_new() / $D{Key2_ProjNameL}_delete() to manage the new / delete normal object
//
// 3) use $D{Key2_ProjNameL}_new_qobj() / $D{Key2_ProjNameL}_delete_qobj() to manage qobject
//
// ////////////////////////////////////////////////////////////////////////////
class $D{Key2_ProjNameU}_API MemCntr {
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
#if defined( $D{Key2_ProjNameU}_QT5_ENABLED )
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

#if defined( $D{Key2_ProjNameU}_CFG_MEM_TRACE )

#define  $D{Key2_ProjNameL}_new( t, ... )       ( $D{Key2_ProjNameN}::MemCntr::incrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t ),  static_cast< t *>( $D{Key2_ProjNameN}::MemCntr::_assertNonNullPtr( new t ( __VA_ARGS__ ), true )) )
#define  $D{Key2_ProjNameL}_delete( o, t )      do{ $D{Key2_ProjNameN}::MemCntr::decrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t ); delete( static_cast< t *>( $D{Key2_ProjNameN}::MemCntr::_assertNonNullPtr( o, false )) ); }while(0)
#define  $D{Key2_ProjNameL}_incr_new_cntr( t )  $D{Key2_ProjNameN}::MemCntr::incrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t )
#define  $D{Key2_ProjNameL}_decr_new_cntr( t )  $D{Key2_ProjNameN}::MemCntr::decrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t )

#define  $D{Key2_ProjNameL}_alloc( sz )         ( $D{Key2_ProjNameN}::MemCntr::incrNewCntr(1,__FILE__,__FUNCTION__,__LINE__), $D{Key2_ProjNameN}::MemCntr::_assertNonNullPtr( $D{Key2_ProjNameN}::MemCntr::allocMemory( sz ), true ))
#define  $D{Key2_ProjNameL}_realloc( p, sz )    ( $D{Key2_ProjNameN}::MemCntr::incrNewCntr(( p != nullptr ? 0 : 1 ),__FILE__,__FUNCTION__,__LINE__), $D{Key2_ProjNameN}::MemCntr::_assertNonNullPtr( $D{Key2_ProjNameN}::MemCntr::reallocMemory( p, sz ), true ))
#define  $D{Key2_ProjNameL}_free( p )           do{ $D{Key2_ProjNameN}::MemCntr::decrNewCntr(1,__FILE__,__FUNCTION__,__LINE__); $D{Key2_ProjNameN}::MemCntr::freeMemory( $D{Key2_ProjNameN}::MemCntr::_assertNonNullPtr( p, false )); }while(0)


#else

#define  $D{Key2_ProjNameL}_new( t, ... )       ( $D{Key2_ProjNameN}::MemCntr::incrNewCntr(1 ), static_cast< t *>( $D{Key2_ProjNameN}::MemCntr::_assertNonNullPtr( new t ( __VA_ARGS__ ), true ) ))
#define  $D{Key2_ProjNameL}_delete( o, t )      do{ $D{Key2_ProjNameN}::MemCntr::decrNewCntr(1); delete( static_cast< t *>( $D{Key2_ProjNameN}::MemCntr::_assertNonNullPtr( o, false ) )); }while(0)
#define  $D{Key2_ProjNameL}_incr_new_cntr( t )  $D{Key2_ProjNameN}::MemCntr::incrNewCntr(1)
#define  $D{Key2_ProjNameL}_decr_new_cntr( t )  $D{Key2_ProjNameN}::MemCntr::decrNewCntr(1)

#define  $D{Key2_ProjNameL}_alloc( sz )         ( $D{Key2_ProjNameN}::MemCntr::incrNewCntr(1), $D{Key2_ProjNameN}::MemCntr::_assertNonNullPtr( $D{Key2_ProjNameN}::MemCntr::allocMemory( sz ), true ))
#define  $D{Key2_ProjNameL}_realloc( p, sz )    ( $D{Key2_ProjNameN}::MemCntr::incrNewCntr(( p != nullptr ? 0 : 1 )), $D{Key2_ProjNameN}::MemCntr::_assertNonNullPtr( $D{Key2_ProjNameN}::MemCntr::reallocMemory( p, sz ), true ))
#define  $D{Key2_ProjNameL}_free( p )           do{ $D{Key2_ProjNameN}::MemCntr::decrNewCntr(1); $D{Key2_ProjNameN}::MemCntr::freeMemory( $D{Key2_ProjNameN}::MemCntr::_assertNonNullPtr( p, false )); }while(0)

#endif

#define  $D{Key2_ProjNameL}_curr_new_cntr           ( $D{Key2_ProjNameN}::MemCntr::currNewCntr())
#define  $D{Key2_ProjNameL}_enable_mem_trace( sw )  $D{Key2_ProjNameN}::MemCntr::enableMemTrace( sw )
#define  $D{Key2_ProjNameL}_enable_mem_cntr( sw )   $D{Key2_ProjNameN}::MemCntr::enableMemCntr( sw )



#ifdef $D{Key2_ProjNameU}_QT5_ENABLED
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
 *     manual free it use $D{Key2_ProjNameL}_delete_qobj( o )
 */
// ============================================================================
#define $D{Key2_ProjNameL}_new_qobj( t, ... ) \
    ( \
     [=]( t* _t, const char *fn, const char *func, int ln, const char *type )-> t * { \
         return qobject_cast< t *>( $D{Key2_ProjNameN}::MemCntr::memCntOnQObj( _t, fn, func, ln, type )); \
     } \
    )( new t ( __VA_ARGS__ ), __FILE__, __FUNCTION__, __LINE__, #t )



#define $D{Key2_ProjNameL}_delete_qobj( o ) \
    do{ $D{Key2_ProjNameN}::MemCntr::memCntOnDelObj( qobject_cast<QObject*>(o), false ); } while(0)

#define $D{Key2_ProjNameL}_delete_qobj_later( o ) \
    do{ $D{Key2_ProjNameN}::MemCntr::memCntOnDelObj( qobject_cast<QObject*>(o), true ); } while(0)

#endif




#endif
