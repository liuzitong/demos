#ifndef QMLICQUICKIMAGEITEMDEMO_MM_MAINVM_HXX
#define QMLICQUICKIMAGEITEMDEMO_MM_MAINVM_HXX

#include "qmlicquickimageitemdemo/base/common/qmlicquickimageitemdemo_def.h"
#include <QObject>
#include <QVariantList>
#include <QJsonObject>
#include <QVector2D>

namespace qmlIcquickimageitemDemo {

// ////////////////////////////////////////////////////////////////////////////
/*!
 * @brief the main view model
 * @note  this class abstract the view element, so view can only display them and \n
 *  pass all user input to it.
 */
// ////////////////////////////////////////////////////////////////////////////
class QMLICQUICKIMAGEITEMDEMO_HIDDEN  MmMainVm : public QObject {
    Q_OBJECT
    Q_PROPERTY( qreal      imageListCount  READ  imageListCount  NOTIFY imageListCountChanged )
    Q_PROPERTY( qreal      currImageIndex  READ  currImageIndex  WRITE  setCurrImageIndex NOTIFY currImageIndexChanged )
    Q_PROPERTY( QObject*   imageDataSource READ  imageDataSource NOTIFY imageDataSourceChanged )
public:
    //! ctor
    Q_INVOKABLE explicit MmMainVm( const QVariantList & );

    //! dtor
    virtual ~MmMainVm( ) Q_DECL_OVERRIDE;

    // ------------------------------------------------------------------------
    // the image list range access
    // ------------------------------------------------------------------------
    int  imageListCount( ) const;
    Q_SIGNAL  void   imageListCountChanged( );

    // ------------------------------------------------------------------------
    // current image index
    // ------------------------------------------------------------------------
    qreal   currImageIndex ( ) const;
    void    setCurrImageIndex( qreal );
    Q_SIGNAL  void   currImageIndexChanged( );

    // ------------------------------------------------------------------------
    // image data source
    // ------------------------------------------------------------------------
    QObject*  imageDataSource( ) const;
    Q_SIGNAL  void  imageDataSourceChanged( );

private:
    friend class MmMainVmPriv; // [HINT] use this to make private object visite signal and slot
    void *m_obj;
    Q_DISABLE_COPY( MmMainVm )
};

}

#endif
