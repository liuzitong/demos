#ifndef QMLCONFIRMBUSDEMO_OBJMGRSVC_HXX
#define QMLCONFIRMBUSDEMO_OBJMGRSVC_HXX

#include "qmlconfirmbusdemo/base/common/qmlconfirmbusdemo_def.h"
#include "qxpack/indcom/afw/qxpack_ic_objmgr.hxx"

namespace qmlconfirmbusdemo {

// ////////////////////////////////////////////////////////////////////////////
//
//  objects manager
//
// ////////////////////////////////////////////////////////////////////////////
class QMLCONFIRMBUSDEMO_API ObjMgrSvc : public QxPack::IcObjMgr {
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
