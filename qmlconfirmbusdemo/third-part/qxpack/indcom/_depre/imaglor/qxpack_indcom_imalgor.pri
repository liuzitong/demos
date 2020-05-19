
!contains( QXPACK_IC, indcom_imalgor ) {
    QXPACK_IC *= indcom_imalgor
   
    INCLUDEPATH *= $$PWD/../../../

    ! contains( QXPACK_IC_CFG, indcom_imalgor_cfg_no_hdr ) {
        HEADERS += 

        message( qxpack_indcom_imalgor cfg. req. header )
    } else {
        message( qxpack_indcom_imalgor cfg. req. no header )
    }

    ! contains( QXPACK_IC, indcom_imalgor_cfg_no_src ) {
        SOURCES += 

        message( qxpack_indcom_imalgor cfg. req. source )
    } else {
        message( qxpack_indcom_imalgor cfg. req. no source )
    }
}

include( $$PWD/../common/qxpack_indcom_common.pri )
