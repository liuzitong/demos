#include <QCoreApplication>
#include <QObject>
#include <QDebug>
#include "qxpack/indcom/net/qxpack_ic_jsonrpc2_cli.hxx"
#include "qxpack/indcom/sys/qxpack_ic_jsonrpc2.hxx"
#include "qxpack/indcom/net/qxpack_ic_jsonrpc2_resphde.hxx"
#include "qxpack/indcom/common/qxpack_ic_memcntr.hxx"
#include "qxpack/indcom/sys/qxpack_ic_appdcl_priv.hxx"
#include <QTime>
#include <QList>
#include <QProcess>
#include <QThread>
#include <QByteArray>
#include <QString>

#define SVR_EXE  "jsonrpc2-svr.exe"
#define SVR_NAME "jsonrpc2-svr"

#ifdef TEST_JSONRPC2_TCP
#define RS_TYPE  "rsTcp"
#define CONN_HOST "127.0.0.1"
#define CONN_PORT 9999
#else
#define RS_TYPE "rsLoc"
#define CONN_HOST SVR_NAME
#define CONN_PORT 9999
#endif


// ////////////////////////////////////////////////////////////////////////////
// static functions
// ////////////////////////////////////////////////////////////////////////////
// ============================================================================
// make quit method call
// ============================================================================
static QVector<QxPack::IcJsonRpc2Request>  gMakeQuitCall( )
{
    QxPack::IcJsonRpc2Request req;
    req.setId(1000);
    req.setMethod("rpc.quitSvr");
    return QVector<QxPack::IcJsonRpc2Request>() << req;
}

// ============================================================================
// make subtract method call
// ============================================================================
static QVector<QxPack::IcJsonRpc2Request>  gMakeSubtractCall( )
{
    QxPack::IcJsonRpc2Request req;
    req.setId(1000);
    req.setMethod("subtract");
    QJsonArray pa_ja;
    pa_ja.append( qrand() % 100 ); pa_ja.append( qrand() % 100 );
    req.setParams( pa_ja );
    return QVector<QxPack::IcJsonRpc2Request>() << req;
}

// ============================================================================
// enum all process that try to find named process
// ============================================================================
static bool  gFindProcessByName ( const char *name )
{
    bool is_found = false;
#ifdef WIN32
    QProcess proc;
    proc.start("tasklist", QProcess::ReadOnly);
    bool ret = proc.waitForStarted();
    if ( ret ) { ret = proc.waitForFinished(); }
    if ( ret ) {
        QByteArray ba = proc.readAll();
        if ( ! ba.isEmpty()) {
            is_found = ( ba.indexOf(name) != -1 );
        }
    } else {
        qFatal("could not start the tasklist.");
    }
#else
    #error this platform is not supported.
#endif
    return is_found;
}

// ============================================================================
// start the process
// ============================================================================
static bool gStartProcess( )
{
#ifdef WIN32
    QProcess proc;
    proc.startDetached( SVR_EXE );

    bool is_proc_existed = false;
    for ( int i = 0; i < 5; i ++ ) {
        QThread::msleep(1000);
        if ( gFindProcessByName( SVR_NAME )) {
            is_proc_existed = true; break;
        }
    }

    QThread::msleep(500);
    return is_proc_existed;
#else
    #error this platform is not supported.
#endif
}

// ////////////////////////////////////////////////////////////////////////////
// request generator
// ////////////////////////////////////////////////////////////////////////////
class RequestGen {
private: QList<QVector<QxPack::IcJsonRpc2Request>> m_req_list;
public :
    explicit RequestGen ( )
    {
        int expect_loop = qrand()% 20 + 10;
        if ( expect_loop < 1 ) { expect_loop = 1; }
        for ( int x = 0; x < expect_loop; x ++ ) {
            m_req_list.append( gMakeSubtractCall( ));
        }
    }
    inline QList<QVector<QxPack::IcJsonRpc2Request>> requestList( ) { return m_req_list; }
};


// ////////////////////////////////////////////////////////////////////////////
// rpc client
// ////////////////////////////////////////////////////////////////////////////
class RpcClient : public QObject {
    Q_OBJECT
private:
    QList<QVector<QxPack::IcJsonRpc2Request>> m_data_list;
    int m_total_req;
    QxPack::IcJsonRpc2Cli      m_cli;
    QxPack::IcJsonRpc2RespHde  m_resp_hde;
protected:
    Q_SLOT void cli_onHostConnected( ) { this->postRequest(); }
    Q_SLOT void cli_onNewNotice    ( );
    Q_SLOT void cli_onNewResponse  ( );
    Q_SLOT void cli_onFinished     ( ) { qInfo("connection finished"); QCoreApplication::quit(); }

    // subtract method response call handler
    Q_SLOT void resp_onUnhandledResp( const QxPack::IcJsonRpc2Response& );

    // response handler
    Q_INVOKABLE void onResp_subtract(
      const QxPack::IcJsonRpc2Request&, const QxPack::IcJsonRpc2Response&, const QVariant&
    );

           void postRequest ( );
           void tryFinishSvr( );
public :
    explicit RpcClient( );
    virtual ~RpcClient( ) Q_DECL_OVERRIDE;
};

// ============================================================================
// ctor
// ============================================================================
RpcClient :: RpcClient ( ) : m_cli( RS_TYPE )
{
    m_resp_hde.build( this );

    m_data_list = RequestGen().requestList();

    m_total_req = 0;
    QList<QVector<QxPack::IcJsonRpc2Request>>::const_iterator c_itr = m_data_list.constBegin();
    while ( c_itr != m_data_list.constEnd()) {
        const QVector<QxPack::IcJsonRpc2Request> &v_req = (* c_itr ++ );
        m_total_req += v_req.size();
    }

    QObject::connect ( &m_cli, SIGNAL(hostConnected()), this, SLOT(cli_onHostConnected()));
    QObject::connect ( &m_cli, SIGNAL(newNotice()),     this, SLOT(cli_onNewNotice()));
    QObject::connect ( &m_cli, SIGNAL(newResponse()),   this, SLOT(cli_onNewResponse()));
    QObject::connect ( &m_cli, SIGNAL(finished()),      this, SLOT(cli_onFinished()));

    m_cli.connectToHost( CONN_HOST, CONN_PORT );
}

// ============================================================================
// dtor
// ============================================================================
RpcClient :: ~RpcClient( )
{
    QObject::disconnect( &m_cli, Q_NULLPTR, this, Q_NULLPTR );
}

// ============================================================================
// post a request
// ============================================================================
void  RpcClient :: postRequest()
{
    if ( ! m_data_list.isEmpty() ) {
        QVector<QxPack::IcJsonRpc2Request> v_req = m_data_list.takeFirst();

        QVector<QxPack::IcJsonRpc2Request>::const_iterator c_itr = v_req.constBegin();
        while ( c_itr != v_req.constEnd()) {
            const QxPack::IcJsonRpc2Request &req = (*c_itr ++ );
            m_resp_hde.addDispoCall( req, QStringLiteral("subtract") );
        }

        m_cli.post( v_req.first() );
    }
}

// ============================================================================
// try finish server if all request has been solved
// ============================================================================
void  RpcClient :: tryFinishSvr()
{
    if ( -- m_total_req <= 0 ) { // all request has been solved.
        QVector<QxPack::IcJsonRpc2Request> v_req = gMakeQuitCall();
        m_cli.post( v_req );
    }
}

// ============================================================================
// slot: handle the new notice from server
// ============================================================================
void  RpcClient :: cli_onNewNotice()
{
    QxPack::IcJsonRpc2Request req;
    if ( m_cli.takeNextPendingNotice( req )) {
        QJsonObject par = req.params();
        qInfo("the notice is '%s'", par.value("message").toString().toUtf8().constData() );
    }
}

// ============================================================================
// slot: handle the response from server
// ============================================================================
void  RpcClient :: cli_onNewResponse()
{
    qInfo("new response package recv.");
    QVector<QxPack::IcJsonRpc2Response> vr;
    if ( m_cli.takeNextPendingResponse(vr)) {
        m_resp_hde.handleResponse( vr );
        this->postRequest();
    }
}

// ============================================================================
// slot: unhandled response from resp hde
// ============================================================================
void  RpcClient :: resp_onUnhandledResp( const QxPack::IcJsonRpc2Response &resp )
{
    if ( resp.isRequest() ) {
        QxPack::IcJsonRpc2Request req = resp.request();
        if ( req.isIdStr()) {
            qInfo("unhandled req: is notice?(%s), id=%s", (req.isNotification()?"yes":"no"), req.idStr().toUtf8().constData());
        } else {
            qInfo("unhandled req: is notice?(%s), id=%d", (req.isNotification()?"yes":"no"), req.id());
        }
    } else if ( resp.isError() ) {
        QxPack::IcJsonRpc2Error err = resp.error();
        if ( err.isIdStr()) {
            qInfo("unhandled error: id=%s, msg=%s", err.idStr(), err.errorMessag().toUtf8().constData());
        } else {
            qInfo("unhandled error: id=%d, msg=%s", err.id(), err.errorMessag().toUtf8().constData());
        }
    } else if ( resp.isResult() ) {
        QxPack::IcJsonRpc2Result rsl = resp.result();
        if ( rsl.isIdStr()) {
            qInfo("unhandled result: id=%s", rsl.idStr().toUtf8().constData());
        } else {
            qInfo("unhandled result: id=%d", rsl.id());
        }
    } else {
        qInfo("unrecognized resp.");
    }
    this->postRequest();

    // call this to count how many response has been received.
    this->tryFinishSvr();
}

// ============================================================================
// call: response call
// ============================================================================
void RpcClient :: onResp_subtract(
  const QxPack::IcJsonRpc2Request &req, const QxPack::IcJsonRpc2Response &resp, const QVariant&
) {
    // get the p0, p1 param
    QJsonArray param_ja = req.paramsJa();
    int p0 = param_ja.at(0).toInt(1); int p1 = param_ja.at(1).toInt(2);

    // get the result
    int rsl = resp.result().resultJv().toInt(3);
    if ( p0 - p1 != rsl ) {
        qInfo("error : %d - %d = %d, but recv. %d", p0, p1, p0 - p1, rsl );
    } else {
        qInfo("result: %d - %d = %d", p0, p1, rsl );
    }

    this->tryFinishSvr();
}




// ////////////////////////////////////////////////////////////////////////////
// main entry
// ////////////////////////////////////////////////////////////////////////////
int   main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    int ret = 0; QTime tm; tm.start();
    qsrand( uint( tm.msecsSinceStartOfDay()) );

    if ( ! gFindProcessByName( SVR_NAME )) {
        if ( ! gStartProcess()) {
            qInfo("cound not start server!");
            exit(-1);
        }
    }

    int prev_mem_cntr = QxPack::IcMemCntr::currNewCntr();
    {
        RpcClient cli;
        ret = app.exec();
    }
    QxPack::IcAppDclPriv::barrier(16);
    int curr_mem_cntr = QxPack::IcMemCntr::currNewCntr();
    if ( curr_mem_cntr != prev_mem_cntr) {
        qInfo("current mem counter is:%d, not equal to before cntr:%d", curr_mem_cntr, prev_mem_cntr);
    } else {
        qInfo("after exec, mem cntr is %d", curr_mem_cntr );
    }


    return ret;
}

#include "cli.moc"
