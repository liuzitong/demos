#ifndef QMLBASE_CFMBUSSVC_HXX
#define QMLBASE_CFMBUSSVC_HXX

#include "qmlbase/base/common/qmlbase_def.h"
#include "qxpack/indcom/afw/qxpack_ic_confirmbus.hxx"

namespace qmlbase {

// ////////////////////////////////////////////////////////////////////////////
//
//  confirm bus manager
//
// ////////////////////////////////////////////////////////////////////////////
class QMLBASE_API CfmBusSvc : public QxPack::IcConfirmBus {
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
