#ifndef $D{Key2_ProjNameU}_MSGBUSSVC_HXX
#define $D{Key2_ProjNameU}_MSGBUSSVC_HXX

#include "$D{Key2_ProjNameL}/base/common/$D{Key2_ProjNameL}_def.h"
#include "qxpack/indcom/afw/qxpack_ic_msgbus.hxx"

namespace $D{Key2_ProjNameN} {

// ////////////////////////////////////////////////////////////////////////////
//
//  message bus manager
//
// ////////////////////////////////////////////////////////////////////////////
class $D{Key2_ProjNameU}_API MsgBusSvc : public QxPack::IcMsgBus {
    Q_OBJECT
private:
    explicit MsgBusSvc( QObject *pa = Q_NULLPTR );
    virtual ~MsgBusSvc( );
public :
    static MsgBusSvc*  getInstance ( );
    static void        freeInstance( );
};

}

#endif
