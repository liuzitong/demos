#ifndef  _QXPACK_IC_QUICKIMAGEITEM_CXX
#define  _QXPACK_IC_QUICKIMAGEITEM_CXX

#include "qxpack_ic_quickimageitem.hxx"
#include <qxpack/indcom/common/qxpack_ic_memcntr.hxx>
#include <qxpack/indcom/ui/base/qxpack_ic_quickimagesource.hxx>
#include <qxpack/indcom/ui/base/qxpack_ic_qsgtextureprov.hxx>
#include <qxpack/indcom/ui/base/qxpack_ic_qsgimtexture.hxx>
#include <qsgtexture>
#include <qsgsimpletexturenode>
#include <QImage>
#include <QAtomicInt>
#include <qDebug>
#include <qxpack/indcom/common/qxpack_ic_dyncinit_priv.hxx>
#include <qxpack/indcom/ui/qxpack_ic_ui_reg.hxx>
#include <QCoreApplication>

namespace  QxPack {

// //////////////////////////////////////////////////////////////
// register
// //////////////////////////////////////////////////////////////
static void QxPack_IcQuickImageItem_Reg( )
{
    static bool is_reg = false;
    if ( ! is_reg ) {
        is_reg = true;
        qmlRegisterType<QxPack::IcQuickImageItem>("qxpack.indcom.ui.base", 1, 0, "IcQuickImageItem");
    }
}
Q_COREAPP_STARTUP_FUNCTION( QxPack_IcQuickImageItem_Reg )


// //////////////////////////////////////////////////////////////
// local functions
// //////////////////////////////////////////////////////////////
// ==============================================================
// fit the source size into dest. size
// ==============================================================
static  QSizeF  fitSize ( QSizeF  src_sz, QSizeF dst_sz )
{
    QSizeF  rsl;

    if ( ( ! src_sz.isEmpty())  &&  (! dst_sz.isEmpty() ) ) {
        rsl.rwidth()  = dst_sz.width();
        rsl.rheight() = dst_sz.width() * src_sz.height() / src_sz.width();
        if ( rsl.rheight() > dst_sz.height() ) {
            rsl.rwidth()  = dst_sz.height() * rsl.width() / rsl.height();
            rsl.rheight() = dst_sz.height();
        }
    } else {
        rsl.rwidth()  = 0;  rsl.rheight() = 0;
    }
    return  rsl;
}

// ===============================================================
//  alloc a texture
// ===============================================================
static IcQSGImTexture*  allocTex( int  width,  int  height, QImage::Format fmt_idx )
{
    QSGTexture *   tex;
    GLint  max_tex_sz = IcQSGImTexture::maxTextureSize();

    // -------------------------------------------
    // check if texture is too big
    // -------------------------------------------
    if ( ( width > max_tex_sz ) | ( height > max_tex_sz )) {
        QSizeF tex_sz = fitSize ( QSizeF( width, height ), QSizeF( max_tex_sz, max_tex_sz ) );
        width = ( int )( tex_sz.width() );  height = ( int )( tex_sz.height() );
    }

    // ------------------------------------------
    // create the texture
    // ------------------------------------------
    IcQSGImTexture *im_tex = new IcQSGImTexture( width, height, fmt_idx );
    if ( im_tex != Q_NULLPTR ) {
        tex = qobject_cast< QSGTexture *>( im_tex );
        tex->setFiltering ( QSGTexture::Linear );
        tex->setHorizontalWrapMode( QSGTexture::ClampToEdge );
        tex->setVerticalWrapMode  ( QSGTexture::ClampToEdge );
    }

    return im_tex;
}

// //////////////////////////////////////////////////////////////
//    private object
// //////////////////////////////////////////////////////////////
#define T_PrivPtr( o )  (( IcQuickImageItemPriv *) o )
class QXPACK_IC_HIDDEN  IcQuickImageItemPriv {
    friend class IcQuickImageItem;
private:
    IcQuickImageItem  *m_parent;
    IcQuickImageSource m_im_source;
    IcQSGTextureProv   m_qsg_tex_prov;
    QAtomicInt         m_im_source_req_cntr;
    QSizeF  m_node_sz, m_fit_im_sz, m_im_sz;
    bool    m_is_auto_delete_im_src_data;
    bool    m_req_v_flip,  m_req_h_flip;
    bool    m_curr_v_flip, m_curr_h_flip;

protected:
    inline void  clearImSourceReqCntr( )
    {
        int cntr = 0;
        while ( ( cntr = m_im_source_req_cntr.loadAcquire() ) != 0 ) {
            if ( m_im_source_req_cntr.testAndSetOrdered( cntr, 0 )) { break; }
        }
    }

    inline void  markImageSourceReqCntr()
    {
        int cntr = 0;
        while ( ( cntr = m_im_source_req_cntr.loadAcquire()) == 0 ) {
            if ( m_im_source_req_cntr.testAndSetOrdered( cntr, 1 )) { break; }
        }
    }

public:

    IcQuickImageItemPriv ( IcQuickImageItem * );
    ~IcQuickImageItemPriv( );

    inline IcQSGTextureProv *  textureProv( ) { return &m_qsg_tex_prov; }
    inline QObject *   imageSourceObj ( )       { return &m_im_source; }
           void  setImageSource ( QObject *obj );
    inline bool  isAutoDeleteImageSourceData( ) const    { return m_is_auto_delete_im_src_data; }
    inline void  setAutoDeleteImageSourceData( bool sw ) { m_is_auto_delete_im_src_data = sw; }

    inline bool  cmpReqAndCurrFlipV( ) { bool ret = false; if ( m_req_v_flip != m_curr_v_flip ) { m_curr_v_flip = ! m_curr_v_flip; ret = true; } return ret; }
    inline bool  cmpReqAndCurrFlipH( ) { bool ret = false; if ( m_req_h_flip != m_curr_h_flip ) { m_curr_h_flip = ! m_curr_h_flip; ret = true; } return ret; }
    inline bool  reqFlipV ( )  const { return m_req_v_flip;  }
    inline bool  reqFlipH ( )  const { return m_req_h_flip;  }
    inline bool  currFlipV( )  const { return m_curr_v_flip; }
    inline bool  currFlipH( )  const { return m_curr_h_flip; }
    inline void  setReqFlipV( bool sw ) { m_req_v_flip = sw; }
    inline void  setReqFlipH( bool sw ) { m_req_h_flip = sw; }

    inline QSizeF  lastNodeSize( ) const  { return m_node_sz; }
    inline QSizeF  lastImageSize( ) const { return m_im_sz;   }
    inline QSizeF  lastFitImSize( ) const { return m_fit_im_sz; }
    inline void    setLastNodeSize ( const QSizeF &n_sz   ) { m_node_sz = n_sz; }
    inline void    setLastImageSize( const QSizeF &im_sz  ) { if ( m_im_sz != im_sz ) { m_im_sz = im_sz; emit m_parent->textureSizeChanged(); } }
    inline void    setLastFitImSize( const QSizeF &fit_sz ) { m_fit_im_sz = fit_sz; }

    void  onNewDataReady( );
};

// =====================================================
// ctor
// =====================================================
    IcQuickImageItemPriv :: IcQuickImageItemPriv ( IcQuickImageItem *pa )
{
    m_parent = pa;
    m_is_auto_delete_im_src_data = false;
    m_req_v_flip  = false; m_req_h_flip = false;
    m_curr_v_flip = false; m_curr_h_flip= false;
    m_im_source_req_cntr.store(0);
    // -----------------------------------------------------
    // DO NOT directly to udate() due to read image source
    // -----------------------------------------------------
    QObject::connect(
        & m_im_source, & IcQuickImageSource::imageChanged,
        [this]() { this->onNewDataReady(); }
    );
}

// =====================================================
// dtor
// =====================================================
    IcQuickImageItemPriv :: ~IcQuickImageItemPriv ( )
{
    QObject::disconnect( & m_im_source, 0, m_parent, 0 );
}

// =====================================================
//  set the image source
// =====================================================
void  IcQuickImageItemPriv :: setImageSource( QObject *obj )
{
    IcQuickImageSource *im_src = qobject_cast< IcQuickImageSource *>( obj );
    if ( im_src != Q_NULLPTR ) {
        m_im_source = *im_src; // attach to another
    } else {
        m_im_source = IcQuickImageSource(); // drop current
    }
    this->clearImSourceReqCntr();
    emit m_parent->imageSourceChanged();
    this->onNewDataReady(); // force read an image
}

// =====================================================
// read an image and update it
// =====================================================
void  IcQuickImageItemPriv :: onNewDataReady()
{
    this->markImageSourceReqCntr();
    m_parent->update();
}


// //////////////////////////////////////////////////////
//
// QuickImageItem
//
// //////////////////////////////////////////////////////
// =====================================================
// ctor
// =====================================================
    IcQuickImageItem :: IcQuickImageItem ( QQuickItem *pa ) : QQuickItem( pa )
{
    m_obj = qxpack_ic_new( IcQuickImageItemPriv, this );
    this->setFlags ( QQuickItem::ItemHasContents ); // this is needed. make the object draw
}

// =====================================================
// dtor
// =====================================================
    IcQuickImageItem :: ~IcQuickImageItem ( )
{
    if ( m_obj != Q_NULLPTR ) {
        qxpack_ic_delete( m_obj, IcQuickImageItemPriv );
        m_obj = Q_NULLPTR;
    }
}

// =====================================================
// the image source object access
// =====================================================
QObject *   IcQuickImageItem :: imageSource() const
{ return T_PrivPtr( m_obj )->imageSourceObj(); }

void        IcQuickImageItem :: setImageSource( QObject *obj )
{ T_PrivPtr( m_obj )->setImageSource( obj ); }

// =====================================================
// flip the image
// =====================================================
bool        IcQuickImageItem :: flipHorizontal( ) const
{ return T_PrivPtr( m_obj )->reqFlipH(); }

bool        IcQuickImageItem :: flipVertical( ) const
{ return T_PrivPtr( m_obj )->reqFlipV(); }

void        IcQuickImageItem :: setFlipHorizontal( bool sw )
{  T_PrivPtr( m_obj )->setReqFlipH( sw ); }

void        IcQuickImageItem :: setFlipVertical  ( bool sw )
{  T_PrivPtr( m_obj )->setReqFlipV( sw ); }

// ======================================================
// return the texture size etc..
// ======================================================
QSizeF      IcQuickImageItem :: textureSize( ) const
{
    // -------------------------------------------------------
    // NOTE: the setLastImageSize() saved the texture size,
    // -------------------------------------------------------
    return T_PrivPtr( m_obj )->lastImageSize();
}

// ======================================================
// map the coord. between image and item
// ======================================================
QPointF     IcQuickImageItem :: mapImagePointToItemPoint ( const QPointF &im_pt )
{
    QSizeF bd_sz  = T_PrivPtr( m_obj )->lastNodeSize();
    QSizeF fit_sz = T_PrivPtr( m_obj )->lastFitImSize();
    QSizeF im_sz  = T_PrivPtr( m_obj )->lastImageSize();
    if ( ! bd_sz.isValid()  ||  ! fit_sz.isValid()  || ! im_sz.isValid()) { return QPointF(); }

    qreal x = ( bd_sz.width() - fit_sz.width() ) * 0.5;
    qreal y = ( bd_sz.height() - fit_sz.height() ) * 0.5;
    qreal scale_x = fit_sz.width()  / im_sz.width();
    qreal scale_y = fit_sz.height() / im_sz.height();

    return QPointF( x + im_pt.x() * scale_x, y + im_pt.y() * scale_y );
}

QPointF     IcQuickImageItem :: mapItemPointToImagePoint ( const QPointF &pt )
{
    QSizeF bd_sz  = T_PrivPtr( m_obj )->lastNodeSize();
    QSizeF fit_sz = T_PrivPtr( m_obj )->lastFitImSize();
    QSizeF im_sz  = T_PrivPtr( m_obj )->lastImageSize();
    if ( ! bd_sz.isValid()  ||  ! fit_sz.isValid()  || ! im_sz.isValid()) { return QPointF(); }

    QRectF fit_rect ( ( bd_sz.width() - fit_sz.width() ) * 0.5, ( bd_sz.height() - fit_sz.height() ) * 0.5,
                      fit_sz.width(), fit_sz.height() );
    if ( ! fit_rect.contains( pt )) { return QPointF(); }

    qreal scale_x = im_sz.width()  / fit_sz.width();
    qreal scale_y = im_sz.height() / fit_sz.height();
    QPointF  hit_pt( pt.x() - fit_rect.x(), pt.y() - fit_rect.y() );

    return QPointF( hit_pt.x() * scale_x, hit_pt.y() * scale_y );
}

// =========================================================
// texture provider support ( overrie the parent )
// =========================================================
QSGTextureProvider *   IcQuickImageItem :: textureProvider() const
{ return T_PrivPtr( m_obj )->textureProv(); }

bool   IcQuickImageItem :: isTextureProvider() const
{ return true; }

// =========================================================
// QSG functions
// =========================================================
QSGNode*   IcQuickImageItem :: qsgUpnImSourceDataNull( QSGNode *old )
{
    QSGSimpleTextureNode *node = Q_NULLPTR;
    IcQuickImageItemPriv *priv = T_PrivPtr( m_obj );

    if ( old == Q_NULLPTR ) { return old; }
    node = static_cast< QSGSimpleTextureNode *>( old );

    // nw: here grantee the tex is not NULL!
    QSGTexture *tex = node->texture();
    QSizeF  bd_sz( this->width(), this->height() );
    QSizeF  fit_sz  = fitSize( tex->textureSize(), bd_sz );

    if ( fit_sz.width() * fit_sz.height() > 0 ) {
        node->setRect ( ( bd_sz.width() - fit_sz.width()) * 0.5f, ( bd_sz.height() - fit_sz.height()) * 0.5f, fit_sz.width(), fit_sz.height() );
        priv->setLastNodeSize( bd_sz );  priv->setLastFitImSize( fit_sz );
    } else {
        priv->setLastNodeSize( QSizeF()); priv->setLastFitImSize( QSizeF());
    }
    priv->setLastImageSize( tex->textureSize() );

    return node;
}

QSGNode*  IcQuickImageItem :: qsgUpnNullNode ( QSGNode *old, const QImage &im )
{
    QSGSimpleTextureNode *node = Q_NULLPTR;
    IcQuickImageItemPriv *priv = T_PrivPtr( m_obj );

    if ( old != Q_NULLPTR ) { return old; }

    // -----------------------------------
    // create a texture node
    // ------------------------------------
    node = new QSGSimpleTextureNode;
    if ( node == Q_NULLPTR ) { return node; }

    // ------------------------------------
    // setup the texture node
    // ------------------------------------
    node->setFiltering( QSGTexture::Linear );
    node->setFlag ( QSGNode::OwnedByParent, true );
    node->setOwnsTexture( true ); // nw: important! need to take ownership of the texture

    // ------------------------------------
    // remap the texture rectangle
    // ------------------------------------
    QSizeF  bd_sz( this->width(), this->height() );
    QSizeF  fit_sz  = fitSize( im.size(), bd_sz );
    if ( fit_sz.width() * fit_sz.height() > 0 ) {
        node->setRect ( ( bd_sz.width() - fit_sz.width()) * 0.5f, ( bd_sz.height() - fit_sz.height()) * 0.5f, fit_sz.width(), fit_sz.height() );
        priv->setLastNodeSize( bd_sz );  priv->setLastFitImSize( fit_sz );
    } else {
        priv->setLastNodeSize( QSizeF( ));  priv->setLastFitImSize( QSizeF());
    }
    priv->setLastImageSize( im.size() );

    // ------------------------------------
    // prepare the texture
    // ------------------------------------
    IcQSGImTexture *im_tex = allocTex ( im.width(), im.height(), im.format( ) );
    if ( im_tex != Q_NULLPTR ) {
        node->setTexture ( im_tex ); // NOTE: if the node currently has a texture, it automatically delete it, then set new texture.
        if ( this->isVisible()) {
            node->markDirty( QSGNode::DirtyMaterial | QSGNode::DirtyGeometry );
        }
        im_tex->updateTexture( im );
    }

    return node;
}

QSGNode*   IcQuickImageItem :: qsgUpnExistedNode ( QSGNode *old, const QImage &im )
{
    QSGSimpleTextureNode *node = Q_NULLPTR;
    IcQuickImageItemPriv *priv = T_PrivPtr( m_obj );
    QSGTexture *tex = Q_NULLPTR;
    IcQSGImTexture *im_tex = Q_NULLPTR;

    if ( old == Q_NULLPTR ) { return old; }
    node = static_cast< QSGSimpleTextureNode *>( old );

    // -----------------------------------------
    // remap the texture rectangle
    // -----------------------------------------
    QSizeF  bd_sz( this->width(), this->height() );
    QSizeF  fit_sz  = fitSize( im.size(), bd_sz );
    if ( fit_sz.width() * fit_sz.height() > 0 ) {
        node->setRect ( ( bd_sz.width() - fit_sz.width()) * 0.5f, ( bd_sz.height() - fit_sz.height()) * 0.5f, fit_sz.width(), fit_sz.height() );
        priv->setLastNodeSize( bd_sz );   priv->setLastFitImSize( fit_sz );
    } else {
        priv->setLastNodeSize( QSizeF( )); priv->setLastFitImSize( QSizeF());
    }
    priv->setLastImageSize( im.size() );

    // -----------------------------------------
    // check if the PBO is enabled
    // -----------------------------------------
    tex = node->texture();

    // -----------------------------------------
    // check if the texture size is same
    // -----------------------------------------
    if ( tex != Q_NULLPTR ) {
        im_tex = static_cast< IcQSGImTexture *>( tex );
        if ( im_tex->textureSize() != im.size() ) { tex = Q_NULLPTR; }
    }

    // -----------------------------------------
    // create texture and upload image data
    // -----------------------------------------
    if ( tex == Q_NULLPTR ) {
        im_tex = allocTex ( im.width(), im.height(), im.format( ) );
        if ( im_tex != Q_NULLPTR ) {
            tex = im_tex;
            node->setTexture ( im_tex ); // NOTE: if the node currently has a texture, it automatically delete it, then set new texture.
            if ( this->isVisible()) {
                node->markDirty( QSGNode::DirtyMaterial | QSGNode::DirtyGeometry );
            }
        }
    } else {
        im_tex = static_cast< IcQSGImTexture *>( tex );
        if ( this->isVisible() ) {
            node->markDirty( QSGNode::DirtyMaterial );
        }
    }
    if ( im_tex != Q_NULLPTR ) { im_tex->updateTexture( im ); }

    return node;
}

void      IcQuickImageItem :: qsgEnsureFlipMode ( QSGNode *old )
{
    IcQuickImageItemPriv *t_this = T_PrivPtr( m_obj );
    QSGSimpleTextureNode *node = static_cast< QSGSimpleTextureNode *>( old );
    bool is_need_set = false;

    if ( node == Q_NULLPTR ) { return; } // need not to update.

    QSGSimpleTextureNode::TextureCoordinatesTransformMode  trans_mode = QSGSimpleTextureNode::NoTransform;
    if ( t_this->cmpReqAndCurrFlipV() ) {
        if ( t_this->currFlipV()) { trans_mode |= QSGSimpleTextureNode::MirrorVertically; }
        is_need_set = true;
    }
    if ( t_this->cmpReqAndCurrFlipH() ) {
        if ( t_this->currFlipH()) { trans_mode |= QSGSimpleTextureNode::MirrorHorizontally; }
        is_need_set = true;
    }

    if ( is_need_set ) {
        QSGSimpleTextureNode *node = static_cast< QSGSimpleTextureNode *>( old );
        if ( node != Q_NULLPTR ) { node->setTextureCoordinatesTransform( trans_mode ); }
    }
}

// =========================================================
// QSG thread visit this
// =========================================================
QSGNode*  IcQuickImageItem :: updatePaintNode ( QSGNode *old,UpdatePaintNodeData*)
{
    IcQuickImageItemPriv *t_this = T_PrivPtr( m_obj );
    QSGNode *node = Q_NULLPTR;

    if ( ! QSizeF( this->width(), this->height() ).isValid( ) ) { return old; }

    // --------------------------------------------------------
    // A. image source data changed
    //   A-1) the image is NULL.
    //      A-1-1) the old is NULL, directly return.
    //      A-1-2) the old is not NULL, clear content, return
    //   A-2) the image is not NULL
    //      A-2-1) the old is NULL, build and upload
    //      A-2-2) the old is not NULL, upload
    // B. normally update
    //      directly return old.
    // --------------------------------------------------------
    if ( t_this->m_im_source_req_cntr.loadAcquire() > 0 ) {
        QImage im;
        t_this->m_im_source_req_cntr.fetchAndSubOrdered(1);
        t_this->m_im_source.pickImage( im );
        t_this->clearImSourceReqCntr();

        if ( im.isNull()) {
            if ( old == Q_NULLPTR ) { // A-1-2
                return old;
            } else {                  // A-1-2
                node = this->qsgUpnImSourceDataNull( old );
            }
        } else {
            if ( old == Q_NULLPTR ) { // A-2-1
                node = this->qsgUpnNullNode( old, im );
            } else {                  // A-2-2
                node = this->qsgUpnExistedNode( old, im );
            }
        }
    } else {
        return old;
    }

    this->qsgEnsureFlipMode( node );

    // -----------------------------------------------------------
    // check if the texture provider
    // -----------------------------------------------------------
    if ( t_this->textureProv() != Q_NULLPTR ) {
        QSGSimpleTextureNode *tex_node = static_cast< QSGSimpleTextureNode *>( node );
        if ( tex_node != Q_NULLPTR ) {
            t_this->textureProv()->setTexture( tex_node->texture(), true );
        }
    }

    return node;
}


}

#endif
