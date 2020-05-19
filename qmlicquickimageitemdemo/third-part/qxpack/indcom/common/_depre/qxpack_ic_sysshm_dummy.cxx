#ifndef QXPACK_IC_SYSSHM_DUMMY_CXX
#define QXPACK_IC_SYSSHM_DUMMY_CXX

#include "qxpack_ic_sysshm.hxx"

namespace QxPack {

// ============================================================
// CTOR
// ============================================================
     IcSysShm :: IcSysShm () { m_obj = nullptr; }

// ============================================================
// CTOR, attach to the specified name shared memory
// ============================================================
     IcSysShm :: IcSysShm ( const char *name )
{
     QXPACK_IC_UNUSED( name );
     m_obj = nullptr;
}

// ============================================================
// CTOR with parameter
// ============================================================
     IcSysShm :: IcSysShm ( int size, const char *name )
{
    QXPACK_IC_UNUSED(size); QXPACK_IC_UNUSED(name);
    m_obj = nullptr;
}

// ===========================================================
// DTOR
// ===========================================================
    IcSysShm :: ~IcSysShm( ) { }

// ===========================================================
// initalize the shared memory
// ===========================================================
bool  IcSysShm :: init ( int size, const char *name )
{
    QXPACK_IC_UNUSED( size ); QXPACK_IC_UNUSED( name );
    return false;
}

// ===========================================================
// attach to the shared memory
// ===========================================================
bool  IcSysShm :: init ( const char *name )
{
    QXPACK_IC_UNUSED( name );
    return false;
}

// ===========================================================
// detach from the shared memory
// ===========================================================
void  IcSysShm :: deinit( ) { }

// ===========================================================
// check if syssem is work
// ===========================================================
bool  IcSysShm :: isWorked( ) const { return false; }

// ===========================================================
// return the size of the shared memory
// ===========================================================
int   IcSysShm :: size( ) const { return 0; }

// ===========================================================
// return the data pointer of the shared memory
// ===========================================================
void*  IcSysShm :: data( ) { return nullptr; }


}

#endif
