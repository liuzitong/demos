#ifndef QXPACK_IC_JSONRPC2CMDBASE_HXX
#define QXPACK_IC_JSONRPC2CMDBASE_HXX

#include "qxpack/indcom/common/qxpack_ic_def.h"
#include "qxpack/indcom/sys/qxpack_ic_jsonrpc2.hxx"
#include <QObject>
#include <QVector>
#include <QVariantList>

namespace QxPack {

// ////////////////////////////////////////////////////////////////////////////
/*!
 * @brief  the command base is used for execute JsonRpc2 package
 */
// ////////////////////////////////////////////////////////////////////////////
class QXPACK_IC_API  IcJsonRpc2CmdBase : public QObject {
    Q_OBJECT
public:
    //! ctor
    explicit IcJsonRpc2CmdBase ( QObject *pa = Q_NULLPTR );

    //! dtor
    virtual ~IcJsonRpc2CmdBase( ) Q_DECL_OVERRIDE;

    //! return the supported methods
    Q_INVOKABLE virtual QVector<QString>   supportedMethods( ) const;

    //! dispatch the jsonrpc2 requests
    Q_INVOKABLE virtual IcJsonRpc2Response dispatchRequest(
        const IcJsonRpc2Request &req, const QVariantList & = QVariantList()
    );

private:
   void *m_obj;
   Q_DISABLE_COPY( IcJsonRpc2CmdBase )
};


}

#endif
