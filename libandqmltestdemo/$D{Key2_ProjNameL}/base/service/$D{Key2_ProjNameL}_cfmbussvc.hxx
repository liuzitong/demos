#ifndef $D{Key2_ProjNameU}_CFMBUSSVC_HXX
#define $D{Key2_ProjNameU}_CFMBUSSVC_HXX

#include "$D{Key2_ProjNameL}/base/common/$D{Key2_ProjNameL}_def.h"
#include "qxpack/indcom/afw/qxpack_ic_confirmbus.hxx"

namespace $D{Key2_ProjNameN} {

// ////////////////////////////////////////////////////////////////////////////
//
//  confirm bus manager
//
// ////////////////////////////////////////////////////////////////////////////
class $D{Key2_ProjNameU}_API CfmBusSvc : public QxPack::IcConfirmBus {
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
