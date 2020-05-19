#ifndef QMLBASE_OBJMGRSVC_HXX
#define QMLBASE_OBJMGRSVC_HXX

#include "qmlbase/base/common/qmlbase_def.h"
#include "qxpack/indcom/afw/qxpack_ic_objmgr.hxx"

namespace qmlbase {

// ////////////////////////////////////////////////////////////////////////////
//
//  objects manager
//
// ////////////////////////////////////////////////////////////////////////////
class QMLBASE_API ObjMgrSvc : public QxPack::IcObjMgr {
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
