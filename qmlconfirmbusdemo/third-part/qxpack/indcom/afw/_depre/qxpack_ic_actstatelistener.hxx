// ////////////////////////////////////////////////////////////////////////////
/*!
  @file    qxpack_ic_actstatelistener
  @author  night wing
  @date    2018/09, 2019/05
  @version 0.1.1.0
*/
// ////////////////////////////////////////////////////////////////////////////
#ifndef QXPACK_IC_ACTSTATELISTENER_HXX
#define QXPACK_IC_ACTSTATELISTENER_HXX

#include "qxpack/indcom/common/qxpack_ic_def.h"
#include <QObject>

namespace QxPack {

// ////////////////////////////////////////////////////////////////////////////
/*!
 * @brief Action State Listener
 * @details \n
 *    The Action State Listener is respond for receive the signal from ActionExec.
 */
// ////////////////////////////////////////////////////////////////////////////
class QXPACK_IC_API  IcActStateListener : public QObject {
    Q_OBJECT
public:
    /*!
     * @brief callback handler
     * @param [in] the context pointer used by user
     * @param [in] the action id
     * @param [in] current state of action
     * @param [in] target  state of action
     */
    typedef void (* Handler )( void *ctxt, int act_id, int curr_state, int target_state );

    //! CTOR
    IcActStateListener ( QObject *pa = Q_NULLPTR );

    //! DTOR
    virtual ~IcActStateListener ( );

    /*!
     * @brief setupHandler
     * @param [in] act_id, specified this handler receive
     * @param [in] h, callback handler
     * @param [in] ctxt, context passed by user
     * @warning  DO NOT CALL THIS directly or in-directly by Handler() function
     */
    void   setupHandler ( int act_id, Handler h, void *ctxt );

    /*!
     * @brief slot of the action state changed
     */
    Q_SLOT void  onStateChanged ( int act_id, int curr_state, int target_state );

private:
    void *m_obj;
    Q_DISABLE_COPY( IcActStateListener )
};

}

#endif
