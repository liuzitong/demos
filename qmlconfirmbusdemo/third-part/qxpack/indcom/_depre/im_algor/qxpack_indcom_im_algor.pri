!contains( QXPACK_IC, indcom_im_algor ) {
    QXPACK_IC *= indcom_im_algor

    ! contains( QXPACK_IC_CFG, indcom_im_algor_cfg_no_hdr ) {
        HEADERS += \
          #  $$PWD/qxpack_ic_randcolor.hxx \

        message( qxpack_indcom_im_algor cfg. req. header )
    } else {
        message( qxpack_indcom_im_algor cfg. req. no header )
    }

    ! contains( QXPACK_IC_CFG, indcom_im_algor_cfg_no_src ) {
        SOURCES += \
         #   $$PWD/qxpack_ic_randcolor.cxx \

        message( qxpack_indcom_im_algor cfg. req. source )
    } else {
        message( qxpack_indcom_im_algor cfg. req. no source )
    }

}



