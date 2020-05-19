#ifndef QXPACK_IC_TIMEDOBJCACHE_HXX
#define QXPACK_IC_TIMEDOBJCACHE_HXX

#include "qxpack/indcom/common/qxpack_ic_def.h"
#include "qxpack/indcom/common/qxpack_ic_objectcache.hxx"
#include <QObject>

namespace QxPack {

// ////////////////////////////////////////////////////////////////////////////
/*!
 * @brief the timed object cache, it provide a timed cleanup mechnique
 */
// ////////////////////////////////////////////////////////////////////////////
class QXPACK_IC_API  IcTimedObjCache : public QObject {
    Q_OBJECT
public:
    /*!
     * @brief ctor
     * @param rt_ms   [in] time of the cleanup routine called in million-seconds
     * @param cb      [in] alloc or free object callback function
     * @param cb_obj  [in] context of callback function
     * @param max_num [in] maxinum cached object in default, -1 means no limit
     */
    explicit IcTimedObjCache (
        IcObjectCache::Callback cb, void *cb_obj,
        int max_num = 100, int rt_ms = 1000, QObject *pa = Q_NULLPTR
    );

    //! dtor
    virtual ~IcTimedObjCache( ) Q_DECL_OVERRIDE;

    /*!
     *  @return  a allocated object, nullptr means failed.
     */
    void *   allocate ( );

    /*!
     *  @param [in] obj -- the object that will be cached or free
     *  @note  the object will be cache or freed.
     *  @warning in some scene, user should release some singleton resources.
     */
    void     recycle  ( void *obj );

private:
    void  *m_obj;
    Q_DISABLE_COPY( IcTimedObjCache )
};

}

#endif
