#ifndef CORE_CFMBUSSVC_HXX
#define CORE_CFMBUSSVC_HXX

#include "core/base/common/core_def.h"
#include "qxpack/indcom/afw/qxpack_ic_confirmbus.hxx"

namespace core {

// ////////////////////////////////////////////////////////////////////////////
//
//  confirm bus manager
//
// ////////////////////////////////////////////////////////////////////////////
class CORE_API CfmBusSvc : public QxPack::IcConfirmBus {
    Q_OBJECT
private:
    explicit CfmBusSvc( QObject *pa = Q_NULLPTR );
    virtual ~CfmBusSvc( );
public :
    static CfmBusSvc*  getInstance ( );
    static void        freeInstance( );
};

}

#endif
