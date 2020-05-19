#ifndef QXPACK_IC_JSONRPC2_SVR_HXX
#define QXPACK_IC_JSONRPC2_SVR_HXX

#include "qxpack/indcom/common/qxpack_ic_def.h"
#include "qxpack/indcom/sys/qxpack_ic_jsonrpc2.hxx"
#include "qxpack/indcom/net/qxpack_ic_datatrans_buff.hxx"
#include "qxpack/indcom/net/qxpack_ic_datatrans_svr.hxx"

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QVector>

namespace QxPack {

// ////////////////////////////////////////////////////////////////////////////
/*!
 * @brief the jsonrpc2 request handler
 */
// ////////////////////////////////////////////////////////////////////////////
class QXPACK_IC_API IcJsonRpc2Handler : public QObject {
    Q_OBJECT
public:
    //! dtor
    virtual ~IcJsonRpc2Handler( ) Q_DECL_OVERRIDE;

    //! check if it is work
    //! @note user should check this again after taked from server.\n
    bool  isWorking( ) const;

    //! check the pending request
    bool  hasPendingRequest ( ) const;

    //! take next pending request from inner list
    //! @details user should handle the request one by one. \n
    //! see below steps: \n
    //! 1) take request by using takeNextPendingRequest() \n
    //! 2) handle it \n
    //! 3) write response by using post(), the notice request should post \n
    //!    a empty response object as answere.
    bool  takeNextPendingRequest ( QxPack::IcJsonRpc2Request &req );

    // check current batch size
    // a batch is in a vector, JsonRpc2 request is  more than one,
    // we can add a method that can identify remain requests in the vector,
    // because handle the request is order by order, we can call takeNextPendingRequest() repeatly
    // for many times to get all requests in a batch request.
    //int   pendingVectReqSize ( int &remain ) const;

    //! write response to remote
    //! @note if current existed remain request, will trigger an newRequest() signal.
    bool  post ( const QxPack::IcJsonRpc2Response &resp );

    //! write the notice to remote directly
    //! @note notice has no response.
    //! @note this notice will directly passed to client regardless of 'request and response' mechanism
    bool  notice ( const QxPack::IcJsonRpc2Request &req );

    //! this handler is finished, means remote closed or socket error
    //! @note user can check isWork() to determine object is still working or not. \n
    //! If object is not working, user can delete it.
    Q_SIGNAL void  finished();

    /*!
     * @brief remote send a or a batch request(s)
     * @details The handler will emit the request in a queued manner.\n
     *  user do not have to care about the request is a single object or an array.\n
     */
    Q_SIGNAL void  newRequest();

    //! if remote connected
    Q_SIGNAL void  hostConnected( );

    //! if post package has been sent
    Q_SIGNAL void  dataSent( );

    //! network error string
    Q_SIGNAL void  errorMessage ( int, const QString &err_msg );

protected:
    //! ctor
    explicit IcJsonRpc2Handler(
        const QString &rs_type,
        qintptr sck_dscr, IcDataTransFactory fact, void *ctxt,
        QObject *pa = Q_NULLPTR
    );

private:
    void *m_obj;
    Q_DISABLE_COPY( IcJsonRpc2Handler )
    friend class IcJsonRpc2Svr;
    friend class IcJsonRpc2SvrPriv;
};


// ////////////////////////////////////////////////////////////////////////////
/*!
 * @brief the server of JsonRPC2
 */
// ////////////////////////////////////////////////////////////////////////////
class QXPACK_IC_API  IcJsonRpc2Svr : public IcDataTransSvr {
    Q_OBJECT
public:
    //! ctor
    //! @note after construct the object, use should use startServer() to \n
    //! start listening.
    explicit IcJsonRpc2Svr (
        const QString &rs_type, const QString &host,  quint16 port,
        IcDataTransFactory fact = Q_NULLPTR, void *fact_ctxt = Q_NULLPTR,
        QObject *pa = Q_NULLPTR
    );

    //! dtor
    virtual ~IcJsonRpc2Svr( ) Q_DECL_OVERRIDE;

protected:
    //! override the handler creator
    virtual QObject*  createHandler (
        const QString &rs_type, qintptr dscr, IcDataTransFactory fact, void *fact_ctxt
    );
private:
    void   *m_obj;
    Q_DISABLE_COPY( IcJsonRpc2Svr )
};




}

#endif
