#ifndef QXPACK_IC_RANDCOLORGEN_HXX
#define QXPACK_IC_RANDCOLORGEN_HXX

#include "qxpack/indcom/common/qxpack_ic_def.h"
#include <QColor>
#include <QVector>

namespace QxPack {

// ////////////////////////////////////////////////////////////////////////////
/*!
 * @brief random color generator
 */
// ////////////////////////////////////////////////////////////////////////////
class QXPACK_IC_API  IcRandColorGen {
public:
   //! ctor ( default )
   explicit IcRandColorGen( );

   //! dtor
   virtual ~IcRandColorGen( );

   //! generate spec. number color from HSV, H are randomized number
   QVector<QColor>   make ( int num, float s = 0.68, float v = 0.95 );
   
   //! get the color table
   QVector<QColor>   lastResult( ) const;

private:
   void *m_obj;
   QXPACK_IC_DISABLE_COPY( IcRandColorGen )
};


}

#endif
