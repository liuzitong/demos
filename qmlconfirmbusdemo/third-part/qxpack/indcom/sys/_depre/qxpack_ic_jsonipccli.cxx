#ifndef QXPACK_IC_JSONIPCCLI_CXX
#define QXPACK_IC_JSONIPCCLI_CXX

#include "qxpack_ic_jsonipccli.hxx"
#include <qxpack/indcom/common/qxpack_ic_memcntr.hxx>
#include "qxpack_ic_jsonrpc2.hxx"
#include <QObject>
#include <QLocalSocket>
#include <QByteArray>
#include <QJsonDocument>

#define qxpack_priv_new( t, ... )     ( qxpack_ic_incr_new_cntr(1), new t ( __VA_ARGS__ ))
#define qxpack_priv_delete( p, t   )  do{ delete ( ( t *)( p ) ); qxpack_ic_decr_new_cntr(t); }while(0)
#define qxpack_priv_decr_cntr( t )    qxpack_ic_decr_new_cntr(t)

#define T_PrivCli( o )  (( IcJsonIpcCliPriv *) o )

namespace QxPack {

// /////////////////////////////////////////////
//
//  Json IPC client inner class
//
// /////////////////////////////////////////////
class QXPACK_IC_HIDDEN IcJsonIpcCliPriv : public QObject {
    Q_OBJECT
private:
    IcJsonIpcCli *m_parent;
    QString       m_ipc_name;
    QLocalSocket  m_sck;
protected:
    Q_SLOT void  sock_onReadyRead( );
    Q_SLOT void  sock_onDisconnected( );
public :
    IcJsonIpcCliPriv ( IcJsonIpcCli *pa, const QString &ipc_name );
    virtual ~IcJsonIpcCliPriv( );
    bool     isWorked( ) { return ( m_sck.state() == QLocalSocket::ConnectedState ); }
    bool     waitForConnected( int msec ) { return m_sck.waitForConnected( msec ); }
    bool     postRequest ( const IcJsonRpc2Request & );
};

// =============================================
// CTOR
// =============================================
    IcJsonIpcCliPriv :: IcJsonIpcCliPriv ( IcJsonIpcCli *pa, const QString &ipc_name )
{
    m_parent = pa;
    m_ipc_name = ipc_name;
    QObject::connect( & m_sck, SIGNAL(readyRead()),    this, SLOT(sock_onReadyRead()));
    QObject::connect( & m_sck, SIGNAL(disconnected()), this, SLOT(sock_onDisconnected()));
    m_sck.connectToServer( ipc_name );
}

// =============================================
// DTOR
// =============================================
    IcJsonIpcCliPriv :: ~IcJsonIpcCliPriv ( )
{
    QObject::disconnect( & m_sck, 0, this, 0 );
    m_sck.close();
}

// ==============================================
// read to read data
// ==============================================
void  IcJsonIpcCliPriv :: sock_onReadyRead( )
{
    QByteArray ba = m_sck.readAll();
    IcJsonRpc2Result rsl;
    if ( rsl.loadFrom( ba )) {
        emit m_parent->newResult( rsl );
        return;
    }
    IcJsonRpc2Error  err;
    if ( err.loadFrom( ba )) {
        emit m_parent->newError( err );
        return;
    }
}

// ==============================================
// disconnect from server
// ===============================================
void  IcJsonIpcCliPriv :: sock_onDisconnected( )
{
    emit m_parent->ipcClosed();
}

// ===============================================
// post the request to server
// ===============================================
bool   IcJsonIpcCliPriv :: postRequest ( const IcJsonRpc2Request &req )
{
    if ( ! this->isWorked() ) { return false; }
    QJsonObject jo = req.make();
    return ( m_sck.write( QJsonDocument( jo ).toJson( QJsonDocument::Compact )) > 0 ? true : false );
}


// /////////////////////////////////////////////
//
//  Json IPC client wrapper
//
// /////////////////////////////////////////////
// ================================================
// CTOR
// ================================================
    IcJsonIpcCli :: IcJsonIpcCli ( const QString &name )
{
    m_obj = qxpack_ic_new( IcJsonIpcCliPriv, this, name );
}

// ================================================
// DTOR
// ================================================
    IcJsonIpcCli :: ~IcJsonIpcCli( )
{
    if ( m_obj != Q_NULLPTR ) {
        qxpack_ic_delete( m_obj, IcJsonIpcCliPriv );
    }
}

// =================================================
// check if this object is worked.
// =================================================
bool  IcJsonIpcCli :: isWorked( ) const { return T_PrivCli( m_obj )->isWorked(); }
bool  IcJsonIpcCli :: postRequest ( const IcJsonRpc2Request &req )
{ return T_PrivCli( m_obj )->postRequest( req ); }
bool  IcJsonIpcCli :: waitForConnected( int msec ) { return T_PrivCli( m_obj )->waitForConnected( msec ); }


}
#include "qxpack_ic_jsonipccli.moc"
#endif
