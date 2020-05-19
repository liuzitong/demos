import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Window  2.0

import qxpack.indcom.ui_qml_base 1.0 // [HINT] this is the pre-registered module name.

Window {
    id: idRoot;
    visible: true;
    width: 600; height: 400;

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
    Component.onCompleted:   { idPriv.init();   }  // [HINT] this attached slot will be called
    Component.onDestruction: { idPriv.deinit(); }  //  while idRoot component loaded completed.
                                   //  NOTE: the inner object's Component.onCompleted
                                   //  maybe ocurred before this.
    QtObject {
        id: idPriv;
        property var msgbus_vm : null;

        // ====================================================================
        // init
        // ====================================================================
        function init( )
        {
            // [HINT] here also use true option to create Mm
            msgbus_vm = IcUiQmlApi.appCtrl.objMgr.attachObj( "qmlMsgbusMultiwinDemo::MmMainVm", true );

            idMsgDisp.text = Qt.binding (
                function() {
                    return ( idPriv.msgbus_vm !== null ? idPriv.msgbus_vm.animalMsg : "" );
                }
            );
        }

        // ====================================================================
        // deinit
        // ====================================================================
        function  deinit( )
        {
            idMsgDisp.text = "";  // [TIP] here reset the data binding..

            // [HINT] must free it before destruct this page
            IcUiQmlApi.appCtrl.objMgr.detachObj( "qmlMsgbusMultiwinDemo::MmMainVm", msgbus_vm );
        }

    }
}
