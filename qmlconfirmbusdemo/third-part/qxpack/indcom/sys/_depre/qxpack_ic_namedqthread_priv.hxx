#ifndef QXPACK_IC_NAMEDQTHREAD_PRIV_HXX
#define QXPACK_IC_NAMEDQTHREAD_PRIV_HXX

#include <qxpack/indcom/common/qxpack_ic_def.h>
#include <QThread>
#include <QObject>
#include <QString>

namespace QxPack {

// ////////////////////////////////////////////////////////
/*!
 * @brief manage the QThread by name
 */
// ////////////////////////////////////////////////////////
class QXPACK_IC_HIDDEN  IcNamedQThreadPriv {

public:
    /*!
     * @brief CTOR
     * @param [in] name, the name of this object.
     * @note  if the name is not existed in the pool, \n
     *    create new QThread, otherwise, shared the QThread.
     */
    IcNamedQThreadPriv( const QString &name );

    //! dtor
    virtual ~IcNamedQThreadPriv( );

    //! check if it is null
    bool      isNull () const;

    //! name of this object
    QString   name() const;

    //! @return the owner qthread.
    QThread *  thread() const;

    /*!
     * @brief the thread name of local level 0, level 1 thread...
     */
    static QString  sysThreadName( int id = -1 ); // 0, 1 etc..

private:
    void    *m_obj;
    QString  m_name;
    Q_DISABLE_COPY( IcNamedQThreadPriv )
};

}

#endif
