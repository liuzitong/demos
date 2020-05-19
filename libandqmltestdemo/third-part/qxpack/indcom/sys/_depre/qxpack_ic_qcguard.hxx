// /////////////////////////////////////////////////////////////////////
/*!
  @file    qxpack_ic_qcguard
  @author  night wing
  @date    2018/09
  @version 0.1.0.0
*/
// /////////////////////////////////////////////////////////////////////
#ifndef QXPACK_IC_QCGUARD_HXX
#define QXPACK_IC_QCGUARD_HXX

#include <qxpack/indcom/common/qxpack_ic_def.h>
#include <QMetaType>

namespace QxPack {

// ////////////////////////////////////////////////////////////////
/*!
 * @brief Queued Call Guarder, used for tell the target function \n
 *        whether the caller is alive or died
 * @note  this class object is used between UI and async-thread. \n
 *        it is expensive due to two stage waitting.
 */
// ////////////////////////////////////////////////////////////////
class QXPACK_IC_API IcQcGuard {
public :
    //! create a empty object
    IcQcGuard ( );

    //! create a object by pass 'cr' with true
    IcQcGuard ( bool cr );

    /*!
     * @note shared content with other object
     */
    IcQcGuard ( const IcQcGuard &other );

    /*!
     * @note  shared content with other object
     */
    IcQcGuard &  operator = ( const IcQcGuard & );

    /*!
     * @note decrease the counter, if zero, delete the shared content
     */
    virtual ~IcQcGuard( );

    /*!
     * @brief  wait for target function accept [ caller ]
     * @param [in] accept_mill,  the accept time-out million seconds, -1 means no time limit
     * @return true for work done.
     */
    bool   waitForDone( int accept_mill );

    /*!
     * @brief  begin to work [ target function ]
     * @return true for the caller is waitting. false for the caller is already not waitting.
     * @note   this make the waitForDone() go next waiting stage ( execute waiting )
     */
    bool   beginWork( );

    /*!
     * @brief end work [ target function ]
     * @note  this make the waitForDone() complete the execute waitting stage.
     */
    void   endWork( bool is_done = true );

    /*!
     * @brief clean up cached IcQcGuard
     */
    static void cleanupCache( );

private:
    void  *m_obj;
};

}

Q_DECLARE_METATYPE( QxPack::IcQcGuard )

#endif
