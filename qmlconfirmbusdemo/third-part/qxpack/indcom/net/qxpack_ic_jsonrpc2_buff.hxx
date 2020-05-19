#ifndef QXPACK_IC_JSONRPC2_BUFF_HXX
#define QXPACK_IC_JSONRPC2_BUFF_HXX

#include "qxpack/indcom/common/qxpack_ic_def.h"
#include "qxpack/indcom/net/qxpack_ic_datatrans_buff.hxx"
#include <QObject>
#include <QIODevice>
#include <QByteArray>

namespace QxPack {

// ////////////////////////////////////////////////////////////////////////////
/*!
 * @brief provide a buffer to splicing JSONRPC2 package ( default )
 * @details  the Stream device is not split package, we should use a separator string \n
 *  to split every package.\n
 */
// ////////////////////////////////////////////////////////////////////////////
class QXPACK_IC_API  IcJsonRpc2PkgReadBuff : public IcDataTransPkgReadBuff {
    Q_OBJECT
public:
    //! ctor
    explicit IcJsonRpc2PkgReadBuff ( QSharedPointer<QIODevice> &, QObject *pa = Q_NULLPTR );

    //! dtor
    virtual ~IcJsonRpc2PkgReadBuff ( ) Q_DECL_OVERRIDE;

    //! override, analysis the data
    virtual bool analyData( const QByteArray &ba, AnalyInfo &ai ) Q_DECL_OVERRIDE;

private:
    void *m_obj;
    Q_DISABLE_COPY( IcJsonRpc2PkgReadBuff )
};


// ////////////////////////////////////////////////////////////////////////////
/*!
 * @brief provide a inner buffer to write all posted data
 */
// ////////////////////////////////////////////////////////////////////////////
class QXPACK_IC_API IcJsonRpc2PkgWriteBuff : public IcDataTransPkgWriteBuff {
    Q_OBJECT
public:
    //! ctor
    explicit IcJsonRpc2PkgWriteBuff( QSharedPointer<QIODevice> &, QObject *pa = Q_NULLPTR );
    
    //! dtor
    virtual ~IcJsonRpc2PkgWriteBuff( ) Q_DECL_OVERRIDE;
    
    //! override the packData
    virtual bool packData(const QByteArray &dat, QVector<QPair<bool, QByteArray> > &ba_list) Q_DECL_OVERRIDE;

private:
    void *m_obj;
    Q_DISABLE_COPY( IcJsonRpc2PkgWriteBuff )
};

// ////////////////////////////////////////////////////////////////////////////
/*!
 * @brief default jsonrpc2 factory
 */
// ////////////////////////////////////////////////////////////////////////////
class QXPACK_IC_API  IcJsonRpc2PkgDefaultFact {
private:
    void *m_obj; IcDataTransFactory m_ext_fact; void *m_ext_ctxt;
protected:
    QVariant  doFact( const QString &, const QVariant& );
public :
    explicit IcJsonRpc2PkgDefaultFact( IcDataTransFactory e_fact, void *e_ctxt );
    virtual ~IcJsonRpc2PkgDefaultFact( );
    static QVariant factory( void*, const QString &, const QVariant& );

    // 20190724-1059 added
    inline IcDataTransFactory  extFact( ) { return m_ext_fact; }
    inline void *  extCtxt( ) { return m_ext_ctxt; }
};

}

#endif
