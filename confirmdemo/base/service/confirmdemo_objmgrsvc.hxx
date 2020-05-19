#ifndef CONFIRMDEMO_OBJMGRSVC_HXX
#define CONFIRMDEMO_OBJMGRSVC_HXX

#include "confirmdemo/base/common/confirmdemo_def.h"
#include "qxpack/indcom/afw/qxpack_ic_objmgr.hxx"

namespace confirmDemo {

// ////////////////////////////////////////////////////////////////////////////
//
//  objects manager
//
// ////////////////////////////////////////////////////////////////////////////
class CONFIRMDEMO_API ObjMgrSvc : public QxPack::IcObjMgr {
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
