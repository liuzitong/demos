#ifndef QMLUSERLISTMODELDEMO_OBJMGRSVC_HXX
#define QMLUSERLISTMODELDEMO_OBJMGRSVC_HXX

#include "qmluserlistmodeldemo/base/common/qmluserlistmodeldemo_def.h"
#include "qxpack/indcom/afw/qxpack_ic_objmgr.hxx"

namespace qmlUserListmodelDemo {

// ////////////////////////////////////////////////////////////////////////////
//
//  objects manager
//
// ////////////////////////////////////////////////////////////////////////////
class QMLUSERLISTMODELDEMO_API ObjMgrSvc : public QxPack::IcObjMgr {
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
