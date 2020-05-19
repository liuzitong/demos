#ifndef QMLUSERLISTMODELDEMO_CFMBUSSVC_HXX
#define QMLUSERLISTMODELDEMO_CFMBUSSVC_HXX

#include "qmluserlistmodeldemo/base/common/qmluserlistmodeldemo_def.h"
#include "qxpack/indcom/afw/qxpack_ic_confirmbus.hxx"

namespace qmlUserListmodelDemo {

// ////////////////////////////////////////////////////////////////////////////
//
//  confirm bus manager
//
// ////////////////////////////////////////////////////////////////////////////
class QMLUSERLISTMODELDEMO_API CfmBusSvc : public QxPack::IcConfirmBus {
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
