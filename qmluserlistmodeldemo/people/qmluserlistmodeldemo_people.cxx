#ifndef QMLUSERLISTMODELDEMO_PEOPLE_CXX
#define QMLUSERLISTMODELDEMO_PEOPLE_CXX

#include "qmluserlistmodeldemo_people.hxx"

// services list

// viewmodel list
#include "qmluserlistmodeldemo/people/viewmodel/qmluserlistmodeldemo_pp_peoplemgrvm.hxx"

namespace qmlUserListmodelDemo {

// ////////////////////////////////////////////////////////////////////////////
//  wrap API
// ////////////////////////////////////////////////////////////////////////////
// ============================================================================
// ctor
// ============================================================================
People :: People ( QObject *pa ) : QxPack::IcModMgrBase( pa )
{

}

// ============================================================================
// dtor
// ============================================================================
People :: ~People ( )
{

}

// ============================================================================
// register types
// ============================================================================
void  People :: registerTypes( QxPack::IcAppCtrlBase *app_base )
{
    // ------------------------------------------------------------------------
    // check param.
    // ------------------------------------------------------------------------
    if ( app_base == Q_NULLPTR ) { return; }
    QxPack::IcObjMgr *obj_mgr = app_base->objMgr();
    if ( obj_mgr  == Q_NULLPTR ) { return; }

    // ------------------------------------------------------------------------
    // register all public types of this module
    // ------------------------------------------------------------------------
    obj_mgr->registerType<qmlUserListmodelDemo::PpPeopleMgrVm>(
        QStringLiteral("qmlUserListmodelDemo::PpPeopleMgrVm"), Q_NULLPTR
    );
}


}

#endif
