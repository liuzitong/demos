// ============================================================================
// author: night wing
// date stamp: 20190523 1603
//  1) use qxpack_ic_new_qobj instead new QObject..
//  2) blockSignals while dtor calling.
// ============================================================================
#ifndef QXPACK_IC_CONFIRMBUS_CXX
#define QXPACK_IC_CONFIRMBUS_CXX

#include "qxpack_ic_confirmbus.hxx"
#include "qxpack/indcom/common/qxpack_ic_memcntr.hxx"
#include "qxpack/indcom/common/qxpack_ic_pimplprivtemp.hpp"
#include "qxpack/indcom/sys/qxpack_ic_workercreatortemp_priv.hpp"
#include "qxpack/indcom/sys/qxpack_ic_sysevtqthread_priv.hxx"

#include <QDebug>
#include <QtGlobal>
#include <QMap>
#include <QMutex>
#include <QMetaObject>
#include <QMetaMethod>
#include <QList>
#include <QSemaphore>
#include <QAtomicPointer>

namespace QxPack {

// ////////////////////////////////////////////////////////////////////////////
//
//        used for caller and cfm swap information
//
// ////////////////////////////////////////////////////////////////////////////
class QXPACK_IC_HIDDEN  IcConfirmBus_ConfirmContext {
private:
    QSemaphore  *m_caller_sem, *m_cfm_sem;
    QAtomicPointer<QObject>  m_atom_obj;
    QThread *  m_caller_thread;
public :
    IcConfirmBus_ConfirmContext( QThread *t_caller, bool use_sem = false )
    {
        m_caller_thread = t_caller;
        m_caller_sem = ( use_sem ? new QSemaphore : Q_NULLPTR );
        m_cfm_sem    = ( use_sem ? new QSemaphore : Q_NULLPTR );
        m_atom_obj.store( Q_NULLPTR );
    }
    ~IcConfirmBus_ConfirmContext( )
    {
        if ( m_caller_sem != Q_NULLPTR ) { delete m_caller_sem; }
        if ( m_cfm_sem    != Q_NULLPTR ) { delete m_cfm_sem;    }
    }

    inline QThread*     callerThread( ) { return m_caller_thread; }
    inline QSemaphore*  callerSem( )    { return m_caller_sem; }
    inline QSemaphore*  cfmSem ( )      { return m_cfm_sem;    }
    inline QAtomicPointer<QObject>  &  atomObj() { return m_atom_obj; }
};





// ////////////////////////////////////////////////////////////////////////////
//
// the confirm list group is used for store the listener of the confirm
//
// ////////////////////////////////////////////////////////////////////////////
typedef  QList<QObject*>  IcConfirmBus_GroupMember_List;
class QXPACK_IC_HIDDEN IcConfirmBus_Group : public QObject {
    Q_OBJECT
private:
    QMutex m_locker; IcConfirmBus_GroupMember_List m_member_list;
protected:
    Q_SLOT void member_onDestoryed( QObject *o ) { this->rmv( o, true ); }
public :
    IcConfirmBus_Group ( QObject *pa = Q_NULLPTR );
    virtual ~IcConfirmBus_Group( ) Q_DECL_OVERRIDE;

    inline bool isEmpty() { m_locker.lock(); bool ret = m_member_list.isEmpty(); m_locker.unlock(); return ret; }
    bool   add ( QObject* );
    bool   rmv ( QObject*, bool is_destoryed = false );
    void   invokeConfirm( IcConfirmBusPkg &pkg, IcConfirmBus_ConfirmContext *ctxt );
};

// ============================================================================
// ctor
// ============================================================================
IcConfirmBus_Group :: IcConfirmBus_Group ( QObject *pa )
      : QObject( pa ), m_locker( QMutex::Recursive )
{
}

// ============================================================================
// dtor
// ============================================================================
IcConfirmBus_Group :: ~IcConfirmBus_Group ( )
{
    m_locker.lock();

    IcConfirmBus_GroupMember_List::const_iterator c_itr = m_member_list.constBegin();
    while ( c_itr != m_member_list.constEnd()) {
        QObject *obj = (*c_itr ++ );
        if ( obj != Q_NULLPTR ) {
            QObject::disconnect( this, Q_NULLPTR, obj, Q_NULLPTR );
            QObject::disconnect( obj, Q_NULLPTR, this, Q_NULLPTR );
        }
    }
    m_locker.unlock();
}

// ============================================================================
// add a new member into group
// ============================================================================
bool  IcConfirmBus_Group :: add ( QObject *m )
{
    if ( m == Q_NULLPTR ) { return false; }

    IcConfirmBusPkg();  // ensure register the meta info into Qt5 meta system

    m_locker.lock();
 
    // ------------------------------------------------------------------------
    // NOTE: here always pre-append object in the list
    // ------------------------------------------------------------------------
    if ( m_member_list.indexOf(m) == -1 ) {
        m_member_list.push_front( m );
        QObject::connect(  // m::destoryed() -> this::onDestoryed()
            m,        SIGNAL(destroyed(QObject*)),
            this,     SLOT(member_onDestoryed(QObject*)),
            Qt::DirectConnection
        );
    }

    m_locker.unlock();

    return true;
}

// ============================================================================
// remove a member from group
// ============================================================================
bool   IcConfirmBus_Group :: rmv ( QObject *m, bool )
{
    bool ret = false;
    if ( m == Q_NULLPTR ) { return ret; }

    m_locker.lock();

    // ------------------------------------------------------------------------
    //  remove object in list
    // ------------------------------------------------------------------------
    QObject::disconnect( m, Q_NULLPTR, this, Q_NULLPTR );
    QObject::disconnect( this, Q_NULLPTR, m, Q_NULLPTR );
    ret = ( m_member_list.removeAll( m ) > 0 );

    m_locker.unlock();

    return ret;
}

// ============================================================================
// invoke the confirm
// ============================================================================
void   IcConfirmBus_Group :: invokeConfirm(IcConfirmBusPkg &pkg, IcConfirmBus_ConfirmContext *ctxt )
{
    // ------------------------------------------------------------------------
    // Below conditions in calling scenario:
    //   call(T) as T1, cfm(T) as T2, obj(T) as T3
    // 1) [ T1, T2 ] [ T3 ]    blocking queued
    // 2) [ T1, T2, T3 ]       directly call
    // 3) [ T1 ] [ T2, T3 ]    directly call
    // 4) [ T1, T3 ], [ T2 ]   tell T1 to call
    // 5) [ T1 ] [ T2 ] [ T3 ] blocking queued
    //     directly call(0), blocking queued (1), tell T1 to call (2)
    // ------------------------------------------------------------------------
    QThread *t_cfm = this->thread();
    QThread *t_call= ctxt->callerThread();
    m_locker.lock();

    QList<QObject*>::const_iterator c_itr = m_member_list.constBegin();
    while ( c_itr != m_member_list.constEnd()) {
        QObject *obj = (*c_itr ++ );  
        QThread *t_obj = obj->thread();

        if        ( t_call == t_cfm  &&  t_cfm != t_obj ) {
            QMetaObject::invokeMethod (
              obj, "IcConfirmBus_onReqConfirm", Qt::BlockingQueuedConnection, Q_ARG( QxPack::IcConfirmBusPkg&, pkg )
            );
        } else if ( t_call == t_cfm  &&  t_cfm == t_obj ) {
            QMetaObject::invokeMethod (
                obj, "IcConfirmBus_onReqConfirm", Qt::DirectConnection, Q_ARG( QxPack::IcConfirmBusPkg&, pkg )
            );
        } else if ( t_call != t_cfm  &&  t_cfm == t_obj ) {
            QMetaObject::invokeMethod (
                obj, "IcConfirmBus_onReqConfirm", Qt::DirectConnection, Q_ARG( QxPack::IcConfirmBusPkg&, pkg )
            );
        } else if( t_call == t_obj  &&  t_cfm != t_obj ) {
            // NOTE: t_call != t_cfm always wait on caller_sem..
            while ( ! ctxt->atomObj().testAndSetOrdered( Q_NULLPTR, obj )) { }
            ctxt->callerSem()->release(1); // wake up caller to do...
            ctxt->cfmSem()->acquire(1);    // wait it finished
        }
        else if( t_call != t_cfm  &&  t_cfm != t_obj &&  t_call != t_obj ) {
            QMetaObject::invokeMethod (
              obj, "IcConfirmBus_onReqConfirm", Qt::BlockingQueuedConnection, Q_ARG( QxPack::IcConfirmBusPkg&, pkg )
            );
            ctxt->callerSem()->release(1);
        }
        else { qFatal("impossible cond.!");  }

        if ( pkg.isGotResult()) { break; }
    }

    m_locker.unlock();

    // ------------------------------------------------------------------------
    // IMPORTANT: wake up caller
    // ------------------------------------------------------------------------
    if ( ctxt->callerSem() != Q_NULLPTR ) { ctxt->callerSem()->release(1); }
}




// ////////////////////////////////////////////////////////////////////////////
//
//          all methods go here is always ordered
//
// ////////////////////////////////////////////////////////////////////////////
#define T_PrivPtr( o )  T_ObjCast( IcConfirmBusWorker*, o )
class QXPACK_IC_HIDDEN  IcConfirmBusWorker :
    public QObject, public IcPImplPrivTemp< IcConfirmBusWorker >{
    Q_OBJECT
private:
    QMap<QString,IcConfirmBus_Group*>  m_groups_map;
public :
    explicit IcConfirmBusWorker( QObject *pa = Q_NULLPTR );
    IcConfirmBusWorker( const IcConfirmBusWorker & );
    virtual ~IcConfirmBusWorker( ) Q_DECL_OVERRIDE;
    Q_INVOKABLE bool  add( QObject *, const QString & );
    Q_INVOKABLE bool  rmv( QObject *, const QString & );
    Q_INVOKABLE void  reqConfirm( QxPack::IcConfirmBusPkg &pkg, void *ctxt );
};

// ============================================================================
// ctor
// ============================================================================
IcConfirmBusWorker :: IcConfirmBusWorker( QObject *pa ) : QObject( pa )
{
}

// ============================================================================
// ctor ( copy )
// ============================================================================
IcConfirmBusWorker :: IcConfirmBusWorker ( const IcConfirmBusWorker& ) : QObject( Q_NULLPTR )
{
    qFatal("this is not supported!");
}

// ============================================================================
// dtor
// ============================================================================
IcConfirmBusWorker :: ~IcConfirmBusWorker( )
{
    QMap<QString,IcConfirmBus_Group*>::const_iterator c_itr = m_groups_map.constBegin();
    while ( c_itr != m_groups_map.constEnd()) {
        IcConfirmBus_Group *group = c_itr.value();
        if ( group != Q_NULLPTR ) {
            group->blockSignals(true);
            delete group;
        }
        ++ c_itr;
    }
}

// ============================================================================
// add the object into a specified group
// ============================================================================
bool   IcConfirmBusWorker :: add( QObject *m, const QString &grp_name )
{
    bool is_added = false;
    if ( m == Q_NULLPTR || grp_name.isEmpty() ) { return is_added; }

    // add into group
    IcConfirmBus_Group *group = Q_NULLPTR;
    QMap<QString,IcConfirmBus_Group*>::const_iterator c_itr = m_groups_map.constFind( grp_name );
    if ( c_itr == m_groups_map.constEnd() ) {
        group = qxpack_ic_new_qobj( IcConfirmBus_Group, Q_NULLPTR );
        is_added = group->add( m );
        if ( is_added ) {
            m_groups_map.insert( grp_name, group );
        } else {
            qxpack_ic_delete_qobj( group );
        }
    } else {
        group = c_itr.value();
        is_added = group->add( m );
    }

    return is_added;
}

// ============================================================================
// remove the object from specified group
// ============================================================================
bool  IcConfirmBusWorker :: rmv( QObject *m, const QString &grp_name )
{
    bool is_rmv = false;
    if ( m != Q_NULLPTR || grp_name.isEmpty() ) { return is_rmv; }

    IcConfirmBus_Group *group = Q_NULLPTR;
    QMap<QString,IcConfirmBus_Group*>::iterator itr = m_groups_map.find( grp_name );
    if ( itr != m_groups_map.end()) {
        group  = itr.value();  is_rmv = group->rmv( m );
        if ( group->isEmpty()) {
            m_groups_map.erase( itr );
            qxpack_ic_delete_qobj_later( group );
        }
    }

    return is_rmv;
}

// ============================================================================
// inovke the message
// ============================================================================
void   IcConfirmBusWorker :: reqConfirm( QxPack::IcConfirmBusPkg &pkg, void *ctxt )
{
    if ( pkg.groupName().isEmpty() ) { return; }

    // ------------------------------------------------------------------------
    // try find the group by name
    // ------------------------------------------------------------------------
    QMap<QString,IcConfirmBus_Group*>::const_iterator grp_itr = m_groups_map.constFind( pkg.groupName() );
    if ( grp_itr != m_groups_map.constEnd()) {
        IcConfirmBus_Group *group = grp_itr.value();
        group->invokeConfirm( pkg, reinterpret_cast<IcConfirmBus_ConfirmContext*>( ctxt ));
    }
}


// ////////////////////////////////////////////////////////////////////////////
//
//                  keep the life-cycle of worker
//
// ////////////////////////////////////////////////////////////////////////////
#define T_Creator( o )   T_ObjCast( IcConfirmBusWorkerCreator*, o )
class QXPACK_IC_HIDDEN  IcConfirmBusWorkerCreator :
    public QObject, public IcWorkerCreatorTemp< IcConfirmBusWorker >
{
    Q_OBJECT
private: IcSysEvtQThreadPriv m_sys_evt_thread;
public :
    IcConfirmBusWorkerCreator ( ) : QObject( Q_NULLPTR )
    {
        this->moveToThread( m_sys_evt_thread.thread());
    }

    virtual ~IcConfirmBusWorkerCreator( ) Q_DECL_OVERRIDE
    {
    }

    Q_INVOKABLE void  createWorker ( )
    {
        this->deleteWorker();
        m_wkr = qxpack_ic_new_qobj( IcConfirmBusWorker );
    }

    // ========================================================================
    //  user should use this to create a new creator that live in sys thread
    // ========================================================================
    static IcConfirmBusWorkerCreator*  newCreator( )
    {
        IcConfirmBusWorkerCreator *wkr = new IcConfirmBusWorkerCreator;
        if ( wkr->thread() != QThread::currentThread()) {
            QMetaObject::invokeMethod( wkr, "createWorker", Qt::BlockingQueuedConnection );
        } else {
            wkr->createWorker( );
        }
        return wkr;
    }

    // ========================================================================
    // delete the creator directly
    // ========================================================================
    static void  deleteCreator( IcConfirmBusWorkerCreator *wkr_cr )
    {
        wkr_cr->deleteWorker();
        wkr_cr->deleteLater();
    }
};

// ////////////////////////////////////////////////////////////////////////////
//                      wrapper API of IcConfirmBus
// <mem_cntr>: need not to count it due to worker creator object
// ////////////////////////////////////////////////////////////////////////////
// ============================================================================
// ctor
// ============================================================================
IcConfirmBus :: IcConfirmBus ( QObject *pa ) : QObject( pa )
{
    IcConfirmBusWorkerCreator *wkr_creator = IcConfirmBusWorkerCreator::newCreator();
    m_obj = wkr_creator;
}

// ============================================================================
// dtor
// ============================================================================
IcConfirmBus :: ~IcConfirmBus ( )
{
    IcConfirmBusWorkerCreator::deleteCreator( T_Creator( m_obj ) );
}

// ============================================================================
// add new object item into group
// ============================================================================
bool   IcConfirmBus :: add ( QObject *obj, const QString &grp_name )
{
    // ------------------------------------------------------------------------
    // check object
    // ------------------------------------------------------------------------
    if ( obj == Q_NULLPTR || grp_name.isEmpty() ) { return false; }

    // ------------------------------------------------------------------------
    // test whether exist the SLOT/Q_INVOKABLE or not
    // ------------------------------------------------------------------------
    const QMetaObject *meta_obj = obj->metaObject();
    int mc = meta_obj->methodCount();
    bool is_found_slot = false;
    for ( int x = meta_obj->methodOffset(); x < mc; x ++ ) {
        QMetaMethod mm = meta_obj->method(x);
        if ( mm.methodType() == QMetaMethod::Slot ) {
            QString mn = QString::fromLatin1(mm.methodSignature());
            if ( mn == QStringLiteral("IcConfirmBus_onReqConfirm(QxPack::IcConfirmBusPkg&)"))
            {
                is_found_slot = true; break;
            }
        }
    }

    // ------------------------------------------------------------------------
    // if found the method, then this object can add into message group
    // ------------------------------------------------------------------------
    bool is_added = false;
    if ( is_found_slot ) {
        if ( QThread::currentThread() != T_Creator( m_obj )->worker_di()->thread() ) {
            QMetaObject::invokeMethod (
                T_Creator( m_obj )->worker_di(),
                "add", Qt::BlockingQueuedConnection,
                Q_RETURN_ARG( bool, is_added ),
                Q_ARG( QObject*, obj ), Q_ARG( const QString&, grp_name )
            );
        } else {
            is_added = T_Creator( m_obj )->worker_di()->add( obj, grp_name );
        }
    }

    return is_added;
}

// ============================================================================
// remove an existed object from group
// ============================================================================
bool   IcConfirmBus :: rmv ( QObject *obj, const QString &grp_name )
{
    // ------------------------------------------------------------------------
    // check objects
    // ------------------------------------------------------------------------
    if ( obj == Q_NULLPTR || grp_name.isEmpty() ) { return false; }

    // ------------------------------------------------------------------------
    // remove from message group
    // ------------------------------------------------------------------------
    bool is_rmv = false;
    if ( QThread::currentThread() != T_Creator( m_obj )->worker_di()->thread()) {
        QMetaObject::invokeMethod(
            T_Creator( m_obj )->worker_di(),
            "rmv", Qt::BlockingQueuedConnection,
            Q_RETURN_ARG( bool, is_rmv ),
            Q_ARG( QObject*, obj ), Q_ARG( const QString &, grp_name )
        );
    } else {
        is_rmv = T_Creator( m_obj )->worker_di()->rmv( obj, grp_name );
    }

    return is_rmv;
}

// ============================================================================
// do request confirm
// ============================================================================
void   IcConfirmBus :: reqConfirm ( IcConfirmBusPkg &pkg )
{
    if ( pkg.groupName().isEmpty()) { return; }

    if ( QThread::currentThread() != T_Creator( m_obj )->worker_di()->thread() ) {
        // --------------------------------------------------------------------
        // caller(T) != Cfm(T), need do post action to cfm
        // --------------------------------------------------------------------
        IcConfirmBus_ConfirmContext ctxt( QThread::currentThread(), true );
        bool c_ret = QMetaObject::invokeMethod (
            T_Creator( m_obj )->worker_di(), "reqConfirm", Qt::QueuedConnection,
            Q_ARG( QxPack::IcConfirmBusPkg&, pkg ), Q_ARG( void*, reinterpret_cast<void*>( &ctxt ))
        );

        if ( c_ret ) {
            // ----------------------------------------------------------------
            // this loop is used to do the directly call confirm while meet cond.:
            // caller(T) != Cfm(T), but caller(T) == Obj(T)
            // ----------------------------------------------------------------
            do {
                // 0. waiting.
                ctxt.callerSem()->acquire(1);

                // 1. if cfm thread passed a object, means action: do confirm
                // otherwise, means end.
                QObject *cfm_obj = ctxt.atomObj().loadAcquire();
                ctxt.atomObj().store( Q_NULLPTR );
                if ( cfm_obj == Q_NULLPTR ) { break; }
                if ( cfm_obj->thread() != QThread::currentThread()) {
                    qFatal("cfm_obj->thread() != QThread::currentThread");
                }

                // 2. do confirm
                QMetaObject::invokeMethod (
                    cfm_obj, "IcConfirmBus_onReqConfirm", Qt::DirectConnection,
                    Q_ARG( QxPack::IcConfirmBusPkg&, pkg )
                );

                // 3. finish confirm, wake up confirm thread
                ctxt.cfmSem()->release(1);
            } while ( true );
        }
    } else {
        // --------------------------------------------------------------------
        // caller(T) == Cfm(T)
        // --------------------------------------------------------------------
        IcConfirmBus_ConfirmContext ctxt( QThread::currentThread(), false );
        T_Creator( m_obj )->worker_di()->reqConfirm( pkg, reinterpret_cast<void*>( &ctxt ));
    }
}




// ////////////////////////////////////////////////////////////////////////////
//
//                   Confirm  Message Package
//  this object is a shared data, not the Copy On Write.
//
// ////////////////////////////////////////////////////////////////////////////
#define T_MsgPkgPriv( o )  T_ObjCast( IcConfirmBusPkgPriv*, o)
class QXPACK_IC_HIDDEN IcConfirmBusPkgPriv : public IcPImplPrivTemp< IcConfirmBusPkgPriv > {
private:
    QMutex      m_locker;
    QString     m_group, m_msg;
    QJsonObject m_jo_param, m_jo_rsl;
    bool        m_is_got_rsl;
public :
    IcConfirmBusPkgPriv ( )
    {
        m_is_got_rsl = false;
    }
    IcConfirmBusPkgPriv ( const IcConfirmBusPkgPriv & )
    {
        qFatal("this function should not be called!");
    }
    virtual ~IcConfirmBusPkgPriv( );

    inline void setGroupName( const QString &nm ) { m_locker.lock(); m_group = nm;  m_locker.unlock(); }
    inline void setMessage  ( const QString &msg) { m_locker.lock(); m_msg   = msg; m_locker.unlock(); }
    inline void setJoParam  ( const QJsonObject &jo ) { m_locker.lock(); m_jo_param = jo; m_locker.unlock(); }
    inline void setResult   ( const QJsonObject &jo ) { m_locker.lock(); m_jo_rsl   = jo; m_is_got_rsl = true; m_locker.unlock(); }

    inline QString      groupName( ) { m_locker.lock(); QString g = m_group; m_locker.unlock(); return g; }
    inline QString      message  ( ) { m_locker.lock(); QString m = m_msg;   m_locker.unlock(); return m; }
    inline QJsonObject  joParam  ( ) { m_locker.lock(); QJsonObject j = m_jo_param; m_locker.unlock(); return j; }
    inline QJsonObject  result   ( ) { m_locker.lock(); QJsonObject r = m_jo_rsl;   m_locker.unlock(); return r; }

    inline bool         isGotRsl ( ) { m_locker.lock(); bool sw = m_is_got_rsl; m_locker.unlock(); return sw; }
};

IcConfirmBusPkgPriv :: ~IcConfirmBusPkgPriv ( )
{
}

// ////////////////////////////////////////////////////////////////////////////
//               confirm message package wrapper
// <mem_cntr>: need not to count due to used priv PIMPL
// ////////////////////////////////////////////////////////////////////////////
static bool g_is_cfm_msg_bus_pkg_reg = false;
static void gIcConfirmBusPkg_regInQt5( )
{
    if ( ! g_is_cfm_msg_bus_pkg_reg ) {
        qRegisterMetaType<QxPack::IcConfirmBusPkg>("QxPack::IcConfirmBusPkg");
        qRegisterMetaType<QxPack::IcConfirmBusPkg>("QxPack::IcConfirmBusPkg&");
        g_is_cfm_msg_bus_pkg_reg = true;
    }
}

// ============================================================================
// ctor
// ============================================================================
IcConfirmBusPkg :: IcConfirmBusPkg ( )
{
    m_obj = nullptr; gIcConfirmBusPkg_regInQt5();
}

// ============================================================================
// ctor ( copy )
// ============================================================================
IcConfirmBusPkg :: IcConfirmBusPkg ( const IcConfirmBusPkg &other )
{
    m_obj = nullptr; gIcConfirmBusPkg_regInQt5();
    IcConfirmBusPkgPriv::attach( &m_obj, const_cast<void**>( &other.m_obj ));
}

// ============================================================================
// operator =
// ============================================================================
IcConfirmBusPkg &  IcConfirmBusPkg :: operator = ( const IcConfirmBusPkg &other )
{
    IcConfirmBusPkgPriv::attach( &m_obj, const_cast<void**>( &other.m_obj ));
    return *this;
}

// ============================================================================
// dtor
// ============================================================================
IcConfirmBusPkg :: ~IcConfirmBusPkg ( )
{
    if ( m_obj != nullptr ) {
        IcConfirmBusPkgPriv::attach( & m_obj, nullptr );
        m_obj = nullptr;
    }
}

// ============================================================================
// property access
// ============================================================================
void    IcConfirmBusPkg :: setGroupName( const QString &nm )
{ IcConfirmBusPkgPriv::buildIfNull( & m_obj )->setGroupName( nm ); }

void    IcConfirmBusPkg :: setMessage  ( const QString &msg )
{ IcConfirmBusPkgPriv::buildIfNull( & m_obj )->setMessage( msg );    }

void    IcConfirmBusPkg :: setJoParam  ( const QJsonObject &jo )
{ IcConfirmBusPkgPriv::buildIfNull( & m_obj )->setJoParam( jo ); }

void    IcConfirmBusPkg :: setResult   ( const QJsonObject &jo )
{ IcConfirmBusPkgPriv::buildIfNull( & m_obj )->setResult ( jo ); }

QString  IcConfirmBusPkg :: groupName( ) const
{ return ( m_obj != Q_NULLPTR ? T_MsgPkgPriv( m_obj )->groupName() : QString() ); }

QString  IcConfirmBusPkg :: message( )  const
{ return ( m_obj != Q_NULLPTR ? T_MsgPkgPriv( m_obj )->message() : QString() );   }

QJsonObject  IcConfirmBusPkg :: joParam( ) const
{ return ( m_obj != Q_NULLPTR ? T_MsgPkgPriv( m_obj )->joParam() : QJsonObject() ); }

QJsonObject  IcConfirmBusPkg :: result ( ) const
{ return ( m_obj != Q_NULLPTR ? T_MsgPkgPriv( m_obj )->result() : QJsonObject() );  }

bool     IcConfirmBusPkg :: isGotResult( ) const
{ return ( m_obj != Q_NULLPTR ? T_MsgPkgPriv( m_obj )->isGotRsl() : false ); }


// ////////////////////////////////////////////////////////////////////////////
//
//               confirm message package qobject wrapper
//
// ////////////////////////////////////////////////////////////////////////////
IcConfirmBusPkgQObj :: IcConfirmBusPkgQObj ( QObject *pa ) : QObject( pa )
{
    m_ext = Q_NULLPTR;
}

IcConfirmBusPkgQObj :: ~IcConfirmBusPkgQObj ( )
{

}

void IcConfirmBusPkgQObj :: setContent( IcConfirmBusPkg &pkg )
{
    m_pkg = pkg;
    QMetaObject::invokeMethod( this, "groupNameChanged",   Qt::QueuedConnection );
    QMetaObject::invokeMethod( this, "messageChanged",     Qt::QueuedConnection );
    QMetaObject::invokeMethod( this, "joParamChanged",     Qt::QueuedConnection );
    QMetaObject::invokeMethod( this, "resultChanged",      Qt::QueuedConnection );
    QMetaObject::invokeMethod( this, "isGotResultChanged", Qt::QueuedConnection );
}

IcConfirmBusPkg  IcConfirmBusPkgQObj :: content() const
{   return m_pkg; }

QString   IcConfirmBusPkgQObj :: groupName( ) const
{ return m_pkg.groupName(); }

QString   IcConfirmBusPkgQObj :: message( ) const
{ return m_pkg.message(); }

QJsonObject  IcConfirmBusPkgQObj :: joParam( ) const
{ return m_pkg.joParam(); }

QJsonObject  IcConfirmBusPkgQObj :: result( ) const
{ return m_pkg.result(); }

bool    IcConfirmBusPkgQObj :: isGotResult() const
{ return m_pkg.isGotResult(); }

void    IcConfirmBusPkgQObj :: setGroupName( const QString &nm )
{
    m_pkg.setGroupName( nm );
    QMetaObject::invokeMethod( this, "groupNameChanged", Qt::QueuedConnection );
}

void    IcConfirmBusPkgQObj :: setMessage( const QString &m )
{
    m_pkg.setMessage( m );
    QMetaObject::invokeMethod( this, "messageChanged", Qt::QueuedConnection );
}

void    IcConfirmBusPkgQObj :: setJoParam (const QJsonObject &jo )
{
    m_pkg.setJoParam( jo );
    QMetaObject::invokeMethod( this, "setJoParam", Qt::QueuedConnection );
}

void    IcConfirmBusPkgQObj :: setResult ( const QJsonObject &r )
{
    m_pkg.setResult( r );
    QMetaObject::invokeMethod( this, "resultChanged", Qt::QueuedConnection );
    QMetaObject::invokeMethod( this, "isGotResultChanged", Qt::QueuedConnection );
}



}

#include "qxpack_ic_confirmbus.moc"
#endif
