
! contains( LIBANDQMLTESTDEMO_MODULES, libandqmltestdemo_common ) {
    LIBANDQMLTESTDEMO_MODULES *= libandqmltestdemo_common 
    CONFIG *= c++11

    contains( QT, core ) {
        DEFINES *= LIBANDQMLTESTDEMO_QT5_ENABLED
    }
    contains( CONFIG, qt ) {
        DEFINES *= LIBANDQMLTESTDEMO_QT5_ENABLED
    }

    INCLUDEPATH *= $$PWD/../../

    ! contains( LIBANDQMLTESTDEMO_CFG, libandqmltestdemo_common_cfg_no_hdr ) {
        HEADERS_COPY += \
               $$PWD/libandqmltestdemo_def.h \
               $$PWD/libandqmltestdemo_memcntr.hxx \
               $$PWD/libandqmltestdemo_logging.hxx \

        HEADERS += \
               $${HEADERS_COPY} \
               \
               $$PWD/libandqmltestdemo_pimplprivtemp.hpp \
               $$PWD/libandqmltestdemo_dequetemp.hpp \
               $$PWD/libandqmltestdemo_semtemp.hpp \              

         message( libandqmltestdemo_common cfg. req. header )
     } else {
         message( libandqmltestdemo_common cfg. req. no header )
     }

     ! contains( LIBANDQMLTESTDEMO_CFG, libandqmltestdemo_common_cfg_no_src ) {
        SOURCES += \
               \
               $$PWD/libandqmltestdemo_memcntr.cxx \
               $$PWD/libandqmltestdemo_logging.cxx \

        win32 {
            message( libandqmltestdemo_common platform is win32 )
            SOURCES += \

        } else:linux {
            !android {
                 message( libandqmltestdemo_common platform is linux )
                 SOURCES += \

                 LIBS *= -lpthread
            } else {
                 message( libandqmltestdemo_common platform is android )
                 SOURCES += \

            }
        } else {
            message ( libandqmltestdemo_common platform is unknown )
            SOURCES += \
                $$PWD/qxpack_ic_ipcsem_dummy.cxx \
        }

        message( libandqmltestdemo_common cfg. req. source )
    } else {
        message( libandqmltestdemo_common cfg. req. no source )
    }

}

