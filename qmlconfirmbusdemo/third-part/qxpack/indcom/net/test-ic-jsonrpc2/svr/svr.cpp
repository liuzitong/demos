#include <QCoreApplication>
#include <QDebug>
#include <QObject>
#include <QtTest>
#include <QMetaObject>
#include <QVariant>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QProcess>
#include <QByteArray>

#include "qxpack/indcom/common/qxpack_ic_memcntr.hxx"
#include "qxpack/indcom/net/qxpack_ic_jsonrpc2_svr.hxx"
#include "qxpack/indcom/sys/qxpack_ic_appdcl_priv.hxx"

#define CLI_NAME "jsonrpc2-cli"
#define CLI_EXE  "jsonrpc2-cli.exe"

#define SVR_NAME "jsonrpc2-svr"

#ifdef TEST_JSONRPC2_TCP
#define RS_TYPE "rsTcp"
#else
#define RS_TYPE "rsLoc"
#endif


// ////////////////////////////////////////////////////////////////////////////
// local function
// ////////////////////////////////////////////////////////////////////////////
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


// ////////////////////////////////////////////////////////////////////////////
// latercall
// ////////////////////////////////////////////////////////////////////////////
class LaterCall : public QObject {
    Q_OBJECT
public : typedef void (* Callback )( void *ctxt );
private: Callback m_cb; void *m_ctxt;
public:
    LaterCall( Callback cb, void *ctxt )
    {
        m_cb = cb; m_ctxt = ctxt;
        QMetaObject::invokeMethod( this, "doMethod", Qt::QueuedConnection );
    }
    Q_INVOKABLE void doMethod( )
    { if ( m_cb != Q_NULLPTR ) { m_cb( m_ctxt ); }}
};

// ////////////////////////////////////////////////////////////////////////////
//  simple request handler
// ////////////////////////////////////////////////////////////////////////////
class ReqHandler : public QObject {
    Q_OBJECT
public :
    typedef void (* Callback )( ReqHandler *t_this, const QxPack::IcJsonRpc2Request&);
private:
    Callback m_cb; void *m_ctxt;
    QxPack::IcJsonRpc2Handler *m_hde; QxPack::IcJsonRpc2Svr *m_svr;
    QTimer m_notice_tmr;
public :
    // ========================================================================
    // ctor
    // ========================================================================
    ReqHandler ( QxPack::IcJsonRpc2Svr *svr, Callback cb )
    {
        m_cb = cb; m_svr = svr; m_hde = Q_NULLPTR;

        // --------------------------------------------------------------------
        // notice timer used to trigger post message to client
        // --------------------------------------------------------------------
        m_notice_tmr.setInterval(5000);
        m_notice_tmr.setSingleShot(false);
       // m_notice_tmr.start();
        QObject::connect(
            &m_notice_tmr, &QTimer::timeout,
            [this]() {
                if ( this->m_hde != Q_NULLPTR ) {
                    QxPack::IcJsonRpc2Request n_req;
                    QJsonObject par;
                    par.insert("message","show me the money");
                    n_req.setParams( par );
                    n_req.setMethod("noticeTimer");
                    n_req.setIsNotification(true);
                    m_hde->notice( n_req );
                }
            }
        );

        QObject::connect (
            m_svr, &QxPack::IcJsonRpc2Svr::newHandler,
            [this](){
                if ( m_hde != Q_NULLPTR ) { m_hde->deleteLater(); m_hde = Q_NULLPTR; }
                QObject *hde_obj = m_svr->takeNextPendingHandler();
                if ( hde_obj != Q_NULLPTR ) {
                    m_hde = qobject_cast<QxPack::IcJsonRpc2Handler*>(hde_obj);
                    if ( m_hde == Q_NULLPTR ) { hde_obj->deleteLater(); hde_obj = Q_NULLPTR; }
                }
                if ( m_hde != Q_NULLPTR ) {
                    QObject::connect( m_hde, SIGNAL(newRequest()), this, SLOT(onNewRequest()) );
                }
            }

        );
    }

    // ========================================================================
    // dtor
    // ========================================================================
    virtual ~ReqHandler ( ) Q_DECL_OVERRIDE
    {
        m_notice_tmr.stop();
        QObject::disconnect ( m_svr, Q_NULLPTR, this, Q_NULLPTR );
        if ( m_hde != Q_NULLPTR ) { m_hde->deleteLater(); }
    }

    // ========================================================================
    // post the response to client
    // ========================================================================
    void  post( const QxPack::IcJsonRpc2Response &resp )
    {
        m_hde->post( resp );
    }

    // ========================================================================
    // while recv. the request, invoke callback function
    // ========================================================================
    Q_SLOT void  onNewRequest(  )
    {
        QxPack::IcJsonRpc2Request req;
        if ( m_hde != Q_NULLPTR && ! m_hde->takeNextPendingRequest( req )) { return; }
        if ( m_cb != Q_NULLPTR ) { m_cb( this, req ); }
    }
};


// ////////////////////////////////////////////////////////////////////////////
// test object
// ////////////////////////////////////////////////////////////////////////////
class  Test : public QObject {
    Q_OBJECT
private:
Q_SLOT  void initTestCase   ( ) {  }
Q_SLOT  void cleanupTestCase( );
Q_SLOT  void testMethodSubtract( );
public :
    Test ( ) { }
    ~Test( ) { }
};

// ============================================================================
// final cleanup
// ============================================================================
void  Test :: cleanupTestCase()
{
    qInfo() << "current MemCntr:" << QxPack::IcMemCntr::currNewCntr();
}

// ============================================================================
// test accept a single object request.
// --> {"jsonrpc": "2.0", "method": "subtract", "params": [42, 23], "id": 1}
// <-- {"jsonrpc": "2.0", "result": 19, "id": 1}
// ============================================================================
static void  gSubtract_svc ( ReqHandler *req_h, const QxPack::IcJsonRpc2Request &rp )
{
    QxPack::IcJsonRpc2Result rsl;
    QxPack::IcJsonRpc2Error::Code err_code = QxPack::IcJsonRpc2Error::Code_User;

    if ( rp.isNull()) {
        err_code = QxPack::IcJsonRpc2Error::Code_InvalidRequest;
    } else {
        if ( rp.method() == QStringLiteral("subtract")) {
            QJsonArray pa_ja = rp.paramsJa();
            int v1 = pa_ja.at(0).toInt(0);
            int v2 = pa_ja.at(1).toInt(0);
            rsl = QxPack::IcJsonRpc2Result::makeResult ( rp, v1 - v2 );
        } else if ( rp.method() == QStringLiteral("rpc.quitSvr")) {
            qInfo() << "recv. quit svr.";
            QCoreApplication::quit();
        } else {
            err_code = QxPack::IcJsonRpc2Error::Code_MethodNotFound;
        }
    }
    QxPack::IcJsonRpc2Response resp;
    if ( err_code == QxPack::IcJsonRpc2Error::Code_User ) {
        resp = QxPack::IcJsonRpc2Response( rsl );
    } else {
        resp = QxPack::IcJsonRpc2Response(
          QxPack::IcJsonRpc2Error::makeError( rp, err_code )
        );
    }
    req_h->post( resp );
}

// ============================================================================
// test the sub
// ============================================================================
void   Test :: testMethodSubtract( )
{
    int mem_cntr = QxPack::IcMemCntr::currNewCntr();
    {
        // create a server
        QxPack::IcJsonRpc2Svr *svr = new QxPack::IcJsonRpc2Svr( RS_TYPE, SVR_NAME, 9999 );
        svr->startServer();
        if ( ! svr->isListening() ) {
            delete svr; QVERIFY2( false, "the jsonrpc2 server is not working!");
        }

        // create request handler, and use gSubtract_svc() as callback function
        ReqHandler *rh = new ReqHandler ( svr, & gSubtract_svc );

        // detect the cli program
        #if defined(WIN32)
        if ( ! gFindProcessByName( CLI_NAME )) {
           if ( QFile::exists( QCoreApplication::applicationDirPath() + "/" + CLI_EXE )) {
                qInfo("found " CLI_EXE );
                QProcess::startDetached( QCoreApplication::applicationDirPath() + "/" CLI_EXE );
            }
        }
        #endif

        QCoreApplication::instance()->exec(); // enter eventloop

        // delete all objects
        rh->deleteLater();
        svr->deleteLater();
        QxPack::IcAppDclPriv::barrier( 16 );
    }
    QVERIFY2 ( mem_cntr == QxPack::IcMemCntr::currNewCntr(),
        "current memory counter is not equal"
    );
}


QTEST_MAIN( Test )
#include "svr.moc"
