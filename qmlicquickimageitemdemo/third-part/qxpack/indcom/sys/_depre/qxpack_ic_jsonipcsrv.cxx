#ifndef QXPACK_IC_JSONIPCSRV_CXX
#define QXPACK_IC_JSONIPCSRV_CXX

#include "qxpack_ic_jsonipcsrv.hxx"
#include <qxpack/indcom/common/qxpack_ic_memcntr.hxx>
#include "qxpack_ic_jsonrpc2.hxx"

#define qxpack_priv_new( t, ... )     ( qxpack_ic_incr_new_cntr(1), new t ( __VA_ARGS__ ))
#define qxpack_priv_delete( p, t   )  do{ delete ( ( t *)( p ) ); qxpack_ic_decr_new_cntr(t); }while(0)
#define qxpack_priv_decr_cntr( t )    qxpack_ic_decr_new_cntr(t)

#include <QObject>
#include <QLocalServer>
#include <QLocalSocket>
#include <QByteArray>
#include <QJsonDocument>
#include <QMutex>

#define T_PrivHan( o ) (( IcJsonIpcHandlerPriv *) o )
#define T_PrivSrv( o ) (( IcJsonIpcSrvPriv *) o )

namespace QxPack {

// ////////////////////////////////////////////////
//
//   Json IPC handler private object, handle the
// new connection.
//
// ////////////////////////////////////////////////
class QXPACK_IC_HIDDEN IcJsonIpcHandlerPriv : public QObject {
    Q_OBJECT
private:
    IcJsonIpcHandler *m_parent;
    QLocalSocket     *m_sck;

protected:
    Q_SLOT void  deleteSocket( )
    {
        if ( m_sck != Q_NULLPTR ) {
            QObject::disconnect( m_sck, 0, this, 0 );
            m_sck->deleteLater();
            m_sck = Q_NULLPTR;
        }
    }

    Q_SLOT  void  socket_onDestroyed( QObject *obj )
    { if ( obj == m_sck ) { m_sck = Q_NULLPTR; emit m_parent->ipcClosed(); } } // already to destoried..

    Q_SLOT  void  socket_onReadyRead( );

    Q_SLOT  void  socket_onDisconnected( )
    {
        this->deleteSocket();
        emit m_parent->ipcClosed();
    }

public :
    IcJsonIpcHandlerPriv ( IcJsonIpcHandler *pa, QLocalSocket *sck );
    virtual ~IcJsonIpcHandlerPriv ( );
    bool  postResult( const IcJsonRpc2Result & );
    bool  postError ( const IcJsonRpc2Error  & );
};

// ===========================================
// CTOR
// ===========================================
    IcJsonIpcHandlerPriv :: IcJsonIpcHandlerPriv ( IcJsonIpcHandler *pa, QLocalSocket *sck )
{
    m_parent = pa;  m_sck = sck;
    if ( m_sck != Q_NULLPTR ) {
        QObject::connect( m_sck, SIGNAL(destroyed(QObject*)), this, SLOT(socket_onDestroyed(QObject*)));
        QObject::connect( m_sck, SIGNAL(readyRead()),         this, SLOT(socket_onReadyRead()),    Qt::QueuedConnection );
        QObject::connect( m_sck, SIGNAL(disconnected()),      this, SLOT(socket_onDisconnected()), Qt::QueuedConnection );
    }
}

// ===========================================
// DTOR
// ===========================================
    IcJsonIpcHandlerPriv :: ~IcJsonIpcHandlerPriv ( )
{
   this->deleteSocket();
}

// ==============================================
//  Handle the socket ready read
// ==============================================
void  IcJsonIpcHandlerPriv :: socket_onReadyRead( )
{
    if ( m_sck == Q_NULLPTR ) { return; }
    QByteArray ba = m_sck->readAll();
    IcJsonRpc2Request req;
    if ( req.loadFrom( ba )) {
        emit m_parent->newRequest( req );
    }
}

// ===============================================
// post the result or error
// ===============================================
bool  IcJsonIpcHandlerPriv :: postResult( const IcJsonRpc2Result &rsl )
{
    if ( m_sck == Q_NULLPTR ) { return false; }
    QJsonObject jo = rsl.make();
    if ( jo.isEmpty()) { return false; }
    return ( m_sck->write( QJsonDocument( jo ).toJson( QJsonDocument::Compact ) ) > 0 ? true : false );
}

bool  IcJsonIpcHandlerPriv :: postError ( const IcJsonRpc2Error  &err )
{
    if ( m_sck == Q_NULLPTR ) { return false; }
    QJsonObject jo = err.make();
    if ( jo.isEmpty()) { return false; }
    return ( m_sck->write( QJsonDocument( jo ).toJson( QJsonDocument::Compact )) > 0 ? true : false );
}



// //////////////////////////////////////////
//
//  Json IPC Handler wrapper
//
// //////////////////////////////////////////
// ==========================================
// CTOR
// ==========================================
    IcJsonIpcHandler :: IcJsonIpcHandler (void *sck , QObject *pa )
                    : QObject( pa )
{
    m_obj = ( sck != Q_NULLPTR ? qxpack_ic_new( IcJsonIpcHandlerPriv, this, ( QLocalSocket *)( sck )) : Q_NULLPTR );
}

// ==========================================
// DTOR
// ==========================================
    IcJsonIpcHandler :: ~IcJsonIpcHandler ( )
{
    if ( m_obj != Q_NULLPTR ) {
        qxpack_ic_delete( m_obj, IcJsonIpcHandlerPriv );
    }
}

// ==========================================
// function wrapper
// ==========================================
bool  IcJsonIpcHandler :: postResult( const IcJsonRpc2Result &rsl )
{  return ( m_obj != Q_NULLPTR ? T_PrivHan( m_obj )->postResult( rsl ) : false ); }

bool  IcJsonIpcHandler :: postError ( const IcJsonRpc2Error &err )
{  return ( m_obj != Q_NULLPTR ? T_PrivHan( m_obj )->postError ( err ) : false ); }





// //////////////////////////////////////////////////////////
//
//   Json IPC server private, listen on the specified name
//
// //////////////////////////////////////////////////////////
class  QXPACK_IC_HIDDEN IcJsonIpcSrvPriv : public QObject {
    Q_OBJECT
private:
    IcJsonIpcSrv   *m_parent;
    QLocalServer  m_server;
    QString       m_ipc_name;
protected:
    Q_SLOT void server_onNewConnection( );
public :
    IcJsonIpcSrvPriv ( IcJsonIpcSrv *pa, const QString &nm, int max_conn = 30 );
    virtual ~IcJsonIpcSrvPriv ( );
    bool    start( ) { return ( ! m_server.isListening() && ! m_ipc_name.isEmpty() ?  m_server.listen( m_ipc_name ) : false ); }
    void    stop ( ) { if ( m_server.isListening()) { m_server.close(); } }
};

// =============================================
// CTOR
// =============================================
    IcJsonIpcSrvPriv :: IcJsonIpcSrvPriv ( IcJsonIpcSrv *pa, const QString &nm, int max_conn )
{
    m_parent = pa;
    m_server.setMaxPendingConnections( max_conn );
    QObject::connect( & m_server, SIGNAL(newConnection()), this, SLOT(server_onNewConnection()),
                      Qt::QueuedConnection
                    );
    m_ipc_name = nm;
}

// ==============================================
// DTOR
// ==============================================
    IcJsonIpcSrvPriv :: ~IcJsonIpcSrvPriv ( )
{
   QObject::disconnect( &m_server, 0, this, 0 );
   if ( m_server.isListening()) { m_server.close(); }
}

// ===============================================
// handle the new connection
// ===============================================
void  IcJsonIpcSrvPriv :: server_onNewConnection( )
{
    QLocalSocket *sck = m_server.nextPendingConnection();

    // make hde object as IcJsonIpcSrvPriv child
    IcJsonIpcHandler *hde = new IcJsonIpcHandler( sck, this );
    emit m_parent->newConnection( hde );
}



// //////////////////////////////////////////////////
//
//   Json IPC server
//
// //////////////////////////////////////////////////
// ==================================================
// CTOR
// ==================================================
    IcJsonIpcSrv :: IcJsonIpcSrv ( const QString &ipc_name, int max_conn )
{
    m_obj = qxpack_ic_new( IcJsonIpcSrvPriv, this, ipc_name, max_conn );
}

// ==================================================
// DTOR
// ==================================================
    IcJsonIpcSrv :: ~IcJsonIpcSrv ( )
{
    if ( m_obj != Q_NULLPTR ) {
        qxpack_ic_delete( m_obj, IcJsonIpcSrvPriv );
    }
}

// ==================================================
// method wrapper
// ==================================================
bool   IcJsonIpcSrv :: start( ) { return T_PrivSrv( m_obj )->start(); }
void   IcJsonIpcSrv :: stop ( ) { T_PrivSrv( m_obj )->stop();  }

}
#include "qxpack_ic_jsonipcsrv.moc"
#endif
