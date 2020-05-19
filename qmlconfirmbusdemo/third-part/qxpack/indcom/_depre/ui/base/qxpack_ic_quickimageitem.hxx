#ifndef QXPACK_IC_QUICKIMAGEITEM_HXX
#define QXPACK_IC_QUICKIMAGEITEM_HXX

#include <qxpack/indcom/common/qxpack_ic_def.h>
#include <QQuickItem>
#include <QSGNode>
#include <QImage>
#include <QPointF>
#include <QSizeF>
#include <QSGTextureProvider>

namespace QxPack {

// ///////////////////////////////////////////////////////////
/*!
 * @brief  this item is used in Quick scene.
 * @details It need IcSharedImSource object for display.
 */
// ///////////////////////////////////////////////////////////
class QXPACK_IC_API  IcQuickImageItem : public QQuickItem {
    Q_OBJECT

    Q_PROPERTY( QObject*  imageSource      READ  imageSource         WRITE  setImageSource     NOTIFY imageSourceChanged )
    Q_PROPERTY( bool      flipHorizontal   READ  flipHorizontal      WRITE  setFlipHorizontal   )
    Q_PROPERTY( bool      flipVertical     READ  flipVertical        WRITE  setFlipVertical     )
    Q_PROPERTY( QSizeF    textureSize      READ  textureSize         NOTIFY textureSizeChanged  )
public:
     //! ctor
     IcQuickImageItem ( QQuickItem *pa = 0 );

     //! dtor
     virtual ~IcQuickImageItem (  );

     /*!
      * @brief setup the source object
      * @param [in] src, should be 'IcShareImSource'
      */
     void      setImageSource( QObject *src );

     //! return the source
     QObject*  imageSource() const;

     //! the image flip function
     bool      flipHorizontal( ) const;
     bool      flipVertical( )   const;
     void      setFlipHorizontal( bool sw );
     void      setFlipVertical  ( bool sw );

     //! return current texture size
     QSizeF    textureSize( ) const;

     //! return the point from image point
     Q_INVOKABLE QPointF  mapImagePointToItemPoint ( const QPointF &im_pt );

     //! return the item point
     Q_INVOKABLE QPointF  mapItemPointToImagePoint ( const QPointF &item_pt );

     // ========================================
     // override functions
     // ========================================
     virtual QSGNode*   updatePaintNode(QSGNode*,UpdatePaintNodeData*) Q_DECL_OVERRIDE;
     virtual QSGTextureProvider *  textureProvider() const Q_DECL_OVERRIDE;
     virtual bool       isTextureProvider() const Q_DECL_OVERRIDE;

     //! emit while call setImSource
     Q_SIGNAL  void  imageSourceChanged( );

     //! emit while texture size changed
     Q_SIGNAL  void  textureSizeChanged( );

protected:
    QSGNode*  qsgUpnImSourceDataNull( QSGNode* );
    QSGNode*  qsgUpnNullNode    ( QSGNode *old, const QImage &im );
    QSGNode*  qsgUpnExistedNode ( QSGNode *old, const QImage &im );
    void      qsgEnsureFlipMode ( QSGNode* );

private:
    void *m_obj;
    Q_DISABLE_COPY( IcQuickImageItem )
};

}

#endif
