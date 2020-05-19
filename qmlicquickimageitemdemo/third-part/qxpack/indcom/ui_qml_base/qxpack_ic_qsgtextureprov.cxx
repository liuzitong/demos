#ifndef  _QXPACK_IC_QSGTEXTUREPROV_CXX
#define  _QXPACK_IC_QSGTEXTUREPROV_CXX

#include "qxpack_ic_qsgtextureprov.hxx"

namespace  QxPack {

// ////////////////////////////////////////////////////////////////////////////
//        TextureProv
// ////////////////////////////////////////////////////////////////////////////
// ============================================================================
// ctor
// ============================================================================
IcQSGTextureProv :: IcQSGTextureProv ( )
{
    m_obj = Q_NULLPTR;
}

// ============================================================================
// dtor
// ============================================================================
IcQSGTextureProv :: ~IcQSGTextureProv ( )
{
}

// ============================================================================
// setup the current texture
// ============================================================================
void  IcQSGTextureProv :: setTexture ( QSGTexture *tex , bool force_dirty )
{
    bool is_chg = false;
    if ( m_obj != (void*)tex ) { m_obj = tex; is_chg = true; }
    if ( is_chg || force_dirty ) { emit this->textureChanged(); }
}

// ============================================================================
// return current texture
// ============================================================================
QSGTexture *   IcQSGTextureProv :: texture( ) const
{
    return reinterpret_cast<QSGTexture*>( m_obj );
}

}

#endif
