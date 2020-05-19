!contains( QXPACK_IC, indcom_net ) {
    QXPACK_IC *= indcom_net

    ! contains( QXPACK_IC_CFG, indcom_net_cfg_no_hdr ) {
        HEADERS += \
               $$PWD/qxpack_ic_jsonrpc2_resphde.hxx  \
               $$PWD/qxpack_ic_jsonrpc2_buff.hxx \
               $$PWD/qxpack_ic_jsonrpc2_svr.hxx \
               $$PWD/qxpack_ic_jsonrpc2_cli.hxx \
               $$PWD/qxpack_ic_datatrans_buff.hxx \
               $$PWD/qxpack_ic_datatrans_svr.hxx \
               $$PWD/qxpack_ic_datatrans_cli.hxx \
               $$PWD/qxpack_ic_datatrans_rshdebase.hxx \
               $$PWD/qxpack_ic_datatrans_rshde_impl_bundle.hxx \
               $$PWD/qxpack_ic_datatrans_rssvrbase.hxx \
               $$PWD/qxpack_ic_datatrans_rssvr_impl_bundle.hxx \

        message( qxpack_indcom_net cfg. req. header )
    } else {
        message( qxpack_indcom_net cfg. req. no header )
    }

    ! contains( QXPACK_IC_CFG, indcom_net_cfg_no_src ) {
        SOURCES += \
               $$PWD/qxpack_ic_jsonrpc2_resphde.cxx \
               $$PWD/qxpack_ic_jsonrpc2_buff.cxx \
               $$PWD/qxpack_ic_jsonrpc2_svr.cxx \
               $$PWD/qxpack_ic_jsonrpc2_cli.cxx \
               $$PWD/qxpack_ic_datatrans_buff.cxx \
               $$PWD/qxpack_ic_datatrans_svr.cxx \
               $$PWD/qxpack_ic_datatrans_cli.cxx \
               $$PWD/qxpack_ic_datatrans_rshdebase.cxx \
               $$PWD/qxpack_ic_datatrans_rshde_impl_bundle.cxx \
               $$PWD/qxpack_ic_datatrans_rssvrbase.cxx \
               $$PWD/qxpack_ic_datatrans_rssvr_impl_bundle.cxx \

        message( qxpack_indcom_net cfg. req. source )
    } else {
        message( qxpack_indcom_net cfg. req. no source )
    }

}



