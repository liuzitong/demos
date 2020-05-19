// /////////////////////////////////////////////////////////////////////
/*!
  @file    qxpack_ic_jsonrpchttp
  @author  night wing
  @date    2018/09
  @brief   http request toolkit, jsonrpc2 http request accessor
  @version 0.1.0.0
*/
// /////////////////////////////////////////////////////////////////////
#ifndef QXPACK_IC_JSONRPCHTTP_HXX
#define QXPACK_IC_JSONRPCHTTP_HXX

/*!
  @addtogroup  QxPack
  @{
*/
/*!
  @addtogroup  indcom
  @{
*/
/*!
  @addtogroup  sys_Exported_Classes
  @{
*/

#include <QObject>
#include <QString>
#include <QByteArray>
#include <qxpack/indcom/common/qxpack_ic_def.h>

namespace QxPack {

// ////////////////////////////////////////////////////////////
/*!
 * @brief IcJsonRpcHttp
 * @details this class is used for send and recieve the \n
 *          json data from remote server.
 * @note  the HTTP header type is "application/json; charset=utf-8"
 */
// ////////////////////////////////////////////////////////////
class  IcJsonRpc2Request;
class  IcJsonRpc2Result;
class  IcJsonRpc2Error;
class QXPACK_IC_API IcJsonRpcHttp : public QObject  {
    Q_OBJECT
public:
    /*!
     * @brief  constuctor
     * @param [in] url, the target URL
     * @param [in] pa,  parent object
     */
    IcJsonRpcHttp( const QString &url, QObject *pa = Q_NULLPTR );

    virtual ~IcJsonRpcHttp( );

    /*!
     * @brief post the request to target URL
     * @param [in] req, the JsonRPC2 request object
     * @return true for post the json rpc data to remove server
     * @note  to get the result, see signal operationFinishedOK() \n
     *        and operationFinishedErr()
     * @warning do not post request again if previous request has not been completed. \n
     *        if needed, use multiply IcJsonHttp instead.
     */
    bool  post ( const QxPack::IcJsonRpc2Request &req );

    //! emited while remote server return a result
    Q_SIGNAL void  operationFinishedOK ( const QxPack::IcJsonRpc2Result & );

    //! emited while remote server return a error
    Q_SIGNAL void  operationFinishedErr( const QxPack::IcJsonRpc2Error  & );

    //! emited while network error
    Q_SIGNAL void  operationError   ( const QString & );

    //! emited while upload progress signal got from http component
    Q_SIGNAL void  uploadProgress  ( qint64, qint64 );

    //! emited while download progress signal got from http component
    Q_SIGNAL void  downloadProgress( qint64, qint64 );

private:
    void *m_obj;
    Q_DISABLE_COPY( IcJsonRpcHttp )
};

}

/*!
  @}
*/
/*!
  @}
*/
/*!
  @}
*/

#endif
