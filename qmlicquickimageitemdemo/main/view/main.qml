import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

import qxpack.indcom.ui_qml_base 1.0 // [HINT] this is the pre-registered module name.
import qxpack.indcom.ui_qml_control 1.0

ApplicationWindow {
    id: idRoot; visible: true;  width: 640;  height: 480;
    title: qsTr("Hello World");

    // ////////////////////////////////////////////////////////////////////////
    // layout
    // ////////////////////////////////////////////////////////////////////////
    ColumnLayout {
        anchors.fill: parent;
        IcQuickImageItem {
            id: idImageItem; Layout.fillWidth: true;  Layout.preferredHeight: parent.height * 0.8;
            Layout.alignment: Qt.AlignTop | Qt.AlignHCenter;

        }
        Slider {
            Layout.fillWidth: true;  Layout.fillHeight: true;
            Layout.alignment: Qt.AlignBottom | Qt.AlignHCenter;
            from: 0; to: idPriv.main_vm !== null ? idPriv.main_vm.imageListCount - 1 : 0;
            value: idPriv.main_vm !== null ? idPriv.main_vm.currImageIndex : 0;
            stepSize: 1;
            snapMode: Slider.SnapAlways;
            onPositionChanged:  {
                if ( idPriv.main_vm !== null ) {
                    idPriv.main_vm.currImageIndex = ( to - from ) * position;
                }
            }

//            onValueChanged: function ( ) {
//                if ( idPriv.main_vm !== null ) {
//                    idPriv.main_vm.setC
//                }
//            }
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

        // ==================================================================
        // initailize the page
        // ==================================================================
        function init( )
        {
            // [HINT] create the main view model by name via objMgr
            main_vm = IcUiQmlApi.appCtrl.objMgr.attachObj( "qmlIcquickimageitemDemo::MmMainVm", false );
            idImageItem.imageData = main_vm.imageDataSource;
        }

        // ====================================================================
        // de-init the page
        // ====================================================================
        function deinit( )
        {
            idImageItem.imageData = null;

            // [HINT] must free it before destruct this page
            IcUiQmlApi.appCtrl.objMgr.detachObj( "qmlIcquickimageitemDemo::MmMainVm", main_vm );
        }

    }
}
