/* ///////////////////////////////////////////////////////////////////// */
/*! 
  @file    qxpack_ic_sysshm.hxx
  @author  night wing
  @date    2019/1
  @brief   The class describe the system shared memory
  @par     History
  @verbatim
          <author>    <time>   <version>  <desc>                                   
         nightwing   2019/01   0.1.0     build
  @endverbatim
*/
/* ///////////////////////////////////////////////////////////////////// */
#ifndef  QXPACK_IC_SYSSHM_HXX
#define  QXPACK_IC_SYSSHM_HXX

/*!
  @addtogroup  QxPack
  @{
*/
/*!
  @addtogroup  indcom
  @{
*/
/*!
  @addtogroup  common_Exported_Classes
  @{
*/

#include <stdint.h>
#include "qxpack/indcom/common/qxpack_ic_def.h"

namespace QxPack {

// /////////////////////////////////////////////////////////////////////
//! IcSysShm
/*!
   Inter Process Communite Share Data Object, used for share data between \n
the process
*/
// ///////////////////////////////////////////////////////////////////// 
class QXPACK_IC_API IcSysShm {

public:
    /*!
     * @brief create specified size shared memory
     * @param size [in]  the size of the expected memory
     * @param name [in]  the name of the share memory
     * @note  if size is <= 0 or name is nullptr, means a null object
     */
    IcSysShm ( int size, const char *name );

    /*!
     * @brief attach to an existed shared memory
     * @param name [in]  the name of the shared memory
     * @note  if name is nullptr, means a null object
     */
    IcSysShm ( const char *name );

    //! create a null object
    IcSysShm ( );

    //! dtor
    virtual ~IcSysShm( );

    //! initalize the share memory
    //! @note this operation will drop current context and try create a new one
    //! @see dtor
    //! @return true means created
    bool init ( int size, const char *name );

    //! attach to the named shared memory
    //! @return true means attached
    bool init ( const char *name );

    //! detach from the shared memory
    void  deinit( );

    //! check if IcSysShm is a worked object or not
    bool isWorked( ) const;

    //! return the size of this shared memory
    int    size( ) const;

    //! return the writable pointer of this shared memory
    void*  data( );    

private:
    void *m_obj;
    QXPACK_IC_DISABLE_COPY( IcSysShm )
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
