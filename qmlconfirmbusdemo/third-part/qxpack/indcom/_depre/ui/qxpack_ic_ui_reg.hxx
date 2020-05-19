#ifndef QXPACK_IC_UI_REG_HXX
#define QXPACK_IC_UI_REG_HXX

#include <qxpack/indcom/common/qxpack_ic_def.h>
#include <QObject>
#include <QCoreApplication>

#include <QQmlEngine>

namespace QxPack {

// ////////////////////////////////////////////////////
/*!
 * @brief this class register some value into QML engine
 * @details \n
 *   the registered name list below: \n
 *   1) use "import qxpack.indcom.ui.api 1.0" in QML to import, \n
 *      use "QxIcUiApi" to access APIs 
 */
// ////////////////////////////////////////////////////
class QXPACK_IC_API  IcUiReg : public QObject {
    Q_OBJECT
public:
    IcUiReg( );
    virtual ~IcUiReg( );

private:
    void *m_obj;
    Q_DISABLE_COPY( IcUiReg )
};


}

#endif
