! contains( CORE_MODULES, qxpackic_all ) {
    CORE_MODULES *= qxpackic_all

    DEFINES *= QXPACK_IC_CFG_STATIC
    include( $$PWD/qxpack/indcom/common/qxpack_indcom_common.pri )
    include( $$PWD/qxpack/indcom/sys/qxpack_indcom_sys.pri )
    include( $$PWD/qxpack/indcom/afw/qxpack_indcom_afw.pri )
    include( $$PWD/qxpack/indcom/net/qxpack_indcom_net.pri )

}
