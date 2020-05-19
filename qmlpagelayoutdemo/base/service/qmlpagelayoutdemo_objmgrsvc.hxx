#ifndef QMLPAGELAYOUTDEMO_OBJMGRSVC_HXX
#define QMLPAGELAYOUTDEMO_OBJMGRSVC_HXX

#include "qmlpagelayoutdemo/base/common/qmlpagelayoutdemo_def.h"
#include "qxpack/indcom/afw/qxpack_ic_objmgr.hxx"

namespace qmlPagelayoutDemo {

// ////////////////////////////////////////////////////////////////////////////
//
//  objects manager
//
// ////////////////////////////////////////////////////////////////////////////
class QMLPAGELAYOUTDEMO_API ObjMgrSvc : public QxPack::IcObjMgr {
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
