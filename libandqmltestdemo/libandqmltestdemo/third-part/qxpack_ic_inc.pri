! contains( LIBANDQMLTESTDEMO_MODULES, libandqmltestdemo_qxpack_ic ) {
    LIBANDQMLTESTDEMO_MODULES *= libandqmltestdemo_qxpack_ic
    CONFIG *= c++11

    DEFINES *= QXPACK_CFG_STATIC
    include( $$PWD/../../third-part/qxpack/indcom/qxiccore.pri )
    include( $$PWD/../../third-part/qxpack/indcom/sys/qxpack_indcom_sys.pri )
    
    CONFIG( debug, debug|release ) {
        DEFINES *= QXPACK_IC_CFG_MEM_TRACE
    } else {

    }
}