#ifndef QXPACK_IC_SPOOKYHASHV2_CXX
#define QXPACK_IC_SPOOKYHASHV2_CXX

#include "qxpack_ic_spookyhashv2.hxx"
#include <qxpack/indcom/common/qxpack_ic_memcntr.hxx>

// include the source for compile
#include <qxpack/indcom/third-part/spookyhash/SpookyV2.cpp>

namespace QxPack {

// /////////////////////////////////////////////////////////////
//
//  spooky hash 32
//  spooky_hash.hash32("hello world")  2617184861
//  spooky_hash.hash64("hello world")  14865987102431973981
//  spooky_hash.Hash64("hello ").update("world")  14865987102431973981
//                                                (5d12ff9b81984ece)
//  spooky_hash.hash128("hello world") 123716849286372619103118623513034416523
//                                     ( 5d12ff9b81984ece25103f0dee88e18b )
//
//  spooky_hash.hash32("hello world", seed=4)  4130951021
//  spooky_hash.hash64("hello world", seed=8)  hex: e54162c401e00c21
//  spooky_hash.hash128("hello world", seed1=15,seed2=16) hex:1d46e376a416468b6c5c3a8f3798042b
// /////////////////////////////////////////////////////////////
// =============================================================
// CTOR
// =============================================================
    IcSpookyHashV2 :: IcSpookyHashV2 ( )
{
//    m_obj = qxpack_ic_new( SpookyHash );
}

// =============================================================
// DTOR
// =============================================================
    IcSpookyHashV2 :: ~IcSpookyHashV2 ( )
{
//    if ( m_obj != nullptr ) {
//        qxpack_ic_delete( m_obj, SpookyHash );
//        m_obj = nullptr;
//    }
}

// =============================================================
// calc. 32bit hash value directly [ static ]
// =============================================================
uint32_t  IcSpookyHashV2 :: hash32Di( const IcByteArray &ba )
{
    return ( uint32_t ) SpookyHash::Hash32( ba.constData(), ba.size(), 0 );
}

}

#endif
