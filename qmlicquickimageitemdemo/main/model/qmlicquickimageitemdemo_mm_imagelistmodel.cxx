#ifndef QMLICQUICKIMAGEITEMDEMO_MM_IMAGELISTMODEL_CXX
#define QMLICQUICKIMAGEITEMDEMO_MM_IMAGELISTMODEL_CXX

#include "qmlicquickimageitemdemo_mm_imagelistmodel.hxx"
#include <QFile>
#include <QImage>
#include <QList>

namespace qmlIcquickimageitemDemo {

// ////////////////////////////////////////////////////////////////////////////
// private object
// ////////////////////////////////////////////////////////////////////////////
#define T_PrivPtr( o )  qmlicquickimageitemdemo_objcast( MmImageListModelPriv*, o )
class QMLICQUICKIMAGEITEMDEMO_HIDDEN  MmImageListModelPriv {
private:
    QList<QImage>  m_im_list;
public :
    explicit MmImageListModelPriv ( );
    ~MmImageListModelPriv( ) { }

    inline QImage imageAt( int idx ) const
    { return  ( idx >= 0 && idx < m_im_list.size() ? m_im_list.at(idx) : QImage() );}

    inline  int    count ( ) const { return m_im_list.size(); }
};

// ============================================================================
// ctor
// ============================================================================
MmImageListModelPriv :: MmImageListModelPriv ( )
{
    m_im_list.push_back( QImage(":/qmlicquickimageitemdemo/main/view/birdfly4/im0.bmp").convertToFormat( QImage::Format_RGBA8888 ) );
    m_im_list.push_back( QImage(":/qmlicquickimageitemdemo/main/view/birdfly4/im1.bmp").convertToFormat( QImage::Format_RGBA8888 ) );
    m_im_list.push_back( QImage(":/qmlicquickimageitemdemo/main/view/birdfly4/im2.bmp").convertToFormat( QImage::Format_RGBA8888 ) );
    m_im_list.push_back( QImage(":/qmlicquickimageitemdemo/main/view/birdfly4/im3.bmp").convertToFormat( QImage::Format_RGBA8888 ) );
}

// ////////////////////////////////////////////////////////////////////////////
// wrap API
// ////////////////////////////////////////////////////////////////////////////
// ============================================================================
// ctor
// ============================================================================
MmImageListModel :: MmImageListModel ( QObject *pa ) : QObject( pa )
{ m_obj = new MmImageListModelPriv(); }

// ============================================================================
// dtor
// ============================================================================
MmImageListModel :: ~MmImageListModel( )
{ delete T_PrivPtr( m_obj );  }

// ============================================================================
// methods
// ============================================================================
int  MmImageListModel :: count( ) const
{ return T_PrivPtr( m_obj )->count(); }

QImage  MmImageListModel :: imageAt( int idx ) const
{ return T_PrivPtr( m_obj )->imageAt( idx ); }


}
#endif
