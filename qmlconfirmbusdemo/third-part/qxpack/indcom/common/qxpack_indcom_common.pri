
! contains( QXPACK_IC, indcom_common ) {
    QXPACK_IC *= indcom_common 
    CONFIG *= c++11

    contains( QT, core ) {
        DEFINES *= QXPACK_IC_QT5_ENABLED
    }
    contains( CONFIG, qt ) {
        DEFINES *= QXPACK_IC_QT5_ENABLED
    }

    INCLUDEPATH *= $$PWD/../../../

    ! contains( QXPACK_IC_CFG, indcom_common_cfg_no_hdr ) {
        HEADERS += $$PWD/qxpack_ic_def.h \
               $$PWD/qxpack_ic_memcntr.hxx \
               $$PWD/qxpack_ic_bytearray.hxx \
               $$PWD/qxpack_ic_base64.hxx \
               $$PWD/qxpack_ic_logging.hxx \
               $$PWD/qxpack_ic_minizip.hxx \
               $$PWD/qxpack_ic_semaphore.hxx \
               $$PWD/qxpack_ic_timer.hxx \
               $$PWD/qxpack_ic_global.hxx \
               $$PWD/qxpack_ic_objectcache.hxx \
               $$PWD/qxpack_ic_ipcsem.hxx \
               \
               $$PWD/qxpack_ic_heapvectortemp.hpp \
               $$PWD/qxpack_ic_pimplprivtemp.hpp \
               $$PWD/qxpack_ic_dequetemp.hpp \
               $$PWD/qxpack_ic_semtemp.hpp \
               $$PWD/qxpack_ic_tripleobjtemp.hpp \
               $$PWD/qxpack_ic_numstatqueuetemp.hpp \
               $$PWD/qxpack_ic_stacktemp.hpp \
               $$PWD/qxpack_ic_queuetemp.hpp \

         message( qxpack_indcom_common cfg. req. header )
     } else {
         message( qxpack_indcom_common cfg. req. no header )
     }

     ! contains( QXPACK_IC_CFG, indcom_common_cfg_no_src ) {
        SOURCES += \
               \
               $$PWD/qxpack_ic_memcntr.cxx \
               $$PWD/qxpack_ic_bytearray.cxx \
               $$PWD/qxpack_ic_base64.cxx \
               $$PWD/qxpack_ic_logging.cxx \
               $$PWD/qxpack_ic_minizip.cxx \
               $$PWD/qxpack_ic_semaphore.cxx \
               $$PWD/qxpack_ic_timer.cxx \
               $$PWD/qxpack_ic_global.cxx \
               $$PWD/qxpack_ic_objectcache.cxx \

        win32 {
            message( qxpack_indcom_common platform is win32 )
            DEFINES *= QXPACK_IC_PLATFORM_WIN32
            SOURCES += \
                $$PWD/qxpack_ic_ipcsem_win32.cxx \

        } else:linux {
            !android {
                 message( qxpack_indcom_common platform is linux )
                 DEFINES *= QXPACK_IC_PLATFORM_LINUX
                 SOURCES += \
                     $$PWD/qxpack_ic_ipcsem_linux.cxx

                 LIBS *= -lpthread
            } else {
                 message( qxpack_indcom_common platform is android )
                 DEFINES *= QXPACK_IC_PLATFORM_ANDROID
                 SOURCES += \
                     $$PWD/qxpack_ic_ipcsem_dummy.cxx \
            }
        } else {
            message ( qxpack_indcom_common platform is unknown )
            SOURCES += \
                $$PWD/qxpack_ic_ipcsem_dummy.cxx \
        }

        message( qxpack_indcom_common cfg. req. source )
    } else {
        message( qxpack_indcom_common cfg. req. no source )
    }

}

