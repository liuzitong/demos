// /////////////////////////////////////////////////////////////////////
/*!
  @file    qxpack_ic_spookyhashv2
  @author  night wing
  @date    2018/10
  @brief   provide a common hash algorithm
  @version 0.1.0.0
*/
// /////////////////////////////////////////////////////////////////////
#ifndef QXPACK_IC_SPOOKYHASHV2_HXX
#define QXPACK_IC_SPOOKYHASHV2_HXX

/*!
  @addtogroup  QxPack
  @{
*/
/*!
  @addtogroup  indcom
  @{
*/
/*!
  @addtogroup  algor_Exported_Classes
  @{
*/

#include <qxpack/indcom/common/qxpack_ic_def.h>
#include <qxpack/indcom/common/qxpack_ic_bytearray.hxx>

namespace QxPack {

// //////////////////////////////////////////////////////////
/*!
 * @brief wrapper of the SpookyHashV2 algorithm
 */
// //////////////////////////////////////////////////////////
class QXPACK_IC_API  IcSpookyHashV2 {
public:
    //! ctor, default init.
    IcSpookyHashV2 ( );

    //! dtor
    virtual ~IcSpookyHashV2( );

    //! directly calc. hash32 value
    static uint32_t  hash32Di( const IcByteArray &ba );

private:
    void *m_obj;
    QXPACK_IC_DISABLE_COPY( IcSpookyHashV2 )
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
