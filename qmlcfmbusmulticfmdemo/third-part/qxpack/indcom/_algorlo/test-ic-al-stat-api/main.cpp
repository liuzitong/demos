#include <QCoreApplication>

#include <QString>
#include <QDebug>
#include <QtTest>
#include <QThread>
#include <random>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <list>
#include <QtGlobal>

#include <cstdio>
#include <algorithm>
#include <vector>
#include <list>

#include "../qxpack_ic_al_stat_series.hpp"

// ////////////////////////////////////////////////////////////////////////////
//
// local data set
//
// ////////////////////////////////////////////////////////////////////////////
static double series_data_set_dbl[] = {
    25590, 23306, 26780, 29463, 22485, 262490, 24985, 1689.5,
    17700, 18257, 24132, 21479, 19304, 16511, 18573, 26606,
    20917, 19394, 24322, 28383, 16478, 29062, 23232, 17580,
    26804, 28500, 25018, 24001, 27530, 27459, 22776, 19835,
    16475, 15201, 28801, 19509, 19098, 17993, 15131, 26187,
    25022, 27430, 19114, 18295, 25365, 23550, 20413, 19335,
    21286, 22911
};

static int series_data_set_int[] = {
    25590, 23306, 26780, 29463, 22485, 262490, 24985, 1689,
    17700, 18257, 24132, 21479, 19304, 16511, 18573, 26606,
    20917, 19394, 24322, 28383, 16478, 29062, 23232, 17580,
    26804, 28500, 25018, 24001, 27530, 27459, 22776, 19835,
    16475, 15201, 28801, 19509, 19098, 17993, 15131, 26187,
    25022, 27430, 19114, 18295, 25365, 23550, 20413, 19335,
    21286, 22911
};

static intptr_t series_data_set_intptr[] = {
    25590, 23306, 26780, 29463, 22485, 262490, 24985, 1689,
    17700, 18257, 24132, 21479, 19304, 16511, 18573, 26606,
    20917, 19394, 24322, 28383, 16478, 29062, 23232, 17580,
    26804, 28500, 25018, 24001, 27530, 27459, 22776, 19835,
    16475, 15201, 28801, 19509, 19098, 17993, 15131, 26187,
    25022, 27430, 19114, 18295, 25365, 23550, 20413, 19335,
    21286, 22911
};


// ////////////////////////////////////////////////////////////////////////////
//
// tester object
//
// ////////////////////////////////////////////////////////////////////////////
class  Tester : public QObject {
    Q_OBJECT
private:
    Q_SLOT  void  initTestCase( ) { }
    Q_SLOT  void  cleanupTestCase ( );
    Q_SLOT  void  test_IcAl_stat_series_evalQ_dataDbl( );
    Q_SLOT  void  test_IcAl_stat_series_evalQ_dataInt( );
    Q_SLOT  void  test_IcAl_stat_series_sum( );
    Q_SLOT  void  test_IcAl_stat_series_outlinerBound( );
    Q_SLOT  void  myTest( );
    static  void  prnSeries( const std::vector<double> & );
public :

};

// ============================================================================
// clean up
// ============================================================================
void   Tester :: cleanupTestCase( )
{

}

// ============================================================================
// print series value
// ============================================================================
void   Tester :: prnSeries( const std::vector<double> &ds )
{
    std::printf("==== prn series ====\n");
    std::vector<double>::const_iterator c_itr = ds.cbegin();
    int x = 0;
    while ( c_itr != ds.cend()) {
        if ( x % 8 == 0 ) { std::printf("\n"); }
        std::printf("%f,", *c_itr ++ ); ++ x;
    }
    std::printf("\n");
}

// ============================================================================
// test the series evalQ by using double data set
// ============================================================================
void   Tester :: test_IcAl_stat_series_evalQ_dataDbl( )
{
    // building data set vector
    std::vector<double> ds;
    for ( int x = 0; x < sizeof( series_data_set_dbl ) / sizeof( series_data_set_dbl[0] ); x ++ ) {
        ds.push_back( series_data_set_dbl[ x ] );
    }

    // should be sort
    std::sort( ds.begin(), ds.end() );  // A to Z order..

    // eval 25%, 50%, 75% position value
    double q14 = QxPack::IcAl_stat_series_evalQ( ds.data(), ds.size(), 0.25 );
    double q12 = QxPack::IcAl_stat_series_evalQ( ds.data(), ds.size(), 0.5  );
    double q34 = QxPack::IcAl_stat_series_evalQ( ds.data(), ds.size(), 0.75 );

    double tgt_14 = 19102.0;
    double tgt_12 = 22843.5;
    double tgt_34 = 26037.75;

    bool ret = QxPack::IcAl_fuzzyCompare( q14, tgt_14 );
    QVERIFY2( ret, QString("1/4 pct: %1, correct value is %2.").arg( q14 ).arg( tgt_14 ).toUtf8().constData());

    ret = QxPack::IcAl_fuzzyCompare( q12, tgt_12 );
    QVERIFY2( ret, QString("1/2 pct: %1, correct value is %2.").arg( q12 ).arg( tgt_12 ).toUtf8().constData());

    ret = QxPack::IcAl_fuzzyCompare( q34, tgt_34 );
    QVERIFY2( ret, QString("3/4 pct: %1, correct value is %2.").arg( q34 ).arg( tgt_34 ).toUtf8().constData());
}

// ============================================================================
// test the series evalQ by using integer value
// ============================================================================
void   Tester :: test_IcAl_stat_series_evalQ_dataInt( )
{
    // building data set vector
    std::vector<int> ds;
    for ( int x = 0; x < sizeof( series_data_set_int ) / sizeof( series_data_set_int[0] ); x ++ ) {
        ds.push_back( series_data_set_int[ x ] );
    }
    std::sort( ds.begin(), ds.end() );  // A to Z order..

    int q14 = QxPack::IcAl_stat_series_evalQ( ds.data(), ds.size(), 0.25 );
    int q12 = QxPack::IcAl_stat_series_evalQ( ds.data(), ds.size(), 0.5  );
    int q34 = QxPack::IcAl_stat_series_evalQ( ds.data(), ds.size(), 0.75 );

    int tgt_14 = 19102;
    int tgt_12 = 22844;
    int tgt_34 = 26038;

    bool ret = ( q14 == tgt_14 );
    QVERIFY2( ret, QString("1/4 pct: %1, correct value is %2.").arg( q14 ).arg( tgt_14 ).toUtf8().constData());

    ret = ( q12 == tgt_12 );
    QVERIFY2( ret, QString("1/2 pct: %1, correct value is %2.").arg( q12 ).arg( tgt_12 ).toUtf8().constData());

    ret = ( q34 == tgt_34 );
    QVERIFY2( ret, QString("3/4 pct: %1, correct value is %2.").arg( q34 ).arg( tgt_34 ).toUtf8().constData());

}

// ============================================================================
// test the series sum
// ============================================================================
void  Tester :: test_IcAl_stat_series_sum()
{
    intptr_t sumi = QxPack::IcAl_stat_series_sumi( series_data_set_intptr, sizeof( series_data_set_intptr ) / sizeof( series_data_set_intptr[0]), 0 );
    double   sumf = QxPack::IcAl_stat_series_sumf( series_data_set_dbl, sizeof( series_data_set_dbl ) / sizeof( series_data_set_dbl[0]), 0 );
}

// ============================================================================
// test the outliner boundary
// ============================================================================
void  Tester :: test_IcAl_stat_series_outlinerBound()
{
    // building data set vector
    std::vector<double> ds;
    for ( int x = 0; x < sizeof( series_data_set_dbl ) / sizeof( series_data_set_dbl[0] ); x ++ ) {
        ds.push_back( series_data_set_dbl[ x ] );
    }

    // should be sort
    std::sort( ds.begin(), ds.end() );  // A to Z order..

    double q14 = QxPack::IcAl_stat_series_evalQ( ds.data(), ds.size(), 0.25 );
    double q34 = QxPack::IcAl_stat_series_evalQ( ds.data(), ds.size(), 0.75 );
    QxPack::IcAl_Pair<double>  pair = QxPack::IcAl_stat_series_outlinerBound( q14, q34 );

    // print out all outliner
    std::vector<double>::const_iterator c_itr = ds.cbegin();
    while ( c_itr != ds.cend()) {
        if ( *c_itr < pair.first || *c_itr > pair.second) {
            qInfo("find outline value: %f", *c_itr );
        }
        ++ c_itr;
    }

}



// =============================================================================
// do result filter, keep the stable data
// =============================================================================
static double gFilterRsl_stdev( std::list<double> &rsl, double &avg )
{
    std::list<double>::iterator itr = rsl.begin();
    while ( itr != rsl.end()) { avg += (*itr ++ );}
    avg /= rsl.size();

    itr = rsl.begin(); double e = 0, stdev = 0;
    while ( itr != rsl.end()) {
        e = (* itr ++ ) - avg;
        stdev += e * e;
    }
    stdev = std::sqrt( stdev / ( rsl.size() - 1 ));
    return stdev;
}

static double  gFilterRsl( std::list<double> &rsl, double min_stdev )
{
    if ( rsl.size() < 2 ) { return ( rsl.size() > 0 ? rsl.front() : 0 ); }
    double avg = 0;
    while ( rsl.size() >= 2 ) {
        // estimate the average and stdev
        avg = 0;
        double stdev = gFilterRsl_stdev( rsl, avg );
        if ( stdev <= min_stdev ) { break; }

        // remove the value that maximum distance of avg..
        std::list<double>::const_iterator itr = rsl.begin();
        std::list<double>::const_iterator max_itr = rsl.end();
        double max_dist = 0;
        while ( itr != rsl.end() ) {
            double curr_dist = std::fabs((*itr) - avg );
            if ( curr_dist >= max_dist ) { max_itr = itr; max_dist = curr_dist; }
            ++ itr;
        }
        if ( max_itr != rsl.end()) { rsl.erase( max_itr ); }
    }
    return avg;
}

void Tester :: myTest( )
{
    static double mytest_data[] = { 96.5, 96, 19, 0 };

    // building data set vector
    std::list<double> ds;
    for ( int x = 0; x < sizeof( mytest_data ) / sizeof( mytest_data[0] ); x ++ ) {
        ds.push_back( mytest_data[ x ] );
    }

    qInfo("rsl:%f",gFilterRsl( ds, 1.2f ));

//    // should be sort
//    std::sort( ds.begin(), ds.end() );  // A to Z order..

//    double q14 = QxPack::IcAl_stat_series_evalQ( ds.data(), ds.size(), 0.25 );
//    double q34 = QxPack::IcAl_stat_series_evalQ( ds.data(), ds.size(), 0.75 );
//    qInfo("q14=%f, q34=%f", q14, q34);
//    QxPack::IcAl_Pair<double>  pair = QxPack::IcAl_stat_series_outlinerBound( q14, q34, 0.4 );

//    // print out all outliner
//    std::vector<double>::const_iterator c_itr = ds.cbegin();
//    while ( c_itr != ds.cend()) {
//        if ( *c_itr < pair.first || *c_itr > pair.second) {
//            qInfo("find outline value: %f", *c_itr );
//        }
//        ++ c_itr;
//    }

}





QTEST_MAIN( Tester )

#include "main.moc"

