// ============================================================================
// author: nightwing
// date  : 2019/05/24
// to do : check point
// ============================================================================
#ifndef QXPACK_IC_JSONRPC2_RESPHDE_CXX
#define QXPACK_IC_JSONRPC2_RESPHDE_CXX

#include "qxpack_ic_jsonrpc2_resphde.hxx"
#include "qxpack/indcom/common/qxpack_ic_memcntr.hxx"

#include <QMetaObject>
#include <QMetaMethod>
#include <QMap>
#include <QString>
#include <QPair>

namespace QxPack {


// ////////////////////////////////////////////////////////////////////////////
// private object
// ////////////////////////////////////////////////////////////////////////////
typedef QPair<QxPack::IcJsonRpc2Request,QVariant>  ReqInfoValue;
typedef QPair<QMetaMethod,ReqInfoValue>            ReqInfo;

#define T_PrivPtr( o )  T_ObjCast( IcJsonRpc2RespHdePriv*, o )
class QXPACK_IC_HIDDEN IcJsonRpc2RespHdePriv : public QObject {
    Q_OBJECT
private:
    QMap< QString, QMetaMethod >  m_tgt_mm_map;  // < func_name,method >
    QMap< QString, ReqInfo >  m_idstr_mm_map;
    QMap< qintptr, ReqInfo >  m_id_mm_map;
    QObject  *m_tgt;
protected:
    Q_SLOT void  onTgt_destroyed( QObject* );
public :
    explicit IcJsonRpc2RespHdePriv( );
    virtual ~IcJsonRpc2RespHdePriv( ) Q_DECL_OVERRIDE;
    void  build ( QObject* );
    void  clear ( );
    bool  addDispoCall ( const IcJsonRpc2Request&, const QString &sfx_name, const QVariant& );
    bool  rmvDispoCall ( const IcJsonRpc2Request& );
    void  handleResp   ( const IcJsonRpc2Response& );

    Q_SIGNAL void unhandleResponse( const QxPack::IcJsonRpc2Response& );
};

// ============================================================================
// ctor
// ============================================================================
IcJsonRpc2RespHdePriv :: IcJsonRpc2RespHdePriv ( ) : QObject( Q_NULLPTR )
{
    m_tgt = Q_NULLPTR;
}

// ============================================================================
// dtor
// ============================================================================
IcJsonRpc2RespHdePriv :: ~IcJsonRpc2RespHdePriv ( )
{
    if ( m_tgt != Q_NULLPTR ) {
       QObject::disconnect( m_tgt, Q_NULLPTR, this, Q_NULLPTR );
    }
}

// ============================================================================
// clear all added disposable call
// ============================================================================
void   IcJsonRpc2RespHdePriv :: clear ( )
{
    m_idstr_mm_map = QMap<QString,ReqInfo>();
    m_id_mm_map    = QMap<qintptr,ReqInfo>();
}

// ============================================================================
// build information from target
// ============================================================================
void   IcJsonRpc2RespHdePriv :: build ( QObject *tgt )
{
    if ( tgt == Q_NULLPTR || m_tgt != Q_NULLPTR ) { return; }

    #define RESP_PREFIX_STR  "onResp_"
    #define RESP_SFX_STR     "(QxPack::IcJsonRpc2Request,QxPack::IcJsonRpc2Response,QVariant)"

    int pre_len = qstrlen( RESP_PREFIX_STR );
    int sfx_len = qstrlen( RESP_SFX_STR );

    // ------------------------------------------------------------------------
    // build the search map
    // ------------------------------------------------------------------------
    m_tgt_mm_map = QMap<QString,QMetaMethod>();
    const QMetaObject *mo = tgt->metaObject();
    int me_cnt = mo->methodCount();

    for ( int i = mo->methodOffset(); i < me_cnt; i ++ ) {
        QMetaMethod mm = mo->method(i);

        // skip if not slot or invokable
        if ( mm.methodType() != QMetaMethod::Slot && mm.methodType() != QMetaMethod::Method )
        { continue; }

        // skip if not the onResp_XXXXXX(QxPack::IcJsonRpc2Request,QxPack::IcJsonRpc2Response) format
        // example:
        //    Q_INVOKABLE void onResp_MyFunc(
        //       const QxPack::IcJsonRpc2Request&,const QxPack::IcJsonRpc2Response &
        //    );
        // The const ref. param. will be converted to value copy param.
        QString name_str = mm.methodSignature();
        if ( ! name_str.startsWith( QStringLiteral( RESP_PREFIX_STR ) ) ||
             ! name_str.endsWith  ( QStringLiteral( RESP_SFX_STR ) )
        ) { continue; }

        // pickup the XXXXXX ( user function name ) as key, build as a node
        QString sfx_name = name_str.mid( pre_len, name_str.size() - pre_len - sfx_len );
        m_tgt_mm_map.insert( sfx_name, mm );
    }

    // ------------------------------------------------------------------------
    // here need to remember the destoryed signal
    // ------------------------------------------------------------------------
    m_tgt = tgt;
    QObject::connect (
        m_tgt, SIGNAL(destroyed(QObject*)), this, SLOT(onTgt_destroyed(QObject*))
    );
}

// ============================================================================
// slot: handle the target object destoryed event
// ============================================================================
void  IcJsonRpc2RespHdePriv :: onTgt_destroyed( QObject* )
{
    m_tgt = Q_NULLPTR;
}

// ============================================================================
// add the disposable call, true means added.
// ============================================================================
bool IcJsonRpc2RespHdePriv :: addDispoCall (
    const IcJsonRpc2Request &req, const QString &sfx_name, const QVariant &opt
) {
    // check param.
    if ( req.isNull() || sfx_name.isEmpty()) { return false; }

    // check if the spec. function is existed in the method map
    QMap<QString,QMetaMethod>::const_iterator me_itr = m_tgt_mm_map.constFind( sfx_name );
    if ( me_itr == m_tgt_mm_map.constEnd()) { return false; }

    // check if the request id is unique
    bool is_added = false;
    if ( req.isIdStr()) {
        QString id_str = req.idStr();
        QMap<QString,ReqInfo>::const_iterator str_itr =
            m_idstr_mm_map.constFind( id_str );
        if ( str_itr == m_idstr_mm_map.constEnd() ) {
            m_idstr_mm_map.insert (
                id_str, ReqInfo( me_itr.value(), ReqInfoValue( req, opt ))
            );
            is_added = true;
        }
    } else {
        qintptr id_int = req.id();
        QMap<qintptr,ReqInfo>::const_iterator int_itr =
            m_id_mm_map.constFind( id_int );
        if ( int_itr == m_id_mm_map.constEnd() ) {
            m_id_mm_map.insert (
                id_int, ReqInfo( me_itr.value(), ReqInfoValue( req, opt ))
            );
        }
    }

    return is_added;
}

// ============================================================================
// remove the dispo-call spec. by request
// ============================================================================
bool   IcJsonRpc2RespHdePriv :: rmvDispoCall ( const IcJsonRpc2Request &req )
{
    if ( req.isNull()) { return false; }

    ReqInfo req_info;
    if ( req.isIdStr()) {
        req_info = m_idstr_mm_map.take( req.idStr() );
    } else {
        req_info = m_id_mm_map.take( req.id());
    }

    return  req_info.first.isValid();
}

// ============================================================================
// handle the response
// ============================================================================
void   IcJsonRpc2RespHdePriv :: handleResp ( const IcJsonRpc2Response &resp )
{
    if ( resp.isNull()) { return; }

    // ------------------------------------------------------------------------
    // take the added disposable call
    // ------------------------------------------------------------------------
    ReqInfo req_info;
    if ( m_tgt != Q_NULLPTR && resp.isResult() ) {
        IcJsonRpc2Result rsl = resp.result();
        if ( rsl.isIdStr()) {
            req_info = m_idstr_mm_map.take( rsl.idStr());
        } else {
            req_info = m_id_mm_map.take( rsl.id());
        }
    } else if ( m_tgt != Q_NULLPTR && resp.isError() ) {
        IcJsonRpc2Error  err = resp.error();
        if ( err.isIdStr()) {
            req_info = m_idstr_mm_map.take( err.idStr());
        } else {
            req_info = m_id_mm_map.take( err.id());
        }
    } else { ; }

    // ------------------------------------------------------------------------
    // call handle function or emit unhandled
    // ------------------------------------------------------------------------
    if ( req_info.first.isValid()) {
        req_info.first.invoke (
            m_tgt, Qt::QueuedConnection,
            Q_ARG( const QxPack::IcJsonRpc2Request&,  req_info.second.first ),
            Q_ARG( const QxPack::IcJsonRpc2Response&, resp ),
            Q_ARG( const QVariant&, req_info.second.second )
        );
    } else {
        QMetaObject::invokeMethod(
            this, "unhandleResponse", Qt::QueuedConnection,
            Q_ARG( const QxPack::IcJsonRpc2Response&, resp )
        );
    }
}


// ////////////////////////////////////////////////////////////////////////////
// wrap API
// ////////////////////////////////////////////////////////////////////////////
// ============================================================================
// ctor
// ============================================================================
IcJsonRpc2RespHde :: IcJsonRpc2RespHde ( QObject *pa ) : QObject( pa )
{
    m_obj = qxpack_ic_new( IcJsonRpc2RespHdePriv );
    QObject::connect(
        T_PrivPtr( m_obj ), SIGNAL(unhandleResponse(const QxPack::IcJsonRpc2Response &)),
        this, SIGNAL(unhandledResponse(const QxPack::IcJsonRpc2Response&))
    );
}

// ============================================================================
// dtor
// ============================================================================
IcJsonRpc2RespHde :: ~IcJsonRpc2RespHde( )
{
    QObject::disconnect( T_PrivPtr( m_obj ), Q_NULLPTR, this, Q_NULLPTR );
    qxpack_ic_delete( m_obj, IcJsonRpc2RespHdePriv );
    m_obj = Q_NULLPTR;
}

// ============================================================================
// build the response handler information
// ============================================================================
void  IcJsonRpc2RespHde :: build ( QObject *tgt )
{ T_PrivPtr( m_obj )->build( tgt ); }

// ============================================================================
// clear all disposable call
// ============================================================================
void  IcJsonRpc2RespHde :: clear( )
{ T_PrivPtr( m_obj )->clear(); }

// ============================================================================
// add disposable call
// ============================================================================
bool  IcJsonRpc2RespHde :: addDispoCall (
    const IcJsonRpc2Request &req, const QString &sfx, const QVariant& opt
) { return T_PrivPtr( m_obj )->addDispoCall( req, sfx, opt ); }

// ============================================================================
// remove disposable call by request
// ============================================================================
bool  IcJsonRpc2RespHde :: rmvDispoCall( const IcJsonRpc2Request &req )
{ return T_PrivPtr( m_obj )->rmvDispoCall( req ); }

// ============================================================================
// slot: handle the response
// ============================================================================
void  IcJsonRpc2RespHde :: handleResponse( const QxPack::IcJsonRpc2Response &resp )
{ T_PrivPtr( m_obj )->handleResp( resp ); }

// ============================================================================
// slot: handle the response vector
// ============================================================================
void  IcJsonRpc2RespHde :: handleResponse( const QVector<IcJsonRpc2Response> &v_resp )
{
    QVector<IcJsonRpc2Response>::const_iterator c_itr = v_resp.constBegin();
    while ( c_itr != v_resp.constEnd()) {
        const IcJsonRpc2Response &resp = (*c_itr ++ );
        T_PrivPtr( m_obj )->handleResp( resp );
    }
}

}

#include "qxpack_ic_jsonrpc2_resphde.moc"
#endif
