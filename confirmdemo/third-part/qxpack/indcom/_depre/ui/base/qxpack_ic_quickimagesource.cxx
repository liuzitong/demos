#ifndef QXPACK_IC_QUICKIMAGESOURCE_CXX
#define QXPACK_IC_QUICKIMAGESOURCE_CXX

#include "qxpack_ic_quickimagesource.hxx"
#include <qDebug>
#include <QMutex>

namespace QxPack { static void  gDeletePrivObj( void *obj ); }
#define DeletePrivObj( o, t )      do{ QxPack::gDeletePrivObj(( t *) o ); }while(0)
#define QXPACK_IcPImplPrivTemp_new( t, ... )  ( new t( __VA_ARGS__ ))
#define QXPACK_IcPImplPrivTemp_delete DeletePrivObj
#include <qxpack/indcom/common/qxpack_ic_pimplprivtemp.hpp>

namespace QxPack {

// /////////////////////////////////////////////////
//  private object
// /////////////////////////////////////////////////
#define T_PrivPtr( o )  (( IcQuickImageSourcePriv *) o )
class QXPACK_IC_HIDDEN IcQuickImageSourcePriv :
    public QObject,
    public IcPImplPrivTemp< IcQuickImageSourcePriv >
{
    Q_OBJECT
private:
    QMutex  m_locker;
    QImage  m_image;
    IcQuickImageSource::FlagID  m_flag_id;
public :
    IcQuickImageSourcePriv ( ) { m_flag_id = IcQuickImageSource::FlagID_Normal; }
    IcQuickImageSourcePriv ( const IcQuickImageSourcePriv & )
    { qFatal("IcQuickImageSourcePriv::ctor(copy) did not support!"); }
    virtual ~IcQuickImageSourcePriv( ) { }
    inline bool  init( IcQuickImageSource::FlagID f )
    { m_flag_id = f; return true; }
    inline void  updateImage( const QImage &im ) { m_locker.lock(); m_image = im; m_locker.unlock(); emit imageChanged(); }
    inline void  pickImage( QImage &im )
    {
        m_locker.lock();
        im = m_image;
        if ( m_flag_id == IcQuickImageSource::FlagID_DeleteAfterPick )
        { m_image = QImage(); }
        m_locker.unlock();
    }

    Q_SIGNAL void imageChanged( );
};

// =====================================================
// delete the object
// =====================================================
static void  gDeletePrivObj( void *obj )
{
    if ( obj != Q_NULLPTR ) { T_PrivPtr( obj )->deleteLater(); }
}

// /////////////////////////////////////////////////
// API wrapper
// /////////////////////////////////////////////////
    IcQuickImageSource :: IcQuickImageSource ( ) { m_obj = nullptr; }
    IcQuickImageSource :: IcQuickImageSource ( FlagID f )
{
    m_obj = nullptr;
    IcQuickImageSourcePriv::buildIfNull( & m_obj );
    T_PrivPtr( m_obj )->init( f );
}

    IcQuickImageSource :: ~IcQuickImageSource ( )
{
    if ( m_obj != nullptr ) {
        QObject::disconnect( T_PrivPtr( m_obj ), 0, this, 0 );
        IcQuickImageSourcePriv::attach( & m_obj, nullptr );
    }
}

    IcQuickImageSource :: IcQuickImageSource( const IcQuickImageSource &other )
{
    m_obj = nullptr;
    IcQuickImageSourcePriv::attach( & m_obj, ( void **)& other.m_obj );
    QObject::connect( T_PrivPtr( m_obj ), SIGNAL(imageChanged()), this, SIGNAL(imageChanged()));
}

IcQuickImageSource &  IcQuickImageSource :: operator = ( const IcQuickImageSource &other )
{
    if ( m_obj != nullptr ) {
        QObject::disconnect( T_PrivPtr( m_obj ), 0 , this,  0 );
        IcQuickImageSourcePriv::attach( & m_obj, nullptr );
        m_obj = nullptr;
    }
    IcQuickImageSourcePriv::attach( & m_obj, ( void **) & other.m_obj );
    if ( m_obj != nullptr ) {
        QObject::connect( T_PrivPtr( m_obj ), SIGNAL(imageChanged()), this, SIGNAL(imageChanged()));
    }
    return *this;
}

bool  IcQuickImageSource :: isNull() const { return ( m_obj == nullptr ); }

void  IcQuickImageSource :: updateImage( const QImage &im )
{
    if ( m_obj != nullptr ) {
        T_PrivPtr( m_obj )->updateImage( im );
    }
}

void  IcQuickImageSource :: pickImage( QImage &im ) const
{
    if ( m_obj != nullptr ) {
        T_PrivPtr( m_obj )->pickImage( im );
    }
}



}

#include "qxpack_ic_quickimagesource.moc"
#endif
