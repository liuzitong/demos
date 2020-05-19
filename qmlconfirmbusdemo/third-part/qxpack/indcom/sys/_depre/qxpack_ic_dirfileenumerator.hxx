// /////////////////////////////////////////////////////////////////////
/*!
  @file    qxpack_ic_dirfilescanner
  @author  night wing
  @date    2018/11
  @brief   enumerate the files in the directory
  @version 0.1.0.0
*/
// /////////////////////////////////////////////////////////////////////
#ifndef QXPACK_IC_DIRFILEENUMERATOR_HXX
#define QXPACK_IC_DIRFILEENUMERATOR_HXX

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

namespace QxPack {

// //////////////////////////////////////////////////////////
/*!
 * @brief enumerate the directory files [ priv-worker-thread ]
 * @details this class used to enumerate all files that match \n
 *   user rule.
 */
// //////////////////////////////////////////////////////////
class QXPACK_IC_API IcDirFileEnumerator : public QObject {
    Q_OBJECT
public:
    /*!
     * @brief ctor
     * @param [in] pattern, the pattern string table. e.g "*.jpg"
     * @param [in] enum_sub_dir,  if it is true, will enter ever sub_dir.
     */
    IcDirFileEnumerator (
        const QStringList &pattern, const QString &dir, bool enum_sub_dir,
        QObject *pa = Q_NULLPTR
    );

    //! ctor
    virtual ~IcDirFileEnumerator ( );

    //! emited while data source added more data
    Q_SIGNAL void  dataFound ( const QVector<QString> &data_list );

    //! finished signal
    Q_SIGNAL void  finished( );

private:
    void *m_obj;
    Q_DISABLE_COPY( IcDirFileEnumerator )
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

