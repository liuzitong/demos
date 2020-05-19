import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

import qxpack.indcom.ui_qml_base 1.0 // [HINT] this is the pre-registered module name.

ApplicationWindow {
    id: idRoot; visible: true;  width: 640;  height: 480;
    title: qsTr("Hello World");

    // ////////////////////////////////////////////////////////////////////////
    // layout
    // ////////////////////////////////////////////////////////////////////////
    Button {
        anchors.centerIn: parent;   width: 140; height: 80;
        text : qsTr("try demo0 & demoInBg");
        onClicked: {
            IcUiQmlApi.postMetaCall( idPriv.main_vm, "doDemo0" );
            IcUiQmlApi.postMetaCall( idPriv.main_vm, "doDemoInBg" );
        }
    }

    // ////////////////////////////////////////////////////////////////////////
    // logic
    // ////////////////////////////////////////////////////////////////////////
    Component.onCompleted:   { idPriv.init();   }
    Component.onDestruction: { idPriv.deinit(); }

    // [HINT] use this object to wrap all local functions
    // the wrapped function is not visible externally
    QtObject {
        id: idPriv;
        property var main_vm : null;
        property int  cfm_tm : 0;
        property real cfm_x  : 0;
        property real cfm_y  : 0;

        // ==================================================================
        // initailize the page
        // ==================================================================
        function init( )
        {
            // [HINT] create the main view model by name via objMgr
            main_vm = IcUiQmlApi.appCtrl.objMgr.attachObj( "qmlCfmBusMulticfmDemo::MmMainVm", false );

            // [TIP] user can use javascript connection syntax to connect to signals
            main_vm.demo0_reqConfirm.connect( idPriv.onDemo0_reqConfirm );
        }

        // ====================================================================
        // de-init the page
        // ====================================================================
        function deinit( )
        {
            // ----------------------------------------------------------------
            // NOTE: do not set a breakpoint at here, that will be dead-lock
            // while quit the app.
            // ----------------------------------------------------------------
            // [TIP] must be disconnect signal before detach
            main_vm.demo0_reqConfirm.disconnect( idPriv.onDemo0_reqConfirm );

            // [HINT] must free it before destruct this page
            IcUiQmlApi.appCtrl.objMgr.detachObj( "qmlCfmBusMulticfmDemo::MmMainVm", main_vm );
        }

        // ====================================================================
        // slot: handle the demo0 confirm message
        // ====================================================================
        function  onDemo0_reqConfirm( cfm_pkg ) // cfm_pkg is a QObject* type
        {
            //[HINT] below code are demo. how to dyn. create component and object

            // a) before use the spec. custom control, create it's component
            var cmpt = Qt.createComponent("qrc:/qmlcfmbusmulticfmdemo/main/view/confirmdiag.qml");
            if ( cmpt.status !== Component.Ready ) { console.error("create confirmdiag failed!"); return; }

            // b) now create the object, set it parent to this control
            var cfm_diag = cmpt.createObject( idRoot );

            // c) setup information about this confirm object
            cfm_diag.message = cfm_pkg.message;
            cfm_diag.contentWidth = 300; cfm_diag.contentHeight = 200;

            // make sure a offset of dialog, so we can see two dialogs in this demo
            if ( cfm_tm % 2 === 0 ) { cfm_x = cfm_y = 0; }
            cfm_diag.popupX = cfm_x; cfm_x += 150;
            cfm_diag.popupY = cfm_y; cfm_y += 100;
            cfm_tm ++;


            // d) create a new confirm dialog
            // NOTE: the enterEventLoop (false) and destroyOnClose( true ) must be set here.
            // these condition ensure dialog show in non event modal mode.
            cfm_diag.reqEnterEventLoop = false;
            cfm_diag.destroyOnClose    = true;
            cfm_diag.modal = false;
            cfm_diag.closed.connect (
                function() { // this function used to get back result
                   var rsl_info = {};
                   rsl_info["result"] = ( cfm_diag.isOK ? "yes" : "no" );
                   cfm_pkg.result = rsl_info;
                }
            );

            // e) makesure grab this package, so the confirm sender will be wait on this
            cfm_pkg.grab();

            // f) open the confirm dialog
            cfm_diag.open();

            // cleanup
            cmpt.destroy();
        }
    }
}
