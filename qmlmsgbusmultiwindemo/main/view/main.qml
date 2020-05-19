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
    ColumnLayout {
        anchors.fill: parent;
        Text {
            id: idMsgDisp; text: "ready.";
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere;
            Layout.fillWidth: true; Layout.preferredHeight: 300;
            Layout.alignment: Qt.AlignCenter;
        }
        Item {
            Layout.alignment: Qt.AlignHCenter;
            Layout.fillWidth: true; Layout.fillHeight: true;
        }
        Button {
            id: idCreateAux; text: qsTr("create Aux Window");
            Layout.preferredWidth: 300; Layout.preferredHeight: 80;
            Layout.alignment: Qt.AlignBottom | Qt.AlignHCenter;
            onClicked: idPriv.createAuxWin();
        }
    }

    // ////////////////////////////////////////////////////////////////////////
    // logic
    // ////////////////////////////////////////////////////////////////////////
    Component.onCompleted:   { idPriv.init();   }  // [HINT] here create the aux win
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
            // here use true to means the MmMainVm is a global viewmodel, it can be
            // shared with other view.
            main_vm = IcUiQmlApi.appCtrl.objMgr.attachObj( "qmlMsgbusMultiwinDemo::MmMainVm", true );

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
            IcUiQmlApi.appCtrl.objMgr.detachObj( "qmlMsgbusMultiwinDemo::MmMainVm", main_vm );
        }

        // ====================================================================
        // create a aux window to show shared viewmodel message
        // ====================================================================
        function createAuxWin( )
        {
            // [HINT] here we directly used a qrc path to locate the page, in real
            // scene, maybe the page is created by StackView, Loader or other containcer.
            var cmpt = Qt.createComponent("qrc:/qmlmsgbusmultiwindemo/main/view/main2.qml");
            while ( cmpt.status !== Component.Ready ) { }

            // [HINT] the parent set to null means top window. Normally, user create
            // object should use destory() method to delete.
            // var aux_win = cmpt.createObject( null ); // nw: 20190608 corrected it
            var aux_win = cmpt.createObject( idRoot );  // nw: this must be assign it to current root object, set to null maybe cause it 
                                                        // automatically deleted by system.
            cmpt.destroy(); // Need not to use the compiled component, release it
        }

    }
}
