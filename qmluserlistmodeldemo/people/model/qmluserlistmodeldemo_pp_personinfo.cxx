#ifndef QMLUSERLISTMODELDEMO_PERSONINFO_CXX
#define QMLUSERLISTMODELDEMO_PERSONINFO_CXX

#include "qmluserlistmodeldemo_pp_personinfo.hxx"
#include "qxpack/indcom/common/qxpack_ic_pimplprivtemp.hpp" // [HINT] a template used to PImpl
namespace qmlUserListmodelDemo {

// ////////////////////////////////////////////////////////////////////////////
//
//                           PpPersonInfo
//
// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// PpPersonInfo private object
// ////////////////////////////////////////////////////////////////////////////
// [HINT] here we use PImplPrivTemp to generate a Copy On Write object
#define T_PidPriv( o )  qmluserlistmodeldemo_objcast( PpPersonInfoPriv*, o )
class QMLUSERLISTMODELDEMO_HIDDEN  PpPersonInfoPriv : public QxPack::IcPImplPrivTemp<PpPersonInfoPriv> {
private:
   QString  m_name, m_id; QDate m_dob;
public :
   PpPersonInfoPriv( );                             // [HINT] PImpl required default constructor
   PpPersonInfoPriv( const PpPersonInfoPriv & );  // [HINT] PImpl required copy ctor
   virtual ~PpPersonInfoPriv( ) Q_DECL_OVERRIDE {}

   inline QString  name( ) const { return m_name; }
   inline QString  id  ( ) const { return m_id;   }
   inline QDate    dob ( ) const { return m_dob;  }
   inline void     setName ( const QString &nm ) { m_name = nm; }
   inline void     setId   ( const QString &id ) { m_id   = id; }
   inline void     setDob  ( const QDate   &dt ) { m_dob  = dt; }
};

// ============================================================================
// ctor ( default )
// ============================================================================
PpPersonInfoPriv :: PpPersonInfoPriv ( )
{  }

// ============================================================================
// ctor ( copy )
// ============================================================================
PpPersonInfoPriv :: PpPersonInfoPriv ( const PpPersonInfoPriv &other )
{
    // [HINT] implement this for Copy On Write
    m_name = other.m_name; m_id = other.m_id; m_dob = other.m_dob;
}

// ////////////////////////////////////////////////////////////////////////////
// PpPersonInfo wrapper
// ////////////////////////////////////////////////////////////////////////////
PpPersonInfo :: PpPersonInfo ( )
{ m_obj = nullptr; }

PpPersonInfo :: PpPersonInfo ( const PpPersonInfo &other )
{
    // [HINT] use attach method for share other data
    m_obj = nullptr;  // must be init it as null
    PpPersonInfoPriv::attach( & m_obj, const_cast<void**>(&other.m_obj) );
}

PpPersonInfo &  PpPersonInfo :: operator = ( const PpPersonInfo &other )
{
    // [HINT] here directly use attach method, it will automatically drop old context
    // and attach to new context ( other.m_obj )
    PpPersonInfoPriv::attach( & m_obj, const_cast<void**>(&other.m_obj ) );
    return *this;
}

PpPersonInfo :: ~PpPersonInfo( )
{
    if ( m_obj != nullptr ) { // [HINT] directly drop it.
        PpPersonInfoPriv::attach( & m_obj, nullptr );
    }
}

QString   PpPersonInfo :: name( ) const
{ return ( m_obj != nullptr ? T_PidPriv( m_obj )->name() : QString() ); }

QString   PpPersonInfo :: id  ( ) const
{ return ( m_obj != nullptr ? T_PidPriv( m_obj )->id()   : QString() ); }

QDate     PpPersonInfo :: dob ( ) const
{ return ( m_obj != nullptr ? T_PidPriv( m_obj )->dob()  : QDate() ); }

// [HINT] instanceCow() will do a copy if shared with other object
void      PpPersonInfo :: setName ( const QString &nm )
{ PpPersonInfoPriv::instanceCow( & m_obj )->setName( nm ); }

void      PpPersonInfo :: setId   ( const QString &id )
{ PpPersonInfoPriv::instanceCow( & m_obj )->setId( id ); }

void      PpPersonInfo :: setDob  ( const QDate &dob )
{ PpPersonInfoPriv::instanceCow( & m_obj )->setDob( dob ); }













}

#endif
