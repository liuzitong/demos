#ifndef QMLCFMBUSMULTICFMDEMO_OBJMGRSVC_HXX
#define QMLCFMBUSMULTICFMDEMO_OBJMGRSVC_HXX

#include "qmlcfmbusmulticfmdemo/base/common/qmlcfmbusmulticfmdemo_def.h"
#include "qxpack/indcom/afw/qxpack_ic_objmgr.hxx"

namespace qmlCfmBusMulticfmDemo {

// ////////////////////////////////////////////////////////////////////////////
//
//  objects manager
//
// ////////////////////////////////////////////////////////////////////////////
class QMLCFMBUSMULTICFMDEMO_API ObjMgrSvc : public QxPack::IcObjMgr {
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
