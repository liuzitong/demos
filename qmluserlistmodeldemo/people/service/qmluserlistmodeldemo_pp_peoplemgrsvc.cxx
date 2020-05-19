#ifndef QMLUSERLISTMODELDEMO_PP_PEOPLEMGRSVC_CXX
#define QMLUSERLISTMODELDEMO_PP_PEOPLEMGRSVC_CXX

#include "qmluserlistmodeldemo_pp_peoplemgrsvc.hxx"
#include "qmluserlistmodeldemo/base/common/qmluserlistmodeldemo_memcntr.hxx"
#include "qmluserlistmodeldemo/people/model/qmluserlistmodeldemo_pp_peoplelistmodel.hxx"

namespace qmlUserListmodelDemo {

// ////////////////////////////////////////////////////////////////////////////
// private object
// ////////////////////////////////////////////////////////////////////////////
#define T_PrivPtr( o )  qmluserlistmodeldemo_objcast( PpPeopleMgrSvcPriv*, o )
class QMLUSERLISTMODELDEMO_HIDDEN  PpPeopleMgrSvcPriv {
private:
    PpPeopleMgrSvc    *m_parent;
    PpPeopleListModel  m_people_list;
public :
    explicit PpPeopleMgrSvcPriv ( PpPeopleMgrSvc *pa );
    ~PpPeopleMgrSvcPriv( ) { }

    inline QObject*  peopleListObj ( ) { return & m_people_list; }

    void  addPerson     ( const QJsonObject & );
    void  randGenPeople ( ) { m_people_list.randomGen(); }
    void  clearPeople   ( ) { m_people_list.clear(); }
};

// ============================================================================
// ctor
// ============================================================================
PpPeopleMgrSvcPriv :: PpPeopleMgrSvcPriv ( PpPeopleMgrSvc *pa ) : m_people_list( Q_NULLPTR )
{
    m_parent = pa;
}

// ============================================================================
// add new person
// ============================================================================
void  PpPeopleMgrSvcPriv :: addPerson(const QJsonObject &jo )
{
    PpPersonInfo pi;
    pi.setName( jo.value("name").toString());
    pi.setId  ( jo.value("id").toString());
    m_people_list.addPerson( pi );
}


// ////////////////////////////////////////////////////////////////////////////
// wrap API
// ////////////////////////////////////////////////////////////////////////////
// ============================================================================
// ctor
// ============================================================================
PpPeopleMgrSvc :: PpPeopleMgrSvc( const QVariantList & )
{
    m_obj = qmluserlistmodeldemo_new( PpPeopleMgrSvcPriv, this );
}

// ============================================================================
// dtor
// ============================================================================
PpPeopleMgrSvc :: ~PpPeopleMgrSvc ( )
{
    qmluserlistmodeldemo_delete( m_obj, PpPeopleMgrSvcPriv );
}

// ============================================================================
// return the people list
// ============================================================================
QObject*    PpPeopleMgrSvc :: peopleListObj( )
{  return T_PrivPtr( m_obj )->peopleListObj(); }

// ============================================================================
// add new person information
// ============================================================================
void        PpPeopleMgrSvc :: addPerson ( const QJsonObject &jo )
{ T_PrivPtr( m_obj )->addPerson( jo ); }

// ============================================================================
// clear people in list
// ============================================================================
void        PpPeopleMgrSvc :: clearPeople( )
{ T_PrivPtr( m_obj )->clearPeople(); }

// ============================================================================
// randomize generate the people info.
// ============================================================================
void        PpPeopleMgrSvc :: randGenPeople( )
{ T_PrivPtr( m_obj )->randGenPeople(); }





}

#endif
