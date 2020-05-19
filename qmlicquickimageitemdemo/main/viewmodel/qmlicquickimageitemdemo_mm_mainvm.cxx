#ifndef QMLICQUICKIMAGEITEMDEMO_MM_MAINVM_CXX
#define QMLICQUICKIMAGEITEMDEMO_MM_MAINVM_CXX

#include "qmlicquickimageitemdemo/base/common/qmlicquickimageitemdemo_guns.hxx"
#include "qmlicquickimageitemdemo/base/common/qmlicquickimageitemdemo_memcntr.hxx"
#include "qmlicquickimageitemdemo_mm_mainvm.hxx"
#include "qxpack/indcom/afw/qxpack_ic_appctrlbase.hxx"

#include <QMetaObject>

// [HINT] add needed data model here
#include "qmlicquickimageitemdemo/main/model/qmlicquickimageitemdemo_mm_imagelistmodel.hxx"
#include "qxpack/indcom/ui_qml_base/qxpack_ic_quickimagedata.hxx"

namespace qmlIcquickimageitemDemo {

// ////////////////////////////////////////////////////////////////////////////
// private object
// ////////////////////////////////////////////////////////////////////////////
#define T_PrivPtr( o )  qmlicquickimageitemdemo_objcast( MmMainVmPriv*, o )
class QMLICQUICKIMAGEITEMDEMO_HIDDEN  MmMainVmPriv : public QObject {
    Q_OBJECT
private:
    MmMainVm *m_parent;
    MmImageListModel     m_im_list_model;
    QxPack::IcQuickImageData  m_im_data_src;
    int                  m_curr_im_idx;
protected:
    Q_SLOT  void  onCurrImageIndexChanged( );

public :
    explicit MmMainVmPriv( MmMainVm *pa );
    virtual ~MmMainVmPriv( ) Q_DECL_OVERRIDE;
    inline QObject *  imageDataSource( ) { return & m_im_data_src; }
    inline int        imageListCount( )  { return m_im_list_model.count(); }
    inline int        currImageIndex( )  { return m_curr_im_idx; }
           void       setCurrImageIndex( int );
    Q_SIGNAL  void    currImageIndexChanged( );
};

// ============================================================================
//  ctor
// ============================================================================
MmMainVmPriv :: MmMainVmPriv ( MmMainVm *pa ) :
    m_im_data_src( QxPack::IcQuickImageData::UseMode_Normal )
{
    m_parent = pa;
    m_curr_im_idx = -1;
    this->setCurrImageIndex( 0 );
    QObject::connect( this, SIGNAL(currImageIndexChanged()), this, SLOT(onCurrImageIndexChanged()));
}

// ============================================================================
//  dtor
// ============================================================================
MmMainVmPriv :: ~MmMainVmPriv ( )
{

}

// ============================================================================
// set the current image index
// ============================================================================
void  MmMainVmPriv :: setCurrImageIndex( int i )
{
    if ( i >= 0 && i < m_im_list_model.count()) {
        if ( i != m_curr_im_idx ) {
            m_curr_im_idx = i;
            QMetaObject::invokeMethod( this, "currImageIndexChanged", Qt::QueuedConnection );
        }
    }
}

// ============================================================================
// slot: handle the image changed ...
// ============================================================================
void  MmMainVmPriv :: onCurrImageIndexChanged()
{
    // update the image data source...
    if ( m_curr_im_idx < m_im_list_model.count() ) {
        QxPack::IcImageData im_data;
        im_data.setImage( m_im_list_model.imageAt(m_curr_im_idx));
        m_im_data_src.setImageData( im_data );
    }
}


// ////////////////////////////////////////////////////////////////////////////
// wrap API
// ////////////////////////////////////////////////////////////////////////////
// ============================================================================
// ctor
// ============================================================================
MmMainVm :: MmMainVm ( const QVariantList & )
{
    m_obj = qmlicquickimageitemdemo_new( MmMainVmPriv, this );
    QObject::connect (
        T_PrivPtr( m_obj ), SIGNAL(currImageIndexChanged()), this, SIGNAL(currImageIndexChanged())
    );
}

// ============================================================================
// dtor
// ============================================================================
MmMainVm :: ~MmMainVm ( )
{
    QObject::disconnect( T_PrivPtr( m_obj ), Q_NULLPTR, this, Q_NULLPTR );
    qmlicquickimageitemdemo_delete( m_obj, MmMainVmPriv );
}

// ============================================================================
// list count of the image property
// ============================================================================
int  MmMainVm :: imageListCount( ) const
{
    return T_PrivPtr( m_obj )->imageListCount();
}

// ============================================================================
// current image index property
// ============================================================================
qreal   MmMainVm :: currImageIndex ( ) const
{ return static_cast<qreal>(T_PrivPtr( m_obj )->currImageIndex()); }

void  MmMainVm :: setCurrImageIndex( qreal i)
{ T_PrivPtr( m_obj )->setCurrImageIndex( int(i) );}

// ============================================================================
// the image data source
// ============================================================================
QObject*  MmMainVm :: imageDataSource( ) const
{ return T_PrivPtr( m_obj )->imageDataSource(); }

}

#include "qmlicquickimageitemdemo_mm_mainvm.moc"
#endif
