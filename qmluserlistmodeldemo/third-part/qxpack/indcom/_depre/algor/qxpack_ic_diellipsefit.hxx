#ifndef QXPACK_IC_DIELLIPSEFIT_HXX
#define QXPACK_IC_DIELLIPSEFIT_HXX

#include <qxpack/indcom/common/qxpack_ic_def.h>
#include <vector> // C++ 11 container
#include <list>   // C++ 11 container

namespace QxPack {

// /////////////////////////////////////////////////////////
//
// Directly Ellipse Fit
//
// /////////////////////////////////////////////////////////
class QXPACK_IC_API  IcDiEllipseFit {

public :
    // ======================================================
    // typedef a structure that means x, y point
    // ======================================================
    struct PointF {
        double x, y;
        PointF( ) { x = y = 0; }
        PointF( double _x, double _y ) { x = _x; y = _y; }
    };

    // ======================================================
    // functions
    // ======================================================
    IcDiEllipseFit( );
    IcDiEllipseFit( const IcDiEllipseFit & );
    IcDiEllipseFit &  operator = ( const IcDiEllipseFit & );

    IcDiEllipseFit( const std::vector<PointF> &pt_list );
    IcDiEllipseFit( const std::list<PointF>   &pt_list );
    IcDiEllipseFit( const double *x, const double *y, int num );
    IcDiEllipseFit( const IcDiEllipseFit::PointF *pt, int num );
    virtual ~IcDiEllipseFit( );

    bool   isEmpty( ) const;

    double cx( ) const;
    double cy( ) const;
    double ru( ) const;
    double rv( ) const;
    double thetaRadius( ) const;

private:
    void *m_obj;
};

}

#endif
