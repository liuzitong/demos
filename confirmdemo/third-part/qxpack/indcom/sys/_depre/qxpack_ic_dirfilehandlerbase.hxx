// /////////////////////////////////////////////////////////////////////
/*!
  @file    qxpack_ic_dirfilehandlerBase
  @author  night wing
  @date    2018/10
  @brief   
  @version 0.1.0.0
*/
// /////////////////////////////////////////////////////////////////////
#ifndef QXPACK_IC_DIRFILEHANDLERBASE_HXX
#define QXPACK_IC_DIRFILEHANDLERBASE_HXX

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

#include <QObject>
#include <QString>
#include <qxpack/indcom/common/qxpack_ic_def.h>
#include <qxpack/indcom/sys/qxpack_ic_vardata.hxx>

namespace QxPack {

// ///////////////////////////////////////////////////////////////////////
/*!
 * @brief handle the file 
 */
// ///////////////////////////////////////////////////////////////////////
class QXPACK_IC_API IcDirFileHandlerBase {
public:
    /*!
     * @brief factory
     * @param [in] req, the request string. \n
     *       'reqCreate' -- request create a loader \n
     *       'reqDelete' -- request delete spec. loader
     * @param [in_out] ldr, \n
     *      'createLoader' will output the loader. \n
     *      'deleteLoader', ldr must not be null.
     * @return true means request solved.
     */
    typedef bool  (* Factory )( const QString &req, IcDirFileHandlerBase **ldr );

    //! ctor
    IcDirFileHandlerBase ( );

    //! dtor
    virtual ~IcDirFileHandlerBase( );

    /*!
     * @brief handle the spec. file. Called by other worker threads.
     * @param [in] sfx, the suffix name. e.g "jpg"
     * @param [in] full_path, the full path of the file.
     * @warning one or more threads will call this function at same time. \n
     *    So user need to guarantee the thread-safe.
     * @note Be care of use lock/unlock here, due to this will make mult-threads \n
     *    performance down to single thread performance. User should consider the \n
     *    multi threads reentrant problem. \n
     *    Hint: do it like a static function
     */
    virtual IcVarData  handle ( const QString &sfx, const QString &full_path );

private:
    void *m_obj;
    Q_DISABLE_COPY( IcDirFileHandlerBase )
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

