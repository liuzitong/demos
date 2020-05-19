import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import qxpack.indcom.ui_qml_control 1.0

IcPageBase {
    id: idRoot; implicitWidth: 640; implicitHeight: 480;
    pageName: "Page0";

    // [HINT] override the IcPageBase.doOffline
    function  doOffline ( ) { idPriv.doOffline(); }

    // ////////////////////////////////////////////////////////////////////////
    // layout
    // ////////////////////////////////////////////////////////////////////////
    ColumnLayout {
        anchors.fill: parent; spacing: 0;
        Item {  // padding head
            Layout.fillWidth: true; Layout.preferredHeight: 80; Layout.alignment: Qt.AlignTop
        }
        Item {  // context area
            Layout.fillWidth: true; Layout.fillHeight: true; Layout.alignment: Qt.AlignCenter;

        }
        Item {  // command area
            Layout.fillWidth: true; Layout.preferredHeight: 80; Layout.alignment: Qt.AlignBottom;
            RowLayout { // we put the command button in this area
                anchors.fill: parent; spacing: 3;

                Item   { Layout.preferredWidth: 0; Layout.fillHeight: true; Layout.alignment: Qt.AlignLeft; }
                Button {
                    Layout.preferredWidth: 120; Layout.preferredHeight: 70; Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter;
                    text: qsTr("Go Back"); onClicked: idRoot.request( "reqGoBack", null );
                }
                Item   { Layout.preferredWidth: parent.width * 0.1; Layout.fillHeight: true; Layout.alignment: Qt.AlignLeft; }
                Button {
                    Layout.preferredWidth: 120; Layout.preferredHeight: 70; Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter;
                    text: qsTr("Call Page1"); onClicked: idRoot.request( "reqGoPage1", null );
                }
                Item   { Layout.fillWidth: true; Layout.fillHeight: true; }
                Button {
                    Layout.preferredWidth: 120; Layout.preferredHeight: 70; Layout.alignment: Qt.AlignRight | Qt.AlignVCenter;
                    text: qsTr("Demo0");
                }
                Button {
                    Layout.preferredWidth: 120; Layout.preferredHeight: 70; Layout.alignment: Qt.AlignRight | Qt.AlignVCenter;
                    text: qsTr("Demo1");
                }
                Item { Layout.preferredWidth: 0; Layout.fillHeight: true; Layout.alignment: Qt.AlignRight; }
            }
        }
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
        // init. this page
        // ====================================================================
        function init( )
        {
            console.debug("page0 init.");
            isOnline = true;
            idRoot.pageInit( ); // emit this signal the page has been init
        }

        // ====================================================================
        // de-init this page
        // ====================================================================
        function  deinit( )
        {
            doOffline();
            idRoot.pageDeinit( ); // emit this signal the page deinit.
            console.debug("page0 deinit.");
        }

        // ====================================================================
        // do offline
        // ====================================================================
        function  doOffline( )
        {
            if ( ! isOnline ) { return; }


            console.debug("page0 offline.");
            isOnline = false;
        }
    }
}
