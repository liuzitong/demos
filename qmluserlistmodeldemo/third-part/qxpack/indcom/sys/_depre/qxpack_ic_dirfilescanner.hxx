// /////////////////////////////////////////////////////////////////////
/*!
  @file    qxpack_ic_dirfilescanner
  @author  night wing
  @date    2018/09
  @brief   scan the files in the directory
  @version 0.1.0.0
*/
// /////////////////////////////////////////////////////////////////////
#ifndef QXPACK_IC_DIRFILESCANNER_HXX
#define QXPACK_IC_DIRFILESCANNER_HXX

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
#include <QVector>
#include <qxpack/indcom/common/qxpack_ic_def.h>
#include <qxpack/indcom/sys/qxpack_ic_vardata.hxx>
#include <qxpack/indcom/sys/qxpack_ic_dirfilehandlerbase.hxx>

namespace QxPack {

// ////////////////////////////////////////////////////////
/*!
 * @brief scanner of a directory [ priv-worker-thread ] 
 * @details this class inner used thread to process all needed \n
 *     tasks. User can focus on the signals and callback handlers.
 */
// ////////////////////////////////////////////////////////
class QXPACK_IC_API IcDirFileScanner : public QObject {
    Q_OBJECT
public:
    /*!
     * @brief ctor
     * @param [in] pattern, the pattern string table. e.g "*.jpg"
     * @param [in] ldr_cr,  the loader creator callback function.
     * @note  the LoaderCreator will create a loader used in this class. \n
     *   And the Loader will be deleted after the inner object destoryed.
     */
    IcDirFileScanner (
        const QStringList & pattern, IcDirFileHandlerBase::Factory ldr_fact = Q_NULLPTR, QObject *pa = Q_NULLPTR
    );

    //! ctor
    virtual ~IcDirFileScanner ( );

    /*!
     * @brief setup work path
     * @note  once setup, it will emit workPathChanged(), can cause the data model  \n
     *       reset its content.
     */
    void     setWorkPath( const QString &fp );

    //! return current work path.
    QString  workPath( ) const;

    /*!
     * @brief emited while call setWorkPath()
     * @details  this signal implicit IcImageListModel() should reset.
     */
    Q_SIGNAL void  workPathChanged ( const QString &wk_path );

    //! emited while data source added more data
    Q_SIGNAL void  dataAdded   ( const QVector<QxPack::IcVarData> &data_list );

    //! emited while data source removed these data
    Q_SIGNAL void  dataRemoved ( const QVector<QxPack::IcVarData> &data_list );

    //! notice model, the a data has been loaded
    Q_SIGNAL void  dataLoaded    ( const QxPack::IcVarData &data );

    //! notice model, the data failed to loaded
    Q_SIGNAL void  dataLoadFailed( const QxPack::IcVarData &data );

    //! handle the load data request from model.
    Q_SLOT   void  onReqLoadData ( const QxPack::IcVarData &data );

private:
    void *m_obj;
    Q_DISABLE_COPY( IcDirFileScanner )
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
