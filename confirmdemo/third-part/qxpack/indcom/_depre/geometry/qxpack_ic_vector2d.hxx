// /////////////////////////////////////////////////////////////////////
/*!
  @file    qxpack_ic_vector
  @author  night wing
  @date    2018/11
  @brief   vector2d object, describe a 2D vector
  @version 0.1.0
*/
// /////////////////////////////////////////////////////////////////////
#ifndef QXPACK_IC_VECTOR2D_HXX
#define QXPACK_IC_VECTOR2D_HXX

/*!
  @addtogroup  QxPack
  @{
*/
/*!
  @addtogroup  indcom
  @{
*/
/*!
  @addtogroup  gemotry_Exported_Classes
  @{
*/

#include <qxpack/indcom/common/qxpack_ic_def.h>
#include <qxpack/indcom/common/qxpack_ic_global.hxx>
#include <qxpack/indcom/geometry/qxpack_ic_point.hxx>
#include <cassert>


namespace QxPack {

// ////////////////////////////////////////////////////////
/*!
 * @brief a float vector2d version
 */
// ////////////////////////////////////////////////////////
class QXPACK_IC_API  IcVector2D {
public:
    IcVector2D( ) : xp(0), yp(0) { }
    IcVector2D( float x, float y ) : xp(x), yp(y) { }
    IcVector2D( const IcPoint  &pt ) : xp(pt.x()), yp(pt.y()){ }
    IcVector2D( const IcPointF &pt ) : xp(pt.x()), yp(pt.y()){ }
    IcVector2D( const IcVector2D &s ) : xp(s.xp), yp(s.yp) { }
    IcVector2D &  operator = ( const IcVector2D &s )  { xp = s.xp; yp = s.yp; return *this;  }
    ~IcVector2D() { }

    inline IcPoint  toPoint() const { return IcPoint( QxPack::round(xp), QxPack::round(yp));}
    inline IcPointF toPointF()const { return IcPointF( xp, yp ); }

    inline bool isNull( ) const { return QxPack::isNull( xp ) && QxPack::isNull( yp ); }

    inline float  x() const { return xp; }
    inline float  y() const { return yp; }
    inline float& rx() { return xp; }
    inline float& ry() { return yp; }

    inline void  setX( float x ) { xp = x; }
    inline void  setY( float y ) { yp = y; }

           float length () const;
    inline float lengthSquared() const { return xp * xp + yp * yp; }

    IcVector2D normalized( ) const;
    void       normalize ( );
    float      distanceToPoint( const IcVector2D &pt ) const;
    float      distanceToLine ( const IcVector2D &pt, const IcVector2D &direction) const;

    inline float& operator[]( int i ) { assert( unsigned int(i) < 2u ); return *( &xp + i ); }
    inline float  operator[]( int i ) { assert( unsigned int(i) < 2u ); return *( &xp + i ); }

    inline IcVector2D& operator+=( const IcVector2D &v ) { xp += v.xp; yp += v.yp; return *this; }
    inline IcVector2D& operator-=( const IcVector2D &v ) { xp -= v.xp; yp -= v.yp; return *this; }
    inline IcVector2D& operator*=( float f ) { xp *= f; yp *= f; return *this; }
    inline IcVector2D& operator/=( float d ) { xp /= d; yp /= d; return *this; }
    inline IcVector2D& operator*=( const IcVector2D &v ) { xp *= v.xp; yp *= v.yp; return *this; }
    inline IcVector2D& operator/=( const IcVector2D &v ) { xp /= v.xp; yp /= v.yp; return *this; }

    inline static float dotProduct  ( const IcVector2D &v1, const IcVector2D &v2 ) { return v1.xp * v2.xp + v1.yp * v2.yp; }
    inline static float crossProduct( const IcVector2D &v1, const IcVector2D &v2 ) { return v1.xp * v2.yp - v1.yp * v2.xp; }

    friend inline bool operator==( const IcVector2D &v1, const IcVector2D &v2 ) { return  QxPack::fuzzyCompare( v1.xp, v2.xp ) &&  QxPack::fuzzyCompare( v1.yp, v2.yp ); }
    friend inline bool operator!=( const IcVector2D &v1, const IcVector2D &v2 ) { return !QxPack::fuzzyCompare( v1.xp, v2.xp ) || !QxPack::fuzzyCompare( v1.yp, v2.yp ); }
    friend inline const IcVector2D operator+( const IcVector2D &v1, const IcVector2D &v2 ) { return IcVector2D( v1.xp + v2.xp, v1.yp + v2.yp ); }
    friend inline const IcVector2D operator-( const IcVector2D &v1, const IcVector2D &v2 ) { return IcVector2D( v1.xp - v2.xp, v1.yp - v2.yp ); }
    friend inline const IcVector2D operator*( const IcVector2D &v, float fact ) { return IcVector2D( v.xp * fact, v.yp * fact ); }
    friend inline const IcVector2D operator*( float fact, const IcVector2D &v ) { return IcVector2D( v.xp * fact, v.yp * fact ); }
    friend inline const IcVector2D operator*( const IcVector2D &v1, const IcVector2D &v2 ) { return IcVector2D( v1.xp * v2.xp, v2.yp * v2.yp ); }
    friend inline const IcVector2D operator-( const IcVector2D &v ) { return IcVector2D( -v.xp, -v.yp ); }
    friend inline const IcVector2D operator+( const IcVector2D &v ) { return p; }
    friend inline const IcVector2D operator/( const IcVector2D &v, float d ) { return IcVector2D( v.xp / d, v.yp / d ); }
    friend inline const IcVector2D operator/( const IcVector2D &v, const IcVector2D &d ) { return IcVector2D( v.xp / d.xp, v.yp / d.yp ); }

private:
    float xp, yp;
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
