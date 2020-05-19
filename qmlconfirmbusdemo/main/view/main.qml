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
        anchors.centerIn: parent;   width: 120; height: 80;
        text : qsTr("try demo0");
        onClicked: IcUiQmlApi.postMetaCall( idPriv.main_vm, "doDemo0" );
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

        // ==================================================================
        // initailize the page
        // ==================================================================
        function init( )
        {
            // [HINT] create the main view model by name via objMgr
            main_vm = IcUiQmlApi.appCtrl.objMgr.attachObj( "qmlconfirmbusdemo::MmMainVm", false );

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
            IcUiQmlApi.appCtrl.objMgr.detachObj( "qmlconfirmbusdemo::MmMainVm", main_vm );
        }

        // ====================================================================
        // slot: handle the demo0 confirm message
        // ====================================================================
        function  onDemo0_reqConfirm( cfm_pkg )
        {
            //[HINT] below code are demo. how to dyn. create component and object

            // a) before use the spec. custom control, create it's component
            var cmpt = Qt.createComponent("qrc:/qmlconfirmbusdemo/main/view/confirmdiag.qml");
            if ( cmpt.status !== Component.Ready ) { console.error("create confirmdiag failed!"); return; }

            // b) now create the object, set it parent to this control
            var cfm_diag = cmpt.createObject( idRoot );

            // c) setup information about this confirm object
            cfm_diag.message = cfm_pkg.message;
            cfm_diag.contentWidth = 400; cfm_diag.contentHeight = 300;

            // d) now enter the modal dialog event...
            cfm_diag.open();

            // e) after it closed, we can get information
            var rsl_info = {};
            rsl_info["result"] = ( cfm_diag.isOK ? "yes" : "no" );
            cfm_pkg.result = rsl_info;

            // f) destory resources
            cfm_diag.destroy();
            cmpt.destroy();
        }
    }
}
