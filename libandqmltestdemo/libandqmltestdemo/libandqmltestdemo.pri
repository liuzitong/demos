# ==================================================
#
#    This file used in user application 
#
# ==================================================
! contains( LIBANDQMLTESTDEMO_PRI, libandqmltestdemo ) {
    LIBANDQMLTESTDEMO_PRI *= libandqmltestdemo
    CONFIG *= c++11
    INCLUDEPATH *= $$PWD/../

    # ----------------------------------------------- 
    # 'libandqmltestdemo_common_cfg_no_src' : common module, do not include sources
    # 'libandqmltestdemo_cfg_no_hdr'     : all modules, do not include headers. ( defined by parent )
    # -----------------------------------------------
    ! contains ( LIBANDQMLTESTDEMO_CFG, libandqmltestdemo_cfg_hdr_and_src ) {
        LIBANDQMLTESTDEMO_CFG *= libandqmltestdemo_common_cfg_no_src  libandqmltestdemo_main_cfg_no_src

        contains( LIBANDQMLTESTDEMO_CFG, libandqmltestdemo_cfg_no_hdr ) {
            LIBANDQMLTESTDEMO_CFG *= libandqmltestdemo_common_cfg_no_hdr libandqmltestdemo_main_cfg_no_hdr
        }
        LIBANDQMLTESTDEMO_CFG *= libandqmltestdemo_cfg_only_lib

    } else {
        # here means include source and header
    }

    # -----------------------------------------------
    # 'libandqmltestdemo_cfg_only_lib': do not include all modules ( defined by parent )
    # -----------------------------------------------
    ! contains( LIBANDQMLTESTDEMO_CFG, libandqmltestdemo_cfg_only_lib ) {
        include( $$PWD/common/libandqmltestdemo_common.pri  )
        include( $$PWD/main/libandqmltestdemo_main.pri )

    } else {
        message( libandqmltestdemo only contain library )

        LIBANDQMLTESTDEMO_MODULE_QT_BUILD  = $$section( QMAKESPEC, /, -3, -3)
        LIBANDQMLTESTDEMO_MODULE_QT_VER    = $$section( QMAKESPEC, /, -4, -4)
        LIBANDQMLTESTDEMO_MODULE_QT_MMVER  = $$section( LIBANDQMLTESTDEMO_MODULE_QT_VER, ., 0, 1 )

        LIBS *= -L"$$PWD/build/$$LIBANDQMLTESTDEMO_MODULE_QT_BUILD"

            CONFIG( debug, debug|release ) {
                LIBS *= -llibandqmltestdemod
            } else {
                LIBS *= -llibandqmltestdemo
            }

    }
}
