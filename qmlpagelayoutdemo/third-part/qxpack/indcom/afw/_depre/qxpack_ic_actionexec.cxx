// ============================================================================
// author: night wing
// date stamp: 20190604 1101
//  1) use IcRmtObjSigBlocker to block signals
// ============================================================================
// ============================================================================
// author: night wing
// date stamp: 20190523 1320
//  1) re-check
// ============================================================================
// ============================================================================
// author: night wing
// date stamp: 20190522 1544
//  1) use qxpack_ic_new_qobj instead new QObject..
//  2) blockSignals while dtor calling.
// ============================================================================

#ifndef QXPACK_IC_ACTIONEXEC_CXX
#define QXPACK_IC_ACTIONEXEC_CXX

#include "qxpack_ic_actionexec.hxx"
#include "qxpack/indcom/common/qxpack_ic_memcntr.hxx"
#include "qxpack/indcom/common/qxpack_ic_logging.hxx"
#include "qxpack/indcom/common/qxpack_ic_objectcache.hxx"
#include "qxpack/indcom/sys/qxpack_ic_rmtobjsigblocker_priv.hxx"

#include <QMap>
#include <QMutex>
#include <QEventLoop>
#include <QQueue>
#include <QSemaphore>
#include <QList>
#include <QMetaObject>
#include <QMetaMethod>
#include <QByteArray>

namespace QxPack {

// ////////////////////////////////////////////////////////////////////////////
//
//               Action Request, describe the request
//
// ////////////////////////////////////////////////////////////////////////////
class QXPACK_IC_HIDDEN IcActionExec_Req {
public :
    enum ReqType { ReqType_Call, ReqType_CancelWorking, ReqType_Freeze, ReqType_Unfreeze };
private:
    ReqType  m_req_type; int m_act_id; QJsonObject m_jo; IcVarData m_ext_param;
public :
    IcActionExec_Req ( ) { this->clear(); }
    ~IcActionExec_Req( ) { }

    inline void   clear( )
    { m_act_id = 0; m_ext_param = IcVarData(); m_jo = QJsonObject(); m_req_type = ReqType_Call; }

    inline void   setupCall ( int act_id, const QJsonObject &jo, const IcVarData &ext_param )
    { m_act_id = act_id; m_jo = jo; m_ext_param = ext_param; m_req_type = ReqType_Call; }
    inline void   setupCancel  ( int act_id ) { this->clear(); m_act_id = act_id; m_req_type = ReqType_CancelWorking; }
    inline void   setupFreeze  ( int act_id ) { this->clear(); m_act_id = act_id; m_req_type = ReqType_Freeze;    }
    inline void   setupUnfreeze( int act_id ) { this->clear(); m_act_id = act_id; m_req_type = ReqType_Unfreeze;  }

    inline int            actId() const   { return m_act_id;    }
    inline QJsonObject &  paramJoRef()    { return m_jo;        }
    inline IcVarData &    paramExtRef()   { return m_ext_param; }
    inline ReqType        reqType() const { return m_req_type;  }
};




// ////////////////////////////////////////////////////////////////////////////
//
//                      Action Request Cache
//
// ////////////////////////////////////////////////////////////////////////////
static QMutex         g_reqcache_locker;
static volatile IcObjectCache *g_reqcache_ptr = Q_NULLPTR;
static QAtomicInt     g_reqcache_ref_cntr(0);

// ============================================================================
// handle the allocate and free operation
// ============================================================================
static void  gReqCache_Handler( void *, void **obj_pptr, IcObjectCache::HandlerType h_type )
{
    Q_ASSERT( obj_pptr != Q_NULLPTR );
    if ( h_type == IcObjectCache::HandlerType_Alloc ) {
        *obj_pptr = qxpack_ic_new ( IcActionExec_Req );
    } else {
        if ( *obj_pptr != nullptr ) {
            qxpack_ic_delete ( *obj_pptr, IcActionExec_Req );
            *obj_pptr = nullptr;
        }
    }
}

// ============================================================================
// get the global object cache reference
// ============================================================================
static IcObjectCache *   gReqCache_getInstance( )
{
    g_reqcache_locker.lock();
    if ( g_reqcache_ptr == Q_NULLPTR ) {
        g_reqcache_ptr = qxpack_ic_new( IcObjectCache, & gReqCache_Handler, Q_NULLPTR, 48 );
        g_reqcache_ref_cntr.store(1);
    } else {
        g_reqcache_ref_cntr.fetchAndAddOrdered(1);
    }
    g_reqcache_locker.unlock();

    return const_cast<IcObjectCache*>( g_reqcache_ptr );
}

// ============================================================================
// free the global object cache reference
// ============================================================================
static bool     gReqCache_freeInstance( )
{
    bool is_del = false;

    g_reqcache_locker.lock();
    if ( g_reqcache_ptr != Q_NULLPTR ) {
        if ( g_reqcache_ref_cntr.fetchAndSubOrdered(1) - 1 == 0 ) {
            qxpack_ic_delete( const_cast<IcObjectCache*>( g_reqcache_ptr ), IcObjectCache );
            g_reqcache_ptr = Q_NULLPTR;
            is_del  = true;
        }
    }
    g_reqcache_locker.unlock();

    return is_del;
}




// ////////////////////////////////////////////////////////////////////////////
//
//                      Action Request List
//
// ////////////////////////////////////////////////////////////////////////////
class QXPACK_IC_HIDDEN IcActionExec_ReqList {
private: QQueue<void*> m_list; QMutex m_locker; IcObjectCache *m_reqcache;
public :
    IcActionExec_ReqList ( ) { m_reqcache = gReqCache_getInstance(); }
    ~IcActionExec_ReqList( )
    {
        m_locker.lock();
        IcActionExec_Req *req = Q_NULLPTR;
        if ( ! m_list.isEmpty() ) {
            while ( ( req = ( IcActionExec_Req *) m_list.takeFirst()) != Q_NULLPTR ) {
                m_reqcache->recycle( req );
            }
        }
        m_locker.unlock();
        gReqCache_freeInstance();
    }
    inline int   size( ) { m_locker.lock(); int sz = m_list.size(); m_locker.unlock(); return sz; }

    bool  enqueue( IcActionExec_Req *req )
    {
        if ( req == Q_NULLPTR ) { return false; }
        m_locker.lock(); m_list.push_back( req ); m_locker.unlock();
        return true;
    }

    IcActionExec_Req *  deque( )
    {
        IcActionExec_Req *req = Q_NULLPTR;
        m_locker.lock();
        if ( m_list.size() > 0 ) { req = ( IcActionExec_Req *) m_list.front(); m_list.pop_front(); }
        m_locker.unlock();
        return req;
    }
};




// ////////////////////////////////////////////////////////////////////////////
//
//                 Action Node, describe the action
//
// ////////////////////////////////////////////////////////////////////////////
class QXPACK_IC_HIDDEN IcActionExec_Node {
public :
    typedef bool (* ExtHandler )( void *, const QJsonObject &,
        const QxPack::IcVarData &, int, int, const QByteArray &method_name );
private:
    IcActionExec::ActionType     m_type;
    IcActionExec::State          m_state;
    IcActionExec::ActionHandler  m_handler;
    QMetaMethod  m_method;  QObject *m_method_obj;
    void *m_ctxt;  bool  m_is_freeze; int m_act_id;
protected:
    bool   callMethod   ( const QJsonObject &jo, const IcVarData &vd, int, int );
    bool   actTrgHandler( IcActionExec_Req & );
    bool   actSwHandler ( IcActionExec_Req & );
public :
    ~IcActionExec_Node ( ) { }
    IcActionExec_Node  (
        int act_id, IcActionExec::ActionType type,
        IcActionExec::ActionHandler handler, void *ctxt
    );
    IcActionExec_Node  (
        int act_id, IcActionExec::ActionType type,
        const QMetaMethod &mm,  QObject *mo, void *ctxt
    );

    inline int  actId() { return m_act_id; }
    inline IcActionExec::ActionType type()  { return m_type;  }
    inline IcActionExec::State      state() { return m_state; }
    bool  doRequest ( IcActionExec_Req & );
};

// ============================================================================
// ctor ( normally node )
// ============================================================================
IcActionExec_Node :: IcActionExec_Node(
    int act_id, IcActionExec::ActionType type, IcActionExec::ActionHandler handler, void *ctxt
) {   
    m_act_id  = act_id;   m_type = type;
    m_handler = handler;  m_ctxt = ctxt;
    m_state = IcActionExec::State_Idle;
    m_is_freeze = false;  m_method_obj = Q_NULLPTR;
}

// ============================================================================
// ctor ( qobject invoke node )
// ============================================================================
IcActionExec_Node :: IcActionExec_Node (
    int act_id, IcActionExec::ActionType type, const QMetaMethod &mm, QObject *mo, void *ctxt
) {
    m_act_id = act_id;     m_type = type;
    m_handler= Q_NULLPTR;  m_ctxt = ctxt;
    m_state  = IcActionExec::State_Idle;
    m_is_freeze = false;   m_method = mm; m_method_obj = mo;
}

// ============================================================================
// do the request
// ============================================================================
bool   IcActionExec_Node :: doRequest( IcActionExec_Req &req )
{
    switch( m_type ) {
    case IcActionExec::ActionType_Switch : return this->actSwHandler ( req ); break;
    case IcActionExec::ActionType_Trigger: return this->actTrgHandler( req ); break;
    default: return false;
    }
}

// ============================================================================
// call the method
// ============================================================================
bool   IcActionExec_Node :: callMethod(const QJsonObject &jo, const IcVarData &vd, int c_st, int t_st )
{
    if ( m_handler != Q_NULLPTR ) {
        return (* m_handler )( m_ctxt, jo, vd, c_st, t_st );
    }
    if ( m_method.isValid() &&  m_method_obj != Q_NULLPTR ) {
        bool ret = false;
        bool call_ret = m_method.invoke( m_method_obj, Qt::DirectConnection,
            Q_RETURN_ARG( bool, ret ), Q_ARG( const QJsonObject &, jo ),
            Q_ARG( const QxPack::IcVarData &, vd ), Q_ARG( int, c_st ), Q_ARG( int, t_st )
        );

        if ( ! call_ret ) { ret = false; }
        return ret;
    }
    return false;
}

// ============================================================================
// switch action handler
// ============================================================================
bool   IcActionExec_Node :: actSwHandler( IcActionExec_Req &req )
{
    bool ret = false;

    switch( req.reqType()) {
    case IcActionExec_Req::ReqType_Call :
        if ( ! m_is_freeze  &&  m_state == IcActionExec::State_Idle ) {
            ret = this->callMethod ( req.paramJoRef(), req.paramExtRef(), m_state, IcActionExec::State_Working );
            if ( ret ) { m_state = IcActionExec::State_Working; }
        }
        break;
    case IcActionExec_Req::ReqType_CancelWorking :
        if ( ! m_is_freeze  &&  m_state == IcActionExec::State_Working ) {
            ret = this->callMethod ( req.paramJoRef(), req.paramExtRef(), m_state, IcActionExec::State_Idle );
            if ( ret ) { m_state = IcActionExec::State_Idle; }
        }
        break;
    case IcActionExec_Req::ReqType_Freeze :
        if ( ! m_is_freeze ) {
            ret = this->callMethod ( req.paramJoRef(), req.paramExtRef(), m_state, IcActionExec::State_Freeze );
            if ( ret ) { m_is_freeze = true; }
        }
        break;
    case IcActionExec_Req::ReqType_Unfreeze :
        if ( m_is_freeze ) {
            ret = this->callMethod ( req.paramJoRef(), req.paramExtRef(), IcActionExec::State_Freeze, m_state );
            if ( ret ) { m_is_freeze = false; }
        }
        break;
    default: break;
    }
    return ret;
}

// ============================================================================
// trigger action handler
// ============================================================================
bool   IcActionExec_Node :: actTrgHandler( IcActionExec_Req &req )
{
    bool ret = false;

    switch( req.reqType()) {
    case IcActionExec_Req::ReqType_Call :
        if ( ! m_is_freeze && m_state == IcActionExec::State_Idle ) {
            ret = this->callMethod( req.paramJoRef(), req.paramExtRef(), m_state, IcActionExec::State_Working );
        }
        break;
    case IcActionExec_Req::ReqType_CancelWorking : break;
    case IcActionExec_Req::ReqType_Freeze   :
        if ( ! m_is_freeze ) {
            ret = this->callMethod( req.paramJoRef(), req.paramExtRef(), m_state, IcActionExec::State_Freeze );
            if ( ret ) { m_is_freeze = true; }
        }
        break;
    case IcActionExec_Req::ReqType_Unfreeze :
        if ( m_is_freeze ) {
            ret = this->callMethod( req.paramJoRef(), req.paramExtRef(), IcActionExec::State_Freeze, m_state );
            if ( ret ) { m_is_freeze = false; }
        }
        break;
    default: break;
    }

    return ret;
}




// ////////////////////////////////////////////////////////////////////////////
//
//                    map the action by action ID
//
// ////////////////////////////////////////////////////////////////////////////
class QXPACK_IC_HIDDEN IcActionExec_NodeMap {
private: QMap<int,IcActionExec_Node*> m_act_map;
public :
    IcActionExec_NodeMap ( ) { }
    ~IcActionExec_NodeMap( );
    inline int  size ( ) { return m_act_map.size(); }
    IcActionExec_Node*  query ( int act_id );
    bool   takeAndAdd ( IcActionExec_Node * );
    IcActionExec_Node *   find( int act_id );
};

// ============================================================================
// dtor
// ============================================================================
IcActionExec_NodeMap :: ~IcActionExec_NodeMap( )
{
    QMap<int,IcActionExec_Node*>::const_iterator itr = m_act_map.cbegin();
    while ( itr != m_act_map.cend()) {
        qxpack_ic_delete( itr.value(), IcActionExec_Node );
        ++ itr;
    }
    m_act_map = QMap<int,IcActionExec_Node*>();
}

// ============================================================================
// query node by action ID
// ============================================================================
IcActionExec_Node *  IcActionExec_NodeMap :: query ( int act_id )
{
    QMap<int,IcActionExec_Node*>::const_iterator itr = m_act_map.constFind(act_id);
    if ( itr != m_act_map.cend()) {
        return itr.value();
    } else {
        return Q_NULLPTR;
    }
}

// ============================================================================
// add new node
// ============================================================================
bool   IcActionExec_NodeMap :: takeAndAdd ( IcActionExec_Node *node )
{
    if ( this->query( node->actId()) == Q_NULLPTR ) {
        m_act_map.insert( node->actId(), node );
        return true;
    } else {
        return false;
    }
}

// ============================================================================
// find the action node by action id
// ============================================================================
IcActionExec_Node *   IcActionExec_NodeMap :: find ( int act_id )
{
    QMap<int,IcActionExec_Node*>::const_iterator itr = m_act_map.constFind( act_id );
    return ( itr != m_act_map.cend() ? itr.value() : Q_NULLPTR );
}




// ////////////////////////////////////////////////////////////////////////////
//
//             define a event jumper to do event quit callback
//
// ////////////////////////////////////////////////////////////////////////////
class QXPACK_IC_HIDDEN IcActionExec_EvtJumper : public QObject {
     Q_OBJECT
public:
     IcActionExec_EvtJumper( QObject *pa ) : QObject( pa ) { }
     virtual ~IcActionExec_EvtJumper( ) Q_DECL_OVERRIDE { }
     Q_INVOKABLE void callEventQuit( QObject *tgt, QObject *evt )
     { QMetaObject::invokeMethod( tgt, "doEventQuit", Qt::QueuedConnection, Q_ARG( QObject*, evt )); }
};


// ////////////////////////////////////////////////////////////////////////////
//
//     ActionExecPrivate object ( maybe live in other QThread )
//
// ////////////////////////////////////////////////////////////////////////////
#define T_PrivPtr( o )  (( IcActionExecPriv *) o )
class QXPACK_IC_HIDDEN  IcActionExecPriv : public QObject {
    Q_OBJECT
private:
    IcActionExec *m_parent;
    QThread      *m_act_thread;  // if it is creator thread, setup it to NULL
    IcActionExec_NodeMap  m_node_map;
    IcActionExec_ReqList  m_req_list;
    IcObjectCache *m_reqcache;
public :
    IcActionExecPriv ( IcActionExec *pa, QThread *t );
    virtual ~IcActionExecPriv ( );

    // below functions can call it directly, suffix 'di' means 'directly'
    inline IcObjectCache *  reqCache_di () { return m_reqcache; }
    inline QThread *        actThread_di() { return m_act_thread; }
    inline bool   hasActionReq_di() { return ( m_req_list.size() > 0 ); }
    inline bool   enqueueReq_di( IcActionExec_Req *req ) { return m_req_list.enqueue( req ); }

    // these functions running in the act_thread or caller thread
    Q_SLOT void  processAllAction( void* );
    Q_SLOT void  doEventQuit     ( QObject* );
    Q_SLOT bool  takeAndAddNode ( void *node );
    Q_SLOT bool  doAction ( void *req );

    Q_SIGNAL void stateChanged( int act_id, int curr_state, int target_state );
};

// ============================================================================
// CTOR
// ============================================================================
IcActionExecPriv :: IcActionExecPriv ( IcActionExec *pa, QThread *t )
                 : QObject( Q_NULLPTR )
{
    m_act_thread = t; m_parent = pa;
    m_reqcache   = gReqCache_getInstance();
}

// ============================================================================
// DTOR
// ============================================================================
IcActionExecPriv :: ~IcActionExecPriv ( )
{
    gReqCache_freeInstance();
}

// ============================================================================
// SLOT: do the event quit call back function
// ============================================================================
void  IcActionExecPriv :: doEventQuit( QObject *obj )
{
    QEventLoop *evt = qobject_cast< QEventLoop *>( obj );
    if ( evt != Q_NULLPTR ) {
        QMetaObject::invokeMethod( evt, "quit", Qt::AutoConnection );
    }
}

// ============================================================================
// SLOT: process all actions
// ============================================================================
void  IcActionExecPriv :: processAllAction( void *sem_ptr )
{
    QSemaphore *sem = ( QSemaphore*) sem_ptr;
    while ( m_req_list.size() > 0 ) { this->doAction( Q_NULLPTR ); }
    if ( sem != Q_NULLPTR ) { sem->release(1); }
}

// ============================================================================
// add a new node into map
// ============================================================================
bool   IcActionExecPriv :: takeAndAddNode ( void *ptr )
{
    if ( ptr != Q_NULLPTR ) {
        return m_node_map.takeAndAdd( ( IcActionExec_Node *) ptr );
    } else {
        return false;
    }
}

// ============================================================================
// pick action from list, and process it
// ============================================================================
bool   IcActionExecPriv :: doAction( void *req_ptr )
{
    bool is_post_call = true;

    // pick up request
    IcActionExec_Req *req = ( IcActionExec_Req *)( req_ptr );
    if ( req == Q_NULLPTR ) {
        req = ( IcActionExec_Req *) m_req_list.deque();
    } else {
        is_post_call = false;
    }
    if ( req == Q_NULLPTR ) { return false; }

    // try find the action node
    IcActionExec_Node *node = m_node_map.find( req->actId());
    if ( node == Q_NULLPTR ) {
        if ( is_post_call ) { m_reqcache->recycle( req ); } // send call will recycle the request
        return false;
    }

    // process it
    IcActionExec::State prev_st = node->state();
    bool ret = node->doRequest( *req );
    if ( is_post_call ) { m_reqcache->recycle( req ); } // send call will recycle request.

    // while do request success, the state changed.
    if ( ret ) { emit stateChanged( req->actId(), prev_st, node->state() ); }
    return ret;
}




// ////////////////////////////////////////////////////////////////////////////
//
//                         Action Exec wrapper
//
// ////////////////////////////////////////////////////////////////////////////
// ============================================================================
// ctor
// ============================================================================
IcActionExec :: IcActionExec ( QThread *act_thread, QObject *pa) : QObject( pa )
{
    if ( act_thread != Q_NULLPTR  &&  act_thread != QThread::currentThread() ) {
        m_obj = qxpack_ic_new_qobj( IcActionExecPriv, this, act_thread );
        T_PrivPtr( m_obj )->moveToThread( act_thread );
    } else {
        m_obj = qxpack_ic_new_qobj( IcActionExecPriv, this, Q_NULLPTR );
    }
    QObject::connect(
        T_PrivPtr( m_obj ), SIGNAL(stateChanged(int,int,int)),
        this, SIGNAL(stateChanged(int,int,int)), Qt::QueuedConnection
    );
}

// ============================================================================
// dtor
// ============================================================================
IcActionExec :: ~IcActionExec ( )
{
    if ( m_obj != Q_NULLPTR ) {
        //T_PrivPtr( m_obj )->blockSignals( true ); // nw( 20190522 1658 ): added
        IcRmtObjSigBlocker::blockSignals(   // nw( 20190604 1102 ): replaced
            T_PrivPtr( m_obj ), true
        );

        // -----------------------------------------------------
        // scenario A: obj.thread == priv.thread
        // 1) caller.thread == obj.thread, directly delete it
        // 2) caller.thread != obj.thread, call delete later
        //
        // scenario B: obj.thread != priv.thread
        // * always call delete later
        // -----------------------------------------------------
        if ( this->thread() == T_PrivPtr( m_obj )->thread()) {
            if ( QThread::currentThread() == this->thread()) {
                qxpack_ic_delete_qobj( T_PrivPtr( m_obj ) );
            } else {
                qxpack_ic_delete_qobj_later( T_PrivPtr( m_obj ) );
            }
        } else {
            qxpack_ic_delete_qobj_later( T_PrivPtr( m_obj ) );
        }
        m_obj = Q_NULLPTR;
    }
}

// ============================================================================
// wait for all action has been done
// ============================================================================
static void  gIcActionExec_blockCall_ProcessAllActions( IcActionExecPriv *obj )
{
    QSemaphore sem;
    QMetaObject::invokeMethod(
         obj, "processAllAction", Qt::AutoConnection, Q_ARG( void*, (void*)&sem )
    );
    sem.acquire(1);
}

static void  gIcActionExec_eventCall_callEventQuit( IcActionExecPriv *obj )
{
     // -----------------------------------------------------------------------
     // we use jumper to delay a event call in event queue
     // while target object process this event, means actions before
     // this event posted are all done.
     // -----------------------------------------------------------------------
     QEventLoop evt;
     IcActionExec_EvtJumper *jumper = new IcActionExec_EvtJumper( &evt ); // create object and hook it on parent
     QMetaObject::invokeMethod(
         jumper, "callEventQuit", Qt::QueuedConnection,
         Q_ARG( QObject*, obj ), Q_ARG( QObject*, &evt )
     );
     evt.exec(); // NOTE: here will execute the EvtJumper object queued call.
}

void  IcActionExec :: waitForAllDone ( bool req_proc_event )
{
    if ( ! req_proc_event ) { // no QEventLoop required
         // --------------------------------------------------------------------
         // scenario: priv.thread == obj.thread
         // 1) caller.thread == obj.thread, invokeMethod will be direct  call
         // 2) caller.thread != obj.thread, invokeMethod will be queued  call
         //
         // scenario: priv.thread != obj.thread
         // 1) caller.thread == obj.thread, invokeMethod will be queued   call
         // 2) caller.thread != obj.thread
         //   2-1) caller.thread == priv.thread, direct call
         //   2-2) caller.thread != priv.thread, queued call
         // --------------------------------------------------------------------
         gIcActionExec_blockCall_ProcessAllActions( T_PrivPtr( m_obj ));

    } else {  // QEventLoop required
        // ---------------------------------------------------------------------
        // scenario A: priv.thread == obj.thread
        // 1) caller.thread == obj.thread, block call ( auto, direct call )
        // 2) caller.thread != obj.thread, event call
        //
        // scenario B: priv.thread != obj.thread
        // 1) caller.thread == obj.thread, event call
        // 2) caller.thread != obj.thread
        //   2-1) caller.thread == priv.thread, block call ( auto, direct call )
        //   2-2) caller.thread != priv.thread, event call
        // ---------------------------------------------------------------------
        if ( this->thread() == T_PrivPtr( m_obj )->thread() ) { // A
            if ( this->thread() == QThread::currentThread() ) {  //  A.1
                T_PrivPtr( m_obj )->processAllAction( Q_NULLPTR );
            } else { // A.2
                gIcActionExec_eventCall_callEventQuit( T_PrivPtr( m_obj ));
            }
        } else { // B
            if ( this->thread() == QThread::currentThread()) { // B.1
                gIcActionExec_eventCall_callEventQuit( T_PrivPtr( m_obj ));
            } else { // B.2
                if ( QThread::currentThread() == T_PrivPtr( m_obj )->thread()) { // B.2-1
                    T_PrivPtr( m_obj )->processAllAction( Q_NULLPTR );
                } else { // B.2-2
                    gIcActionExec_eventCall_callEventQuit( T_PrivPtr( m_obj ));
                }
            }
        }
     }
}

// ============================================================================
// check if existed action thread
// ============================================================================
bool   IcActionExec :: hasActionThread( ) const
{   return T_PrivPtr( m_obj )->hasActionReq_di(); }

// ============================================================================
// build new action node
// ============================================================================
static bool  gIcActionExec_bQcCall_addNode( IcActionExecPriv *obj, IcActionExec_Node *n )
{
    bool func_ret = false;
    bool call_ret = QMetaObject::invokeMethod (
            obj, "takeAndAddNode", Qt::BlockingQueuedConnection,
            Q_RETURN_ARG( bool, func_ret ), Q_ARG( void*, n )
        );
    if ( ! call_ret ) { func_ret = false; }
    return func_ret;
}

// ============================================================================
// append a node into list
// ============================================================================
bool   IcActionExec :: appendNode ( void *node_ptr )
{
    bool ret = false;
    if ( node_ptr == Q_NULLPTR ) { return ret; }
    IcActionExec_Node *node = ( IcActionExec_Node *)( node_ptr );

    // ------------------------------------------------------------------------
    //                  dispatch call
    //
    // scenario A: priv.thread == obj.thread
    // 1) caller.thread == priv.thread, direct call
    // 2) caller.thread != priv.thread, blocking call
    //
    // scenario B: priv.thread != obj.thread
    // 1) caller.thread == obj.thread, blocking call
    // 2) caller.thread != obj.thread
    //  2-1) caller.thread == priv.thread, direct call
    //  2-2) caller.thread != priv.thread, blocking call
    // ------------------------------------------------------------------------
    if ( T_PrivPtr( m_obj )->thread() == this->thread()) { // A
        if ( QThread::currentThread() == this->thread()) {   // A.1
            ret = T_PrivPtr( m_obj )->takeAndAddNode( node );
        } else {  // A.2
            ret = gIcActionExec_bQcCall_addNode( T_PrivPtr( m_obj ), node );
        }
    } else {  // B
        if ( QThread::currentThread() == this->thread() ) { // B.1
            ret = gIcActionExec_bQcCall_addNode( T_PrivPtr( m_obj ), node );
        } else { // B.2
            if ( QThread::currentThread() == T_PrivPtr( m_obj )->thread()) { // B.2-1
                ret = T_PrivPtr( m_obj )->takeAndAddNode( node );
            } else {  // B.2-2
                ret = gIcActionExec_bQcCall_addNode( T_PrivPtr( m_obj ), node );
            }
        }
    }

    return ret;
}

// ============================================================================
// build a node and add it
// ============================================================================
bool   IcActionExec :: buildNode (
        int act_id, ActionType act_type, ActionHandler handler, void *ctxt
) {
    // ------------------------------------------------------------------------
    // prepare a node, the node map will destroy in node map DTOR
    // ------------------------------------------------------------------------
    IcActionExec_Node *node = qxpack_ic_new ( IcActionExec_Node, act_id, act_type, handler, ctxt );

    // ------------------------------------------------------------------------
    // add the node
    // ------------------------------------------------------------------------
    bool ret = this->appendNode( node );

    // ------------------------------------------------------------------------
    // if processed failed, need delete the node
    // ------------------------------------------------------------------------
    if ( ! ret ) { qxpack_ic_delete( node, IcActionExec_Node ); }
    return ret;
}

// ============================================================================
// static function, post request or recycle request
// ============================================================================
static bool  gIcActionExec_PostOrRecycleReq( IcActionExec *obj, IcActionExecPriv *priv, IcActionExec_Req *req )
{
    bool ret = priv->enqueueReq_di( req );
    if ( ret ) {
        // --------------------------------------------------------------------
        // scenario A: obj.thread == priv.thread
        // 1) caller.thread == obj.thread, post call to priv.thread ( obj.thread )
        // 2) caller.thread != obj.thread, post call to priv.thread ( obj.thread )
        //
        // scenario B: obj.thread != priv.thread
        // 1) caller.thread == obj.thread, post call to priv.thread
        // 2) caller.thread != obj.thread
        //   2-1) caller.thread == priv.thread, directly call priv.thread
        //   2-2) caller.thread != priv.thread, post call to priv.thread
        // --------------------------------------------------------------------
        if ( priv->thread() == obj->thread()) { // A
            QMetaObject::invokeMethod( priv, "doAction", Qt::QueuedConnection, Q_ARG( void*, Q_NULLPTR ));
        } else { // B
            if ( QThread::currentThread() == obj->thread()) {
                QMetaObject::invokeMethod( priv, "doAction", Qt::QueuedConnection, Q_ARG( void*, Q_NULLPTR ));
            } else {
                if ( QThread::currentThread() == priv->thread() ) {
                    priv->doAction( Q_NULLPTR );
                } else {
                    QMetaObject::invokeMethod( priv, "doAction", Qt::QueuedConnection, Q_ARG( void*, Q_NULLPTR ));
                }
            }
        }
    } else {
        priv->reqCache_di()->recycle( req );
    }
    return ret;
}

// ============================================================================
// post a request to execute the action
// ============================================================================
bool   IcActionExec :: post (
    int act_id, const QJsonObject &jo, const IcVarData &ext_param
) {
    IcActionExec_Req *req = ( IcActionExec_Req *)  T_PrivPtr( m_obj )->reqCache_di()->allocate();
    req->setupCall( act_id, jo, ext_param );
    return gIcActionExec_PostOrRecycleReq( this, T_PrivPtr( m_obj ), req );
}

bool   IcActionExec :: postCancel ( int act_id )
{
    IcActionExec_Req *req = ( IcActionExec_Req *) T_PrivPtr( m_obj )->reqCache_di()->allocate();
    req->setupCancel( act_id );
    return gIcActionExec_PostOrRecycleReq( this, T_PrivPtr( m_obj ), req );
}

bool   IcActionExec :: postFreeze ( int act_id )
{
    IcActionExec_Req *req = ( IcActionExec_Req *) T_PrivPtr( m_obj )->reqCache_di()->allocate();
    req->setupFreeze( act_id );
    return gIcActionExec_PostOrRecycleReq( this, T_PrivPtr( m_obj ), req );
}

bool  IcActionExec :: postUnfreeze( int act_id )
{
    IcActionExec_Req *req = ( IcActionExec_Req *) T_PrivPtr( m_obj )->reqCache_di()->allocate();
    req->setupUnfreeze( act_id );
    return gIcActionExec_PostOrRecycleReq( this, T_PrivPtr( m_obj ), req );
}

// ============================================================================
// static function, send request and recycle request
// ============================================================================
static bool  gIcActionExec_SendReq( IcActionExec *obj, IcActionExecPriv *priv, IcActionExec_Req *req )
{
    bool ret = false; bool call_ret = false;

    // ------------------------------------------------------------------------
    // scenario A: obj.thread == priv.thread
    // 1) caller.thread == obj.thread, directly call
    // 2) caller.thread != obj.thread, blocking call
    //
    // scenario B: obj.thread != priv.thread
    // 1) caller.thread == obj.thread, blocking call
    // 2) caller.thread != obj.thread,
    //   2-1) caller.thread == priv.thread, directly call
    //   2-2) caller.thread != priv.thread, blocking call
    // ------------------------------------------------------------------------
    if ( priv->thread() == obj->thread() ) { // A
        if ( QThread::currentThread() == obj->thread() ) { // A.1
            call_ret = ret = priv->doAction( req );
        } else {  // A.2
            call_ret = QMetaObject::invokeMethod (
                priv, "doAction", Qt::QueuedConnection, Q_ARG( void*, (void*)req )
            );
            gIcActionExec_eventCall_callEventQuit( T_PrivPtr( priv ));
        }
    } else { // B
        if ( QThread::currentThread() == obj->thread() ) { // B.1
            call_ret = QMetaObject::invokeMethod (
                priv, "doAction", Qt::QueuedConnection, Q_ARG( void*, (void*)req )
            );
            gIcActionExec_eventCall_callEventQuit( T_PrivPtr( priv ));
        } else {
            if ( QThread::currentThread() == priv->thread() ) { // B.2-1
                call_ret = ret = priv->doAction( req );
            } else { // B.2-2
                call_ret = QMetaObject::invokeMethod (
                    priv, "doAction", Qt::QueuedConnection, Q_ARG( void*, (void*)req )
                );
                gIcActionExec_eventCall_callEventQuit( T_PrivPtr( priv ));
            }
        }
    }

    if ( ! call_ret ) { ret = false; }
    return ret;
}

// ============================================================================
// send a request and waiting for it processed
// ============================================================================
bool  IcActionExec :: send (
      int act_id, const QJsonObject &jo, const IcVarData &ext_param
) {
    IcActionExec_Req req; req.setupCall( act_id, jo, ext_param );
    return gIcActionExec_SendReq( this, T_PrivPtr( m_obj ), & req );
}

bool  IcActionExec :: sendCancel ( int act_id )
{
    IcActionExec_Req req; req.setupCancel( act_id );
    return gIcActionExec_SendReq( this, T_PrivPtr( m_obj ), & req );
}

bool  IcActionExec :: sendFreeze ( int act_id )
{
    IcActionExec_Req req; req.setupFreeze( act_id );
    return gIcActionExec_SendReq( this, T_PrivPtr( m_obj ), & req );
}

bool  IcActionExec :: sendUnfreeze( int act_id )
{
    IcActionExec_Req req; req.setupUnfreeze( act_id );
    return gIcActionExec_SendReq( this, T_PrivPtr( m_obj ), & req );
}


// ////////////////////////////////////////////////////////////////////////////
//
// helper functions
//
// ////////////////////////////////////////////////////////////////////////////
// ============================================================================
// find the ActionID enum
// ============================================================================
QMetaEnum  IcActionExec :: findActionIDEnum ( QObject *obj )
{
    if ( obj == Q_NULLPTR ) { return QMetaEnum(); }
    const QMetaObject *mo = obj->metaObject();
    int cnt = mo->enumeratorCount();
    QMetaEnum  me;
    for ( int x = 0; x < cnt; x ++ ) {
        me = mo->enumerator( x );
        if ( qstrcmp( me.name(), "ActionID" ) == 0 ) {
            break;
        }
    }
    return me;
}

// ============================================================================
// pick up all methods maybe action handler
// ============================================================================
static QMap<QByteArray,QMetaMethod>  gPickupMethodsStartWith(
    const QMetaObject *mo, const char *str
) {
    QMap<QByteArray,QMetaMethod> func_map;
    int cnt = mo->methodCount();
    for ( int x = 0; x < cnt; x ++ ) {
        QMetaMethod mm = mo->method( x );
        if ( mm.returnType() != QMetaType::Bool ) { continue; }
        if ( mm.methodType() != QMetaMethod::Slot &&
             mm.methodType() != QMetaMethod::Method )
        { continue; }
        QByteArray  ba = mm.name();
        if ( ba.startsWith(str) ) { func_map.insert( ba, mm ); continue; }
    }
    return func_map;
}

// ============================================================================
// build the nodes by QObject
// ============================================================================
void   IcActionExec :: autoBuildNodes( QObject *cb_obj, const QMetaEnum &act_tbl )
{
    if ( cb_obj == Q_NULLPTR ) { return; }
    if ( ! act_tbl.isValid() ) { return; }

    // ------------------------------------------------------------------------
    // pick up all methods that maybe action handler
    // ------------------------------------------------------------------------
    QMap<QByteArray,QMetaMethod> trg_func = gPickupMethodsStartWith( cb_obj->metaObject(), "onTrg_");
    QMap<QByteArray,QMetaMethod> sw_func  = gPickupMethodsStartWith( cb_obj->metaObject(), "onSw_" );

    // ------------------------------------------------------------------------
    // add nodes
    // ------------------------------------------------------------------------
    int cnt = act_tbl.keyCount();
    for ( int x = 0; x < cnt; x ++ ) {
        QString func_name; QMap<QByteArray,QMetaMethod>::const_iterator c_itr;

        // test the trigger func
        func_name = QString("onTrg_%1").arg( QString( act_tbl.key(x)));
        c_itr = trg_func.find( func_name.toUtf8());
        if ( c_itr != trg_func.cend()) {
            IcActionExec_Node *node = qxpack_ic_new(
                IcActionExec_Node, act_tbl.value(x), IcActionExec::ActionType_Trigger,
                c_itr.value(), cb_obj, Q_NULLPTR
            );
            if ( ! this->appendNode( node )) { qxpack_ic_delete( node, IcActionExec_Node );}
            continue;
        }

        // test the switch func
        func_name = QString("onSw_%1").arg( QString( act_tbl.key(x)));
        c_itr = sw_func.find( func_name.toUtf8());
        if ( c_itr != sw_func.cend()) {
            IcActionExec_Node *node = qxpack_ic_new(
                IcActionExec_Node, act_tbl.value(x), IcActionExec::ActionType_Switch,
                c_itr.value(), cb_obj, Q_NULLPTR
            );
            if ( ! this->appendNode( node )) { qxpack_ic_delete( node, IcActionExec_Node );}
            continue;
        }
    }
}

}

#include "qxpack_ic_actionexec.moc"
#endif
