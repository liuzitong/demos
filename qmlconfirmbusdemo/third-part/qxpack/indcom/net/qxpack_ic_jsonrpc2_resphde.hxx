#ifndef QXPACK_IC_JSONRPC2_RESPHDE_HXX
#define QXPACK_IC_JSONRPC2_RESPHDE_HXX

#include "qxpack/indcom/common/qxpack_ic_def.h"
#include "qxpack/indcom/sys/qxpack_ic_jsonrpc2.hxx"

#include <QObject>
#include <QVariant>

namespace QxPack {

// ////////////////////////////////////////////////////////////////////////////
/*!
 * @brief JsonRpc2 response handler, used to automatically call response handler
 */
// ////////////////////////////////////////////////////////////////////////////
class QXPACK_IC_API  IcJsonRpc2RespHde : public QObject {
    Q_OBJECT
public:
    //! ctor
    explicit IcJsonRpc2RespHde ( QObject *pa = Q_NULLPTR );

    //! dtor
    virtual ~IcJsonRpc2RespHde ( ) Q_DECL_OVERRIDE;

    //! build the handler information by spec. handle hde.
    //! @details \n
    //!    All function that can be invoked by IcJsonRpc2RespHde is declared like below: \n
    //!    Q_INVOKABLE  void  onResp_XXXXXX(
    //!       const QxPack::IcJsonRpc2Request, const QxPack::IcJsonRpc2Response&, const QVariant&
    //!    ); \n
    //!    the 'XXXXXX' is a user named string.
    //! @warning Called only once
    void  build( QObject *hde_obj );

    //! clear all appended handler
    void  clear( );

    //! add disposable call
    //! @param req   [in] the request that will be sent to server
    //! @param func  [in] the response handler function ( user name string )
    //! @param opt   [opt_in]  optional user data, will be passed back
    //! @return true for addeded
    //! @warning  after invoked, the call will be removed automatically.
    //! @warning  the request should use an unique id value !
    bool  addDispoCall ( const IcJsonRpc2Request &req, const QString &func, const QVariant& opt=QVariant());

    //! remove a call ref. by request
    //! @return true for removed.
    bool  rmvDispoCall ( const IcJsonRpc2Request &req );

    //! handle the response recieved.
    //! @note user can connect this slot to handle every response. \n
    //! @warning To handle the response, use must call addDispoCall() \n
    //!   before post the request. @see IcJsonRpc2Request::addDispoCall()
    Q_SLOT   void  handleResponse( const QxPack::IcJsonRpc2Response & );

    //! override
    Q_SLOT   void  handleResponse( const QVector<QxPack::IcJsonRpc2Response> &);

    //! emit if exist a unhandled response ( means not found any call )
    Q_SIGNAL void  unhandledResponse( const QxPack::IcJsonRpc2Response & );

private:
    void *m_obj;
    Q_DISABLE_COPY( IcJsonRpc2RespHde )
};


}

#endif
