import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

import qxpack.indcom.ui_qml_base 1.0 as QxPackIc

ApplicationWindow {
    id: idRoot; title: qsTr("Hello World")
    visible: true; width: 800; height: 600;

    // ////////////////////////////////////////////////////////////////////////
    // layout
    // ////////////////////////////////////////////////////////////////////////
    Component {
        id: idCmdItem;

        ColumnLayout {
            anchors.fill: parent; spacing: 9;
            // --------------------------------------------------------------------
            // [HINT] here used a list model to support a button list content
            // --------------------------------------------------------------------
            ListModel { // it is not visible in scene
                id: idButtonModel;
                ListElement { buttonTitle : qsTr("Stack View"); pageUrl : "qrc:/qmlpagelayoutdemo/main/view/demo_stackview.qml"; }
                ListElement { buttonTitle : qsTr("Stack View (Tab)"); pageUrl : "qrc:/qmlpagelayoutdemo/main/view/demo_stackview_tab.qml"; }
                ListElement { buttonTitle : qsTr("Stack Layout"); pageUrl : "qrc:/qmlpagelayoutdemo/main/view/demo_stacklayout.qml"; }
                ListElement { buttonTitle : qsTr("Swipe View"); pageUrl : "qrc:/qmlpagelayoutdemo/main/view/demo_swipeview.qml"; }
            }

            Item { Layout.fillWidth: true; Layout.fillHeight: true; Layout.alignment: Qt.AlignTop; }
            Repeater {
                model: idButtonModel;
                Button {
                    Layout.preferredWidth: parent.width * 0.7; Layout.preferredHeight: 80;
                    Layout.alignment: Qt.AlignCenter; text: model.buttonTitle;
                    onClicked: QxPackIc.IcUiQmlApi.postMetaCall( idPriv, "pushPage", model.pageUrl );
                }
            }
            Item { Layout.fillWidth: true; Layout.fillHeight: true; Layout.alignment: Qt.AlignBottom; }
        }
    }
    StackView {
        id: idPageStk; anchors.fill: parent;
        initialItem: idCmdItem;
    }



    // ////////////////////////////////////////////////////////////////////////
    // logic
    // ////////////////////////////////////////////////////////////////////////
    Component.onCompleted:   { idPriv.init();   }
    Component.onDestruction: { idPriv.deinit(); }
    QtObject {
        id: idPriv;

        // ====================================================================
        // initailize this page
        // ====================================================================
        function init( )
        {

        }

        // ====================================================================
        // de-initailze the page
        // ====================================================================
        function  deinit( )
        {

        }

        // ====================================================================
        // load page and connect the signals
        // ====================================================================
        function pushPage( page_url )
        {
            idPageStk.push( page_url, StackView.Immediate );
            idPageStk.currentItem.request.connect( idPriv.page_onRequest );
        }

        // ====================================================================
        // pop current page
        // ====================================================================
        function popPage( )
        {
            // ----------------------------------------------------------------
            // [HINT] do offline top page and pop it
            // ----------------------------------------------------------------
            if ( idPageStk.depth > 0 ) {
                idPageStk.currentItem.request.disconnect( idPriv.page_onRequest );
                idPageStk.currentItem.doOffline();
                idPageStk.pop( StackView.Immediate );
            }
        }

        // ====================================================================
        // slot: handle the request from page
        // ====================================================================
        function  page_onRequest( req_str, req_arg )
        {
            // ----------------------------------------------------------------
            // [HINT]  use IcUiQmlApi.postMetaCall to make a later call.
            // It will make stack shallow, and CPU is released from the previous JS code
            // ----------------------------------------------------------------
            if ( req_str === "reqGoBack" ) { // NOTE: use queued call to do request!
                QxPackIc.IcUiQmlApi.postMetaCall( idPriv, "popPage" );     
            } else {
                console.info("request should not be recognized (%1)".arg( req_str ));
            }
        }
    }
}
