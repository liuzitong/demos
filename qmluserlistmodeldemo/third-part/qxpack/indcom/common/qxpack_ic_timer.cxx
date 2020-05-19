// //////////////////////////////////////////////////////////
// @author nightwing
// @history
//        2018/12/20  modified, for GCC 5 compiler
// //////////////////////////////////////////////////////////
#ifndef QXPACK_IC_TIMER_CXX
#define QXPACK_IC_TIMER_CXX

#include "qxpack_ic_timer.hxx"
#include <qxpack/indcom/common/qxpack_ic_memcntr.hxx>
#include <qxpack/indcom/common/qxpack_ic_logging.hxx>
#include <qxpack/indcom/common/qxpack_ic_heapvectortemp.hpp>
#include <qxpack/indcom/common/qxpack_ic_semtemp.hpp>

#include <mutex>              // C++ 11
#include <condition_variable> // C++ 11
#include <chrono>             // C++ 11
#include <thread>             // C++ 11
#include <atomic>             // C++ 11
#include <map>                // C++ 11
#include <string>             // C++ 11
#include <cstdio>

//#if defined(WIN32)
//#include <windows.h>
//#endif

#ifdef __GNUC__ // in GCC 5, close below warnings
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#endif


namespace QxPack {

// //////////////////////////////////////////////////////
//
// the workitem of timer queue
//
// //////////////////////////////////////////////////////
typedef std::function<void(intptr_t,void*,void*,int)>  IcTimerQueueHandler;
enum  IcTimerQueueHandlerType {
    IcTimerQueueHandlerType_DoRoutine = 0, IcTimerQueueHandlerType_RecycleParam = 1
};

class QXPACK_IC_HIDDEN  IcTimerQueueWorkItem {
private:
    IcTimer::TimeStamp    m_end;
    IcTimerQueueHandler   m_handler;
    intptr_t  m_id;
    void     *m_ctxt, *m_param;
    QXPACK_IC_DISABLE_COPY( IcTimerQueueWorkItem )
public :
    IcTimerQueueWorkItem (  IcTimer::TimeStamp &pt, IcTimerQueueHandler & h, void *c, void *p = nullptr )
    {
        m_end     = pt;  m_id   = T_IntPtrCast( this );
        m_handler = h;   m_ctxt = c;  m_param = p;
    }
    inline IcTimer::TimeStamp   & endTime ( ) { return m_end;     }
    inline IcTimerQueueHandler  & handler ( ) { return m_handler; }
    inline void *     context( ) { return m_ctxt; }
    inline void *     param( )   { return m_param;}
    inline intptr_t   id( )      { return m_id;   }

    inline void  setup( IcTimer::TimeStamp &pt, IcTimerQueueHandler &h, void *c, void *p = nullptr )
    {
        m_end     = pt;  m_id   = T_IntPtrCast( this );
        m_handler = h;   m_ctxt = c;  m_param = p;
    }
    inline void  makeInvalidate( ) { m_id = 0; }
};

typedef IcTimerQueueWorkItem*  IcTimerQueueWorkItemPtr;
class QXPACK_IC_HIDDEN  IcTimerQueueWorkItemCmp {
public:
    bool  operator()( const IcTimerQueueWorkItemPtr &a, const IcTimerQueueWorkItemPtr &b ) const
    { return IcTimer::isTimeStampLessThan( a->endTime(), b->endTime() ); }
};

// ////////////////////////////////////////////////////////
//
// private object
//
// ////////////////////////////////////////////////////////
#define T_PrivPtr( o ) (( IcTimerQueuePriv *) o )
class QXPACK_IC_HIDDEN  IcTimerQueuePriv {

private:
    std::thread      m_thread;
    IcSemTemp        m_check_work;
    IcHeapVector< IcTimerQueueWorkItemPtr, IcTimerQueueWorkItemCmp > m_items, m_free_items;
    bool  m_finish;

protected:
    void  eraseAllItem_dtor ( );
    void  run_pr   ( );
    void  doWork_pr( );

public :
    IcTimerQueuePriv ( );
    ~IcTimerQueuePriv( );
    intptr_t   add   ( int mill_sec, IcTimerQueueHandler handler, void* ctxt, void *param = nullptr, IcTimer::TimeStamp *tmp = nullptr );
    bool       cancel( intptr_t t_id );
    void       dropByContext( void* ctxt  );
};

// ==========================================================
// CTOR
// ==========================================================
    IcTimerQueuePriv :: IcTimerQueuePriv ( )
                      : m_items(nullptr,nullptr,16), m_free_items(nullptr,nullptr,16)
{
    m_finish = false;
    m_thread = std::thread( [this]{ this->run_pr(); });
}

// ==========================================================
// DTOR
// ==========================================================
    IcTimerQueuePriv :: ~IcTimerQueuePriv ( )
{
    m_finish = true;
    this->m_check_work.notify();
    m_thread.join();
    this->eraseAllItem_dtor();
}

// ==========================================================
// erase all items in map [ protected ]
// ==========================================================
void   IcTimerQueuePriv :: eraseAllItem_dtor( )
{
    m_items.clear     ( []( IcTimerQueueWorkItemPtr &item, void* ){
        if ( item->param() != nullptr ) { item->handler()( item->id(), item->context(), item->param(), IcTimerQueueHandlerType_RecycleParam ); }
        qxpack_ic_delete( item, IcTimerQueueWorkItem ); }, nullptr
    );
    m_free_items.clear( []( IcTimerQueueWorkItemPtr &item, void* ){ qxpack_ic_delete( item, IcTimerQueueWorkItem ); }, nullptr );
}

// ==========================================================
// do the work  [ protected ]
// ==========================================================
void   IcTimerQueuePriv :: doWork_pr ( )
{
    IcTimerQueueWorkItem *item = nullptr;
    while ( m_items.removeAt(0,item ) ) { // get it successfull.
        if ( item->id() == 0 ) { m_free_items.insert( item ); continue; } // recycle invalid item
        if ( item->endTime() <= IcTimer::makeTimeStampFromNow(0)) { // expired time.
            //std::printf("item time_point = %lld\n", item->endTime().time_since_epoch().count() );
            ( item->handler())( item->id(), item->context(), item->param(), IcTimerQueueHandlerType_DoRoutine );
            m_free_items.insert( item );  // solved it
        } else {
            m_items.insert( item );       // push it back
            break;
        }
    }
}

// ==========================================================
// run the timer queue [ protected ]
// ==========================================================
void    IcTimerQueuePriv :: run_pr ( )
{
    volatile bool *is_finish = T_PtrCast( volatile bool *, &m_finish );
    while ( ! (*is_finish )) {
        // -------------------------------------------------
        // do wait the first element.
        // case: item 0 cancel() after waitUntil() called,
        //       cancel() will wake up this thread, will goto doWork_pr()
        // -------------------------------------------------
        IcTimer::TimeStamp tmp;
        bool ret = m_items.visit( 0, [](IcTimerQueueWorkItemPtr &item, void*ctxt ) {
                                  * T_ObjCast( IcTimer::TimeStamp *, ctxt ) = item->endTime();
                              }, &tmp
                            );
        if ( ret ) {
            m_check_work.waitUntil( tmp );
        } else {
            m_check_work.wait();
        }

        // -------------------------------------------------
        // while waked up,
        // -------------------------------------------------
        if ( *is_finish ) { break; }
        this->doWork_pr( );
    }

//#if defined(WIN32)
//    if ( IcAtExitSrvPriv::isInExitProc() ) {
//        ::ExitThread(NULL); // in win32, if thread join in atexit(), will be hang!
//    }
//#endif
}

// ==========================================================
// add a new timer
// ==========================================================
intptr_t   IcTimerQueuePriv :: add (int mill_sec, IcTimerQueueHandler handler, void* ctxt, void *param, IcTimer::TimeStamp *tmp )
{
    if ( handler == nullptr ) { return 0; } // bad paramter

    IcTimerQueueWorkItem *item = nullptr;
    IcTimer::TimeStamp now_ts = IcTimer::makeTimeStampFromNow( mill_sec );
    if ( ! m_free_items.removeAt( 0, item ) ) { // no items
        item = qxpack_ic_new( IcTimerQueueWorkItem,
                              now_ts,
                              handler, ctxt, param
                             );
    } else {
        item->setup( now_ts, handler, ctxt, param );
    }
    intptr_t id = item->id();
    if ( tmp != nullptr ) { *tmp = item->endTime(); }

    // --------------------------------------------
    // if inserted item is at 0, then need wake up
    // thread immediately.
    // --------------------------------------------
    if ( m_items.insert( item ) == 0 ) {
        m_check_work.notify();
    }
    return id;
}

// ==========================================================
// cancel a existed timer if possible
// ==========================================================
bool  IcTimerQueuePriv :: cancel ( intptr_t t_id )
{
    IcTimerQueueWorkItem *item = nullptr; int rmv_pos = -1;
    bool ret = m_items.removeByEnum( [](const IcTimerQueueWorkItemPtr &item_ref, void*ctxt )->bool {
                                         return ( reinterpret_cast< void*>(item_ref->id()) == ctxt );
                                     }, reinterpret_cast< void*>( t_id ), item, & rmv_pos
                                    );
    if ( ret ) {
        if ( item->param() != nullptr ) { item->handler()( item->id(), item->context(), item->param(), IcTimerQueueHandlerType_RecycleParam ); }
        m_free_items.insert( item ); // found and remove it
        if ( rmv_pos == 0 ) { m_check_work.notify(); }
    }

    return ( item != nullptr );
}


// ==========================================================
// drop all items that point to same context
// ==========================================================
void    IcTimerQueuePriv :: dropByContext( void *ctxt )
{
    // ------------------------------------------------------------------------
    // enum all elements, and make all items invalidate that context equal ctxt
    // ------------------------------------------------------------------------
    m_items.enumAll( []( IcTimerQueueWorkItemPtr &item, void *tgt_ctxt ){
                   if ( item->context() == tgt_ctxt ) {
                       if ( item->param() != nullptr ) {
                           item->handler()( item->id(), item->context(), item->param(), IcTimerQueueHandlerType_RecycleParam );
                       }
                       item->makeInvalidate();
                   }
               }, ctxt );
}


// //////////////////////////////////////////////////////////
//
// timer queue private node, used in a map
//
// //////////////////////////////////////////////////////////
class QXPACK_IC_HIDDEN  IcTimerQueuePriv_Node {
private: std::atomic<int> m_ref_cntr; IcTimerQueuePriv  m_tmq;
public :
    IcTimerQueuePriv_Node( ) : m_ref_cntr(1) { }
    ~IcTimerQueuePriv_Node( )  { }
    inline int  incrRefCntr( ) { return m_ref_cntr.fetch_add(1) + 1; }
    inline int  decrRefCntr( ) { return m_ref_cntr.fetch_sub(1) - 1; }
    IcTimerQueuePriv &  tmq( ) { return m_tmq; }
};

// /////////////////////////////////////////////////////////
//
// Timer Queue Map
//
// /////////////////////////////////////////////////////////
typedef std::map<std::string,IcTimerQueuePriv_Node*>  IcTimerQueuePrivMap;
static std::mutex          g_tmq_map_locker;
static IcTimerQueuePrivMap g_tmq_map;

// =================================================================
// get the named timer queue
// =================================================================
static IcTimerQueuePriv *   IcTimerQueuePriv_getInstance( const std::string &name )
{
    IcTimerQueuePriv_Node *node = nullptr;
    g_tmq_map_locker.lock();

    IcTimerQueuePrivMap::const_iterator itr = g_tmq_map.find( name );
    if ( itr != g_tmq_map.cend()) {
        node = itr->second; node->incrRefCntr();
    } else {
        node = new IcTimerQueuePriv_Node;
        g_tmq_map.insert( std::pair<std::string,IcTimerQueuePriv_Node*>( name, node ));
    }

    g_tmq_map_locker.unlock();

    return & node->tmq();
}

// ================================================================
// free the named instance
// ================================================================
static bool  IcTimerQueuePriv_freeInstance( const std::string &name )
{
    bool is_delete = false;
    g_tmq_map_locker.lock();

    IcTimerQueuePrivMap::const_iterator itr = g_tmq_map.find( name );
    if ( itr != g_tmq_map.cend()) {
        if ( ( itr->second )->decrRefCntr() == 0 ) {
            delete itr->second;
            g_tmq_map.erase( itr );
            is_delete = true;
        }
    }

    g_tmq_map_locker.unlock();

    return is_delete;
}


// ///////////////////////////////////////////////////////
//
// Timer object
//
// ///////////////////////////////////////////////////////
#define T_TimerPriv( o )  T_ObjCast( IcTimerPriv *, o )
typedef void (* IcTimerCallback )( void* );
class QXPACK_IC_HIDDEN  IcTimerPriv {
private:
    IcTimerQueuePriv  *m_tmq;
    IcTimerCallback    m_tmr_cb;
    void           *m_tmr_cb_ctxt;
    int             m_mill_sec, m_count;
    std::atomic<int>      m_curr_count;
    std::recursive_mutex  m_locker;
    std::atomic<intptr_t> m_tmr_handle;
    std::atomic<std::thread::id>  m_cb_t_id;    // callback thread id
    const char     *m_tmq_name;
protected:
    void  addTimerProc_ts( );
    void  timerProc_pr ( intptr_t tmr_id );
public :
    IcTimerPriv ( IcTimerCallback cb, void *cb_ctxt,
                  int mill, int count, const char *tmq_name );
    ~IcTimerPriv( );
    void  start ( );
    void  stop  ( );
    inline void  setTimeGap( int mill ) { m_mill_sec = mill; }
};

// ======================================================
// CTOR
// ======================================================
    IcTimerPriv :: IcTimerPriv ( IcTimerCallback cb, void *cb_ctxt,
                                int mill, int count, const char *const_tmr_name )
{
    m_tmq = nullptr; m_tmq_name = ( const_tmr_name != nullptr ? const_tmr_name : "QxPack::IcTimerPriv::TimerQueue" );
    m_tmr_cb = cb;  m_tmr_cb_ctxt = cb_ctxt;
    m_mill_sec = ( mill <= 0 ? 1 : mill );
    m_count    = ( count <= 0 ? -1 : count ); // -1 means infinity..
    m_curr_count.store( ( m_count < 0 ? 1 : m_count ));
    m_tmr_handle.store( 0 );
}

// ======================================================
// DTOR
// ======================================================
    IcTimerPriv :: ~IcTimerPriv ( )
{
    this->stop();
    if ( m_tmq != nullptr ) { IcTimerQueuePriv_freeInstance( m_tmq_name ); }
}

// =======================================================
// add the timer process [ protected ]
// =======================================================
void    IcTimerPriv :: addTimerProc_ts()
{
    m_tmr_handle.store(
        m_tmq->add( m_mill_sec, []( intptr_t tmr_id, void *ctxt, void *, int type ){
                     if ( type != IcTimerQueueHandlerType_RecycleParam ) {
                         T_ObjCast( IcTimerPriv *, ctxt )->timerProc_pr( tmr_id );
                     } else {
                         // nothing happen
                     }
                   }, this, nullptr )
              );
}

// =======================================================
// timer process
// =======================================================
void    IcTimerPriv :: timerProc_pr ( intptr_t tmr_id )
{
    bool req_clear_hd = false;

    m_locker.lock();

    intptr_t t_id = m_tmr_handle.load();
    if ( t_id == tmr_id ) {
        // -------------------------------------
        // do the callback function
        // -------------------------------------
        if ( m_tmr_cb != nullptr ) {
            m_cb_t_id.store( std::this_thread::get_id());
            (* m_tmr_cb )( m_tmr_cb_ctxt );
            m_cb_t_id.store( std::thread::id());
        }

        // -------------------------------------
        // check current counter
        // -------------------------------------
        if ( m_count < 0 ) {
            // --------------------------------
            // infinity timer
            // --------------------------------
            if ( m_curr_count.load() > 0 ) { this->addTimerProc_ts(); }
            else { req_clear_hd = true; }

        } else if ( m_curr_count.load() > 0 ) {
            // ---------------------------------
            //  count timer
            // ---------------------------------
            if ( m_curr_count.fetch_sub(1) - 1 > 0 ) { this->addTimerProc_ts( ); }
            else { req_clear_hd = true; }

        } else {
            // ---------------------------------
            // already finished
            // ---------------------------------
            req_clear_hd = true;
        }
    } else {  // timer is zero or other cases
        if ( t_id != 0 ) {
            qxpack_ic_fatal("in timer procss, the private timer id is not equal to parameter timer_id!");
        } else {
            req_clear_hd = true;
        }
    }
    if ( req_clear_hd ) {
        m_curr_count.store(0);
        m_tmr_handle.store(0);
    }

    m_locker.unlock();
}

// =======================================================
// start the timer
// =======================================================
void    IcTimerPriv :: start ( )
{   
    bool is_called_from_tmq = ( std::this_thread::get_id() == m_cb_t_id.load());

    m_locker.lock();   

    // ----------------------------------------------
    // ensure the timerqueue start.
    // ----------------------------------------------
    if ( m_tmq == nullptr ) { m_tmq = IcTimerQueuePriv_getInstance( m_tmq_name ); }

    // ----------------------------------------------
    // check if start() called from tmq
    // -----------------------------------------------
    if ( is_called_from_tmq ) {
        // called from tmq, add extr 1 count due to it will
        // decrease 1 after return
        m_curr_count.store( ( m_count < 0 ? 1 : m_count + 1 ) );
    } else {
        if ( m_tmr_handle.load() != 0 ) {
            // timer is running, try restore the count.
            m_curr_count.store( ( m_count < 0 ? 1 : m_count ) );
        } else {
            // timer is not runing
            m_curr_count.store( ( m_count < 0 ? 1 : m_count ) );
            this->addTimerProc_ts();
        }
    }
    m_locker.unlock();
}

// ========================================================
// stop the timer
// ========================================================
void    IcTimerPriv :: stop()
{
    // ---------------------------------------------------
    // 1) tmq lock,  stop() wait lock.
    // 2) stop() lock, tmq wait lock.
    // 3) tmq has not lock, stop() not found item ( because using by tmq )
    //    try to wait for tmq process it
    // ---------------------------------------------------   
    // 1. called from tmq, them are aways same. 2. otherwise, they are never equal
    bool is_called_from_tmq = ( std::this_thread::get_id() == m_cb_t_id.load());

    while ( m_tmq != nullptr &&  0 != m_tmr_handle.load() ) {

        m_locker.lock();

        // -----------------------------------------------
        // spec.case: stop() called from tmq
        // -----------------------------------------------
        if ( is_called_from_tmq ) {
            m_curr_count.store(0); // setup it current count as 0
            m_locker.unlock();
            break;                 // exit.
        }

        // -----------------------------------------------
        // normal case
        // -----------------------------------------------
        intptr_t tmr_id = m_tmr_handle.load();
        if ( 0 != tmr_id ) {
            if ( m_tmq->cancel( tmr_id )) {
                m_curr_count.store(0);
                m_tmr_handle.store(0);
            } else {
                // not found, maybe in timer queue call?
                m_curr_count.store(0); // try stop calling..
            }
        }
        m_locker.unlock();
    }
}

// /////////////////////////////////////////////////////
//
// IcTimer wrap API
//
// /////////////////////////////////////////////////////
// =====================================================
// CTOR
// =====================================================
    IcTimer :: IcTimer ( int mill, int rep_count, void (*TimerCallback)(void *), void *ctxt, const char *const_tmq_name )
{
    m_obj = qxpack_ic_new( IcTimerPriv, TimerCallback, ctxt, mill, rep_count, const_tmq_name );
}

// =====================================================
// DTOR
// =====================================================
    IcTimer :: ~IcTimer ( )
{
    if ( m_obj != nullptr ) {
        qxpack_ic_delete( m_obj, IcTimerPriv );
    }
}

// =====================================================
// api wrapper
// =====================================================
void  IcTimer :: start () { T_TimerPriv( m_obj )->start(); }
void  IcTimer :: stop  () { T_TimerPriv( m_obj )->stop();  }
void  IcTimer :: setTimeGap( int mill ) { T_TimerPriv( m_obj )->setTimeGap( mill ); }

// ///////////////////////////////////////////////////////
//
//  IcCaller private implement
//
// ///////////////////////////////////////////////////////
#define T_CallerPriv( o ) T_ObjCast( IcCallerPriv *, o )
class QXPACK_IC_HIDDEN IcCallerPriv {
private:
    IcTimerQueuePriv *m_tmq;
    const char       *m_tmq_name;
    IcCaller::Callback  m_handler;
    void     *m_handler_ctxt;
    std::recursive_mutex m_critical_locker; // the sync locker
    IcTimer::TimeStamp  m_last_post_tm;
public :
    IcCallerPriv( IcCaller::Callback cb, void *cb_ctxt, const char *const_tmr_name );
    ~IcCallerPriv( );
    void  post ( void *param, int delay_ms );
    void  cancelAll( );
};

// ====================================================================
// CTOR
// ====================================================================
    IcCallerPriv :: IcCallerPriv ( IcCaller::Callback cb, void *cb_ctxt, const char *const_tmr_name )
{
    m_handler = cb; m_handler_ctxt = cb_ctxt;
    m_tmq = nullptr;
    m_tmq_name = ( const_tmr_name != nullptr ? const_tmr_name : "QxPack::IcCallerPriv::TimerQueue");
    m_last_post_tm = IcTimer::makeTimeStampFromNow(0);
}

// ====================================================================
// DTOR
// ====================================================================
    IcCallerPriv :: ~IcCallerPriv( )
{
    this->cancelAll();
    if ( m_tmq != nullptr ) { IcTimerQueuePriv_freeInstance( m_tmq_name ); }
}

// ===================================================================
// cancel the operation
// ===================================================================
void  IcCallerPriv :: cancelAll()
{
    m_critical_locker.lock();

    // --------------------------------------------------------------
    // NOTE: here we drop all elements that context equal to this
    // it also can be called in callback function
    // --------------------------------------------------------------
    if ( m_tmq != nullptr ) {
        m_tmq->dropByContext( this );
    }

    m_critical_locker.unlock();
}

// ==================================================================
// post a callback request
// ==================================================================
void   IcCallerPriv :: post ( void *param, int delay_ms )
{
    m_critical_locker.lock();

    if ( m_tmq == nullptr ) { m_tmq = IcTimerQueuePriv_getInstance( m_tmq_name ); }

    // --------------------------------------------------------------
    // BUGS in VS2013, C++11 standard require now() return a value never lower
    // than previous call, but not in VS2013. here force return a bigger value.
    // --------------------------------------------------------------
    while ( IcTimer::makeTimeStampFromNow(0) <= m_last_post_tm )
    { std::this_thread::yield(); } // here make the end_time must be not same as previous call

    m_tmq->add( delay_ms,
         []( intptr_t, void *ctxt, void *param, int type ) {
             IcCallerPriv *t_this = T_ObjCast( IcCallerPriv *, ctxt );
             t_this->m_critical_locker.lock();
             t_this->m_handler( t_this->m_handler_ctxt, param, IcCaller::HandlerType( type )); // directly call the callback function.
             t_this->m_critical_locker.unlock();
         },
         this, param, & m_last_post_tm );

    m_critical_locker.unlock();
}

// //////////////////////////////////////////////////////////////////
// IcCaller wrap API
// //////////////////////////////////////////////////////////////////
// =================================================================
// CTOR
// =================================================================
    IcCaller :: IcCaller ( Callback cb_func, void *cb_obj, const char *thr_name )
{
    m_obj = qxpack_ic_new( IcCallerPriv, cb_func, cb_obj, thr_name );
}

// =================================================================
// DTOR
// =================================================================
    IcCaller :: ~IcCaller ( )
{
    if ( m_obj != nullptr ) {
        qxpack_ic_delete( m_obj, IcCallerPriv );
    }
}

// =================================================================
// method APIs
// =================================================================
void   IcCaller :: post ( void *param, int delay_ms ) { T_CallerPriv( m_obj )->post( param, delay_ms ); }
void   IcCaller :: cancelAll() { T_CallerPriv( m_obj )->cancelAll(); }


}

#ifdef __GNUC__ // in GCC 5, close below warnings
#pragma GCC diagnostic pop
#endif

#endif
