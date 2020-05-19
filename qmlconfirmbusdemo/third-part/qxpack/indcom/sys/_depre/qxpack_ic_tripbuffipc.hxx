// /////////////////////////////////////////////////////////////////////
/*!
  @file    qxpack_ic_tripbuffipc
  @author  night wing
  @date    2018/09
  @brief   triple buffer IPC toolkit
  @version 0.1.0.0
*/
// /////////////////////////////////////////////////////////////////////
#ifndef QXPACK_IC_TRIPBUFFIPC_HXX
#define QXPACK_IC_TRIPBUFFIPC_HXX

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

namespace QxPack {

// /////////////////////////////////////////////////////////
/*!
 * @brief RPC triple buffer object
 */
// /////////////////////////////////////////////////////////
class  QXPACK_IC_API IcTripBuffIpc : public QObject {
    Q_OBJECT

public:
    /*!
     * @brief constructor, create a shared triple buffer
     * @param [in] size, the shared buffer size
     * @param [in] name, the shared buffer name
     * @param [in] pa, the parent QObject
     */
    IcTripBuffIpc ( int size, const QString &name, QObject *pa = Q_NULLPTR );

    /*!
     * @brief constructor, attach to an existed triple buffer
     * @param [in] name, the shared buffer name
     * @param [in] pa, the parent QObject
     */
    IcTripBuffIpc ( const QString &name, QObject *pa = Q_NULLPTR );

    virtual ~IcTripBuffIpc( );

    //! @return  true means data are wrote by writer
    bool  isDirty   ( ) const;

    //! @return the shared data size
    int   size      ( ) const;

    /*!
     * @brief write the data into triple buffer [ writer spec. ]
     * @param [in] data, the source data
     * @param [in] data_size, the source data size count in bytes
     * @return true for wrote.
     */
    bool  write  ( const char *data, int data_size );

    /*!
     * @brief read from triple buffer [ reader spec. ]
     * @param [in] is_ignore_dirty, this flag decide the result.
     * @return non null pointer means got data.
     * @note  if is_ignore_dirty is true, may return last data pointer
     */
    const void* read( bool is_ignore_dirty = false );

    /*!
     * @brief wait for the data has been arrived. [ not implement ]
     * @param [in] time out by million seconds.
     * @return true means existed new data.
     */
    bool  waitForReading( int ms = -1 );


    Q_SIGNAL void  dataWrote( bool is_old_picked );

private:
    void *m_obj;
    Q_DISABLE_COPY( IcTripBuffIpc )
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
