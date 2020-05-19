import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

import qxpack.indcom.ui_qml_base 1.0 // [HINT] this is the pre-registered module name.

ApplicationWindow {
    visible: true;
    width: 640;  height: 480;
    title: qsTr("Hello World");

    // ////////////////////////////////////////////////////////////////////////
    // layout
    // ////////////////////////////////////////////////////////////////////////

    Text {
       id: idMsgDisp; width: 300; height: 200; text: "ready.";
       anchors.centerIn: parent; wrapMode: Text.WrapAtWordBoundaryOrAnywhere;
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
            main_vm = IcUiQmlApi.appCtrl.objMgr.attachObj( "qmlMsgbusDemo::MmMainVm", false );

            // [TIP] here we used a dynmatic data binding for read
            idMsgDisp.text = Qt.binding (
                function() {
                    return ( idPriv.main_vm !== null ? idPriv.main_vm.animalMsg : "" );
                }
            );
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

            idMsgDisp.text = "";  // [TIP] here reset the data binding..

            // [HINT] must free it before destruct this page
            IcUiQmlApi.appCtrl.objMgr.detachObj( "qmlMsgbusDemo::MmMainVm", main_vm );
        }
    }
}
