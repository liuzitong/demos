// ============================================================================
// author: night wing
// date stamp: 20190523  check point
// ============================================================================
// ============================================================================
// author: night wing
// date stamp: 20190522 1701
//  1) blockSignals while dtor calling.
// ============================================================================
#ifndef QXPACK_IC_ACTSTATELISTENER_CXX
#define QXPACK_IC_ACTSTATELISTENER_CXX

#include "qxpack_ic_actstatelistener.hxx"
#include "qxpack/indcom/common/qxpack_ic_memcntr.hxx"
#include "qxpack/indcom/common/qxpack_ic_logging.hxx"
#include <QMap>
#include <QVector>
#include <QMutex>

namespace QxPack {

// ////////////////////////////////////////////////////////////////////////////
//
//                  descriptor of the listener
//
// ////////////////////////////////////////////////////////////////////////////
class QXPACK_IC_HIDDEN IcActStateListener_Node {
private:
    int m_act_id; void *m_ctxt; IcActStateListener::Handler m_handler;
public :
    IcActStateListener_Node  ( int act, void* ctxt, IcActStateListener::Handler h )
    { m_act_id = act; m_ctxt = ctxt; m_handler = h;  }
    ~IcActStateListener_Node ( ) { }

    inline int    actionId() const { return m_act_id; }
    inline void*  context()  const { return m_ctxt;   }
    inline IcActStateListener::Handler  handler( ) const { return m_handler; }
    inline bool   isEqualTo( int act, void *ctxt, IcActStateListener::Handler h )
    {
        return ( m_act_id == act && m_ctxt == ctxt && m_handler == h );
    }
};




// ////////////////////////////////////////////////////////////////////////////
//
//                private object of ActStateListener
//
// ////////////////////////////////////////////////////////////////////////////
#define T_PrivPtr( o ) (( IcActStateListenerPriv *) o )
class QXPACK_IC_HIDDEN IcActStateListenerPriv : public QObject {
    Q_OBJECT
private:
    IcActStateListener  *m_parent;
    QMultiMap<int,void*> m_node_map;
    QMutex m_locker;
protected:
    void   clearNodeMap( );
public :
    IcActStateListenerPriv ( );
    virtual ~IcActStateListenerPriv ( ) Q_DECL_OVERRIDE;
    inline void setupParent( IcActStateListener *pa ) { m_parent = pa; }
    void   setupHandler ( int act_id, IcActStateListener::Handler h, void *ctxt );
    Q_SLOT void  handleStateChanged( int act_id, int curr_st, int tgt_st );
};

// ============================================================================
// ctor
// ============================================================================
IcActStateListenerPriv :: IcActStateListenerPriv ( )
{
   m_parent = Q_NULLPTR;
}

// ============================================================================
// dtor
// ============================================================================
IcActStateListenerPriv :: ~IcActStateListenerPriv ( )
{
   this->blockSignals( true );
   this->clearNodeMap ( );
}

// ============================================================================
// clear the node map
// ============================================================================
void  IcActStateListenerPriv :: clearNodeMap( )
{
    QMultiMap<int,void*>::const_iterator itr = m_node_map.cbegin();
    while ( itr != m_node_map.cend() ) {
        qxpack_ic_delete( itr.value(), IcActStateListener_Node );
        ++ itr;
    }
    m_node_map = QMap<int,void*>();
}

// ============================================================================
// setup the handler
// ============================================================================
void  IcActStateListenerPriv :: setupHandler( int act_id, IcActStateListener::Handler h, void *ctxt )
{
    m_locker.lock();

    bool is_existed = false;

    // find or create new node list
    QMultiMap<int,void*>::const_iterator itr = m_node_map.constFind( act_id );
    while ( itr != m_node_map.constEnd() &&  itr.key() == act_id ) {
        IcActStateListener_Node *node = ( IcActStateListener_Node*) itr.value();
        if ( node->isEqualTo( act_id, ctxt, h )) { is_existed = true; break; }
        ++ itr;
    }

    // if not existed same node, just add it
    if ( ! is_existed ) {
        IcActStateListener_Node *node = qxpack_ic_new( IcActStateListener_Node, act_id, ctxt, h );
        m_node_map.insert( act_id, node );
    }

    m_locker.unlock();
}

// ============================================================================
// handle the state changed
// ============================================================================
void  IcActStateListenerPriv :: handleStateChanged ( int act_id, int curr_st, int tgt_st )
{
    // make a implicit shared copy
    m_locker.lock();
    QMultiMap<int,void*> tmp_map = m_node_map;
    m_locker.unlock();

    // dispatch
    QMultiMap<int,void*>::const_iterator itr = tmp_map.constFind( act_id );
    while ( itr != tmp_map.constEnd() && itr.key() == act_id ) {
        IcActStateListener_Node *node = static_cast< IcActStateListener_Node*>( itr.value());
        (* node->handler())( node->context(), act_id, curr_st, tgt_st );
        ++ itr;
    }
}




// ////////////////////////////////////////////////////////////////////////////
//
//                     wrap API
//
// ////////////////////////////////////////////////////////////////////////////
// ============================================================================
// ctor
// ============================================================================
IcActStateListener :: IcActStateListener ( QObject *pa ) : QObject( pa )
{
    m_obj = qxpack_ic_new( IcActStateListenerPriv );
}

// ============================================================================
// dtor
// ============================================================================
IcActStateListener :: ~IcActStateListener ( )
{
    this->blockSignals( true );
    qxpack_ic_delete( T_PrivPtr( m_obj ), IcActStateListenerPriv );
}

// ============================================================================
// setup callback handler
// ============================================================================
void  IcActStateListener :: setupHandler( int act_id, Handler h, void *ctxt )
{
    T_PrivPtr( m_obj )->setupHandler( act_id, h, ctxt );
}

// ============================================================================
// SLOT: handle the state change
// ============================================================================
void   IcActStateListener :: onStateChanged( int act_id, int curr_state, int target_state )
{
    T_PrivPtr( m_obj )->handleStateChanged( act_id, curr_state, target_state );
}



}

#include "qxpack_ic_actstatelistener.moc"
#endif
