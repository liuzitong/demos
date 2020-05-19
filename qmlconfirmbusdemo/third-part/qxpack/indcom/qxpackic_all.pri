# =============================================================================
#
#    This file used in user application 
#
# =============================================================================
! contains( QXPACK_IC_PRI, qxpackic_all ) {
    QXPACK_IC_PRI *= qxpackic_all
    CONFIG *= c++11
    INCLUDEPATH *= $$PWD/../../

    contains( QT, core ) {
        QT *= network gui
        DEFINES *= QXPACK_IC_QT5_ENABLE
    }
    contains( QT, qml ) {
        message( qxpackic_all detected used QML module ) 
        DEFINES *= QXPACK_IC_DETECTED_QML
    }
    contains( CONFIG, qt ) {
        DEFINES *= QXPACK_IC_QT5_ENABLED
    }

    # ------------------------------------------------------------------------- 
    # 'qxiccore_cfg_no_hdr'     : all modules, do not include headers. ( defined by parent )
    # -------------------------------------------------------------------------
    ! contains ( QXPACK_IC_CFG, qxiccore_cfg_hdr_and_src ) {
        QXPACK_IC_CFG *= indcom_common_cfg_no_src  \
                         indcom_sys_cfg_no_src    indcom_afw_cfg_no_src  \
                         indcom_net_cfg_no_src    indcom_im_algor_cfg_no_src \
                         indcom_ui_qml_base_cfg_no_src  indcom_ui_qml_control_cfg_no_src \                       

        contains( QXPACK_IC_CFG, qxiccore_cfg_no_hdr ) {
            QXPACK_IC_CFG *= indcom_common_cfg_no_hdr  \
                             indcom_sys_cfg_no_hdr    indcom_afw_cfg_no_hdr \
                             indcom_net_cfg_no_hdr    indcom_im_algor_cfg_no_hdr \
                             indcom_ui_qml_base_cfg_no_hdr   indcom_ui_qml_control_cfg_no_hdr \
        }
        QXPACK_IC_CFG *= qxpackic_all_cfg_only_lib

    } else {
        # here means include source and header
    }

    # -------------------------------------------------------------------------
    # 'qxpackic_all_cfg_only_lib': do not include all modules ( defined by parent )
    # -------------------------------------------------------------------------
    ! contains( QXPACK_IC_CFG, qxpackic_all_cfg_only_lib ) {
        include( $$PWD/common/qxpack_indcom_common.pri  )
        include( $$PWD/sys/qxpack_indcom_sys.pri )
        include( $$PWD/afw/qxpack_indcom_afw.pri )
        include( $$PWD/net/qxpack_indcom_net.pri )
        include( $$PWD/ui_qml_base/qxpack_indcom_ui_qml_base.pri )        
        include( $$PWD/ui_qml_control/qxpack_indcom_ui_qml_control.pri )

    } else {
        message( qxpackic_all only contain library )

        LIBS *= -L"$$PWD"
        win32 {
            CONFIG( debug, debug|release ) {
                LIBS *= -lqxpackic_alld
            } else {
                LIBS *= -lqxpackic_all
            }
        } else {
            LIBS *= -lqxpackic_all
        }
    }
}
