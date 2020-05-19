// ////////////////////////////////////////////////////////////////////////////
/*!
 * @file qxpack_ic_jsonrpc2cli
 * @author nightwing
 * @date 2019/03, 2019/05
 * @verbatim
 *     <author>    <date>    <history memo.>
 *     nightwing  2019/02     build
 *     nightwing  2019/03     fixed.
 *     nightwing  2019/05     fixed. changing the package read buffer...
 * @endverbatim
 */
// ////////////////////////////////////////////////////////////////////////////
#ifndef QXPACK_IC_JSONRPC2_CLI_CXX
#define QXPACK_IC_JSONRPC2_CLI_CXX

#include "qxpack_ic_jsonrpc2_cli.hxx"
#include "qxpack/indcom/sys/qxpack_ic_jsonrpc2.hxx"
#include "qxpack/indcom/sys/qxpack_ic_rmtobjcreator_priv.hxx"
#include "qxpack/indcom/sys/qxpack_ic_rmtobjdeletor_priv.hxx"
#include "qxpack/indcom/common/qxpack_ic_memcntr.hxx"
#include "qxpack/indcom/net/qxpack_ic_jsonrpc2_buff.hxx"
#include "qxpack/indcom/net/qxpack_ic_datatrans_rshde_impl_bundle.hxx"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>

namespace QxPack {

// ////////////////////////////////////////////////////////////////////////////
//
//              local functions
//
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
//                data reader
//
// ////////////////////////////////////////////////////////////////////////////
class QXPACK_IC_HIDDEN  IcJsonRpc2Cli_DataRw : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY( IcJsonRpc2Cli_DataRw )
protected:
    bool  readArrayData ( QVector<QxPack::IcJsonRpc2Response> &, const QJsonArray & );
    bool  readSingleData( QxPack::IcJsonRpc2Response &, const QJsonObject & );
    bool  writeArrayData ( QJsonArray &, const QVector<QxPack::IcJsonRpc2Request>& );
public :
    explicit IcJsonRpc2Cli_DataRw ( QObject *pa = Q_NULLPTR ) : QObject( pa ) { }
    virtual ~IcJsonRpc2Cli_DataRw ( ) { }

    Q_SLOT    void  handleNewPackage( const QByteArray &ba );
    Q_SLOT    void  handlePostPackage( const QVector<QxPack::IcJsonRpc2Request> &, bool is_array );

    Q_SIGNAL  void  reqPost ( const QByteArray &, bool );
    Q_SIGNAL  void  newVectResponse ( const QVector<QxPack::IcJsonRpc2Response> &, bool is_array );
    Q_SIGNAL  void  newNotice ( const QxPack::IcJsonRpc2Request & );
};

// ============================================================================
//  read the array data from array object
// ============================================================================
bool  IcJsonRpc2Cli_DataRw :: readArrayData (
    QVector<QxPack::IcJsonRpc2Response> &vr, const QJsonArray &ja
) {
    // ------------------------------------------------------------------------
    // false while param.ja is empty
    // ------------------------------------------------------------------------
    if ( ja.isEmpty() ) {
        vr = QVector<QxPack::IcJsonRpc2Response>();
        return false;
    }

    // ------------------------------------------------------------------------
    // enum all items in the jsonArray, make them as vector
    // ------------------------------------------------------------------------
    QVector<QxPack::IcJsonRpc2Response> vect; vect.reserve( ja.size() );
    QJsonArray::const_iterator c_itr = ja.constBegin();
    while ( c_itr != ja.constEnd()) {
        const QJsonValue &jv = *( c_itr ++ );
        if ( jv.isObject() ) {
            QxPack::IcJsonRpc2Response resp;
            this->readSingleData( resp, jv.toObject());
            vect.push_back( resp );
        } else {
            QxPack::IcJsonRpc2Error  err;
            err.setErrorCode( QxPack::IcJsonRpc2Error::Code_User_Unrecognized_Response );
            vect.push_back( QxPack::IcJsonRpc2Response( err ) );
        }
    }

    vr = vect;
    return true;
}

// ============================================================================
//  read single object from jsonobject
// ============================================================================
bool  IcJsonRpc2Cli_DataRw :: readSingleData (
    QxPack::IcJsonRpc2Response &resp, const QJsonObject &jo
) {
    // guess it is a request or notice
    if ( ! jo.value("method").isUndefined()) {
        IcJsonRpc2Request req;
        if ( req.loadFrom( jo ) ) {
            resp = IcJsonRpc2Response( req ); return true;
        }
    }

    // guess it is an error
    if ( ! jo.value("error").isUndefined()) {
        IcJsonRpc2Error err;
        if ( err.loadFrom( jo )) {
            resp = IcJsonRpc2Response( err ); return true;
        }
    }

    // guess it is a result
    IcJsonRpc2Result rsl;  // test the jsonObject is JsonRpc2 Result?
    if ( rsl.loadFrom( jo )) {
        resp = QxPack::IcJsonRpc2Response( rsl ); return true;
    }

    IcJsonRpc2Error err;
    err.setErrorCode( QxPack::IcJsonRpc2Error::Code_User_Unrecognized_Response );
    resp = QxPack::IcJsonRpc2Response( err );
    return false;
}

// ============================================================================
// write the array request
// ============================================================================
bool  IcJsonRpc2Cli_DataRw :: writeArrayData( QJsonArray &ja, const QVector<IcJsonRpc2Request> &vr )
{
    if ( vr.isEmpty()) { return false; }

    QVector<QxPack::IcJsonRpc2Request>::const_iterator c_itr = vr.constBegin();
    while ( c_itr != vr.constEnd()) {
        const QxPack::IcJsonRpc2Request &req = *( c_itr ++ );
        if ( ! req.isNull() ) { ja.append( req.make() ); }
    }
    return true;
}

// ============================================================================
// slot: handle the new package
// ============================================================================
void  IcJsonRpc2Cli_DataRw :: handleNewPackage( const QByteArray &ba )
{
    QJsonDocument doc = QJsonDocument::fromJson( ba );

    if ( doc.isArray()) {
        // --------------------------------------------------------------------
        // response is an array
        // --------------------------------------------------------------------
        QVector<QxPack::IcJsonRpc2Response> vr;
        this->readArrayData( vr, doc.array() );
        if ( ! vr.isEmpty() ) {  // at least one result or error
            emit this->newVectResponse( vr, true );
        } else {                 // empty array ( no jsonrpc2 object in array )
            IcJsonRpc2Error err( IcJsonRpc2Error::Code_User_Unrecognized_Response, 0 );
            vr.append( QxPack::IcJsonRpc2Response( err ));
            emit this->newVectResponse( vr, false );
        }

    } else if ( doc.isObject()) {
        // --------------------------------------------------------------------
        // remote returned single object as answere
        // --------------------------------------------------------------------
        QxPack::IcJsonRpc2Response resp;
        this->readSingleData( resp, doc.object() );
        if ( ! resp.isRequest() ) {
            emit this->newVectResponse( QVector<QxPack::IcJsonRpc2Response>() << resp, false );
        } else {
            emit this->newNotice( resp.request() );
        }
    } else {
        IcJsonRpc2Error err( IcJsonRpc2Error::Code_User_Unrecognized_Response, 0 );
        QVector<QxPack::IcJsonRpc2Response> vr;
        vr.append( QxPack::IcJsonRpc2Response( err ));
        emit this->newVectResponse( vr, false );
    }
}

// ============================================================================
// slot: handle the  post package
// ============================================================================
void   IcJsonRpc2Cli_DataRw :: handlePostPackage( const QVector<QxPack::IcJsonRpc2Request> &vr, bool is_array )
{
    if ( vr.isEmpty()) { return; }
    if ( is_array ) {
        QJsonArray ja;
        if ( this->writeArrayData( ja, vr )) {
            QByteArray ba = QJsonDocument( ja ).toJson( QJsonDocument::Compact );
            ja = QJsonArray();
            emit this->reqPost( ba, true );
        }
    } else {
        QJsonObject jo = vr.first().make();
        QByteArray  ba = QJsonDocument( jo ).toJson( QJsonDocument::Compact);
        jo = QJsonObject();
        emit this->reqPost( ba, true );
    }
}


// ////////////////////////////////////////////////////////////////////////////
//
//                      client private
//
// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// private object
// ////////////////////////////////////////////////////////////////////////////
#define T_PrivPtr( o ) T_ObjCast( IcJsonRpc2CliPriv*, o )
class QXPACK_IC_HIDDEN  IcJsonRpc2CliPriv : public QObject {
    Q_OBJECT
private:
    QThread m_thread; IcDataTransRsHdeBase  *m_impl; IcJsonRpc2Cli_DataRw *m_drw;
    QList<QVector<QxPack::IcJsonRpc2Response>>  m_resp_list;
    QList<QxPack::IcJsonRpc2Request>            m_notice_list;
    IcJsonRpc2PkgDefaultFact  m_def_fact;   QString m_rs_type;

protected:
    Q_SLOT void  onNewVectResp ( const QVector<QxPack::IcJsonRpc2Response>&, bool );
    Q_SLOT void  onNewNotice   ( const QxPack::IcJsonRpc2Request & );
           void  initImpl  ( );
           void  deInitImpl( );
public :
    explicit IcJsonRpc2CliPriv( const QString &rs_type, IcDataTransFactory fact, void * );
    virtual ~IcJsonRpc2CliPriv( ) Q_DECL_OVERRIDE;

    // 20190724-1055 nw added
    inline QString & rsTypeRef() { return m_rs_type; }
    inline IcJsonRpc2PkgDefaultFact &  defFactRef() { return m_def_fact; }

    inline IcDataTransRsHdeBase*  implement() { return m_impl; }
    inline bool  hasPendingResponse () { return ! m_resp_list.isEmpty(); }
    inline bool  hasPendingNotice   () { return ! m_notice_list.isEmpty(); }

    void  open ( const QString &host, quint16 );
    void  close( );

    bool  takePendingResponse( QVector<QxPack::IcJsonRpc2Response> & );
    bool  takePendingNotice  ( QxPack::IcJsonRpc2Request& );
    bool  post ( const QVector<QxPack::IcJsonRpc2Request> &, bool is_array = true );

    Q_SIGNAL void newResponse( );
    Q_SIGNAL void newNotice  ( );

    Q_SIGNAL void hostConnected( );
    Q_SIGNAL void finished( );
    Q_SIGNAL void dataSent( );
    Q_SIGNAL void errorMessage( int, const QString & );
};

// ============================================================================
// ctor
// ============================================================================
IcJsonRpc2CliPriv :: IcJsonRpc2CliPriv ( const QString &rs_type, IcDataTransFactory fact, void *ctxt )
  : QObject( Q_NULLPTR ), m_impl( Q_NULLPTR ), m_drw( Q_NULLPTR ), m_def_fact( fact, ctxt )
{
    m_rs_type = rs_type;
}

// ============================================================================
// dtor
// ============================================================================
IcJsonRpc2CliPriv :: ~IcJsonRpc2CliPriv ( )
{
    // nw@( 20190522 1515 ) old:  QObject::disconnect ( this, Q_NULLPTR, Q_NULLPTR, Q_NULLPTR );
    // use blockSignals instead
    this->blockSignals( true );
    this->deInitImpl();
}

// ============================================================================
// take the pending response from inner queue
// ============================================================================
bool  IcJsonRpc2CliPriv :: takePendingResponse( QVector<QxPack::IcJsonRpc2Response> &vr )
{
    vr = QVector<QxPack::IcJsonRpc2Response>();
    if ( ! m_resp_list.isEmpty() ) {
        vr = m_resp_list.takeFirst();
        return true;
    } else {
        return false;
    }
}

// ============================================================================
// take the pending notice from inner queue
// ============================================================================
bool  IcJsonRpc2CliPriv :: takePendingNotice( IcJsonRpc2Request &req )
{
    req = QxPack::IcJsonRpc2Request();
    if ( ! m_notice_list.isEmpty()) {
        req = m_notice_list.takeFirst();
        return true;
    } else {
        return false;
    }
}

// ============================================================================
// slot: handle the new response vector signal
// ============================================================================
void  IcJsonRpc2CliPriv :: onNewVectResp( const QVector<IcJsonRpc2Response> &vr, bool )
{
    m_resp_list.push_back( vr );
    emit this->newResponse();
}

// ============================================================================
// slot: handle the new notice signal
// ============================================================================
void  IcJsonRpc2CliPriv :: onNewNotice ( const IcJsonRpc2Request &req )
{
    m_notice_list.push_back( req );
    emit this->newNotice();
}

// ============================================================================
// post the request to target host
// ============================================================================
bool  IcJsonRpc2CliPriv :: post ( const QVector<QxPack::IcJsonRpc2Request> &vr, bool is_array )
{
    bool call_ret = false;

    if ( m_drw != Q_NULLPTR ) {
        call_ret = QMetaObject::invokeMethod (
            m_drw, "handlePostPackage", Qt::QueuedConnection,
            Q_ARG( const QVector<QxPack::IcJsonRpc2Request>&, vr ), Q_ARG( bool, is_array )
        );
    }

    return call_ret;
}

// ============================================================================
// initalize the implement object
// ============================================================================
void   IcJsonRpc2CliPriv :: initImpl ( )
{
    // ------------------------------------------------------------------------
    // create the implement in target thread
    // ------------------------------------------------------------------------ 
    m_thread.start();
    m_impl = qobject_cast<IcDataTransRsHdeBase*>(
      IcRmtObjCreator::createObjInThread (
          & m_thread, []( void *obj )->QObject* {
              IcJsonRpc2CliPriv *t_this = reinterpret_cast< IcJsonRpc2CliPriv*>( obj );
              return IcDataTransRsHdeBase::createObj (
                   t_this->m_rs_type, & IcJsonRpc2PkgDefaultFact::factory, & t_this->m_def_fact
              );
          }, this, false
    ));

    // ------------------------------------------------------------------------
    // create the data read in target thread
    // ------------------------------------------------------------------------
    if ( m_impl != Q_NULLPTR ) {
      m_drw = qobject_cast<IcJsonRpc2Cli_DataRw*> (
         IcRmtObjCreator::createObjInThread (
           & m_thread, []( void *obj )->QObject* {
               IcJsonRpc2CliPriv *t_this = reinterpret_cast< IcJsonRpc2CliPriv*>( obj );
               IcJsonRpc2Cli_DataRw *dr = new IcJsonRpc2Cli_DataRw;
               dr->connect( // handle data from data transfer
                   t_this->m_impl, SIGNAL(newData( const QByteArray &)),
                   SLOT(handleNewPackage( const QByteArray &))
               );
               t_this->m_impl->connect (
                   dr, SIGNAL(reqPost( const QByteArray &, bool)),
                   SLOT(post( const QByteArray&, bool ) )
               );

               return dr;
         }, this, false
      ));
    }

    // ------------------------------------------------------------------------
    // connect all necessary signals
    // ------------------------------------------------------------------------
    if ( m_drw != Q_NULLPTR ) {
        QObject::connect(
            m_drw, SIGNAL(newVectResponse( const QVector<QxPack::IcJsonRpc2Response> &, bool )),
            this,   SLOT(onNewVectResp( const QVector<QxPack::IcJsonRpc2Response> &, bool ))
        );
        QObject::connect(
            m_drw, SIGNAL(newNotice( const QxPack::IcJsonRpc2Request &)),
            this,   SLOT(onNewNotice( const QxPack::IcJsonRpc2Request &))
        );
    }
    if ( m_impl != Q_NULLPTR ) {
        QObject::connect(
            m_impl, SIGNAL(errorMsg(int,const QString &)),
            this,   SIGNAL(errorMessage(int, const QString &))
        );
        QObject::connect( m_impl, SIGNAL(dataSent()), this, SIGNAL(dataSent()));
        QObject::connect( m_impl, SIGNAL(finished()), this, SIGNAL(finished()));
        QObject::connect( m_impl, SIGNAL(hostConnected()), this, SIGNAL(hostConnected()));
    }
}

// ============================================================================
// delete the implement object
// ============================================================================
void   IcJsonRpc2CliPriv :: deInitImpl()
{
    if ( m_drw   != Q_NULLPTR ) {
        QObject::disconnect( m_drw, Q_NULLPTR, this, Q_NULLPTR );
        if ( m_impl != Q_NULLPTR ) {
            QObject::disconnect( m_drw, Q_NULLPTR, m_impl, Q_NULLPTR );
            QObject::disconnect( m_impl, Q_NULLPTR, m_drw, Q_NULLPTR );
        }
        m_drw->deleteLater();  // post quit message
        m_drw = Q_NULLPTR;
    }
    if ( m_impl != Q_NULLPTR ) {
        QObject::disconnect( m_impl, Q_NULLPTR, this, Q_NULLPTR );
        IcRmtObjDeletor::deleteObjInThread(
            & m_thread, []( void *ctxt, QObject* ) {
                IcJsonRpc2CliPriv *t_this = reinterpret_cast< IcJsonRpc2CliPriv*>( ctxt );
                IcDataTransRsHdeBase::deleteObj( t_this->m_impl );
            }, this, Q_NULLPTR, false
        );
        m_impl = Q_NULLPTR;
    }
    if ( m_thread.isRunning() ) { m_thread.quit(); m_thread.wait(); }
}

// ============================================================================
// open the target host
// ============================================================================
void   IcJsonRpc2CliPriv :: open ( const QString &host, quint16 port )
{
    this->deInitImpl();
    this->initImpl();

    if ( m_impl != Q_NULLPTR ) {
        bool call_ret = QMetaObject::invokeMethod (
            m_impl, "open", Qt::QueuedConnection,
            Q_ARG( const QString &, host ), Q_ARG( quint16, port ),
            Q_ARG( int, QIODevice::ReadWrite )
        );
        if ( ! call_ret ) { this->deInitImpl(); } // failed to do open call.
    }
}

// ============================================================================
// close the target host connection
// ============================================================================
void   IcJsonRpc2CliPriv :: close( )
{
    this->deInitImpl();
}


// ////////////////////////////////////////////////////////////////////////////
// wrap API
// ////////////////////////////////////////////////////////////////////////////
// ============================================================================
// ctor
// ============================================================================
IcJsonRpc2Cli :: IcJsonRpc2Cli (
    const QString &rs_type, IcDataTransFactory fact, void *ctxt, QObject *pa
) : QObject( pa )
{
    gRegObj();
    m_obj = qxpack_ic_new( IcJsonRpc2CliPriv, rs_type, fact, ctxt );

    QObject::connect( T_PrivPtr( m_obj ), SIGNAL(newResponse()), this, SIGNAL(newResponse()));
    QObject::connect( T_PrivPtr( m_obj ), SIGNAL(newNotice()),   this, SIGNAL(newNotice()));
    QObject::connect( T_PrivPtr( m_obj ), SIGNAL(hostConnected()), this, SIGNAL(hostConnected()));
    QObject::connect( T_PrivPtr( m_obj ), SIGNAL(dataSent()), this, SIGNAL(dataSent()));
    QObject::connect( T_PrivPtr( m_obj ), SIGNAL(finished()), this, SIGNAL(finished()));
    QObject::connect(
        T_PrivPtr( m_obj ), SIGNAL(errorMessage(int, const QString &)),
        this, SIGNAL(errorMessage(int,const QString &))
    );
}

// ============================================================================
// dtor
// ============================================================================
IcJsonRpc2Cli :: ~IcJsonRpc2Cli ( )
{
    QObject::disconnect( T_PrivPtr( m_obj ), Q_NULLPTR, this, Q_NULLPTR );
    qxpack_ic_delete( m_obj, IcJsonRpc2CliPriv );
}

// ============================================================================
// check if implement is working
// ============================================================================
bool IcJsonRpc2Cli :: isWorking() const
{
    if ( T_PrivPtr( m_obj )->implement() == Q_NULLPTR ) {
        return false;
    } else {
        bool ret = false;
        bool call_ret = QMetaObject::invokeMethod (
            T_PrivPtr( m_obj )->implement(), "isWorking", Qt::BlockingQueuedConnection,
            Q_RETURN_ARG( bool, ret )
        );
        return ( call_ret ? ret : false );
    }
}

// ============================================================================
// connect to target host, the hostConnected() will be emit if connected
// ============================================================================
void  IcJsonRpc2Cli :: connectToHost( const QString &host, quint16 port )
{  T_PrivPtr( m_obj )->open( host, port ); }

// ============================================================================
// disconnect from host, 2019/07/24 added
// ============================================================================
void  IcJsonRpc2Cli :: disconnectFromHost()
{
    // get-back the factory...
    QString rs_type = T_PrivPtr( m_obj )->rsTypeRef();
    IcDataTransFactory df = T_PrivPtr( m_obj )->defFactRef().extFact();
    void *ctxt = T_PrivPtr( m_obj )->defFactRef().extCtxt();

    // drop old object
    QObject::disconnect( T_PrivPtr( m_obj ), Q_NULLPTR, this, Q_NULLPTR );
    qxpack_ic_delete( m_obj, IcJsonRpc2CliPriv );

    // create new object.
    m_obj = qxpack_ic_new( IcJsonRpc2CliPriv, rs_type, df, ctxt );
    QObject::connect( T_PrivPtr( m_obj ), SIGNAL(newResponse()), this, SIGNAL(newResponse()));
    QObject::connect( T_PrivPtr( m_obj ), SIGNAL(newNotice()),   this, SIGNAL(newNotice()));
    QObject::connect( T_PrivPtr( m_obj ), SIGNAL(hostConnected()), this, SIGNAL(hostConnected()));
    QObject::connect( T_PrivPtr( m_obj ), SIGNAL(dataSent()), this, SIGNAL(dataSent()));
    QObject::connect( T_PrivPtr( m_obj ), SIGNAL(finished()), this, SIGNAL(finished()));
    QObject::connect(
        T_PrivPtr( m_obj ), SIGNAL(errorMessage(int, const QString &)),
        this, SIGNAL(errorMessage(int,const QString &))
    );
}

// ============================================================================
// check if has pending response
// ============================================================================
bool  IcJsonRpc2Cli :: hasPendingResponse ( )
{  return T_PrivPtr( m_obj )->hasPendingResponse(); }

// ============================================================================
// check if has pending notice
// ============================================================================
bool  IcJsonRpc2Cli :: hasPendingNotice()
{  return T_PrivPtr( m_obj )->hasPendingNotice(); }

// ============================================================================
// take the next pending response
// ============================================================================
bool  IcJsonRpc2Cli :: takeNextPendingResponse( QVector<QxPack::IcJsonRpc2Response> &vr )
{  return T_PrivPtr( m_obj )->takePendingResponse( vr ); }

// ============================================================================
// take the next pending notice
// ============================================================================
bool  IcJsonRpc2Cli :: takeNextPendingNotice(IcJsonRpc2Request &req )
{  return T_PrivPtr( m_obj )->takePendingNotice( req ); }

// ============================================================================
// post the request to server host
// ============================================================================
bool  IcJsonRpc2Cli :: post( const QVector<QxPack::IcJsonRpc2Request> &vr )
{  return T_PrivPtr( m_obj )->post( vr, true ); }

// ============================================================================
// post the request
// ============================================================================
bool  IcJsonRpc2Cli :: post ( QxPack::IcJsonRpc2Request &req )
{  return T_PrivPtr( m_obj )->post( QVector<QxPack::IcJsonRpc2Request>() << req, false ); }

// ============================================================================
// [ static ] retreive all rs type
// ============================================================================
QStringList  IcJsonRpc2Cli :: avaliableRsTypeList( )
{  return IcDataTransRsHdeBase::avaliableRsTypeList();  }


}

#include "qxpack_ic_jsonrpc2_cli.moc"
#endif
