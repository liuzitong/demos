// ////////////////////////////////////////////////////////////////////////////
// @author nightwing
// @history
//        2018/12/20  modified, for GCC 5 compiler
//        2019/04/29  modified
//        2019/06/17  modified for Android GCC
// ////////////////////////////////////////////////////////////////////////////
#ifndef QXPACK_IC_OBJECTCACHE_CXX
#define QXPACK_IC_OBJECTCACHE_CXX

#include "qxpack_ic_objectcache.hxx"
#include "qxpack/indcom/common/qxpack_ic_memcntr.hxx"
#include "qxpack/indcom/common/qxpack_ic_queuetemp.hpp"
#include "qxpack/indcom/common/qxpack_ic_logging.hxx"
#include <assert.h>
#include <chrono>  // C++ 11
#include <list>    // C++ 11
#include <cstdlib>

#ifdef __GNUC__ // in GCC 5, close below warnings
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#endif

namespace QxPack {

// ////////////////////////////////////////////////////////////////////////////
//
//                   cache content item & node
//
// ////////////////////////////////////////////////////////////////////////////
class QXPACK_IC_HIDDEN  IcObjectCache_Item {
private: int64_t m_tm_stamp; void *m_obj;
public :
    explicit IcObjectCache_Item ( ) { m_tm_stamp = 0; m_obj = nullptr; }
    IcObjectCache_Item ( int64_t t, void *o ) { m_tm_stamp = t; m_obj = o; }
    ~IcObjectCache_Item( ) { }
    inline int64_t  timeStamp() { return m_tm_stamp; }
    inline void* object() { return m_obj;  }
    inline void  setup ( int64_t tm, void *obj ) { m_tm_stamp = tm; m_obj = obj; }
};

// ////////////////////////////////////////////////////////////////////////////
//
//                      object cache private object
//
// ////////////////////////////////////////////////////////////////////////////
typedef  IcNodeQueueTemp<IcObjectCache_Item>::Node  IcObjCache_Node;

#define T_PrivPtr( o )  T_ObjCast( IcObjectCachePriv *, o )
class QXPACK_IC_HIDDEN  IcObjectCachePriv {
private:
    // NOTE: IcNodeQueueTemp is thread-safe for enqueue and dequeue in design
    IcNodeQueueTemp<IcObjectCache_Item> m_recyc_queue, m_free_queue;
    IcObjectCache::Callback m_cb;
    void  *m_cb_ctxt;  int  m_max_num;
protected:
    IcObjCache_Node * allocNode( );
    bool              freeNode ( IcObjCache_Node* );
    int64_t           timeStampFromNow( );
public :
    IcObjectCachePriv  ( IcObjectCache::Callback cb, void *ctxt, int max_num );
    ~IcObjectCachePriv ( );
    inline int  count ( ) { return m_recyc_queue.count(); }
    void *   allocate  ( );
    void     recycle   ( void* );
    void     freeOldest( int limit_mill );
};

// ============================================================================
// ctor
// ============================================================================
IcObjectCachePriv :: IcObjectCachePriv (
    IcObjectCache::Callback cb, void *ctxt, int max_num
) : m_recyc_queue( nullptr, nullptr ), m_free_queue( nullptr, nullptr )
{
    m_cb = cb; m_cb_ctxt = ctxt;
    m_max_num  = ( max_num >= 0 && max_num < 4 ? 4 : max_num );
}

// ============================================================================
// dtor
// ============================================================================
IcObjectCachePriv :: ~IcObjectCachePriv ( )
{
    while ( ! m_recyc_queue.isEmpty() ) {
        this->freeNode( m_recyc_queue.dequeue() );
    }

    while ( ! m_free_queue.isEmpty() ) {
        this->freeNode( m_free_queue.dequeue() );
    }
}

// ============================================================================
// [ protected ] free a node
// ============================================================================
bool   IcObjectCachePriv :: freeNode( IcObjCache_Node *n )
{
    if ( n == nullptr ) { return false; }

    if ( m_cb != nullptr ) {
        void *obj = n->data().object();
        if ( obj != nullptr ) {
            m_cb( m_cb_ctxt, &obj, IcObjectCache::HandlerType_Free );
        }
        qxpack_ic_delete( n, IcObjCache_Node );
    } else {
        //std::fprintf(stderr, "[ IcObjectCachePriv::freeNode ] no callback function!");
        //std::abort();
        qxpack_ic_fatal( "no callback function!" );
    }

    return true;
}

// ============================================================================
// [ protected ] allocate the node, used for carry the object
// ============================================================================
IcObjCache_Node *   IcObjectCachePriv :: allocNode()
{
    IcObjCache_Node *n = m_free_queue.dequeue();
    if ( n == nullptr ) { n = qxpack_ic_new( IcObjCache_Node ); }
    return n;
}

// ============================================================================
// [ protected ] time stamp from now
// ============================================================================
int64_t   IcObjectCachePriv :: timeStampFromNow()
{
    auto now    = std::chrono::steady_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto value  = now_ms.time_since_epoch();
    int64_t tm_ms = value.count();
    return tm_ms;
}

// ============================================================================
// allocate new object
// ============================================================================
void *    IcObjectCachePriv :: allocate( )
{
    void *obj = nullptr;
    IcObjCache_Node *n = m_recyc_queue.dequeue();

    if ( n == nullptr ) {
        m_cb( m_cb_ctxt, & obj, IcObjectCache::HandlerType_Alloc );
    } else {
        obj = n->data().object();
        n->setData( IcObjectCache_Item( 0, nullptr ) );
        m_free_queue.takeAndEnqueue( n );
    }

    return obj;
}

// ============================================================================
// recycle the objects
// ============================================================================
void         IcObjectCachePriv :: recycle ( void *obj )
{
    if ( obj == nullptr ) { return; }

    // ------------------------------------------------------------------------
    // check if the cached number is over the maximum number
    // ------------------------------------------------------------------------
    if ( m_max_num > 0  &&  m_recyc_queue.count() >= m_max_num ) {
        m_cb( m_cb_ctxt, & obj, IcObjectCache::HandlerType_Free );
        return;
    }

    // ------------------------------------------------------------------------
    // get a carrier, used to store the object pointer
    // ------------------------------------------------------------------------
    IcObjCache_Node *node = this->allocNode();
    node->setData( IcObjectCache_Item( this->timeStampFromNow(), obj ));
    m_recyc_queue.takeAndEnqueue( node );
}

// ============================================================================
// free the oldest item
// ============================================================================
void           IcObjectCachePriv :: freeOldest ( int over_mill )
{
    struct DequParam { int64_t m_over_mill, m_now_tm; } deq_param;

    deq_param.m_now_tm    = this->timeStampFromNow();
    deq_param.m_over_mill = ( over_mill >= 0 ? over_mill : 0 );

    // ------------------------------------------------------------------------
    // check node in the recyc queue, if node is need to free ( over the mill ),
    //
    // ------------------------------------------------------------------------
    IcObjCache_Node *n =  nullptr; int curr_max_freeoldest = 0;
    do {
        n = m_recyc_queue.dequeue (
            []( IcObjCache_Node* node, void *ctxt )->bool {
                DequParam *deq_param = reinterpret_cast<DequParam*>( ctxt );
                int64_t tm_stamp = node->data().timeStamp() + deq_param->m_over_mill;
                return ( deq_param->m_now_tm >= tm_stamp ); // is the time stamp has been passed?
            }, & deq_param
        );
        if ( n != nullptr ) { this->freeNode( n ); ++ curr_max_freeoldest; }
    } while ( n != nullptr );
}

// ////////////////////////////////////////////////////////////////////////////
// wrap API
// ////////////////////////////////////////////////////////////////////////////
// ============================================================================
// ctor
// ============================================================================
IcObjectCache :: IcObjectCache ( Callback cb, void *cb_obj, int max_num )
{
    m_obj = qxpack_ic_new( IcObjectCachePriv, cb, cb_obj, max_num  );
}

// ============================================================================
// dtor
// ============================================================================
IcObjectCache :: ~IcObjectCache ( )
{
    if ( m_obj != nullptr ) {
        qxpack_ic_delete( m_obj, IcObjectCachePriv );
    }
}

// ============================================================================
// warp API
// ============================================================================
int      IcObjectCache :: count ( ) const { return T_PrivPtr( m_obj )->count(); }
void *   IcObjectCache :: allocate ( ) { return T_PrivPtr( m_obj )->allocate(); }
void     IcObjectCache :: recycle  ( void *obj )       { T_PrivPtr( m_obj )->recycle( obj ); }
void     IcObjectCache :: freeOldest( int limit_mill ) { T_PrivPtr( m_obj )->freeOldest( limit_mill ); }


}


#ifdef __GNUC__ // in GCC 5, close below warnings
#pragma GCC diagnostic pop
#endif

#endif
