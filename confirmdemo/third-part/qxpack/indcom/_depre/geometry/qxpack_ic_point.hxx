// /////////////////////////////////////////////////////////////////////
/*!
  @file    qxpack_ic_Point
  @author  night wing
  @date    2018/11
  @brief   Point object, describe a 2D Point
  @version 0.1.0
*/
// /////////////////////////////////////////////////////////////////////
#ifndef QXPACK_IC_Point_HXX
#define QXPACK_IC_Point_HXX

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
#include <cassert>

namespace QxPack {

// ////////////////////////////////////////////////////////
/*!
 * @brief integer object
 */
// ////////////////////////////////////////////////////////
class QXPACK_IC_API  IcPoint {
public:
    IcPoint( )                  { m_x = m_y = 0; }
    IcPoint( const IcPoint &p ) { m_x = p.m_x; m_y = p.m_y; }
    IcPoint( int x, int y     ) { m_x = x; m_y = y; }
    IcPoint& operator=( const IcPoint &p ) { m_x = p.m_x; m_y = p.m_y; return *this; }
    ~IcPoint( ) { }

    inline bool isNull() const { return ( m_x == 0 && m_y == 0 ); }

    inline int x() const { return m_x; }
    inline int y() const { return m_y; }
    inline int & rx()    { return m_x; }
    inline int & ry()    { return m_y; }
    inline void setX( int x ) { m_x = x; }
    inline void setY( int y ) { m_y = y; }
    
    inline IcPoint& operator+=( const IcPoint &p ) { m_x += p.m_x; m_y += p.m_y; return *this; }
    inline IcPoint& operator-=( const IcPoint &p ) { m_x -= p.m_x; m_y -= p.m_y; return *this; }
    inline IcPoint& operator*=( int c )    { m_x *= c; m_y *= c; return *this;  }
    inline IcPoint& operator*=( double c ) { m_x = QxPack::round( m_x * c ); m_y = QxPack::round( m_y * c ); return *this; }
    inline IcPoint& operator/=( int c )    { m_x /= c; m_y /= c; return *this;  }
    inline IcPoint& operator/=( double c ) { m_x = QxPack::round( m_x / c ); m_y = QxPack::round( m_y / c ); return *this; }

    friend inline bool operator==( const IcPoint &p1, const IcPoint &p2 ) { return ( p1.m_x == p2.m_x  &&  p1.m_y == p2.m_y ); }
    friend inline bool operator!=( const IcPoint &p1, const IcPoint &p2 ) { return ( p1.m_x != p2.m_x  ||  p1.m_y != p2.m_y ); }
    friend inline const IcPoint operator+( const IcPoint &p1, const IcPoint &p2 ) { return IcPoint( p1.m_x + p2.m_x, p1.m_y + p2.m_y ); }
    friend inline const IcPoint operator-( const IcPoint &p1, const IcPoint &p2 ) { return IcPoint( p1.m_x - p2.m_x, p1.m_y - p2.m_y ); }
    friend inline const IcPoint operator*( const IcPoint &p1, double m ) { return IcPoint( QxPack::round( p1.m_x * m ), QxPack::round( p1.m_y * m )); }
    friend inline const IcPoint operator*( double m, const IcPoint &p1 ) { return IcPoint( QxPack::round( p1.m_x * m ), QxPack::round( p1.m_y * m )); }
    friend inline const IcPoint operator*( const IcPoint &p1, int  m   ) { return IcPoint( p1.m_x * m, p1.m_y * m ); }
    friend inline const IcPoint operator*( int    m, const IcPoint &p1 ) { return IcPoint( p1.m_x * m, p1.m_y * m ); }
    friend inline const IcPoint operator+( const IcPoint &p )            { return p; }
    friend inline const IcPoint operator-( const IcPoint &p )            { return IcPoint( - p.m_x, - p.m_y ); }
    friend inline const IcPoint operator/( const IcPoint &p, int d )     { return IcPoint( p.m_x / d, p.m_y / d ); }
    friend inline const IcPoint operator/( const IcPoint &p, double d )  { return IcPoint( QxPack::round( p.m_x / d ), QxPack::round( p.m_y / d ) ); }

private:
    int m_x, m_y;
};


// /////////////////////////////////////////////////////////////
/*!
 * @brief IcPoint 2D float
 */
// /////////////////////////////////////////////////////////////
class QXPACK_IC_API  IcPointF {
public:
    IcPointF( ) { m_x = m_y = 0; }
    IcPointF( const IcPointF &p ) { m_x = p.m_x; m_y = p.m_y; }
    IcPointF( float x, float y )  { m_x = x; m_y = y; }
    IcPointF& operator=( const IcPointF &p ) { m_x = p.m_x; m_y = p.m_y; return *this; }
    ~IcPointF( ) { }

    inline bool isNull( ) const { return QxPack::isNull( m_x ) && QxPack::isNull( m_y ); }

    inline float  x() const { return m_x; }
    inline float  y() const { return m_y; }
    inline float& rx()      { return m_x; }
    inline float& ry()      { return m_y; }
    inline void setX( float x ) { m_x = x; }
    inline void setY( float y ) { m_y = y; }

    inline IcPointF& operator+=( const IcPointF &p ) { m_x += p.m_x;  m_y += p.m_y;  return *this; }
    inline IcPointF& operator-=( const IcPointF &p ) { m_x -= p.m_x;  m_y -= p.m_y;  return *this; }
    inline IcPointF& operator*=( float c ) { m_x *= c; m_y *= c; return *this; }
    inline IcPointF& operator/=( float c ) { m_x /= c; m_y /= c; return *this; }

    friend inline bool operator==( const IcPointF &p1, const IcPointF &p2 ) { return  QxPack::fuzzyCompare( p1.m_x, p2.m_x ) &&  QxPack::fuzzyCompare( p1.m_y, p2.m_y ); }
    friend inline bool operator!=( const IcPointF &p1, const IcPointF &p2 ) { return !QxPack::fuzzyCompare( p1.m_x, p2.m_x ) || !QxPack::fuzzyCompare( p1.m_y, p2.m_y ); }
    friend inline const IcPointF operator+( const IcPointF &p1, const IcPointF &p2 ) { return IcPointF( p1.m_x + p2.m_x, p1.m_y + p2.m_y ); }
    friend inline const IcPointF operator-( const IcPointF &p1, const IcPointF &p2 ) { return IcPointF( p1.m_x - p2.m_x, p1.m_y - p2.m_y ); }
    friend inline const IcPointF operator*( const IcPointF &p1, float m ) { return IcPointF( p1.m_x * m, p1.m_y * m ); }
    friend inline const IcPointF operator*( float m, const IcPointF &p1 ) { return IcPointF( p1.m_x * m, p1.m_y * m ); }
    friend inline const IcPointF operator+( const IcPointF &p ) { return p; }
    friend inline const IcPointF operator-( const IcPointF &p ) { return IcPointF( - p.m_x, - p.m_y ); }
    friend inline const IcPointF operator/( const IcPointF &p, float d )  { return IcPointF( p.m_x / d, p.m_y / d ); }

private:
    float m_x, m_y;
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
