#ifndef CORE_MSGBUSSVC_HXX
#define CORE_MSGBUSSVC_HXX

#include "core/base/common/core_def.h"
#include "qxpack/indcom/afw/qxpack_ic_msgbus.hxx"

namespace core {

// ////////////////////////////////////////////////////////////////////////////
//
//  message bus manager
//
// ////////////////////////////////////////////////////////////////////////////
class CORE_API MsgBusSvc : public QxPack::IcMsgBus {
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
