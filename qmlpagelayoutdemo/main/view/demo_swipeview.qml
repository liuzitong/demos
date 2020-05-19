import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts  1.3

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
    // background
    // ------------------------------------------------------------------------
    ColumnLayout {   // background
        anchors.fill: parent; spacing: 0;
        Rectangle {  // header area
            Layout.fillWidth: true; Layout.preferredHeight: 80;
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop; color: "#3c3c3c";

            // draw a page indicator by using diff color.
            PageIndicator {
                id: idPgIndi; anchors.bottomMargin: parent.height * 0.1;
                anchors.bottom: parent.bottom; anchors.horizontalCenter: parent.horizontalCenter;
                count: idPageView.count; currentIndex: idPageView.currentIndex;
                delegate: Rectangle { // the color dot
                    implicitWidth: 16; implicitHeight: 16; radius: width / 2; color:"#f58220";
                    opacity: ( idPgIndi.currentIndex === index ? 0.95 : ( pressed ? 0.7 : 0.45 ));
                    Behavior on opacity { OpacityAnimator { duration: 300; } }
                }
            }
        }
        Item { Layout.fillWidth: true; Layout.fillHeight: true; } // middle area
        Rectangle {  // bottom area
            Layout.fillWidth: true; Layout.preferredHeight: 80;
            Layout.alignment: Qt.AlignVCenter | Qt.AlignBottom; color: "#3c3c3c";
        }
    }

    // ------------------------------------------------------------------------
    // hinit text
    // ------------------------------------------------------------------------
    HintText {
        id: idHintPopup;  modal: false; reqEnterEventLoop: false; focus: false;
        contentItem.implicitWidth: parent.width * 0.7;
        contentItem.implicitHeight: parent.height * 0.3;
        message: qsTr(
            "  Demonstrates the use of 'Swipe View'." +
            "All pages is loaded, user can switch them " +
            "by swipe them or click 'Go PageX'."
        );
    }

    // ------------------------------------------------------------------------
    // This app. use SwipeView to manage all pages, all pages are lived!
    // NOTE: this may make GPU memory too large, but in many small pages, switch
    // them are very smooth !
    // ------------------------------------------------------------------------
    SwipeView {
        id: idPageView; anchors.fill: parent; currentIndex: -1;
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
            idPageView.currentIndex = 0;
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
            idRoot.pageDeinit( );
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
            // [HINT] here we manually create object and append it to SwipeView
            // ----------------------------------------------------------------
            var cmpt = Qt.createComponent( page_url );
            while ( cmpt.status !== Component.Ready ) { }

            // append created object to idPageStk's children list
            // your also can create object directly inside the SwipeView
            var obj  = cmpt.createObject( idPageView );
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
        function reqGoBack() { idRoot.request("reqGoBack", null);}
        function  page_onRequest( req_str, req_arg )
        {
            if ( req_str === "reqGoBack" ) {
                IcUiQmlApi.postMetaCall( idPriv, "reqGoBack" );
            } else if ( req_str === "reqGoPage0" ) {
                idPageView.currentIndex = 0;
            } else if ( req_str === "reqGoPage1" ) {
                idPageView.currentIndex = 1;
            } else {
                console.info("request should not be recognized (%1)".arg( req_str ));
            }
        }
    }
}
