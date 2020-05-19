#ifndef QXPACK_IC_OCLBUFFCACHE_CXX
#define QXPACK_IC_OCLBUFFCACHE_CXX

#include "qxpack_ic_oclbuffcache.hxx"
#include "qxpack_ic_oclbuffer.hxx"

#include <qxpack/indcom/common/qxpack_ic_memcntr.hxx>
#define QXPACK_IcPImplPrivTemp_new    qxpack_ic_new
#define QXPACK_IcPImplPrivTemp_delete qxpack_ic_delete
#include <qxpack/indcom/common/qxpack_ic_pimplprivtemp.hpp>
#include <qxpack/indcom/common/qxpack_ic_logging.hxx>
#include <qxpack/indcom/common/qxpack_ic_heapvectortemp.hpp>
#include <qxpack/indcom/common/qxpack_ic_timer.hxx>

#if !defined( CL_USE_DEPRECATED_OPENCL_1_1_APIS )
#define CL_USE_DEPRECATED_OPENCL_1_1_APIS
#endif
#if !defined( CL_USE_DEPRECATED_OPENCL_1_2_APIS )
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#endif
#include "ocl_libs/include/CL/cl.h"

#include <functional> // C++11
#include <mutex>      // C++11

using StdClock = std::chrono::steady_clock;

#define OBJECT_LIFE_TIME  500  // 500ms life-cycle of OclBuffer that not used.

namespace QxPack {

// ///////////////////////////////////////////////////////////////////
//
// node
//
// ///////////////////////////////////////////////////////////////////
class QXPACK_IC_HIDDEN  IcOclBuffCache_Node {
private:  IcTimeStamp  m_tm_pt;  IcOclBuffer  m_buff;
public :
    IcOclBuffCache_Node ( IcTimeStamp &tp, IcOclBuffer &buff )
    { m_tm_pt = tp; m_buff = buff;  }
    inline IcTimeStamp &   timePoint( ) { return m_tm_pt; }
    inline IcOclBuffer &   buffer( ) { return m_buff; }
    inline void   setBuffer   ( const IcOclBuffer &buff  ) { m_buff  = buff; }
    inline void   setTimePoint( IcTimeStamp &tm ) { m_tm_pt = tm; }
};

typedef IcOclBuffCache_Node*  IcOclBuffCache_NodePtr;
class QXPACK_IC_HIDDEN  IcOclBuffCache_NodeLessCmp {
public:
    bool  operator()( const IcOclBuffCache_NodePtr &a, const IcOclBuffCache_NodePtr &b )
    { return IcTimer::isTimeStampLessThan( a->timePoint(), b->timePoint() ); }
};

// ///////////////////////////////////////////////////////////////////
//
//  node cache
//
// ///////////////////////////////////////////////////////////////////
typedef IcHeapVector<IcOclBuffCache_NodePtr,IcOclBuffCache_NodeLessCmp>  IcOclBuffCache_NodeHeapTs;
class QXPACK_IC_HIDDEN  IcOclBuffCache_NodeCacheTs {
private: IcOclBuffCache_NodeHeapTs  m_node;
public :
    IcOclBuffCache_NodeCacheTs ( ) : m_node( nullptr, nullptr ) { }
    ~IcOclBuffCache_NodeCacheTs( )
    { m_node.clear([](IcOclBuffCache_NodePtr &ptr, void*){ qxpack_ic_delete(ptr, IcOclBuffCache_Node ); }, this );}

    // ================================================
    // allocate a new buffer cache node.
    // ================================================
    IcOclBuffCache_Node*  alloc( )
    {
        IcOclBuffCache_Node *node = nullptr;
        if ( m_node.removeLast( node )) { // re-use existed object
            node->setTimePoint( IcTimer::makeTimeStampFromNow( OBJECT_LIFE_TIME ) );
            node->setBuffer   ( IcOclBuffer( ));
            return node;
        } else {    // allocate new object
            return qxpack_ic_new( IcOclBuffCache_Node,
                 IcTimer::makeTimeStampFromNow( OBJECT_LIFE_TIME ), IcOclBuffer() );
        }
    }

    // ================================================
    // free a buffer node, currently we save it in heap
    // ================================================
    void   free( IcOclBuffCache_Node *n )
    {
        n->setBuffer( IcOclBuffer());
        n->setTimePoint( IcTimer::makeTimeStampFromNow( OBJECT_LIFE_TIME ) );
        m_node.insert( n );
    }

    // ================================================
    // delete all items in heap that life cycle has been finished
    // ================================================
    void   gc ( IcTimeStamp &limit_tm_pt )
    {
        IcOclBuffCache_Node *node = nullptr;
        while ( m_node.removeAt( 0, node )) {
            if ( IcTimer::isTimeStampLessThan( node->timePoint(), limit_tm_pt ) ) {
                m_node.insert( node );
                break;
            } else { // over the limit time point
                qxpack_ic_delete( node, IcOclBuffCache_Node );
            }
        }
    }
};

// ////////////////////////////////////////////////////////////
//
//  node cache map
//
// ////////////////////////////////////////////////////////////
static const int g_node_cache_sz[] = {
    4096,   8192,  16384,  32768, 65536,
    131072, 262144, 524288, 1048576, 2097152, 4194304,
    8388608,16777216,33554432,67108864  // max is 64MB block
};

class QXPACK_IC_HIDDEN IcOclBuffCache_NodeCacheMap {
private: std::map<int, IcOclBuffCache_NodeHeapTs*> m_cache;
public :
    IcOclBuffCache_NodeCacheMap ( );
    ~IcOclBuffCache_NodeCacheMap( );

};

// ============================================================
// CTOR
// ============================================================
    IcOclBuffCache_NodeCacheMap :: IcOclBuffCache_NodeCacheMap ( )
{

}

// ============================================================
// DTOR
// ============================================================
    IcOclBuffCache_NodeCacheMap :: ~IcOclBuffCache_NodeCacheMap ( )
{
    std::map<int,IcOclBuffCache_NodeHeapTs*>::iterator itr = m_cache.begin();
    while ( itr != m_cache.end()) {
        IcOclBuffCache_NodeHeapTs *heap = itr->second; ++ itr;
        if ( heap != nullptr ) {
            heap->clear([](IcOclBuffCache_NodePtr &ptr,void*) {
                            qxpack_ic_delete( ptr, IcOclBuffCache_Node );
                        }, this);
            qxpack_ic_delete( heap, IcOclBuffCache_NodeHeapTs );
        }
    }
    m_cache = std::map<int,IcOclBuffCache_NodeHeapTs*>();
}






// /////////////////////////////////////////////////////
//
// IcOclBuffCachePriv
//
// /////////////////////////////////////////////////////

#define T_PrivPtr( o ) (( IcOclBuffCachePriv *) o )
class QXPACK_IC_HIDDEN  IcOclBuffCachePriv : public IcPImplPrivTemp< IcOclBuffCachePriv > {
private:
    std::map<int, IcOclBuffCache_NodeHeapTs* > m_cache;
    std::mutex           m_cache_locker;
    IcOclBuffCache_NodeCacheTs  m_node_cache;
    cl_context           m_ctxt;
    cl_command_queue     m_cmd_queue;
protected:
    void    _buildCache ( );
    void    _clearCache ( );
    IcOclBuffCache_NodeHeapTs * _heapBySize( int sz );
    IcOclBuffCache_NodeHeapTs * _heapBySizeUpper(int sz, int &upper_sz );
    void    _gc_rtw ( );

    static inline const int* _cacheSizeArray( ) { return & g_cache_sz[0]; }
    static inline int  _cacheSizeArrayNum( ) { return ( int ) sizeof( g_cache_sz ) / sizeof( g_cache_sz[0 ]); }
    static inline int  _cacheSizeByIdx( int idx )
    { return (( idx >= 0 && idx < IcOclBuffCachePriv::_cacheSizeArrayNum()) ? g_cache_sz[ idx ] : 0 ); }

public :
    IcOclBuffCachePriv ( );
    IcOclBuffCachePriv ( const IcOclBuffCachePriv & );
    virtual ~IcOclBuffCachePriv ( );
    bool    init ( cl_context, cl_command_queue );

    IcOclBuffer   create  ( int req_size  );
    void          recycle ( IcOclBuffer & );
};

// =====================================================
// CTOR
// =====================================================
    IcOclBuffCachePriv :: IcOclBuffCachePriv ( )
{
    m_ctxt = 0;  m_cmd_queue = 0;
    this->_buildCache();

    m_gc_wkr = IcOclBuffCacheGcWkr_getInstance();
    m_gc_key = m_gc_wkr->add( [](void*ctxt){ (( IcOclBuffCachePriv *)ctxt )->_gc_rtw(); }, this );
}

// =====================================================
// CTOR ( Copy )
// =====================================================
    IcOclBuffCachePriv :: IcOclBuffCachePriv ( const IcOclBuffCachePriv & )
{
    qxpack_ic_fatal("IcOclBuffCachePriv call COPY CTOR is not allowed!");
}

// =====================================================
// DTOR
// =====================================================
    IcOclBuffCachePriv :: ~IcOclBuffCachePriv ( )
{
    while ( ! m_gc_wkr->remove( m_gc_key )) { std::this_thread::sleep_for( std::chrono::milliseconds(16) ); }
    IcOclBuffCacheGcWkr_freeInstance();

    m_cache_locker.lock();
    this->_clearCache();
    m_cache_locker.unlock();

    if ( 0 != m_cmd_queue ) { clReleaseCommandQueue( m_cmd_queue ); }
    if ( 0 != m_ctxt      ) { clReleaseContext ( m_ctxt ); }
}

// =====================================================
// clear the cache [ protected ]
// =====================================================
void   IcOclBuffCachePriv :: _clearCache()
{
    std::map<int,IcOclBuffCache_NodeHeapTs*>::iterator itr = m_cache.begin();
    while ( itr != m_cache.end()) {
        IcOclBuffCache_NodeHeapTs *heap = itr->second; ++ itr;
        if ( heap != nullptr ) {
            heap->clear([](IcOclBuffCache_NodePtr &ptr,void*) {
                            qxpack_ic_delete( ptr, IcOclBuffCache_Node );
                        }, this);
            qxpack_ic_delete( heap, IcOclBuffCache_NodeHeapTs );
        }
    }
    m_cache = std::map<int,IcOclBuffCache_NodeHeapTs*>();
}

// =====================================================
// build the cache [ protected ]
// =====================================================
void    IcOclBuffCachePriv :: _buildCache( )
{
    int  idx = 0;
    int  cc_ar_size = IcOclBuffCachePriv::_cacheSizeArrayNum();
    const int* cc_ar_ptr  = IcOclBuffCachePriv::_cacheSizeArray();
    while ( idx < cc_ar_size ) {
        m_cache.insert( std::pair<int, IcOclBuffCache_NodeHeapTs* >(
                           cc_ar_ptr[ idx ++ ],
                           qxpack_ic_new( IcOclBuffCache_NodeHeapTs, nullptr, nullptr )
                          )
                      );
    }
}

// =====================================================
// get the heap by req-size
// =====================================================
IcOclBuffCache_NodeHeapTs *  IcOclBuffCachePriv :: _heapBySize( int sz )
{
    std::map<int,IcOclBuffCache_NodeHeapTs*>::iterator itr =
        m_cache.find( IcOclBuffCachePriv::_cacheSizeByIdx( sz ) );
    return ( itr != m_cache.end() ? itr->second : nullptr );
}

// =====================================================
// get the heap by upper size
// =====================================================
IcOclBuffCache_NodeHeapTs *  IcOclBuffCachePriv :: _heapBySizeUpper(int sz, int &upper_sz )
{
    std::map<int,IcOclBuffCache_NodeHeapTs*>::iterator itr = m_cache.find( sz );
    if ( itr == m_cache.end()) { itr = m_cache.upper_bound( sz ); }
    IcOclBuffCache_NodeHeapTs *heap = ( itr != m_cache.end() ? itr->second : nullptr );
    upper_sz = ( itr != m_cache.end() ? itr->first : 0 );
    return heap;
}

// =====================================================
// gc [ protected ]
// =====================================================
void    IcOclBuffCachePriv :: _gc_rtw()
{
    IcTimeStamp  limit_tm_pt = IcTimer::makeTimeStampFromNow(0);

    // visit all heap, found out the oldest
    int cc_num = IcOclBuffCachePriv::_cacheSizeArrayNum();
    for ( int x = 0; x < cc_num; x ++ ) {
        m_cache_locker.lock();
        IcOclBuffCache_NodeHeapTs *heap = this->_heapBySize( IcOclBuffCachePriv::_cacheSizeByIdx( x ));
        m_cache_locker.unlock();

        if ( heap != nullptr ) {   // remove all elements that too old
            IcOclBuffCache_Node *node = nullptr;
            while ( heap->removeAt( 0, node )) {
                if ( node->timePoint() < limit_tm_pt ) {
                    heap->insert( node );
                    break; // NOTE: heap is a miniheap. here means not more item greater than limit_tm_pt
                } else {
                    m_node_cache.free( node );
                }
            }
        }
    }

    // free node cache
    m_node_cache.gc( limit_tm_pt );
}

// =====================================================
// initalize the object
// =====================================================
bool   IcOclBuffCachePriv :: init( cl_context ctxt , cl_command_queue cmd_queue )
{
    if ( 0 == ctxt || 0 == cmd_queue ) { return false; }
    clRetainContext     ( ( m_ctxt = ctxt ) );
    clRetainCommandQueue( ( m_cmd_queue = cmd_queue ) );
    return true;
}

// ======================================================
// allocate a buffer by required size
// ======================================================
IcOclBuffer  IcOclBuffCachePriv :: create ( int req_size )
{
    // ---------------------------------------------------
    // find out the buffer heap if possible
    // ---------------------------------------------------
    m_cache_locker.lock();
    int cap_size = 0;
    IcOclBuffCache_NodeHeapTs *heap = this->_heapBySizeUpper( req_size, cap_size );
    m_cache_locker.unlock();

    // ---------------------------------------------------
    // create a buffer or re-use a buffer in the heap
    // ---------------------------------------------------
    IcOclBuffer buff;
    if ( heap != nullptr ) { // we found a entry about this req_size
        IcOclBuffCache_Node *node = nullptr;
        if ( heap->removeLast( node )) { // re-use one in heap
            buff = node->buffer();
            m_node_cache.free( node );  // recycle the carrier object
        } else {  // create a new one
            IcOclBuffer tmp( ( intptr_t ) m_ctxt, ( intptr_t ) m_cmd_queue, cap_size );
            buff = tmp;
        }
    } else { // no found, maybe greate than all entry
        IcOclBuffer tmp( ( intptr_t ) m_ctxt, ( intptr_t ) m_cmd_queue, req_size );
        buff = tmp;
    }

    // -------------------------------------------------
    // adjust the avaliable size
    // -------------------------------------------------
    buff.resize( req_size );
    return buff;
}

// =====================================================
// recycle the buffer
// =====================================================
void         IcOclBuffCachePriv :: recycle( IcOclBuffer &buff )
{
    if ( buff.isNull() ) { return; }

    // ------------------------------------------------
    // found the entry about this buffer
    // ------------------------------------------------
    m_cache_locker.lock();
    int cap_size = 0;
    IcOclBuffCache_NodeHeapTs *heap = this->_heapBySizeUpper( buff.capacity(), cap_size );
    m_cache_locker.unlock();

    // ------------------------------------------------
    // put the node back into heap.
    // ------------------------------------------------
    if ( heap != nullptr ) { // found the entry
        IcOclBuffCache_Node *node = m_node_cache.alloc();
        node->setBuffer( buff );
        heap->insert( node );
    } else {
        // here need not to keep the buffer reference.
        // because while the reference counter is zero, it will be automaticlly free.
        buff = IcOclBuffer();
    }
}

// ////////////////////////////////////////////////////////////
//
// gc worker
//
// ////////////////////////////////////////////////////////////
typedef IcHeapVector<void*,IcHeapVectorLess<void*>> IcOclBuffCache_GcObjHeap;
class QXPACK_IC_HIDDEN  IcOclBuffCache_GcWorker {
private:
    IcTimer  *m_cb_solver, *m_trg_tmr;
    IcOclBuffCache_GcObjHeap  m_gc_obj_heap;
protected:
    void  _gc_rtw( );
public :
    IcOclBuffCache_GcWorker();
    ~IcOclBuffCache_GcWorker();
    inline int  gcObjNum( ) { return m_gc_obj_heap.size(); }
};

// ============================================================
// CTOR
// ============================================================
    IcOclBuffCache_GcWorker :: IcOclBuffCache_GcWorker( )
{
    m_cb_solver = new IcTimer( 0, 1, [](void*ctxt){
            (( IcOclBuffCache_GcWorker *)ctxt)->_gc_rtw();
            }, this, "QxPack::IcOclBuffCache_GcWorker"
         );
    m_trg_tmr   = new IcTimer( 2000, -1, [](void*ctxt){
            ( IcTimer *)

    } )
}



// //////////////////////////////////////////////////////
//
// shared gc.
//
// //////////////////////////////////////////////////////
static std::recursive_mutex  g_gc_locker;    // locker
static IcTimer    *g_gc_cb_solver = nullptr; // callback event solver
static IcTimer    *g_gc_trg_tmr   = nullptr; // trigger by each 2s, use default timer thread
static IcOclBuffCache_GcObjHeap *g_gc_obj_heap = nullptr; // all objects of

// ========================================================
//
// ========================================================


// ========================================================
// add the gc worker object
// ========================================================
static void  IcOclBuffCache_addGcObj( IcOclBuffCachePriv *obj )
{
    g_gc_locker.lock();
    if ( g_gc_obj_heap == nullptr ) { IcOclBuffCache_GcObjHeap = qxpack_ic_new( IcOclBuffCache_GcObjHeap,nullptr,nullptr); }
    if ( g_gc_tmr == nullptr ) { g_gc_tmr = qxpack_ic_new( IcTimer, [](void*){ IcOcl } ); }

    g_gc_locker.unlock();
}

// ========================================================
//
// ========================================================




// ////////////////////////////////////////////////////////
//
//  IcOclBufferCache wrap API ( NOTE: do not use COW! )
//
// ////////////////////////////////////////////////////////
// =======================================================
// CTOR
// =======================================================
    IcOclBuffCache :: IcOclBuffCache ( )
{
    m_obj = nullptr;
}

// ======================================================
// CTOR ( create )
// ======================================================
    IcOclBuffCache :: IcOclBuffCache( intptr_t ctxt, intptr_t cmd_queue )
{
    m_obj = nullptr;
    IcOclBuffCachePriv::buildIfNull( & m_obj );
    if ( ! T_PrivPtr( m_obj )->init( ( cl_context ) ctxt, ( cl_command_queue ) cmd_queue )) {
        IcOclBuffCachePriv::attach( & m_obj, nullptr );
        m_obj = nullptr;
    }
}

// ======================================================
// CTOR ( Copy )
// ======================================================
    IcOclBuffCache :: IcOclBuffCache( const IcOclBuffCache &other )
{
    m_obj = nullptr;
    IcOclBuffCachePriv::attach( & m_obj, ( void **) &other.m_obj );
}

// ======================================================
// Copy assign
// ======================================================
IcOclBuffCache &  IcOclBuffCache :: operator = ( const IcOclBuffCache &other )
{
    IcOclBuffCachePriv::attach( & m_obj, ( void **) & other.m_obj );
    return *this;
}

// =====================================================
// DTOR
// =====================================================
    IcOclBuffCache :: ~IcOclBuffCache( )
{
    if ( m_obj != nullptr ) {
        IcOclBuffCachePriv::attach( & m_obj, nullptr );
    }
}

// =====================================================
// check if the buffercache is ready or not
// =====================================================
bool   IcOclBuffCache :: isReady() const { return ( m_obj != nullptr ); }


// =====================================================
// create a named buffer, if success return a ready buffer
// =====================================================
IcOclBuffer  IcOclBuffCache :: create( int size )
{
    return ( m_obj != nullptr ? T_PrivPtr( m_obj )->create( size ) : IcOclBuffer( ));
}

// =====================================================
// delete a named buffer, if success return true
// =====================================================
void         IcOclBuffCache :: recycle ( IcOclBuffer &buff )
{
    if ( m_obj != nullptr ) {
        T_PrivPtr( m_obj )->recycle( buff );
    } else {
        buff = IcOclBuffer( );
    }
}




}

#endif

