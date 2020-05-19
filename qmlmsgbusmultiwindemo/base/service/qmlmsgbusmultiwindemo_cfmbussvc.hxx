#ifndef QMLMSGBUSMULTIWINDEMO_CFMBUSSVC_HXX
#define QMLMSGBUSMULTIWINDEMO_CFMBUSSVC_HXX

#include "qmlmsgbusmultiwindemo/base/common/qmlmsgbusmultiwindemo_def.h"
#include "qxpack/indcom/afw/qxpack_ic_confirmbus.hxx"

namespace qmlMsgbusMultiwinDemo {

// ////////////////////////////////////////////////////////////////////////////
//
//  confirm bus manager
//
// ////////////////////////////////////////////////////////////////////////////
class QMLMSGBUSMULTIWINDEMO_API CfmBusSvc : public QxPack::IcConfirmBus {
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
