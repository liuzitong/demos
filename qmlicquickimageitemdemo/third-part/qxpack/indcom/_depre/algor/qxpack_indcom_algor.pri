
!contains( QXPACK_IC, indcom_algor ) {
    QXPACK_IC *= indcom_algor
   
    INCLUDEPATH *= $$PWD/../../../

    ! contains( QXPACK_IC_CFG, indcom_algor_cfg_no_hdr ) {
        HEADERS += $$PWD/qxpack_ic_diellipsefit.hxx \
                   $$PWD/qxpack_ic_spookyhashv2.hxx \

        message( qxpack_indcom_algor cfg. req. header )
    } else {
        message( qxpack_indcom_algor cfg. req. no header )
    }

    ! contains( QXPACK_IC, indcom_algor_cfg_no_src ) {
        SOURCES += $$PWD/qxpack_ic_diellipsefit.cxx \
                   $$PWD/qxpack_ic_spookyhashv2.cxx \

        message( qxpack_indcom_algor cfg. req. source )
    } else {
        message( qxpack_indcom_algor cfg. req. no source )
    }
}

include( $$PWD/../common/qxpack_indcom_common.pri )
