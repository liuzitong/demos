#ifndef QMLCFMBUSMULTICFMDEMO_CFMBUSSVC_HXX
#define QMLCFMBUSMULTICFMDEMO_CFMBUSSVC_HXX

#include "qmlcfmbusmulticfmdemo/base/common/qmlcfmbusmulticfmdemo_def.h"
#include "qxpack/indcom/afw/qxpack_ic_confirmbus.hxx"

namespace qmlCfmBusMulticfmDemo {

// ////////////////////////////////////////////////////////////////////////////
//
//  confirm bus manager
//
// ////////////////////////////////////////////////////////////////////////////
class QMLCFMBUSMULTICFMDEMO_API CfmBusSvc : public QxPack::IcConfirmBus {
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
