#ifndef QMLMSGBUSMULTIWINDEMO_OBJMGRSVC_HXX
#define QMLMSGBUSMULTIWINDEMO_OBJMGRSVC_HXX

#include "qmlmsgbusmultiwindemo/base/common/qmlmsgbusmultiwindemo_def.h"
#include "qxpack/indcom/afw/qxpack_ic_objmgr.hxx"

namespace qmlMsgbusMultiwinDemo {

// ////////////////////////////////////////////////////////////////////////////
//
//  objects manager
//
// ////////////////////////////////////////////////////////////////////////////
class QMLMSGBUSMULTIWINDEMO_API ObjMgrSvc : public QxPack::IcObjMgr {
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
