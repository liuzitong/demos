import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

import qxpack.indcom.ui_qml_base 1.0
import qxpack.indcom.ui_qml_control 1.0

IcPageBase {
    id: idRoot; implicitWidth: 640; implicitHeight: 480;

    // [HINT] override the IcPageBase.doOffline()
    function  doOffline ( ) { idPriv.doOffline(); }

    // ////////////////////////////////////////////////////////////////////////
    // layout
    // ////////////////////////////////////////////////////////////////////////
    // ------------------------------------------------------------------------
    // background items
    // ------------------------------------------------------------------------
    ColumnLayout {    // background
        anchors.fill: parent; spacing: 0;
        Rectangle {   // header area
            Layout.fillWidth: true; Layout.preferredHeight: 80;
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop; color: "#3c3c3c";
            Text {    // just display the stack depth only
                anchors.centerIn: parent; font.pixelSize: parent.height * 0.5;
                color: "#a0a0a0";
                text: "stack depth: %1".arg( idPageStk.depth );
            }
        }
        Item { Layout.fillWidth: true; Layout.fillHeight: true; } // middle area
        Rectangle {   // bottom area
            Layout.fillWidth: true; Layout.preferredHeight: 80;
            Layout.alignment: Qt.AlignVCenter | Qt.AlignBottom; color: "#3c3c3c";
        }
    }

    // ------------------------------------------------------------------------
    // hinit text
    // ------------------------------------------------------------------------
    HintText {
        id: idHintPopup;  modal: false; reqEnterEventLoop: false;
        contentItem.implicitWidth: parent.width * 0.7;
        contentItem.implicitHeight: parent.height * 0.5;
        message: qsTr(
            "  Demonstrates the use of 'StackView'." +
            "User can push new page into 'StackView'," +
            "and can pop page from 'StackView'."
        );
    }

    // ------------------------------------------------------------------------
    //                     Demonstrate
    // This app. use StackView to manage Pages, all Pages is stacked, user should
    // click 'go back' to pop current page. last go back will go back to the main view
    // ------------------------------------------------------------------------
    StackView {
        id: idPageStk; anchors.fill: parent;
    }

    // ////////////////////////////////////////////////////////////////////////
    // logic
    // ////////////////////////////////////////////////////////////////////////
    Component.onCompleted: { idPriv.init(); }
    Component.onDestruction: { idPriv.deinit(); }
    QtObject {
        id: idPriv;
        property bool isOnline : false;

        // ====================================================================
        // initailize this page
        // ====================================================================
        function init( )
        {
            pushPage( "qrc:/qmlpagelayoutdemo/main/view/page0.qml");
            idHintPopup.open();
            isOnline = true;
            idRoot.pageInit( );
        }

        // ====================================================================
        // de-initailze the page
        // ====================================================================
        function  deinit( )
        {
            idPriv.doOffline();
            idRoot.pageDeinit();
        }

        // ====================================================================
        // do offline
        // ====================================================================
        function  doOffline( )
        {
            if ( ! isOnline ) { return; }

            isOnline = false;
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
            if ( idPageStk.depth == 1 ) { // last page.., go back to the main
                idRoot.request( "reqGoBack", null );
            } else {
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
                IcUiQmlApi.postMetaCall( idPriv, "popPage" );
            } else if ( req_str === "reqGoPage0" ) {
                IcUiQmlApi.postMetaCall( idPriv, "pushPage", "qrc:/qmlpagelayoutdemo/main/view/page0.qml" );
            } else if ( req_str === "reqGoPage1" ) {
                IcUiQmlApi.postMetaCall( idPriv, "pushPage", "qrc:/qmlpagelayoutdemo/main/view/page1.qml" );
            } else {
                console.info("request should not be recognized (%1)".arg( req_str ));
            }
        }
    }
}
