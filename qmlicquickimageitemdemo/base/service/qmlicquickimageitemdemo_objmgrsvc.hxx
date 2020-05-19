#ifndef QMLICQUICKIMAGEITEMDEMO_OBJMGRSVC_HXX
#define QMLICQUICKIMAGEITEMDEMO_OBJMGRSVC_HXX

#include "qmlicquickimageitemdemo/base/common/qmlicquickimageitemdemo_def.h"
#include "qxpack/indcom/afw/qxpack_ic_objmgr.hxx"

namespace qmlIcquickimageitemDemo {

// ////////////////////////////////////////////////////////////////////////////
//
//  objects manager
//
// ////////////////////////////////////////////////////////////////////////////
class QMLICQUICKIMAGEITEMDEMO_API ObjMgrSvc : public QxPack::IcObjMgr {
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
