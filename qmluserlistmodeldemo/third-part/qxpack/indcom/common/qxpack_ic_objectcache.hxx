// ////////////////////////////////////////////////////////////////////////////
/*!
  @file    qxpack_ic_objectcache
  @author  night wing
  @date    2019/04/29
  @brief   cache the object
  @version 0.2.0.0
*/
// ////////////////////////////////////////////////////////////////////////////
#ifndef QXPACK_IC_OBJECTCACHE_HXX
#define QXPACK_IC_OBJECTCACHE_HXX

#include "qxpack/indcom/common/qxpack_ic_def.h"

namespace QxPack {

// ////////////////////////////////////////////////////////////////////////////
//                         [ Thread Safe Class ]
/*!
 * @brief this class used for caching the user defined objects
 */
// ////////////////////////////////////////////////////////////////////////////
class QXPACK_IC_API  IcObjectCache {

public:
    //! enumeric value about Callback handlerType
    enum HandlerType {
        HandlerType_Alloc,  //!< means Callback function should create an object
        HandlerType_Free    //!< means Callback function should free the object
    };

    //! callback function, used to do allocate or free the object by user.
    typedef void (* Callback )( void *ctxt, void **obj_ptr, HandlerType h_type );

    /*!
     *  @param [in] cb      --  callback function, used to alloc or free the object
     *  @param [in] cb_obj  --  callback parameter, passed back by cb
     *  @param [in] max_num --  the cache size, -1 means no limit.
     */
    explicit IcObjectCache ( Callback cb, void *cb_obj, int max_num = -1 );

    /*!
     *  @note  if existed some free object, it will be passed by \n
     *         'cb' function that registered at CTOR.
     *  @see IcObjectCache
     */
    virtual ~IcObjectCache( );

    /*!
     *  @return  current snapped object count
     */
    int      count ( ) const;

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

    /*!
     *  @brief do free the oldest object that over the limit time stamp.
     *  @param [in] limit_mill -- how many milliseconds the object' alive.
     *  @note  the object's life time stamp is generated while call recycle().\n
     *
     */
    void     freeOldest( int limit_mill );

private:
    void *m_obj;
    QXPACK_IC_DISABLE_COPY( IcObjectCache )
};

}

#endif
