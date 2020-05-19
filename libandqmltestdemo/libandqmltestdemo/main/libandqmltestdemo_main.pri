
! contains( LIBANDQMLTESTDEMO_MODULES, libandqmltestdemo_main ) {
    LIBANDQMLTESTDEMO_MODULES *= libandqmltestdemo_main 
    CONFIG *= c++11

    contains( QT, core ) {
        DEFINES *= LIBANDQMLTESTDEMO_QT5_ENABLED
    }
    contains( CONFIG, qt ) {
        DEFINES *= LIBANDQMLTESTDEMO_QT5_ENABLED
    }

    INCLUDEPATH *= $$PWD/../../

    ! contains( LIBANDQMLTESTDEMO_CFG, libandqmltestdemo_main_cfg_no_hdr ) {
        HEADERS_COPY += \

        HEADERS += \
                $${HEADERS_COPY} \


         message( libandqmltestdemo_main cfg. req. header )
     } else {
         message( libandqmltestdemo_main cfg. req. no header )
     }

     ! contains( LIBANDQMLTESTDEMO_CFG, libandqmltestdemo_main_cfg_no_src ) {
        SOURCES += \
               \

        win32 {
            message( libandqmltestdemo_main platform is win32 )
            SOURCES += \

        } else:linux {
            !android {
                 message( libandqmltestdemo_main platform is linux )
                 SOURCES += \

                 LIBS *= -lpthread
            } else {
                 message( libandqmltestdemo_main platform is android )
                 SOURCES += \

            }
        } else {
            message ( libandqmltestdemo_main platform is unknown )
            SOURCES += \

        }

        message( libandqmltestdemo_main cfg. req. source )
    } else {
        message( libandqmltestdemo_main cfg. req. no source )
    }

}

