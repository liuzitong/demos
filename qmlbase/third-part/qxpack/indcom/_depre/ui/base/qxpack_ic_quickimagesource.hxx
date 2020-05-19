#ifndef QXPACK_IC_QUICKIMAGESOURCE_HXX
#define QXPACK_IC_QUICKIMAGESOURCE_HXX

#include <qxpack/indcom/common/qxpack_ic_def.h>
#include <qxpack/indcom/ui/base/qxpack_ic_qsgimtexture.hxx>
#include <QImage>

namespace QxPack {

// ////////////////////////////////////////////////////
/*!
 * @brief support a texture image
 */
// ////////////////////////////////////////////////////
class QXPACK_IC_API  IcQuickImageSource : public QObject {
    Q_OBJECT
public:
    enum FlagID { FlagID_Normal = 0, FlagID_DeleteAfterPick };

    // ctor ( empty )
    IcQuickImageSource( );

    /*!
     * @brief ctor, create a source object
     */
    IcQuickImageSource( FlagID flag );

    // ctor ( constructor )
    IcQuickImageSource ( const IcQuickImageSource & );

    // ctor ( copy assign )
    IcQuickImageSource &  operator = ( const IcQuickImageSource & );

    // dtor
    virtual ~IcQuickImageSource( );

    //! check if it is null
    bool  isNull( ) const;

    /*!
     * @brief update the image into to source
     * @details this function will make the source data dirty.
     */
    Q_SLOT void  updateImage( const QImage & );

    //! the image ref
    Q_SLOT void  pickImage( QImage &im ) const;

    //! the image emit this signal
    Q_SIGNAL void  imageChanged( );

private:
    void *m_obj;
};

}

#endif 
