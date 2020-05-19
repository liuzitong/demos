#ifndef JSONRPCHTTP_CXX
#define JSONRPCHTTP_CXX

#include "qxpack_ic_jsonrpc2.hxx"
#include "qxpack_ic_jsonrpchttp.hxx"
#include <qxpack/indcom/common/qxpack_ic_memcntr.hxx>

#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
#include <QMutex>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMetaObject>
#include <QString>


#define qxpack_priv_new( t, ... )     ( qxpack_ic_incr_new_cntr(1), new t ( __VA_ARGS__ ))
#define qxpack_priv_delete( p, t   )  do{ delete ( ( t *)( p ) ); qxpack_ic_decr_new_cntr(t); }while(0)
#define qxpack_priv_decr_cntr( t )    qxpack_ic_decr_new_cntr(t)

#define T_PrivPtr( o )  (( IcJsonRpcHttpPriv *) o )

namespace QxPack {

// ////////////////////////////////////////////////////
//
// global network access manager
//
// ////////////////////////////////////////////////////
static QNetworkAccessManager *g_acc_mgr = Q_NULLPTR;
static QMutex  g_locker;
static int g_ref_cntr = 0;

static QNetworkAccessManager &  nam_getInstance( QObject *pa )
{
    g_locker.lock();
    if ( g_acc_mgr == Q_NULLPTR ) {
        g_acc_mgr  = qxpack_priv_new( QNetworkAccessManager, pa );
        g_ref_cntr = 1;
    } else {
        ++ g_ref_cntr;
    }
    g_locker.unlock();
    return *g_acc_mgr;
}

static void  nam_freeInstance( )
{
    g_locker.lock();
    if ( g_acc_mgr != Q_NULLPTR ) {
        if ( -- g_ref_cntr == 0 ) {
            g_acc_mgr->deleteLater( );
            qxpack_priv_decr_cntr( 1 );
            g_acc_mgr = Q_NULLPTR;
        }
    }
    g_locker.unlock();
}

// ///////////////////////////////////////////////////
//
// JsonRptHttpPriv
//
// ///////////////////////////////////////////////////
class QXPACK_IC_HIDDEN IcJsonRpcHttpPriv : public QObject {
    Q_OBJECT
private:
    IcJsonRpcHttp           *m_parent;
    QNetworkAccessManager *m_acc_mgr;
    QNetworkReply         *m_reply;
    QTimer  *m_tmr;
    QString  m_url;
    bool    m_is_uploaded_ocurred;
    bool    m_is_download_ocurred;

protected:
    QNetworkRequest buildRequest ( const QString &url, const QByteArray &payload );
    void            startMonTimer( );
    void            postToHost( const QNetworkRequest &req, const QByteArray &payload );

    Q_SLOT   void onReplyFinished( );
    Q_SLOT   void onReplyError   ( QNetworkReply::NetworkError );
    Q_SLOT   void onCheckNetwork ( );
public :
    IcJsonRpcHttpPriv ( const QString &url, IcJsonRpcHttp *pa );
    virtual ~IcJsonRpcHttpPriv( );

    Q_SLOT void post( const IcJsonRpc2Request & , bool *rsl = Q_NULLPTR );
};

// ====================================================
//  CTOR
// ====================================================
    IcJsonRpcHttpPriv :: IcJsonRpcHttpPriv ( const QString &url, IcJsonRpcHttp *pa ) : QObject( pa )
{
    m_parent  = pa;
    m_tmr     = qxpack_priv_new( QTimer, pa );
    m_acc_mgr = & nam_getInstance( pa );
    m_url     = url;
    m_reply   = Q_NULLPTR;

    m_is_download_ocurred = m_is_uploaded_ocurred = false;
    QObject::connect( m_tmr, SIGNAL(timeout()), this, SLOT(onCheckNetwork()));   
}

// =====================================================
//  DTOR
// =====================================================
    IcJsonRpcHttpPriv :: ~IcJsonRpcHttpPriv ( )
{
    if ( m_tmr != Q_NULLPTR     ) { m_tmr->stop();    qxpack_priv_delete( m_tmr, QTimer ); }
    if ( m_reply != Q_NULLPTR   ) { m_reply->abort(); qxpack_priv_delete( m_reply, QNetworkReply ); }
    nam_freeInstance();
}

// =====================================================
//  SLOT: check the network is reachable or not
// =====================================================
void  IcJsonRpcHttpPriv :: onCheckNetwork( )
{
    if ( m_reply == Q_NULLPTR )  { return; }
    if ( ! m_is_uploaded_ocurred ) {
        m_reply->abort(); // cause the operation canceled.
    }
}

// =====================================================
//  SLOT: handle the reply finished
// =====================================================
void   IcJsonRpcHttpPriv :: onReplyFinished()
{
    if ( m_reply->error() != QNetworkReply::NoError) {
        emit m_parent->operationError( m_reply->errorString());
    } else {
        QByteArray ba = m_reply->readAll();
        IcJsonRpc2Result rsl;
        if ( rsl.loadFrom( ba )) {
            emit m_parent->operationFinishedOK( rsl );
            return;
        }
        IcJsonRpc2Error err;
        if ( err.loadFrom( ba )) {
            emit m_parent->operationFinishedErr( err );
            return;
        }
        emit m_parent->operationError( QStringLiteral("unregconized returned data"));
    }
}

// =====================================================
//  SLOT: handle the reply error
// =====================================================
void   IcJsonRpcHttpPriv :: onReplyError( QNetworkReply::NetworkError )
{

}

// ====================================================
// build the request [ protected ]
// ====================================================
QNetworkRequest IcJsonRpcHttpPriv :: buildRequest( const QString &url, const QByteArray &payload )
{
    QNetworkRequest req;
    req.setUrl( QUrl(url));
    req.setHeader( QNetworkRequest::ContentTypeHeader,   "application/json; charset=utf-8");
    req.setHeader( QNetworkRequest::ContentLengthHeader, QByteArray::number( payload.size()) );
    req.setRawHeader( QByteArray("Connection"), QByteArray("Keep-Alive"));
    return req;
}

// =====================================================
// start the monitor timer [ protected ]
// =====================================================
void     IcJsonRpcHttpPriv :: startMonTimer( )
{
    m_tmr->stop();
    m_tmr->setSingleShot( true );
    m_tmr->start( 5000 );
}

// =====================================================
// post to the host [ protected ]
// =====================================================
void  IcJsonRpcHttpPriv :: postToHost( const QNetworkRequest &req, const QByteArray &payload )
{
    if ( m_reply != Q_NULLPTR ) { delete m_reply;  m_reply = Q_NULLPTR; }
    m_reply = m_acc_mgr->post( req, payload );

    QObject::connect( m_reply, SIGNAL(error( QNetworkReply::NetworkError )), this, SLOT(onReplyError(QNetworkReply::NetworkError)) );
    QObject::connect( m_reply, SIGNAL(finished()), this, SLOT( onReplyFinished()));
    QObject::connect( m_reply, & QNetworkReply::uploadProgress,
                      [this]( qint64 cb, qint64 tb ) {
                         // QDebug() << "u" << ( int )( cb ) << ( int )( tb );
                          this->m_is_uploaded_ocurred = true; emit this->m_parent->uploadProgress( cb, tb );
                      }
                    );
    QObject::connect( m_reply, & QNetworkReply::downloadProgress,
                      [this]( qint64 cb, qint64 tb ) {
                         // QDebug() << "d" << cb << tb;
                          this->m_is_download_ocurred = true; emit this->m_parent->downloadProgress( cb, tb );
                      }
                    );
}

// =====================================================
// post the information
// =====================================================
void  IcJsonRpcHttpPriv :: post ( const IcJsonRpc2Request &j_req, bool *rsl )
{
    QByteArray payload  = QJsonDocument( j_req.make() ).toJson( QJsonDocument::Compact );
    QNetworkRequest req = this->buildRequest( m_url, payload );
    this->postToHost   ( req, payload );
    this->startMonTimer( );

    if ( rsl != Q_NULLPTR ) { *rsl = true; }
    return;
}


// ///////////////////////////////////////////////////
//
//  IcJsonRpcHttp
//
// ///////////////////////////////////////////////////
// ===================================================
// CTOR
// ===================================================
    IcJsonRpcHttp :: IcJsonRpcHttp ( const QString &url, QObject *pa ) : QObject( pa )
{
    m_obj = qxpack_priv_new( IcJsonRpcHttpPriv, url, this );
}

// ===================================================
// DTOR
// ===================================================
    IcJsonRpcHttp :: ~IcJsonRpcHttp ( )
{
    if ( m_obj != Q_NULLPTR ) {
        qxpack_priv_delete( m_obj, IcJsonRpcHttpPriv );
    }
}

// ===================================================
// post to host
// ===================================================
bool  IcJsonRpcHttp :: post (  const IcJsonRpc2Request &req )
{
    if ( QThread::currentThread() == this->thread()) {
        bool ret = false;
        T_PrivPtr( m_obj )->post( req, & ret );
        return ret;
    } else {
        QMetaObject::invokeMethod( T_PrivPtr( m_obj ), "post", Qt::QueuedConnection,
                                   Q_ARG( QxPack::IcJsonRpc2Request, req )
                                 );
        return true;
    }
}

}

#include "qxpack_ic_jsonrpchttp.moc"
#endif
