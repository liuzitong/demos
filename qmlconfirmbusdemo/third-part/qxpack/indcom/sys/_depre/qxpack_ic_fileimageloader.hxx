// /////////////////////////////////////////////////////////////////////
/*!
  @file    qxpack_ic_fileimageloader
  @author  night wing
  @date    2018/09
  @brief   load the file data
  @version 0.1.0.0
*/
// /////////////////////////////////////////////////////////////////////
#ifndef QXPACK_IC_FILEIMAGELOADER_HXX
#define QXPACK_IC_FILEIMAGELOADER_HXX

/*!
  @addtogroup  QxPack
  @{
*/
/*!
  @addtogroup  indcom
  @{
*/
/*!
  @addtogroup  sys_Exported_Classes
  @{
*/

#include <qxpack/indcom/sys/qxpack_ic_fileloader.hxx>
#include <qxpack/indcom/sys/qxpack_ic_imagedata.hxx>
#include <QString>
#include <QByteArray>
#include <QSize>

namespace QxPack {

// ///////////////////////////////////////////////////////
/*!
 * @brief image file loader
 */
// ///////////////////////////////////////////////////////
class QXPACK_IC_API  IcFileImageLoader : public IcFileLoader {
    Q_OBJECT
public:
    IcFileImageLoader ( const QString &file_path, const QSize &sz = QSize());
    virtual ~IcFileImageLoader( );

    //! return the current loader name 'IcFileTextLoader'
    const char *   loaderName() const Q_DECL_OVERRIDE;

    //! check if the data is loaded
    bool    isLoaded ( ) const Q_DECL_OVERRIDE;

    //! return the result
    IcImageData  result( );

    /*!
     * @brief user function, used for pick up the embedded information and scale \n
     *        image
     * @param [in] ba, the image raw data
     * @param [in] dt, the loaded image data
     * @note  the 'ba' contain the raw image data, user can try to pick up the embedded \n
     *     information. 'dt' is the loaded image data, user can replace the information
     *      The defulat implement will try to loaded the embedded json object in JPEG. The \n
     *     tags is (0xff,0xef )
     */
    virtual void  userProcess( const QByteArray &ba, IcImageData &dt );

private:
    void   *m_obj;
    bool    m_is_loaded;
    IcImageData  m_im_data;

    Q_DISABLE_COPY( IcFileImageLoader )
};


}


/*!
  @}
*/
/*!
  @}
*/
/*!
  @}
*/

#endif

