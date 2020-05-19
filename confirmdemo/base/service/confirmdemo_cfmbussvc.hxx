#ifndef CONFIRMDEMO_CFMBUSSVC_HXX
#define CONFIRMDEMO_CFMBUSSVC_HXX

#include "confirmdemo/base/common/confirmdemo_def.h"
#include "qxpack/indcom/afw/qxpack_ic_confirmbus.hxx"

namespace confirmDemo {

// ////////////////////////////////////////////////////////////////////////////
//
//  confirm bus manager
//
// ////////////////////////////////////////////////////////////////////////////
class CONFIRMDEMO_API CfmBusSvc : public QxPack::IcConfirmBus {
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
