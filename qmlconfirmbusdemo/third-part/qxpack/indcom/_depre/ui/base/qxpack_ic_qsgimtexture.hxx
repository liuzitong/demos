// /////////////////////////////////////////////////////////////////////////////////
/*! 
    @file    qxpack_ic_qsgimtexture.hxx
    @author  night wing
    @date    2018/10
    @brief   This file is the image texture object header
    @par     History
    @verbatim
	     <author>    <time>     <version>  <desc>
         nightwing   2018/10    0.1.0    re-designed
    @endverbatim 
*/
// /////////////////////////////////////////////////////////////////////////////////
#ifndef QXPACK_IC_QSGIMTEXTURE_HXX
#define QXPACK_IC_QSGIMTEXTURE_HXX

#include <qxpack/indcom/common/qxpack_ic_def.h>
#include <QSGTexture>
#include <QImage>

/*!
  @addtogroup  QxPack
  @{
*/

/*!
  @addtogroup  ui
  @{
*/

/*!
  @addtogroup  base_Exported_Classes
  @{
*/

namespace  QxPack {

// ///////////////////////////////////////////////////////////////////////////
/*!
 * @brief the image texture  [ It can run in OpenGL Thread ]
 * @details this class is a texture class used for OpenGL
*/
// ///////////////////////////////////////////////////////////////////////////
class QXPACK_IC_API  IcQSGImTexture : public QSGTexture {
    Q_OBJECT
public:
    //! ctor ( empty )
    IcQSGImTexture ( );

    //! ctor ( build a specified format )
    IcQSGImTexture ( int  width, int height, QImage::Format px_fmt );

    //! dtor
    virtual ~IcQSGImTexture ( );

    //! constructor
    IcQSGImTexture ( const IcQSGImTexture & );

    //! shared assign
    IcQSGImTexture & operator = ( const IcQSGImTexture & );

    void   bind ( ) Q_DECL_OVERRIDE;
    bool   hasAlphaChannel ( ) const Q_DECL_OVERRIDE;
    bool   hasMipmaps ( )      const Q_DECL_OVERRIDE;
    int    textureId ( )       const Q_DECL_OVERRIDE;
    QSize  textureSize ( )     const Q_DECL_OVERRIDE;
    bool   updateTexture ( const QImage & im );
    bool   updateTexture ( const void *bits, int width, int height, QImage::Format fmt );

    bool   isNull( ) const;

    //! check if current opengl maximum texture size
    static  int  maxTextureSize ( );

private:
    void *m_obj;
};

}

Q_DECLARE_METATYPE( QxPack::IcQSGImTexture )

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
