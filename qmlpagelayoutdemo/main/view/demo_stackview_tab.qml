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
    ColumnLayout {   // background
        anchors.fill: parent; spacing: 0;
        Rectangle {  // header area
            Layout.fillWidth: true; Layout.preferredHeight: 80;
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop; color: "#3c3c3c";
            Text {
                anchors.centerIn: parent; font.pixelSize: parent.height * 0.5;
                color: "#a0a0a0";
                text: "current tab: %1".arg( idPageStk.currentItem.pageName );
            }
        }
        Item { Layout.fillWidth: true; Layout.fillHeight: true; } // middle area
        Rectangle {  // bottom area
            Layout.fillWidth: true; Layout.preferredHeight: 80;
            Layout.alignment: Qt.AlignVCenter | Qt.AlignBottom; color: "#3c3c3c";
        }
    }

    // ------------------------------------------------------------------------
    //  hint text item
    // ------------------------------------------------------------------------
    HintText {
        id: idHintPopup;  modal: false; reqEnterEventLoop: false;
        contentItem.implicitWidth: parent.width * 0.7;
        contentItem.implicitHeight: parent.height * 0.5;
        message: qsTr(
            "  This demo also use 'StackView', but it only contain " +
            "one page. Each page will replace the old one."
        );
    }

    // ------------------------------------------------------------------------
    // This app. use StackView to emulate Tab Pages, we just create just one page
    // at one time. The TabView will create all pages at one time.
    // ------------------------------------------------------------------------
    StackView {
        id: idPageStk; anchors.fill: parent;
    }

    // ////////////////////////////////////////////////////////////////////////
    // logic
    // ////////////////////////////////////////////////////////////////////////
    Component.onCompleted:   { idPriv.init();   }
    Component.onDestruction: { idPriv.deinit(); }
    QtObject {
        id: idPriv;
        property string  currPage : "";
        property bool    isOnline : false;

        // ====================================================================
        // initailize this page
        // ====================================================================
        function init( )
        {
            loadPage( "qrc:/qmlpagelayoutdemo/main/view/page0.qml");
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
        function loadPage( page_url )
        {
            // ----------------------------------------------------------------
            // [HINT] do offline of the page if current page is not offline.
            // ----------------------------------------------------------------
            if ( idPageStk.depth > 0 ) {
                idPageStk.currentItem.doOffline( );
                idPageStk.currentItem.request.disconnect( idPriv.page_onRequest );
            }
            currPage = page_url;
            idPageStk.replace( page_url, StackView.Immediate );
            idPageStk.currentItem.request.connect( idPriv.page_onRequest );
        }

        // ====================================================================
        // slot: handle the request from page
        // ====================================================================
        function  reqGoBack( ) { idRoot.request( "reqGoBack", null ); }
        function  page_onRequest( req_str, req_arg )
        {
            // ----------------------------------------------------------------
            // [HINT]  use IcUiQmlApi.postMetaCall to make a later call.
            // It will make stack shallow, and CPU is released from the previous JS code
            // ----------------------------------------------------------------
            if ( req_str === "reqGoBack" ) { // NOTE: use queued call to do request!
                IcUiQmlApi.postMetaCall( idPriv, "reqGoBack" );
            } else if ( req_str === "reqGoPage0" ) {
                IcUiQmlApi.postMetaCall( idPriv, "loadPage", "qrc:/qmlpagelayoutdemo/main/view/page0.qml" );
            } else if ( req_str === "reqGoPage1" ) {
                IcUiQmlApi.postMetaCall( idPriv, "loadPage", "qrc:/qmlpagelayoutdemo/main/view/page1.qml" );
            } else {
                console.info("request should not be recognized (%1)".arg( req_str ));
            }
        }
    }
}
