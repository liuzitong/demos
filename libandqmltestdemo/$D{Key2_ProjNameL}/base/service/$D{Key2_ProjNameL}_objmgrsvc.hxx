#ifndef $D{Key2_ProjNameU}_OBJMGRSVC_HXX
#define $D{Key2_ProjNameU}_OBJMGRSVC_HXX

#include "$D{Key2_ProjNameL}/base/common/$D{Key2_ProjNameL}_def.h"
#include "qxpack/indcom/afw/qxpack_ic_objmgr.hxx"

namespace $D{Key2_ProjNameN} {

// ////////////////////////////////////////////////////////////////////////////
//
//  objects manager
//
// ////////////////////////////////////////////////////////////////////////////
class $D{Key2_ProjNameU}_API ObjMgrSvc : public QxPack::IcObjMgr {
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
