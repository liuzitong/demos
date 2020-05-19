#include <QCoreApplication>
#include <qxpack/indcom/common/qxpack_ic_logging.hxx>
#include <qxpack/indcom/accel/oclplat/qxpack_ic_oclsgldev.hxx>
#include <qxpack/indcom/accel/oclplat/qxpack_ic_oclexec.hxx>
#include <qxpack/indcom/accel/oclplat/qxpack_ic_oclbuffer.hxx>
#include <qxpack/indcom/accel/oclplat/qxpack_ic_oclbuffcache.hxx>
#include <qxpack/indcom/common/qxpack_ic_bytearray.hxx>
#include <qxpack/indcom/common/qxpack_ic_memcntr.hxx>

#include <cstdlib>
#include <cstdio>
#include <random> // C++11

// =================================================
// check the memory counter
// =================================================
void   testMemCntr( )
{
    qxpack_ic_info("the new memory counter: %d", QxPack::IcMemCntr::currNewCntr());
}

// =================================================
// verify the buffer read write
// =================================================
bool   testBuffer_ReadWrite( QxPack::IcOclBuffer &buff )
{
    bool is_ok = false;
    QxPack::IcByteArray ba_w( buff.size() );
    QxPack::IcByteArray ba_r( buff.size() );
    {
       std::default_random_engine generator;
       std::uniform_int_distribution<int> dist(1,255);
       int buf_size = buff.size();
       char *dp = ba_w.data();
       for ( int x = 0; x < buf_size; x ++ ) {
           int dice_roll = dist(generator);
           dp[x] = ( char )( dice_roll );
       }
    }
    if ( is_ok = buff.write( ba_w )) {
        qxpack_ic_info("write the data into buffer ok");
    } else {
        qxpack_ic_info("write the data into buffer failed.");
    }
    if (! is_ok ) { return false; }

    is_ok = buff.read( ba_r );
    if ( is_ok ) {
        qxpack_ic_info("read the data from buffer ok");
    } else {
        qxpack_ic_info("read the data from buffer failed");
    }
    if ( ! is_ok ) { return false; }

    if ( std::memcmp( ba_w.constData(), ba_r.constData(), ba_w.size() ) == 0 ) {
        qxpack_ic_info("compare the read write buffer, they are same.");
    } else {
        qxpack_ic_info("compare the read write buffer, they are not same.");
    }
     return true;
}

// =================================================
// main entry
// =================================================
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    {
        // build a gpu device
        QxPack::IcOclSglDev dev( QxPack::IcOclSglDev::Type_Auto );
        if ( dev.isReady()) {
            qxpack_ic_info("opencl device platform id is 0x%x\n", dev.platformId() );
        } else {
            qxpack_ic_fatal("opencl device is not ready.");
        }

        // build the buffer
        QxPack::IcOclBuffer buff = dev.buffCache().create( 8192 );
        if ( ! buff.isReady()) {
            qxpack_ic_fatal("opencl buffer is not ready.");
        } else {
            qxpack_ic_info("opencl buffer is ready, size is %d, cap size is %d", buff.size(), buff.capacity() );
        }
        testBuffer_ReadWrite( buff );
        testMemCntr();
    }

    qxpack_ic_info("all opencl object destoryed");
    testMemCntr();
    return 0;
}
