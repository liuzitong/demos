#ifndef QXPACK_IC_OCLVECTOR4D_HXX
#define QXPACK_IC_OCLVECTOR4D_HXX

#include <qxpack/indcom/common/qxpack_ic_def.h>
#include <stdint.h>

namespace QxPack {

// ///////////////////////////////////////////////
//
// OpenCL vector4D descriptor
//
// ///////////////////////////////////////////////
class QXPACK_IC_API  IcOclVector4D {

private:
    int m_elem[4];
protected:
    inline int safeIndex( int i ) const { return ( i >= 0 && i < ( sizeof( m_elem ) / sizeof( int )) ? i : 3 ); }
public :
    IcOclVector4D( ) { m_elem[0] = m_elem[1] = m_elem[2] = m_elem[3] = 0; }

    IcOclVector4D( int x, int y, int z, int w ) { m_elem[0] = x; m_elem[1] = y; m_elem[2] = z; m_elem[3] = w; }

    IcOclVector4D( const IcOclVector4D &other )
    { m_elem[0] = other.m_elem[0]; m_elem[1] = other.m_elem[1]; m_elem[2] = other.m_elem[2];  }

    IcOclVector4D & operator = ( const IcOclVector4D &other )
    { m_elem[0] = other.m_elem[0]; m_elem[1] = other.m_elem[1]; m_elem[2] = other.m_elem[2]; return *this; }

    virtual ~IcOclVector4D( ) { }

    inline int  size() const { return ( sizeof( m_elem ) / sizeof( int )); }
    inline int  x() const { return m_elem[0]; }
    inline int  y() const { return m_elem[1]; }
    inline int  z() const { return m_elem[2]; }
    inline int  w() const { return m_elem[3]; }
    inline void setX( int x ) { m_elem[0] = x; }
    inline void setY( int y ) { m_elem[1] = y; }
    inline void setZ( int z ) { m_elem[2] = z; }
    inline void setW( int w ) { m_elem[3] = w; }

    inline int& operator[] ( int i ) { return m_elem[ this->safeIndex( i ) ]; }
    inline const int & operator[] ( int i ) const { return m_elem[ this->safeIndex( i ) ]; }
};



}
#endif
