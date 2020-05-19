// ////////////////////////////////////////////////////////////////////////////
/*!
  @file    qxpack_ic_actionexec
  @author  night wing
  @date    2018/09, 2018/10, 2019/05
  @version 0.2.0.0
*/
// ////////////////////////////////////////////////////////////////////////////
#ifndef QXPACK_IC_ACTIONEXEC_HXX
#define QXPACK_IC_ACTIONEXEC_HXX

#include <QObject>
#include <QThread>
#include <QJsonObject>
#include <QString>
#include <QMetaEnum>

#include "qxpack/indcom/sys/qxpack_ic_vardata.hxx"
#include "qxpack/indcom/common/qxpack_ic_def.h"

namespace QxPack {

// ////////////////////////////////////////////////////////////////////////////
/*!
 * @brief Action executor, for manage the action node. [ thread safe ]
 * @details \n
 *     The Action, is a descriptor of an method. \n
 *     A method, has some states: "state_freeze", "state_idle", "state_working". \n
 *     "state_freeze"  -- the method forbid to execute. \n
 *     "state_idle"    -- the method call be called, then the state changed to "state_working". \n
 *     "state_working" -- the method is working, if it is in this state, can not call it again. \n
 *  \n
 *     There are diffrient methods: \n
 *     "type_trigger"  -- this type has two states: "state_freeze", "state_idle" \n
 *     "type_switch"   -- this type has all states: "state_freeze", "state_working", "state_idle" \n
 *  \n
 *     Normally, the IcActionExec created in main UI thread. But in Async-MVVM model, user can \n
 *     specify the async-thread, so all actions running in async-thread. \n
 */
// ////////////////////////////////////////////////////////////////////////////
class QXPACK_IC_API  IcActionExec : public QObject {
    Q_OBJECT
public:
    enum State {
        State_Freeze = 0, State_Idle = 1, State_Working = 2
    };

    enum ActionType {
        ActionType_Trigger = 0, ActionType_Switch = 1
    };

    /*!
     * @brief handle the action proto
     * @param [in] ctxt, the context pointer is the user passed in addNode()
     * @param [in] jo,   json object, also passed by user
     * @param [in] ext_param, extend parameter, passed by user
     * @param [in] curr_state,   current state of this action
     * @param [in] target_state, if handler return true, the target_state will be use.
     * @note  The state translation depends on return value. If return true, the translation \n
     *   will be done, otherwise, nothing happen
     */
    typedef bool (* ActionHandler )(
        void *ctxt, const QJsonObject &jo, const IcVarData &ext_param,
        int  curr_state, int target_state
    );

    /*!
     * @brief constructor
     * @param [in] act_thread, action thread pointer, if specified, all action will \n
     *             execute in the action thread.
     * @param [in] pa, QObject parent
     */
    IcActionExec( QThread *act_thread = Q_NULLPTR, QObject *pa = Q_NULLPTR );

    virtual ~IcActionExec ( ) Q_DECL_OVERRIDE;

    /*!
     * @brief build a action node
     * @param [in] act_id,    the action id defined by user
     * @param [in] act_type,  specified the action type by enumeric
     * @param [in] handler,   the callback handler
     * @param [in] ctxt,      the callback context pointer
     * @param [in] rsvd,      this is reserved for inner use.
     * @note  user can use a enumeric to define the actions
     */
    Q_INVOKABLE bool  buildNode (
        int act_id, ActionType act_type, ActionHandler handler, void *ctxt
    );

    /*!
     * @brief wait for all action done
     * @param [in] req_proc_event, if true, inner will enter a event loop
     * @note  this function automatically invoked by sync. or async.
     */
    Q_INVOKABLE void  waitForAllDone( bool req_proc_event = true );

    /*!
     * @return true for existed a action thread
     */
    Q_INVOKABLE bool  hasActionThread( ) const;

    /*!
     * @brief post a action into queue.
     * @param [in] act_id,    the action ID
     * @param [in] jo,        parameter described by Json
     * @param [in] ext_param, extend parameter pointer
     * @return true for posted.
     * @note  If existed a action thread, this operation post the request \n
     *        and return immediately. Otherwise, this operation queued in caller \n
     *        event queue.
     */
    Q_INVOKABLE bool  post (
        int act_id, const QJsonObject &jo, const IcVarData &ext_param = IcVarData()
    );

    //! post the cancel request
    Q_INVOKABLE bool  postCancel ( int act_id );

    //! post a freeze request
    Q_INVOKABLE bool  postFreeze ( int act_id );

    //! post a unfreeze request
    Q_INVOKABLE bool  postUnfreeze( int act_id );

    //! same as post(), but operation will blocking until action return
    Q_INVOKABLE bool  send (
        int act_id, const QJsonObject &jo, const IcVarData &ext_param = IcVarData()
    );

    //! same as postCancel(), but operation will blocking until action return
    Q_INVOKABLE bool  sendCancel ( int act_id );

    //! same as postFreeze(), but operation will blocking until action return
    Q_INVOKABLE bool  sendFreeze ( int act_id );

    //! same as postUnfreeze(), but operation will blocking until action return
    Q_INVOKABLE bool  sendUnfreeze( int act_id );

    //! emit by a action state changed.
    Q_SIGNAL void  stateChanged  ( int act_id, int curr_state, int target_state );

    /*!
     * @brief a helper function used to build nodes.
     * @details \n
     *   the function must be Q_INVOKABLE or Q_SLOT, name format is "onType_XXXXXX"\n
     *   for example: \n
     *     enum ActionID { ActionID_SendFile }; \n
     *     Q_INVOKABLE bool onTrg_ActionID_SendFile ( ... ) \n
     *     "onTrg" is trigger style action, "onSw" is switch style action.
     * @warning
     *   the 'cb_obj' need to keep alive before this object destoryed. \n
     *
     */
    Q_INVOKABLE void  autoBuildNodes( QObject *cb_obj, const QMetaEnum &act_tbl );

    //! find the Q_ENUM( ActionID ) in the QObject
    static QMetaEnum  findActionIDEnum( QObject *obj );

protected:
    bool   appendNode( void* );

private:
    void *m_obj;    
    Q_DISABLE_COPY( IcActionExec )
};

}

#endif
