import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts  1.3
import qmlpagelayoutdemo.main.view 1.0
import qxpack.indcom.ui_qml_base    1.0
import qxpack.indcom.ui_qml_control 1.0

IcPageBase {
    id: idRoot; implicitWidth: 640; implicitHeight: 480;

    // [HINT] override IcPageBase.doOffline()
    function  doOffline ( ) { idPriv.doOffline(); }

    // ////////////////////////////////////////////////////////////////////////
    // layout
    // ////////////////////////////////////////////////////////////////////////
    // ------------------------------------------------------------------------
    // only display in background
    // ------------------------------------------------------------------------
    ColumnLayout {
        anchors.fill: parent; spacing: 0;
        Rectangle {  // top
            Layout.fillWidth: true; Layout.preferredHeight: 80;
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop; color: "#3c3c3c";
            Text {
                anchors.centerIn: parent; font.pixelSize: parent.height * 0.5;
                color: "#a0a0a0"; text: "index in layout: %1".arg( idPageStk.currentIndex );
            }
        }
        Item { Layout.fillWidth: true; Layout.fillHeight: true; } // middle area
        Rectangle {  // bottom
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
            "  Demonstrates the use of 'StackLayout'." +
            "User can switch between multiple subpages in 'StackLayout'."
        );
    }

    // ------------------------------------------------------------------------
    // This app. use StackLayout to manage all pages, all pages are lived!
    // NOTE: this may make GPU memory too large, but in many small pages, switch
    // them are very smooth and quickly !
    // ------------------------------------------------------------------------
    StackLayout {
        id: idPageStk; anchors.fill: parent; currentIndex: -1;
    }

    // ////////////////////////////////////////////////////////////////////////
    // logic
    // ////////////////////////////////////////////////////////////////////////
    Component.onCompleted:   { idPriv.init();   }
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
            pushPage( "qrc:/qmlpagelayoutdemo/main/view/page1.qml");

            idHintPopup.reqEnterEventLoop = false;
            idHintPopup.modal = false;
            idHintPopup.open();

            idPageStk.currentIndex = 0;
            isOnline = true;
            idRoot.pageInit();
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
            // ----------------------------------------------------------------
            // [HINT] here we manually create object and append it to StackLayout
            // ----------------------------------------------------------------
            var cmpt = Qt.createComponent( page_url );
            while ( cmpt.status !== Component.Ready ) { }

            // append created object to idPageStk's children list
            var obj  = cmpt.createObject( idPageStk );
            obj.request.connect( idPriv.page_onRequest );

            cmpt.destroy(); // need not to use it.
        }

        // ====================================================================
        // pop current page
        // ====================================================================
        function popPage( obj )
        {
            // ----------------------------------------------------------------
            // [HINT] in this demo, StackLayout will automatically delete all
            // childrens ( contain your created object )
            // ----------------------------------------------------------------
            obj.request.disconnect( idPriv.page_onRequest );
            obj.destroy();
        }

        // ====================================================================
        // slot: handle the request from page
        // ====================================================================
        function  reqGoBack( ) { idRoot.request( "reqGoBack", null ); }
        function  page_onRequest( req_str, req_arg )
        {
            if ( req_str === "reqGoBack" ) {
               IcUiQmlApi.postMetaCall( idPriv, "reqGoBack" );
            } else if ( req_str === "reqGoPage0" ) {
                idPageStk.currentIndex = 0;
            } else if ( req_str === "reqGoPage1" ) {
                idPageStk.currentIndex = 1;
            } else {
                console.info("request should not be recognized (%1)".arg( req_str ));
            }
        }
    }
}
