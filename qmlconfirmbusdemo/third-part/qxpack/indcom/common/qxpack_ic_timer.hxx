// /////////////////////////////////////////////////////////////////////
/*!
  @file    qxpack_ic_timer
  @author  night wing
  @date    2018/09
  @brief   a portable timer, timerqueue, and event call
  @version 0.1.0.0
*/
// /////////////////////////////////////////////////////////////////////
#ifndef QXPACK_IC_TIMER_HXX
#define QXPACK_IC_TIMER_HXX

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

#include "qxpack/indcom/common/qxpack_ic_def.h"
#include <stdint.h> // C++99
#include <chrono>   // C++11

namespace QxPack {

// ////////////////////////////////////////////////////////////////////
/*!
 * @brief a portable timer
 */
// ////////////////////////////////////////////////////////////////////
class QXPACK_IC_API  IcTimer {
public:

    /*!
     * @param [in] mill          -- timer gap, millseconds
     * @param [in] rep_count     -- repeat counter, -1 means infinity
     * @param [in] TimerCallback -- callback function
     * @param [in] ctxt          -- context pointer, passed by TimerCallback
     * @param [in] tmq_name      -- a customer timerqueue name. nullptr means use default timerqueue
     * @note   tmq_name must point to static const string, do not use std::string  \n
     *         or any dynmatically allocated string object.
     */
    IcTimer(
              int mill, int rep_count,
              void (* TimerCallback)( void *ctxt ),
              void *ctxt, const char *tmq_name = nullptr
    );

    /*!
     * @note will be automatically stop timer before destruct it
     */
    virtual ~IcTimer( );

    /*!
     * @brief control start the timer
     * @note  if timer already start, it will restore current call count
     */
    void  start( );

    /*!
     * @brief stop current timer
     * @note  if timer already stopped, nothing happend
     */
    void  stop ( );

    /*!
     * @brief setup the timer gap.
     * @param [in] mill -- millionseconds
     */
    void  setTimeGap ( int mill );

    //! time stamp using in qxpack.indcom
    typedef std::chrono::steady_clock::time_point  TimeStamp;

    //! make a time stamp from now
    static inline IcTimer::TimeStamp  makeTimeStampFromNow( int mill )
    { return std::chrono::steady_clock::now() + std::chrono::milliseconds(mill); }

    //! increase time stamp
    static inline void  incrTimeStamp( IcTimer::TimeStamp &tm, int mill )
    { tm += std::chrono::milliseconds( mill ); }

    //! make a time stamp by specified time stamp
    static inline IcTimer::TimeStamp  makeTimeStampFrom( IcTimer::TimeStamp &tm, int mill )
    { return tm + std::chrono::milliseconds( mill );  }

    //! compare two time stamp, return true if a < b
    static inline bool  isTimeStampLessThan( const IcTimer::TimeStamp &a, const IcTimer::TimeStamp &b )
    { return a < b; }

private:
    void *m_obj;
    QXPACK_IC_DISABLE_COPY( IcTimer )
};

// //////////////////////////////////////////////////////////////////////////
/*!
 * @brief a portable event caller
 */
// //////////////////////////////////////////////////////////////////////////
class QXPACK_IC_API  IcCaller {
public:
    //! the callback handle type
    enum HandlerType {
        HandlerType_DoRoutine = 0,   //!< normal handle, user can handle 'param' and free it
        HandlerType_RecycleParam = 1 //!< the 'param' object should only be free by user
    };

    /*!
     * @brief custom callback function proto
     * @param [in] ctxt   -- custom context pointer, it will be passed by callback
     * @param [in] param  -- custom parameter pointer, it will be passed by callback
     * @param [in] h_type -- means current call if it is normally call.
     * @note  if h_type is HandlerType_DoRoutine, user should do custom routine and free the parameter. \n
     *        if h_type is HandlerType_RecycleParam, user only need to free the paramter.
     */
    typedef void (* Callback )( void *ctxt, void *param, HandlerType h_type );

    /*!
     * @brief   construct the callback object.
     * @details cb_func will be called with HandlerType_DoRoutine,\n
     *          if HandlerType_RecycleParam is called, means user \n
     *          should recycle or free the param pointer.
     */
    IcCaller(
        Callback  cb_func,  void *cb_obj,
        const char *thr_name = nullptr
    );

    /*!
     * @note  all unsolved 'param' will be processed in here. \n
     *        'process' means the callback function will handle \n
     *        all unsovled 'param' with HandlerType_RecycleParam
     */
    virtual ~IcCaller( );

    /*!
     * @brief   post a call request
     * @details this function post a call request in future, \n
     *          it will cause inner thread do the CallbackFunc method
     * @param [in] param    -- user parameter pointer.
     * @param [in] delay_ms -- delay time from now, in millionseconds.
     * @note    'param' life-cycle should be managed by user callback function. \n
     *    in Windows, visual studio steady_clock::now() is not steady. qxpack_ic_timer \n
     *    handle it by repeatly call, but lost some performance. that is in Windows, \n
     *    time stamp between every post() call maybe about 10ms.
     */
    void   post ( void *param, int delay_ms = 0 );


    /*!
     * @note  cancel all callback that managed by this object.
     */
    void   cancelAll( );

private:
    void *m_obj;
    QXPACK_IC_DISABLE_COPY( IcCaller )
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
