! contains( $D{Key2_ProjNameU}_MODULES, qxpackic_all ) {
    $D{Key2_ProjNameU}_MODULES *= qxpackic_all

    DEFINES *= QXPACK_IC_CFG_STATIC
    include( $$PWD/../../third-part/qxpack/indcom/common/qxpack_indcom_common.pri )
    include( $$PWD/../../third-part/qxpack/indcom/sys/qxpack_indcom_sys.pri )
    include( $$PWD/../../third-part/qxpack/indcom/afw/qxpack_indcom_afw.pri )
    include( $$PWD/../../third-part/qxpack/indcom/net/qxpack_indcom_net.pri )

    contains( QT, qml ) {
        include( $$PWD/../../third-part/qxpack/indcom/ui_qml_base/qxpack_indcom_ui_qml_base.pri )
        include( $$PWD/../../third-part/qxpack/indcom/ui_qml_control/qxpack_indcom_ui_qml_control.pri )
    }

}
