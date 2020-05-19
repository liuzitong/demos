#ifndef QMLUSERLISTMODELDEMO_PP_PEOPLEMGRVM_CXX
#define QMLUSERLISTMODELDEMO_PP_PEOPLEMGRVM_CXX

#include "qmluserlistmodeldemo/base/common/qmluserlistmodeldemo_guns.hxx"
#include "qmluserlistmodeldemo/base/common/qmluserlistmodeldemo_memcntr.hxx"

#include "qmluserlistmodeldemo_pp_peoplemgrvm.hxx"
#include "qmluserlistmodeldemo/people/service/qmluserlistmodeldemo_pp_peoplemgrsvc.hxx"

#include "qxpack/indcom/afw/qxpack_ic_appctrlbase.hxx"


namespace qmlUserListmodelDemo {

// ////////////////////////////////////////////////////////////////////////////
// private object
// ////////////////////////////////////////////////////////////////////////////
#define T_PrivPtr( o )  qmluserlistmodeldemo_objcast( PpPeopleMgrVmPriv*, o )
class QMLUSERLISTMODELDEMO_HIDDEN  PpPeopleMgrVmPriv : public QObject {
    Q_OBJECT
private:
    PpPeopleMgrSvc *m_people_mgr_svc;

    // here to add other service, like permission...
    // For example: who has right to add person ....

public :
    explicit PpPeopleMgrVmPriv( );
    virtual ~PpPeopleMgrVmPriv( ) Q_DECL_OVERRIDE;
    inline QObject*  peopleListObj( ) { return m_people_mgr_svc->peopleListObj(); }

    inline void      addPerson ( const QJsonObject &jo ) { m_people_mgr_svc->addPerson( jo ); }
    inline void      randGenPeople( ) { m_people_mgr_svc->randGenPeople(); }
    inline void      clearPeople( )   { m_people_mgr_svc->clearPeople();   }
};

// ============================================================================
//  ctor
// ============================================================================
PpPeopleMgrVmPriv :: PpPeopleMgrVmPriv ( )
{
    m_people_mgr_svc = qmluserlistmodeldemo_new( PpPeopleMgrSvc, QVariantList() );
}

// ============================================================================
//  dtor
// ============================================================================
PpPeopleMgrVmPriv :: ~PpPeopleMgrVmPriv ( )
{
    qmluserlistmodeldemo_delete( m_people_mgr_svc, PpPeopleMgrSvc );
}


// ////////////////////////////////////////////////////////////////////////////
// wrap API
// ////////////////////////////////////////////////////////////////////////////
// ============================================================================
// ctor
// ============================================================================
PpPeopleMgrVm :: PpPeopleMgrVm ( const QVariantList & )
{
    m_obj = qmluserlistmodeldemo_new( PpPeopleMgrVmPriv );
}

// ============================================================================
// dtor
// ============================================================================
PpPeopleMgrVm :: ~PpPeopleMgrVm ( )
{
    qmluserlistmodeldemo_delete( m_obj, PpPeopleMgrVmPriv );
}

// ============================================================================
// property list
// ============================================================================
QObject *   PpPeopleMgrVm :: peopleListObj() const
{ return T_PrivPtr( m_obj )->peopleListObj(); }

// ============================================================================
// method wrap
// ============================================================================
void  PpPeopleMgrVm :: addPerson( const QJsonObject &jo )
{ T_PrivPtr( m_obj )->addPerson( jo );  }

void  PpPeopleMgrVm :: clearPeople( )
{ T_PrivPtr( m_obj )->clearPeople(); }

void  PpPeopleMgrVm :: randGenPeople( )
{ T_PrivPtr( m_obj )->randGenPeople(); }


}
#include "qmluserlistmodeldemo_pp_peoplemgrvm.moc"
#endif
