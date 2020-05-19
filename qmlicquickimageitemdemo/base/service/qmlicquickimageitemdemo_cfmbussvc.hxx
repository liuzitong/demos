#ifndef QMLICQUICKIMAGEITEMDEMO_CFMBUSSVC_HXX
#define QMLICQUICKIMAGEITEMDEMO_CFMBUSSVC_HXX

#include "qmlicquickimageitemdemo/base/common/qmlicquickimageitemdemo_def.h"
#include "qxpack/indcom/afw/qxpack_ic_confirmbus.hxx"

namespace qmlIcquickimageitemDemo {

// ////////////////////////////////////////////////////////////////////////////
//
//  confirm bus manager
//
// ////////////////////////////////////////////////////////////////////////////
class QMLICQUICKIMAGEITEMDEMO_API CfmBusSvc : public QxPack::IcConfirmBus {
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
