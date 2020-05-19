// /////////////////////////////////////////////////////////////////////
/*!
  @file    qxpack_ic_fileloader
  @author  night wing
  @date    2018/09
  @brief   load the file data
  @version 0.1.0.0
*/
// /////////////////////////////////////////////////////////////////////
#ifndef QXPACK_IC_FILELOADER_HXX
#define QXPACK_IC_FILELOADER_HXX

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

#include <qxpack/indcom/common/qxpack_ic_def.h>
#include <QObject>
#include <QString>

namespace QxPack {

// ////////////////////////////////////////////////////////
/*!
 * @brief load all the file into memory
 */
// ////////////////////////////////////////////////////////
class QXPACK_IC_API  IcFileLoader : public QObject {
    Q_OBJECT
public :
    //! construct by file name
    IcFileLoader( const QString &file_path );
 
    virtual ~IcFileLoader( );

    //! return the file loader name 
    virtual const char *  loaderName( ) const = 0;

    //! is loaded
    virtual bool   isLoaded( ) const = 0;

private:
    void *m_obj;
    Q_DISABLE_COPY( IcFileLoader )
};

}


#include <QByteArray>

namespace QxPack {

// //////////////////////////////////////////////////////
/*!
 * @brief binary data loader
 */
// ////////////////////////////////////////////////////// 
class QXPACK_IC_API  IcFileBinaryLoader : public IcFileLoader {
    Q_OBJECT
public :
    
    /*!
     * @brief ctor
     * @param [in] file_path, the file path this class want to load
     */
    IcFileBinaryLoader( const QString &file_path );

    virtual ~IcFileBinaryLoader( );

    //! return the current loader name 'IcFileBinaryLoader'
    const char *  loaderName( ) const Q_DECL_OVERRIDE;

    //! check if the data is loaded
    bool    isLoaded( ) const Q_DECL_OVERRIDE;

    //! return the result 
    QByteArray    result( );

private:
    void       *m_obj;
    bool        m_is_loaded;
    QByteArray  m_ba;
    Q_DISABLE_COPY( IcFileBinaryLoader )
};

}


#include <QString>

namespace QxPack {

// /////////////////////////////////////////////////////
/*!
 * @brief text data loader
 */
// /////////////////////////////////////////////////////
class QXPACK_IC_API  IcFileTextLoader : public IcFileLoader {
    Q_OBJECT
public:
    /*!
     * @brief ctor
     * @param [in] file_path, the file path this class want to load
     */
    IcFileTextLoader ( const QString &file_path );

    virtual ~IcFileTextLoader ( );

    //! return the current loader name 'IcFileTextLoader'
    const char *   loaderName() const Q_DECL_OVERRIDE;

    //! check if the data is loaded
    bool    isLoaded ( ) const Q_DECL_OVERRIDE;

    //! return the result
    QString  result( );

private:
    void    *m_obj;
    bool     m_is_loaded;
    QString  m_str;
    Q_DISABLE_COPY( IcFileTextLoader )
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
