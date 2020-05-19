#ifndef CORE_OBJMGRSVC_HXX
#define CORE_OBJMGRSVC_HXX

#include "core/base/common/core_def.h"
#include "qxpack/indcom/afw/qxpack_ic_objmgr.hxx"

namespace core {

// ////////////////////////////////////////////////////////////////////////////
//
//  objects manager
//
// ////////////////////////////////////////////////////////////////////////////
class CORE_API ObjMgrSvc : public QxPack::IcObjMgr {
    Q_OBJECT
private:
    explicit ObjMgrSvc( QObject *pa = Q_NULLPTR );
    virtual ~ObjMgrSvc( );
public :
    static ObjMgrSvc *  getInstance ( );
    static void         freeInstance( );
};

}

#endif
