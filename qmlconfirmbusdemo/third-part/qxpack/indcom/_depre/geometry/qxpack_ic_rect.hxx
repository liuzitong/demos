// /////////////////////////////////////////////////////////////////////
/*!
  @file    qxpack_ic_recct
  @author  night wing
  @date    2018/11
  @brief   rect object, describe a 2D point
  @version 0.1.0
*/
// /////////////////////////////////////////////////////////////////////
#ifndef QXPACK_IC_RECT_HXX
#define QXPACK_IC_RECT_HXX

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
#include <qxpack/indcom/geometry/qxpack_ic_point.hxx>
#include <qxpack/indcom/geometry/qxpack_ic_size.hxx>
#include <QRect>
namespace QxPack {

// ///////////////////////////////////////////////////////
/*!
 * @brief Rectangle ( integer version )
 */
// ///////////////////////////////////////////////////////
class QXPACK_IC_API  IcRect {
public :
    IcRect( ) : x1(0), y1(0), x2(-1), y2(-1) { }
    IcRect( int left, int top, int width, int height ) { x1 = left; y1 = top; x2 = left + width - 1; y2 = top + height - 1; }

    IcRect( const IcPoint &top_left, const IcPoint &bottom_right )
    { x1=top_left.x(); y1=top_left.y(); x2=bottom_right.x(); y2=bottom_right.y(); }

    IcRect( const IcPoint &top_left, const IcSize &size )
    { x1=top_left.x(); y1=top_left.y(); x2=top_left.x() + size.width() - 1; y2 = top_left.y() + size.height() - 1; }

    inline bool  isNull() const { return x2 == x1 -1 && y2 == y1 - 1; }
    inline bool  isEmpty()const { return x1 >  x2 || y1 >  y2; }
    inline bool  isValid()const { return x1 <= x2 && y1 <= y2; }

    inline int   left()   const { return x1; }
    inline int   top()    const { return y1; }
    inline int   right()  const { return x2; }
    inline int   bottom() const { return y2; }
    inline int   x()      const { return x1; }
    inline int   y()      const { return y1; }
    inline int   width()  const { return x2 - x1 + 1; }
    inline int   height() const { return y2 - y1 + 1; }

    inline IcPoint  center()     const { return IcPoint( int((int64_t(x1)+x2)/2), int((int64_t(y1)+y2)/2) ); }
    inline IcPoint  topLeft()    const { return IcPoint( x1, y1 ); }
    inline IcPoint  bottomRight()const { return IcPoint( x2, y2 ); }
    inline IcPoint  topRight()   const { return IcPoint( x2, y1 ); }
    inline IcPoint  bottomLeft() const { return IcPoint( x1, y2 ); }

    inline void  setX( int ax ) { x1 = ax; }
    inline void  setY( int ay ) { y1 = ay; }
    inline void  setWidth ( int w ) { x2 = x1 + w - 1; }
    inline void  setHeight( int h ) { y2 = y1 + h - 1; }
    inline void  setLeft  ( int pos ) { x1 = pos; }
    inline void  setTop   ( int pos ) { y1 = pos; }
    inline void  setRight ( int pos ) { x2 = pos; }
    inline void  setBottom( int pos ) { y2 = pos; }

    inline void  setTopLeft    ( const IcPoint &p ) { x1 = p.x(); y1 = p.y(); }
    inline void  setBottomRight( const IcPoint &p ) { x2 = p.x(); y2 = p.y(); }
    inline void  setTopRight   ( const IcPoint &p ) { x2 = p.x(); y1 = p.y(); }
    inline void  setBottomLeft ( const IcPoint &p ) { x1 = p.x(); y2 = p.y(); }

    inline void  moveLeft  ( int pos ) { x2 += ( pos - x1 ); x1 = pos; }
    inline void  moveTop   ( int pos ) { y2 += ( pos - y1 ); y1 = pos; }
    inline void  moveRight ( int pos ) { x1 += ( pos - x2 ); x2 = pos; }
    inline void  moveBottom( int pos ) { y1 += ( pos - y2 ); y2 = pos; }
    inline void  moveTopLeft    ( const IcPoint &p ) { moveLeft(p.x());  moveTop( p.y()); }
    inline void  moveBottomRight( const IcPoint &p ) { moveRight(p.x()); moveBottom( p.y()); }
    inline void  moveTopRight   ( const IcPoint &p ) { moveRight(p.x()); moveTop( p.y ); }
    inline void  moveBottomLeft ( const IcPoint &p ) { moveLeft(p.x());  moveBottom(p.y()); }
    inline void  moveCenter     ( const IcPoint &p )
    {
        int w = x2 - x1;   int h = y2 - y1;
        x1 = p.x() - w/2;  y1 = p.y() - h/2;
        x2 = x1 + w;       y2 = y1 + h;
    }
    inline void   moveTo( int ax, int ay )  { x2 += ax - x1; y2 += ay - y1; x1 = ax; y1 = ay; }

    inline void   translate ( int dx, int dy )  { x1 += dx; y1 += dy; x2 += dx; y2 += dy; }
    inline IcRect translated( int dx, int dy )  { return IcRect( IcPoint(x1+dx,y1+dy), IcPoint(x2+dx,y2+dx)); }

    inline void   adjust  ( int dx1, int dy1, int dx2, int dy2 )       { x1 += dx1; y1 += dy1; x2 += dx2; y2 += dy2; }
    inline IcRect adjusted( int dx1, int dy1, int dx2, int dy2 ) const { return IcRect( IcPoint( x1 + dx1, y1 + dy1 ), IcPoint( x2 + dx2, y2 + dy2 )); }

    inline bool   contains( ) { }

private:
    int x1,y1,x2,y2;
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
