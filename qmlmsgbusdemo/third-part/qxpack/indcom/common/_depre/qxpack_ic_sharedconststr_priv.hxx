#ifndef QXPACK_IC_SHAREDCONSTSTR_PRIV_HXX
#define QXPACK_IC_SHAREDCONSTSTR_PRIV_HXX

#include <qxpack/indcom/common/qxpack_ic_def.h>
#include <qxpack/indcom/common/qxpack_ic_bytearray.hxx>

namespace QxPack {

// //////////////////////////////////////////////////////
/*!
 * @brief   this class manage the shared const string
 */
// //////////////////////////////////////////////////////
class QXPACK_IC_HIDDEN  IcSharedConstStrPriv {
public:
    /*!
     * @brief ctor
     * @param [in] name, the shared name
     * @note  the same name class use shared string table
     */
    IcSharedConstStrPriv ( const IcByteArray &name );

    //! dtor
    virtual ~IcSharedConstStrPriv( );

    /*!
     * @brief add the string atom value
     * @param [in] str, the const string pointer
     * @return a atom value in the table.
     * @note  if same string in the talbe, it's ref-counter will
     *    be added.
     */
    intptr_t  add  ( const IcByteArray &str );

    /*!
     * @brief query the string atom value
     * @param [in] str,
     */
    intptr_t  query ( const IcByteArray &str );

    /*!
     * @brief remove the string from table
     * @param [in] str, the string need to be removed
     * @note removed the string only it's ref-counter is zero.
     */
    void      remove( const IcByteArray &str );

    /*!
     * @brief  query the string by atom value
     * @return if not existed, return NULL object
     */
    IcByteArray  string( intptr_t atom );

private:
    void *m_obj;
    QXPACK_IC_DISABLE_COPY( IcSharedConstStrPriv )
};


}

#endif
