// ////////////////////////////////////////////////////////////////////////////
/*!
 * @file qxpack_ic_jsonrpc2tcpsvr
 * @author nightwing
 * @date 2019/03, 2019/05, 2019/06
 * @verbatim
 *     <author>    <date>    <history memo.>
 *     nightwing  2019/02     build
 *     nightwing  2019/03     fixed.
 *     nightwing  2019/05     fixed.
 *     nightwing  2019/06     fixed. follow the JsonRpc2 rules
 * @endverbatim
 */
// ////////////////////////////////////////////////////////////////////////////
#ifndef QXPACK_IC_JSONRPC2_SVR_CXX
#define QXPACK_IC_JSONRPC2_SVR_CXX

#include "qxpack_ic_jsonrpc2_svr.hxx"
#include "qxpack/indcom/sys/qxpack_ic_rmtobjcreator_priv.hxx"
#include "qxpack/indcom/sys/qxpack_ic_rmtobjdeletor_priv.hxx"
#include "qxpack/indcom/net/qxpack_ic_jsonrpc2_buff.hxx"
#include "qxpack/indcom/net/qxpack_ic_datatrans_rshdebase.hxx"
#include "qxpack/indcom/net/qxpack_ic_datatrans_rssvrbase.hxx"
#include "qxpack/indcom/common/qxpack_ic_memcntr.hxx"

#include <QThread>
#include <QTcpSocket>
#include <QTcpServer>
#include <QHostAddress>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QMetaObject>
#include <QMutex>
#include <QList>
#include <QPair>
#include <QAtomicInt>

#ifdef __GNUC__ // in GCC 5, close below warnings
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#endif

namespace QxPack {

// ////////////////////////////////////////////////////////////////////////////
//                       local functions
// ////////////////////////////////////////////////////////////////////////////
static bool g_reg_obj = false;
static void gRegObj ( )
{
    if ( ! g_reg_obj ) {
        qRegisterMetaType<QVector<QxPack::IcJsonRpc2Request>>();
        qRegisterMetaType<QVector<QxPack::IcJsonRpc2Response>>();
        qRegisterMetaType<QxPack::IcJsonRpc2Request>();
        qRegisterMetaType<QxPack::IcJsonRpc2Response>();
        g_reg_obj = true;
    }
}

// ////////////////////////////////////////////////////////////////////////////
//
//                      data reader and  writer
//
// ////////////////////////////////////////////////////////////////////////////
class QXPACK_IC_HIDDEN  IcJsonRpc2Svr_DataRw : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY( IcJsonRpc2Svr_DataRw )
protected:
    bool  readArrayData ( QVector<QxPack::IcJsonRpc2Request> &vr, const QJsonArray & );
    bool  writeArrayData( QJsonArray &, const QVector<QxPack::IcJsonRpc2Response>& );

public :
    explicit IcJsonRpc2Svr_DataRw ( QObject *pa = Q_NULLPTR ) : QObject( pa ) { }
    virtual ~IcJsonRpc2Svr_DataRw ( ) { }

    Q_SLOT    void  handleNewPackage ( const QByteArray &ba );
    Q_SLOT    void  handlePostPackage( const QVector<QxPack::IcJsonRpc2Response> &, bool is_array = true );
    Q_SLOT    void  handlePostNotice ( const QxPack::IcJsonRpc2Request & );
    Q_SIGNAL  void  reqPost ( const QByteArray &, bool is_data_pkg );

    // ------------------------------------------------------------------------
    // 1) empty array                         -- client send an empty array
    // 2) array contains some empty request() -- some requests is not a valid request.
    // 3) normal array                        -- all requests are valid.
    // ------------------------------------------------------------------------
    Q_SIGNAL  void  newVectRequest ( const QVector<QxPack::IcJsonRpc2Request> &, bool is_array = true );
};

// ============================================================================
// read array object
// ============================================================================
bool IcJsonRpc2Svr_DataRw :: readArrayData (
    QVector<QxPack::IcJsonRpc2Request> &vr, const QJsonArray &ja
) {
    if ( ja.isEmpty() ) {
        vr = QVector<QxPack::IcJsonRpc2Request>();
        return false;
    }

    // ------------------------------------------------------------------------
    // generated the request vector
    // ------------------------------------------------------------------------
    QVector<QxPack::IcJsonRpc2Request> vect; vect.reserve( ja.size() );
    QJsonArray::const_iterator c_itr = ja.constBegin();
    while ( c_itr != ja.constEnd() ) {
        const QJsonValue &jv = *( c_itr ++);
        if ( jv.isObject() ) { // push valid request
            vect.push_back( QxPack::IcJsonRpc2Request( jv.toObject() ));
        } else {               // push empty request as invalid request.
            vect.push_back( QxPack::IcJsonRpc2Request() );
        }
    }
    vr = vect;

    return true;
}

// ============================================================================
// write array data from response to json
// ============================================================================
bool   IcJsonRpc2Svr_DataRw :: writeArrayData (
    QJsonArray &ja, const QVector<QxPack::IcJsonRpc2Response>& vr
) {
    if ( vr.isEmpty()) { return false; }

    QVector<QxPack::IcJsonRpc2Response>::const_iterator c_itr = vr.constBegin();
    while ( c_itr != vr.constEnd()) {
        const QxPack::IcJsonRpc2Response &resp = *( c_itr ++ );
        if ( ! resp.isNull() ) { ja.append( resp.make() ); }
    }
    return true;
}

// ============================================================================
// slot: handle new package retrieved
// ============================================================================
void    IcJsonRpc2Svr_DataRw :: handleNewPackage( const QByteArray &ba )
{
    QJsonDocument doc = QJsonDocument::fromJson( ba );

    if ( doc.isArray() ) {
        // here emit array regardless empty or not empty
        QVector<QxPack::IcJsonRpc2Request> vr;
        this->readArrayData( vr, doc.array() );
        emit this->newVectRequest( vr, true );

    } else if ( doc.isObject() ) {
        QxPack::IcJsonRpc2Request req;        
        if ( req.loadFrom( doc.object() )) {
            // make one element requests and emit it
            QVector<QxPack::IcJsonRpc2Request> vr; vr.append( req );
            emit this->newVectRequest( vr, false );
        } else {
            // make an empty request as invalid request
            QVector<QxPack::IcJsonRpc2Request> vr; vr.append( IcJsonRpc2Request() );
            emit this->newVectRequest( vr, false );
        }
    } else {
        // not a json format
        QByteArray ba;
        {
            IcJsonRpc2Error err( IcJsonRpc2Error::Code_ParseError, 0 );
            ba = QJsonDocument( err.make()).toJson( QJsonDocument::Compact );
        }
        this->reqPost(  ba, false );
    }
}

// ============================================================================
// slot: handle the posted package from user
// ============================================================================
void  IcJsonRpc2Svr_DataRw :: handlePostPackage( const QVector<QxPack::IcJsonRpc2Response> &vr, bool is_array )
{
    if ( vr.isEmpty() ) { return; }

    if ( is_array ) {
        // ------------------------------------------------------------------------
        // make an array from vr
        // NOTE: user should use null Response as notificaiton dummy response.
        // ------------------------------------------------------------------------
        QJsonArray ja; QVector<IcJsonRpc2Response>::const_iterator c_itr = vr.constBegin();
        while ( c_itr != vr.constEnd() ) {
            const IcJsonRpc2Response &resp = *( c_itr ++ );
            if ( resp.isNull() ) { continue; } // skip dummy response
            ja.append( resp.make() );
        }
        if ( ! ja.isEmpty() ) {
            QByteArray ba = QJsonDocument(ja).toJson(QJsonDocument::Compact);
            ja = QJsonArray();
            emit this->reqPost( ba, true );
        }
    } else {
        // --------------------------------------------------------------------
        // every item in array is single object
        // --------------------------------------------------------------------
        QVector<IcJsonRpc2Response>::const_iterator c_itr = vr.constBegin();
        while ( c_itr != vr.constEnd()) {
            const IcJsonRpc2Response &resp = (* c_itr ++ );
            if ( resp.isNull() ) { continue; }
            QByteArray ba = QJsonDocument( resp.make() ).toJson(QJsonDocument::Compact);
            emit this->reqPost( ba, true );
        }
    }

    return;
}

// ============================================================================
// slot: handle posted notice from user
// ============================================================================
void  IcJsonRpc2Svr_DataRw :: handlePostNotice ( const QxPack::IcJsonRpc2Request &req )
{
    if ( ! req.isNull() ) {
        QByteArray ba = QJsonDocument( req.make()).toJson( QJsonDocument::Compact );
        emit this->reqPost( ba, true );
    }
}


// ////////////////////////////////////////////////////////////////////////////
//
//                        handler resp & req item
//
// ////////////////////////////////////////////////////////////////////////////
#define JRPC_REQ_VECT   QVector<QxPack::IcJsonRpc2Request>
#define JRPC_RESP_VECT  QVector<QxPack::IcJsonRpc2Response>
class QXPACK_IC_HIDDEN  IcJsonRpc2Handler_Item {
private:
    JRPC_REQ_VECT  m_req_vect; JRPC_RESP_VECT  m_resp_vect;
    int m_total_cnt, m_curr_req_cnt, m_curr_resp_cnt; bool m_is_array;
public :
    explicit IcJsonRpc2Handler_Item( )
    { m_total_cnt = m_curr_req_cnt = m_curr_resp_cnt = 0; m_is_array = false; }

    IcJsonRpc2Handler_Item ( const JRPC_REQ_VECT &req, bool is_array )
    {
        m_req_vect = req; m_is_array = is_array; m_total_cnt = req.size();
        m_curr_req_cnt = 0; m_curr_resp_cnt = 0;
        m_resp_vect.resize( m_total_cnt );
    }
    inline bool isArray() const        { return m_is_array; }
    inline bool isRespFull() const     { return m_curr_resp_cnt == m_total_cnt; }
    inline JRPC_RESP_VECT&  respVect() { return m_resp_vect; }

    bool takeNextRequest( IcJsonRpc2Request & );
    bool pushResponse   ( const IcJsonRpc2Response & );
};

// ============================================================================
// take the next request, if OK, return true.
// ============================================================================
bool  IcJsonRpc2Handler_Item :: takeNextRequest( IcJsonRpc2Request &req )
{
    if ( m_curr_req_cnt < m_total_cnt ) {
        req = m_req_vect.at( m_curr_req_cnt );
        m_req_vect[ m_curr_req_cnt ++ ] = IcJsonRpc2Request(); // drop old..
        return true;
    } else {
        return false;
    }
}

// ============================================================================
// push response into response
// ============================================================================
bool  IcJsonRpc2Handler_Item :: pushResponse( const IcJsonRpc2Response &resp )
{
    if ( m_curr_resp_cnt < m_total_cnt ) {
        m_resp_vect[ m_curr_resp_cnt ++ ] = resp;
        return true;
    } else {
        return false; // full!
    }
}

// ////////////////////////////////////////////////////////////////////////////
//
//                        handler
//
// ////////////////////////////////////////////////////////////////////////////
#define JRPC_ERR         QxPack::IcJsonRpc2Error
#define JRPC_RESP        QxPack::IcJsonRpc2Response
#define T_SvrHandler( o )  T_PtrCast( IcJsonRpc2HandlerPriv*, o )
class QXPACK_IC_HIDDEN IcJsonRpc2HandlerPriv : public QObject {
    Q_OBJECT
private:
    QThread m_thread;  IcDataTransRsHdeBase *m_impl; IcJsonRpc2Svr_DataRw *m_drw;
    QString m_rs_type; qintptr m_sck;
    QList<IcJsonRpc2Handler_Item>  m_item_list;
    IcJsonRpc2PkgDefaultFact   m_def_fact;
protected: 
           void  initImpl  ( );
           void  deinitImpl( );
           bool  postVect  ( const JRPC_RESP_VECT & );
           bool  checkIfEmptyVectReq ( const JRPC_REQ_VECT & );
           bool  checkIfAllEmptyReq  ( const JRPC_REQ_VECT &, bool is_array );
    QByteArray   makeInvalidReqErr   ( int cnt ); // cnt > 1, will make an array errors
    Q_SLOT void  onNewVectReq ( const JRPC_REQ_VECT &, bool );

public :
    IcJsonRpc2HandlerPriv ( const QString &rs_type, qintptr sck, IcDataTransFactory, void* );
    virtual ~IcJsonRpc2HandlerPriv() Q_DECL_OVERRIDE;

    inline IcDataTransRsHdeBase* implement() { return m_impl; }
    inline bool  hasPendingRequest() { return ! m_item_list.isEmpty(); }

    bool  takePendingRequest ( QxPack::IcJsonRpc2Request & );
    bool  post   ( const QxPack::IcJsonRpc2Response & );
    bool  notice ( const QxPack::IcJsonRpc2Request &  );

    Q_SIGNAL void newRequest ();
    Q_SIGNAL void hostConnected();
    Q_SIGNAL void finished ();
    Q_SIGNAL void dataSent ();
    Q_SIGNAL void errorMessage( int, const QString & );
};

// ============================================================================
// ctor
// ============================================================================
IcJsonRpc2HandlerPriv :: IcJsonRpc2HandlerPriv (
    const QString &rs_type, qintptr sck , IcDataTransFactory fact, void *fact_ctxt
) : QObject( Q_NULLPTR ), m_impl( Q_NULLPTR ), m_drw( Q_NULLPTR ), m_sck( sck ),
    m_def_fact( fact, fact_ctxt )
{
    m_rs_type = rs_type;
    this->initImpl();
}

// ============================================================================
// dtor
// ============================================================================
IcJsonRpc2HandlerPriv :: ~IcJsonRpc2HandlerPriv ( )
{
    this->deinitImpl();
}

// ============================================================================
// init. implement object
// ============================================================================
void  IcJsonRpc2HandlerPriv :: initImpl ( )
{
    m_thread.start();

    // ------------------------------------------------------------------------
    // create the implement in target thread
    // ------------------------------------------------------------------------
    m_impl = qobject_cast<IcDataTransRsHdeBase*>(
      IcRmtObjCreator::createObjInThread (
          & m_thread, []( void *t )->QObject* {
              IcJsonRpc2HandlerPriv *t_this = reinterpret_cast<IcJsonRpc2HandlerPriv*>( t );
              return IcDataTransRsHdeBase::createObj(
                  t_this->m_rs_type, & IcJsonRpc2PkgDefaultFact::factory, &t_this->m_def_fact
              );
          }, this, false
    ));

    // ------------------------------------------------------------------------
    // create the data read writer in target thread
    // ------------------------------------------------------------------------
    if ( m_impl != Q_NULLPTR ) {
      m_drw = qobject_cast<IcJsonRpc2Svr_DataRw*> (
         IcRmtObjCreator::createObjInThread (
           & m_thread, []( void *obj )->QObject* {
               IcJsonRpc2HandlerPriv *t_this = reinterpret_cast< IcJsonRpc2HandlerPriv*>( obj );
               IcJsonRpc2Svr_DataRw *dr = new IcJsonRpc2Svr_DataRw;
               dr->connect(              // handle data from data transfer
                   t_this->m_impl, SIGNAL(newData( const QByteArray &)),
                   SLOT(handleNewPackage( const QByteArray &))
               );
               t_this->m_impl->connect ( // post the data to implement
                   dr, SIGNAL(reqPost( const QByteArray &, bool)),
                   SLOT(post( const QByteArray&, bool ) )
               );
               return dr;
         }, this, false
      ));
    }

    // ------------------------------------------------------------------------
    // make signal connections
    // ------------------------------------------------------------------------
    if ( m_drw != Q_NULLPTR ) {
        QObject::connect(
            m_drw, SIGNAL(newVectRequest( const QVector<QxPack::IcJsonRpc2Request>&, bool)),
            this,  SLOT( onNewVectReq( const QVector<QxPack::IcJsonRpc2Request>&, bool ))
        );
    }
    if ( m_impl != Q_NULLPTR ) {
        QObject::connect(
            m_impl, SIGNAL(errorMsg(int,const QString &)), this, SIGNAL(errorMessage(int, const QString &))
        );
        QObject::connect( m_impl, SIGNAL(dataSent()), this, SIGNAL(dataSent()));
        QObject::connect( m_impl, SIGNAL(finished()), this, SIGNAL(finished()));
        QObject::connect( m_impl, SIGNAL(hostConnected()), this, SIGNAL(hostConnected()));
    }

    // ---------------------------------------------------------
    // try do initialize the socket
    // ---------------------------------------------------------
    if ( m_impl != Q_NULLPTR ) {
        bool rmt_ret = false;
        QMetaObject::invokeMethod (
            m_impl, "initByDscr", Qt::BlockingQueuedConnection,
            Q_RETURN_ARG(bool,rmt_ret),Q_ARG( QVariant, QVariant::fromValue( m_sck ))
        );
    }
}

// ============================================================================
// deinit the implement
// ============================================================================
void   IcJsonRpc2HandlerPriv :: deinitImpl()
{
    if ( m_drw   != Q_NULLPTR ) {
        QObject::disconnect( m_drw, Q_NULLPTR, this, Q_NULLPTR );
        if ( m_impl != Q_NULLPTR ) {
            QObject::disconnect( m_drw,  Q_NULLPTR, m_impl, Q_NULLPTR );
            QObject::disconnect( m_impl, Q_NULLPTR, m_drw,  Q_NULLPTR );
        }
        m_drw->deleteLater();  // post quit message
        m_drw = Q_NULLPTR;
    }
    if ( m_impl != Q_NULLPTR ) {
        QObject::disconnect( m_impl, Q_NULLPTR, this, Q_NULLPTR );
        IcRmtObjDeletor::deleteObjInThread(
            & m_thread, []( void *ctxt, QObject* ) {
                IcJsonRpc2HandlerPriv *t_this = reinterpret_cast< IcJsonRpc2HandlerPriv*>( ctxt );
                IcDataTransRsHdeBase::deleteObj( t_this->m_impl );
            }, this, Q_NULLPTR, false
        );
        m_impl = Q_NULLPTR;
    }
    if ( m_thread.isRunning() ) { m_thread.quit(); m_thread.wait(); }
}

// ============================================================================
// [ protected ] make error about invalid request
// ============================================================================
QByteArray  IcJsonRpc2HandlerPriv :: makeInvalidReqErr( int cnt )
{
    if ( cnt < 2 ) {
        return QJsonDocument(
            IcJsonRpc2Error( IcJsonRpc2Error::Code_InvalidRequest, 0 ).make()
        ).toJson( QJsonDocument::Compact );
    } else {
        QJsonArray ja; QJsonObject jo = IcJsonRpc2Error( IcJsonRpc2Error::Code_InvalidRequest, 0 ).make();
        while ( cnt -- > 0 ) { ja.append( jo ); }
        return QJsonDocument( ja ).toJson( QJsonDocument::Compact );
    }
}

// ============================================================================
// [ protected ] check if the vector request is empty ?
// ============================================================================
bool IcJsonRpc2HandlerPriv :: checkIfEmptyVectReq( const JRPC_REQ_VECT &v_req )
{
    if ( ! v_req.isEmpty() ) { return false; }
    if ( m_impl != Q_NULLPTR ) {
        QMetaObject::invokeMethod (
            m_impl, "post", Qt::QueuedConnection,
            Q_ARG( const QByteArray&, this->makeInvalidReqErr(1)), Q_ARG( bool, false )
        );
    }
    return true;  // true means already handle this request vector.
}

// ============================================================================
// [ protected ] check if all item is empty
// ============================================================================
bool IcJsonRpc2HandlerPriv :: checkIfAllEmptyReq( const JRPC_REQ_VECT &v_req, bool is_array )
{
    // ------------------------------------------------------------------------
    // enum all member in this vector, try to find non-empty request
    // ------------------------------------------------------------------------
    if ( v_req.isEmpty()) { return false; }
    bool is_all_empty = true; JRPC_REQ_VECT::const_iterator c_itr = v_req.constBegin();
    while ( c_itr != v_req.constEnd()) {
        const IcJsonRpc2Request & req = (*c_itr ++ );
        if ( ! req.isNull()) { is_all_empty = false; break; }
    }

    // ------------------------------------------------------------------------
    // if all member is empty, need to do error response
    // ------------------------------------------------------------------------
    if ( is_all_empty && m_impl != Q_NULLPTR ) {
        if ( is_array ) { // is array means user post [] array as request.
            QMetaObject::invokeMethod(
                m_impl, "post", Qt::QueuedConnection,
                Q_ARG( const QByteArray&, this->makeInvalidReqErr(v_req.size())),
                Q_ARG( bool, false )
             );
        } else {
            QMetaObject::invokeMethod(
                m_impl, "post", Qt::QueuedConnection,
                Q_ARG( const QByteArray&, this->makeInvalidReqErr(1)), Q_ARG( bool, false )
            );
        }
    }

    return is_all_empty; // true means already handle this vector request
}

// ============================================================================
// slot: handle the new request vector
// ============================================================================
void IcJsonRpc2HandlerPriv :: onNewVectReq ( const QVector<QxPack::IcJsonRpc2Request> &v_req, bool is_array )
{
    // ------------------------------------------------------------------------
    // rules:
    // 1) empty array means invalid request, here directly return single error object
    // 2) if all array item is empty request, make array invalid request..
    // ------------------------------------------------------------------------
    if ( this->checkIfEmptyVectReq( v_req )) { return; }
    if ( this->checkIfAllEmptyReq( v_req, is_array )) { return; }

    // ------------------------------------------------------------------------
    // add requst, let user to handle these requets.
    // ------------------------------------------------------------------------
    IcJsonRpc2Handler_Item req_item( v_req, is_array );
    m_item_list.push_back( req_item );

    emit this->newRequest();
}

// ============================================================================
// take the request from pending list
// ============================================================================
bool  IcJsonRpc2HandlerPriv :: takePendingRequest ( QxPack::IcJsonRpc2Request &vr )
{
    // ------------------------------------------------------------------------
    // NOTE: here make a barrier of request.
    // user can only take next request in another vector while complete current request by
    // post the response.
    // ------------------------------------------------------------------------
    bool is_taked_req = false;
    vr = QxPack::IcJsonRpc2Request();
    if ( ! m_item_list.isEmpty()) {
        IcJsonRpc2Handler_Item &item =  m_item_list.first();
        is_taked_req = item.takeNextRequest( vr );
    }
    return is_taked_req;
}

// ============================================================================
// post response vector
// ============================================================================
bool   IcJsonRpc2HandlerPriv :: postVect ( const JRPC_RESP_VECT &v_resp )
{
    if ( m_drw != Q_NULLPTR ) {
        return QMetaObject::invokeMethod (
            m_drw, "handlePostPackage", Qt::QueuedConnection,
            Q_ARG(const QVector<QxPack::IcJsonRpc2Response>&, v_resp )
        );
    } else {
        return false;
    }
}

// ============================================================================
// post the response
// ============================================================================
bool IcJsonRpc2HandlerPriv :: post ( const QxPack::IcJsonRpc2Response &resp )
{  
    if ( m_item_list.isEmpty()) { return false; }

    // ------------------------------------------------------------------------
    // the first item is the current working item
    // ------------------------------------------------------------------------
    IcJsonRpc2Handler_Item &item = m_item_list.first();
    if ( ! item.pushResponse( resp )) { return false; }
    if ( item.isRespFull()) {
        this->postVect( item.respVect() );
        m_item_list.removeFirst();
    }

    // ------------------------------------------------------------------------
    // post the next request signal
    // ------------------------------------------------------------------------
    bool is_need_emit_req = ! m_item_list.isEmpty();
    if ( is_need_emit_req ) {
        QMetaObject::invokeMethod( this, "newRequest", Qt::QueuedConnection );
    }
    return true;
}

// ============================================================================
// notice the remote client
// ============================================================================
bool  IcJsonRpc2HandlerPriv :: notice ( const QxPack::IcJsonRpc2Request &req )
{
    if ( m_drw != Q_NULLPTR ) {
        return QMetaObject::invokeMethod (
               m_drw, "handlePostNotice", Qt::QueuedConnection,
               Q_ARG( const QxPack::IcJsonRpc2Request&, req )
        );
    } else {
        return false;
    }
}

// ////////////////////////////////////////////////////////////////////////////
//                          handler wrap API
// ////////////////////////////////////////////////////////////////////////////
// ============================================================================
// ctor
// ============================================================================
IcJsonRpc2Handler :: IcJsonRpc2Handler (
    const QString &rs_type, qintptr sck, IcDataTransFactory fact, void *ctxt, QObject *pa
) : QObject( pa )
{
    m_obj = qxpack_ic_new( IcJsonRpc2HandlerPriv, rs_type, sck, fact, ctxt );
    QObject::connect( T_SvrHandler( m_obj ), SIGNAL(newRequest()), this, SIGNAL(newRequest()));
    QObject::connect( T_SvrHandler( m_obj ), SIGNAL(finished()),   this, SIGNAL(finished()));
    QObject::connect( T_SvrHandler( m_obj ), SIGNAL(hostConnected()), this,SIGNAL(hostConnected()));
    QObject::connect( T_SvrHandler( m_obj ), SIGNAL(dataSent()), this, SIGNAL(dataSent()));
    QObject::connect(
        T_SvrHandler( m_obj ), SIGNAL(errorMessage(int, const QString &)),
        this, SIGNAL(errorMessage(int,const QString&))
     );
}

// ============================================================================
// dtor
// ============================================================================
IcJsonRpc2Handler :: ~IcJsonRpc2Handler ( )
{
    T_SvrHandler( m_obj )->disconnect();
    qxpack_ic_delete( m_obj, IcJsonRpc2HandlerPriv );
}

// ============================================================================
// check if the socket is working or not
// ============================================================================
bool  IcJsonRpc2Handler :: isWorking() const
{
    bool is_work = false;
    IcDataTransRsHdeBase *impl = T_SvrHandler( m_obj )->implement();
    if ( impl != Q_NULLPTR ) {
        bool call_ret = QMetaObject::invokeMethod (
            impl, "isWorking", Qt::BlockingQueuedConnection, Q_RETURN_ARG(bool,is_work)
        );
        if ( ! call_ret ) { is_work = false; }
    }
    return is_work;
}

// ============================================================================
// post response vector to socket, true means posted.
// ============================================================================
bool  IcJsonRpc2Handler :: post( const QxPack::IcJsonRpc2Response &vr )
{ return T_SvrHandler( m_obj )->post( vr ); }

// ============================================================================
// notice the remote client
// ============================================================================
bool  IcJsonRpc2Handler :: notice( const IcJsonRpc2Request &req )
{ return T_SvrHandler( m_obj )->notice( req ); }

// ============================================================================
// check if exist pending request
// ============================================================================
bool  IcJsonRpc2Handler :: hasPendingRequest ( ) const
{ return T_SvrHandler( m_obj )->hasPendingRequest(); }

// ============================================================================
// get the next pending request
// ============================================================================
bool IcJsonRpc2Handler :: takeNextPendingRequest( QxPack::IcJsonRpc2Request &r )
{ return T_SvrHandler( m_obj )->takePendingRequest( r ); }




// ////////////////////////////////////////////////////////////////////////////
//
//   the jsonrpc2 server
//
// ////////////////////////////////////////////////////////////////////////////
// ============================================================================
// ctor
// ============================================================================
IcJsonRpc2Svr :: IcJsonRpc2Svr (
    const QString &rs_type, const QString & host, quint16 port, IcDataTransFactory fact, void *fact_ctxt, QObject *pa
) : IcDataTransSvr( rs_type, host, port, fact, fact_ctxt, pa )
{
    gRegObj();
    m_obj = Q_NULLPTR;
}

// ============================================================================
// dtor
// ============================================================================
IcJsonRpc2Svr :: ~IcJsonRpc2Svr ( )
{
}

// ============================================================================
// [ virtual override ] create handler
// ============================================================================
QObject*  IcJsonRpc2Svr :: createHandler (
    const QString &rs_type, qintptr dscr, IcDataTransFactory fact, void *fact_ctxt
) {
     return new IcJsonRpc2Handler( rs_type, dscr, fact, fact_ctxt );
}


}


#ifdef __GNUC__ // in GCC 5, close below warnings
#pragma GCC diagnostic pop
#endif

#include "qxpack_ic_jsonrpc2_svr.moc"

#endif
