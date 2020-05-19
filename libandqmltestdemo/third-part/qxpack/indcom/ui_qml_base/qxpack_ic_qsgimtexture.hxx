#ifndef QXPACK_IC_QSGIMTEXTURE_HXX
#define QXPACK_IC_QSGIMTEXTURE_HXX

#include <qxpack/indcom/common/qxpack_ic_def.h>
#include <QSGTexture>
#include <QImage>

namespace  QxPack {

// ///////////////////////////////////////////////////////////////////////////
/*!
 * @brief the image texture  [ It only run in OpenGL Thread ]
 * @details this class is a texture class used for OpenGL
*/
// ///////////////////////////////////////////////////////////////////////////
class QXPACK_IC_API  IcQSGImTexture : public QSGTexture {
    Q_OBJECT
public:
    //! ctor ( empty )
    explicit IcQSGImTexture ( );

    //! ctor ( build a specified format )
    IcQSGImTexture ( int  width, int height, QImage::Format px_fmt );

    //! dtor
    virtual ~IcQSGImTexture ( ) Q_DECL_OVERRIDE;

    //! ctor copy
    //! @note it will share same OpenGL Texture ID
    IcQSGImTexture ( const IcQSGImTexture & );

    //! assign copy.
    //! @note it will share same OpenGL Texture ID
    IcQSGImTexture & operator = ( const IcQSGImTexture & );

    //! bind this texture for current opengl content
    void   bind ( ) Q_DECL_OVERRIDE;

    //! check if this texture has alpha channcel
    bool   hasAlphaChannel ( ) const Q_DECL_OVERRIDE;

    //! check if this texture has Mip maps
    bool   hasMipmaps ( )      const Q_DECL_OVERRIDE;

    //! return opengl texture id
    int    textureId ( )       const Q_DECL_OVERRIDE;

    //! return the texture size that user allocated
    QSize  textureSize ( )     const Q_DECL_OVERRIDE;

    //! upload QImage as texture
    //! @note all shared object content will be changed.
    bool   updateTexture ( const QImage & im );

    //! update textrue by data
    //! @note all shared object content will be changed.
    bool   updateTexture ( const void *bits, int width, int height, QImage::Format fmt );

    //! if it is null ( means ctor(empty))
    bool   isNull( ) const;

    //! check if current opengl maximum texture size
    static  int  maxTextureSize ( );

private:
    void *m_obj;
};

}

Q_DECLARE_METATYPE( QxPack::IcQSGImTexture )

#endif
