#ifndef QXPACK_IC_JSONRPC2_CLI_HXX
#define QXPACK_IC_JSONRPC2_CLI_HXX

#include "qxpack/indcom/common/qxpack_ic_def.h"
#include "qxpack/indcom/sys/qxpack_ic_jsonrpc2.hxx"
#include "qxpack/indcom/net/qxpack_ic_datatrans_buff.hxx"
#include <QObject>
#include <QString>
#include <QVector>
#include <QStringList>

namespace QxPack {

// ////////////////////////////////////////////////////////////////////////////
/*!
 * @brief the Json Rpc2 server client
 */
// ////////////////////////////////////////////////////////////////////////////
class QXPACK_IC_API  IcJsonRpc2Cli : public QObject {
    Q_OBJECT
public:
    //! ctor
    explicit IcJsonRpc2Cli (
        const QString &rs_type, IcDataTransFactory fact = Q_NULLPTR, void *ctxt = Q_NULLPTR,
        QObject *pa = Q_NULLPTR
    );

    //! dtor
    virtual ~IcJsonRpc2Cli ( ) Q_DECL_OVERRIDE;

    //! check it is work
    bool isWorking( ) const;

    //! @note if connected, the hostConnected() signal is emitting
    //! @note to disconnect from host, directly delete this object instead use
    //! disconnectFromHost()
    void connectToHost ( const QString &, quint16 port );

    //! added by nw (20190724-1052)
    //! disconnect from the host
    //! @note after this, this object is not working.
    void disconnectFromHost ( );

    //! check if exist pending response
    bool hasPendingResponse ( );

    //! take first response
    bool takeNextPendingResponse( QVector<QxPack::IcJsonRpc2Response> & );

    //! check if exist pending notice
    bool hasPendingNotice( );

    //! take first notice
    bool takeNextPendingNotice( QxPack::IcJsonRpc2Request & );

    /*!
     * @brief post an array request to do
     * @param vr  [in] the JsonRpc2 response vector
     */
    bool  post( const QVector<QxPack::IcJsonRpc2Request> &vr );

    /*!
     * @brief post a single request to do
     */
    bool  post( QxPack::IcJsonRpc2Request &req );

    //! a new response arrived
    Q_SIGNAL void  newResponse( );

    //! a new notice arrived
    Q_SIGNAL void  newNotice( );

    //! if remote connected
    Q_SIGNAL void  hostConnected( );

    //! if remote closed socket, or network error
    Q_SIGNAL void  finished( );

    //! check if package has been sent
    Q_SIGNAL void  dataSent( );

    //! if raised error
    Q_SIGNAL void  errorMessage( int err, const QString &err_msg );

    // all rs type list
    static QStringList avaliableRsTypeList( );

private:
    void *m_obj;
    Q_DISABLE_COPY( IcJsonRpc2Cli )
};

}

#endif
