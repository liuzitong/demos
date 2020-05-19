#ifndef QXPACK_IC_TRIPBUFFIPC_CXX
#define QXPACK_IC_TRIPBUFFIPC_CXX

#include "qxpack_ic_tripbuffipc.hxx"
#include <qxpack/indcom/common/qxpack_ic_memcntr.hxx>

#include <QSharedMemory>
//#include <QSystemSemaphore>
#include <new>             // replacement new
#include <cstring>
#include <QMutex>

#define T_PrivPtr( o )   (( IcTripBuffIpcPriv *) o )

namespace QxPack {

// ////////////////////////////////////////////////////////
//
// control area 
//
// ////////////////////////////////////////////////////////
class QXPACK_IC_HIDDEN IcTripBuffIpcPrivCtlArea {
private:
    qintptr  m_version,   m_is_dirty, m_size;
    qintptr  m_write_idx, m_read_idx, m_swap_idx;
public:
    IcTripBuffIpcPrivCtlArea( int size  )
    {  
        m_version = 0xc0;  m_is_dirty = 0; m_size = size;
        m_write_idx = 0; m_swap_idx = 1; m_read_idx = 2;
    }
    inline qintptr  version( )    const { return m_version;  }
    inline qintptr  writeIndex( ) const { return m_write_idx;}
    inline qintptr  readIndex ( ) const { return m_read_idx; }
    inline qintptr  swapIndex ( ) const { return m_swap_idx; }
    inline qintptr  isDirty( )    const { return m_is_dirty; }
    inline qintptr  size( )       const { return m_size;     }

    inline int    doWriteSwap( bool & is_old_picked )
    {
        qintptr t   = m_swap_idx; m_swap_idx  = m_write_idx;  m_write_idx = t;
        is_old_picked = ( m_is_dirty == 0 );
        m_is_dirty  = 1;
        return ( int ) m_write_idx;
    }

    inline int    doReadSwap( )
    {
        qintptr t  = m_swap_idx;  m_swap_idx = m_read_idx; m_read_idx = t;
        m_is_dirty = 0;        
        return ( int ) m_read_idx;
    }
};


// ////////////////////////////////////////////////////////
//
// IcTripBuffIpcPriv
//
// ////////////////////////////////////////////////////////
class  QXPACK_IC_HIDDEN IcTripBuffIpcPriv {

private:
    IcTripBuffIpc     *m_parent;
    QSharedMemory     *m_sm_ctl, *m_sm_data[3];
    //QSystemSemaphore  *m_sys_sem;
    QString m_name;
    int     m_last_write_idx, m_last_read_idx;
    int     m_size;
public:
    IcTripBuffIpcPriv( IcTripBuffIpc *, int size, const QString &name );
    IcTripBuffIpcPriv( IcTripBuffIpc *, const QString &name );
    virtual ~IcTripBuffIpcPriv( );
    inline int size( ) { return m_size; }
    bool   isDirty( );
    bool   write ( const char *data, int data_size );
    const void* read( bool );
};

// ==========================================================
//   CTOR
// ==========================================================
    IcTripBuffIpcPriv :: IcTripBuffIpcPriv ( IcTripBuffIpc *pa, int size, const QString &name )
{
    m_parent = pa;  m_name = name;
    m_sm_ctl = Q_NULLPTR; m_sm_data[0] = m_sm_data[1] = m_sm_data[2] = Q_NULLPTR;
    //m_sys_sem = Q_NULLPTR;

    size = ( size >= 0 ? size : 1 );
    size = (( size + 63 )) / 64 * 64;

    m_sm_ctl     = qxpack_ic_new( QSharedMemory, QString("qxpack.indcom.IcTripBuffIpc.%1.ctl"  ).arg( name ));
    m_sm_data[0] = qxpack_ic_new( QSharedMemory, QString("qxpack.indcom.IcTripBuffIpc.%1.data0").arg( name ));
    m_sm_data[1] = qxpack_ic_new( QSharedMemory, QString("qxpack.indcom.IcTripBuffIpc.%1.data1").arg( name ));
    m_sm_data[2] = qxpack_ic_new( QSharedMemory, QString("qxpack.indcom.IcTripBuffIpc.%1.data2").arg( name ));

    m_sm_ctl->create( sizeof( IcTripBuffIpcPrivCtlArea ), QSharedMemory::ReadWrite );
    m_sm_data[0]->create( size, QSharedMemory::ReadWrite );
    m_sm_data[1]->create( size, QSharedMemory::ReadWrite );
    m_sm_data[2]->create( size, QSharedMemory::ReadWrite );

    m_last_write_idx = m_last_read_idx = -1;
    if ( m_sm_ctl->lock( ) ) { // if not attached, return false
        IcTripBuffIpcPrivCtlArea *ctl = ( IcTripBuffIpcPrivCtlArea *) m_sm_ctl->data( );
        if ( ctl != Q_NULLPTR ) {
            new( ctl ) IcTripBuffIpcPrivCtlArea( size );
            m_last_write_idx = ( int ) ctl->writeIndex( );
            m_size           = size;
        }
        m_sm_ctl->unlock( );
    }

    //m_sys_sem = qxpack_ic_new( QSystemSemaphore, QString("qxpack.indcom.IcTripBuffIpc.%1.sem").arg( name ), 0, QSystemSemaphore::Create );
}

// ==========================================================
//   CTOR ( attach )
// ==========================================================
    IcTripBuffIpcPriv :: IcTripBuffIpcPriv ( IcTripBuffIpc *pa, const QString &name )
{
    m_parent = pa;  m_name = name;
    m_sm_ctl     = qxpack_ic_new( QSharedMemory, QString("qxpack.IcTripBuffIpc.%1.ctl"  ).arg( name ));
    m_sm_data[0] = qxpack_ic_new( QSharedMemory, QString("qxpack.IcTripBuffIpc.%1.data0").arg( name ));
    m_sm_data[1] = qxpack_ic_new( QSharedMemory, QString("qxpack.IcTripBuffIpc.%1.data1").arg( name ));
    m_sm_data[2] = qxpack_ic_new( QSharedMemory, QString("qxpack.IcTripBuffIpc.%1.data2").arg( name ));

    m_sm_ctl->attach( QSharedMemory::ReadWrite );
    m_sm_data[0]->attach( QSharedMemory::ReadWrite );
    m_sm_data[1]->attach( QSharedMemory::ReadWrite );
    m_sm_data[2]->attach( QSharedMemory::ReadWrite );

    // NOTE: here always initalize last read or write index as -1
    m_last_write_idx = m_last_read_idx = -1;
    if ( m_sm_ctl->lock( ) ) {
        IcTripBuffIpcPrivCtlArea *ctl = ( IcTripBuffIpcPrivCtlArea *) m_sm_ctl->data( );
        if ( ctl != Q_NULLPTR ) {
            m_last_write_idx = ( int ) ctl->writeIndex( );
            m_size           = ( int ) ctl->size( );
        }
        m_sm_ctl->unlock( );
    }

    //m_sys_sem = qxpack_ic_new( QSystemSemaphore, QString("qxpack.indcom.IcTripBuffIpc.%1.sem").arg( name ), 0, QSystemSemaphore::Open );
}

// ==========================================================
//   DTOR
// ==========================================================
    IcTripBuffIpcPriv :: ~IcTripBuffIpcPriv (  )
{
    //if ( m_sys_sem != Q_NULLPTR ) { qxpack_ic_delete( m_sys_sem, QSystemSemaphore ); }
    if ( m_sm_ctl     != Q_NULLPTR ) { m_sm_ctl->detach( );     qxpack_ic_delete( m_sm_ctl,     QSharedMemory ); }
    if ( m_sm_data[0] != Q_NULLPTR ) { m_sm_data[0]->unlock(); m_sm_data[0]->detach( ); qxpack_ic_delete( m_sm_data[0], QSharedMemory ); }
    if ( m_sm_data[1] != Q_NULLPTR ) { m_sm_data[1]->unlock(); m_sm_data[1]->detach( ); qxpack_ic_delete( m_sm_data[1], QSharedMemory ); }
    if ( m_sm_data[2] != Q_NULLPTR ) { m_sm_data[2]->unlock(); m_sm_data[2]->detach( ); qxpack_ic_delete( m_sm_data[2], QSharedMemory ); }
}

// ==========================================================
//  check if the buffer is dirty.
// ==========================================================
bool   IcTripBuffIpcPriv :: isDirty (  )
{
    if ( m_sm_ctl->lock( )) {
        bool ret = false;
        IcTripBuffIpcPrivCtlArea *ctl = ( IcTripBuffIpcPrivCtlArea *) m_sm_ctl->data( );
        if ( ctl != Q_NULLPTR ) { ret = ( ctl->isDirty( ) != 0 ? true : false ); }
        m_sm_ctl->unlock( );
        return ret;
    } else { 
        return false;
    }
}

// ==========================================================
//  write the data into tripbuffer
// if success, emit the signal to tell user
// ==========================================================
bool    IcTripBuffIpcPriv :: write ( const char *data, int data_size )
{
    // ---------------------------------------------------------
    // check parameter and initalized var.
    // ---------------------------------------------------------
    if ( data_size < 1 ) { return false; }
    if ( m_last_write_idx < 0 ) { return false; } // NOTE: init. err

    // ---------------------------------------------------------
    //  write the data into write buffer
    // ---------------------------------------------------------
    bool ret = true;
    QSharedMemory *sm = m_sm_data[ m_last_write_idx ];       
    if ( ( ret = sm->lock( ))) {
        if ( sm->data( ) != Q_NULLPTR ) {
            int sz = sm->size( );
            std::memcpy( sm->data(), data, ( data_size <= sz ? data_size : sz ));
        }
        sm->unlock( );
    }

    // ------------------------------------------------------------
    // swap the data into swap slot
    // ------------------------------------------------------------
    bool is_old_picked = false;
    if ( ret && m_sm_ctl->lock( )) {
        IcTripBuffIpcPrivCtlArea *ctl = ( IcTripBuffIpcPrivCtlArea *) m_sm_ctl->data( );
        if ( ctl != Q_NULLPTR ) { m_last_write_idx = ctl->doWriteSwap( is_old_picked ); }
        else { ret = false; }
        m_sm_ctl->unlock( );
    } else {
        ret = false;
    }

    // ---------------------------------------------------
    // if swapped success, emit the signal to user
    // ---------------------------------------------------
    if ( ret ) {
        emit m_parent->dataWrote( is_old_picked );
    }

    return ret;
}    


// ===========================================================
//  read the data from tripbuffer
// NOTE: if no new data and is_ignore_dirty is false, return Q_NULLPTR
//       otherwise, always return data point ( maybe new data or last read data )
// ===========================================================
const void *  IcTripBuffIpcPriv :: read ( bool is_ignore_dirty )
{
    bool is_got_data = false;
    if ( m_sm_ctl->lock()) {
        IcTripBuffIpcPrivCtlArea *ctl = ( IcTripBuffIpcPrivCtlArea *) m_sm_ctl->data( );
        if ( ctl == Q_NULLPTR ) { m_sm_ctl->unlock(); return Q_NULLPTR; }

        if ( ctl->isDirty() ) {
            // unlock before we swap to tempory buffer.
            if ( m_last_read_idx != -1 ) { m_sm_data[ m_last_read_idx ]->unlock(); }
            m_last_read_idx = ctl->doReadSwap();
            if ( m_last_read_idx != -1 ) { m_sm_data[ m_last_read_idx ]->lock(); }
            is_got_data = ( m_last_read_idx != -1 );
        } else {
            // if ignore dirty flag, means use last readable data.
            if ( is_ignore_dirty && m_last_read_idx != -1 ) { is_got_data = true; }
        }
        m_sm_ctl->unlock();
    }
    return ( const void *)( is_got_data ? m_sm_data[ m_last_read_idx ]->constData() : Q_NULLPTR );
}


// /////////////////////////////////////////////////////////////////////
//
//  IcTripBuffIpc
//
// /////////////////////////////////////////////////////////////////////
// =====================================================================
//  CTOR ( create )
// =====================================================================
    IcTripBuffIpc :: IcTripBuffIpc ( int size, const QString &name, QObject *pa ) : QObject( pa )
{
    m_obj = qxpack_ic_new( IcTripBuffIpcPriv, this, size, name );
}

// =====================================================================
//  CTOR ( attach )
// =====================================================================
    IcTripBuffIpc :: IcTripBuffIpc ( const QString &name, QObject *pa ) : QObject( pa )
{
    m_obj = qxpack_ic_new( IcTripBuffIpcPriv, this, name );
}

// =====================================================================
//  DTOR
// =====================================================================
    IcTripBuffIpc :: ~IcTripBuffIpc( )
{
    if ( m_obj != Q_NULLPTR ) {
        qxpack_ic_delete( m_obj, IcTripBuffIpcPriv );
        m_obj = Q_NULLPTR;
    }
}

// =====================================================================
// method access
// =====================================================================
bool   IcTripBuffIpc :: isDirty( ) const { return T_PrivPtr( m_obj )->isDirty( ); }
int    IcTripBuffIpc :: size ( )   const { return T_PrivPtr( m_obj )->size   ( );    }
bool   IcTripBuffIpc :: write( const char *data, int size ) { return T_PrivPtr( m_obj )->write ( data, size ); }
const void* IcTripBuffIpc :: read ( bool is_ignore_dirty )  { return T_PrivPtr( m_obj )->read( is_ignore_dirty ); }




}

#endif
