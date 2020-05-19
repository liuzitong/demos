// ============================================================================
// author: night wing
// date stamp: 20190523 1638
// ============================================================================
#ifndef QXPACK_IC_JSONRPC2CMDBASE_CXX
#define QXPACK_IC_JSONRPC2CMDBASE_CXX

#include "qxpack_ic_jsonrpc2cmdbase.hxx"
#include "qxpack/indcom/common/qxpack_ic_memcntr.hxx"

namespace QxPack {

// ////////////////////////////////////////////////////////////////////////////
//                        wrapper API
// ////////////////////////////////////////////////////////////////////////////
// ============================================================================
// ctor
// ============================================================================
IcJsonRpc2CmdBase :: IcJsonRpc2CmdBase ( QObject *pa ) : QObject(pa)
{
    m_obj = Q_NULLPTR;
}

// ============================================================================
// dtor
// ============================================================================
IcJsonRpc2CmdBase :: ~IcJsonRpc2CmdBase ( )
{
}

// ============================================================================
// this function should return the supported methods
// ============================================================================
QVector<QString> IcJsonRpc2CmdBase :: supportedMethods() const
{ return QVector<QString>(); }

// ============================================================================
// this function handle all request
// ============================================================================
IcJsonRpc2Response IcJsonRpc2CmdBase :: dispatchRequest (
    const IcJsonRpc2Request &req, const QVariantList &var_list
) {
  Q_UNUSED( var_list );
  return IcJsonRpc2Error::makeError(req,IcJsonRpc2Error::Code_MethodNotFound);
}


}

#endif
