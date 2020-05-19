import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

import qxpack.indcom.ui_qml_base 1.0     // [HINT] this is the pre-registered module name.
import qxpack.indcom.ui_qml_control 1.0  // [HINT] ModalPopupDialog is in it

ModalPopupDialog {   // this is the wrapped Popup element in ui_qml_control
    id: idPopup;

    property alias message : idMsgDisp.text;
    property bool  isOK    : false;
    property alias contentWidth : idContent.implicitWidth;
    property alias contentHeight: idContent.implicitHeight;

    // ////////////////////////////////////////////////////////////////////////
    // layout
    // ////////////////////////////////////////////////////////////////////////
    backgroundColor: "red"; backgroundVisible: false;
    contentItem: Rectangle {
        // [HINT] Popup element need implicitWidth & implicitHeight to calc. the right position
        id: idContent; implicitWidth: 640; implicitHeight: 480; color:"#c0c0c0";

        // ColumnLayout arrange text area and control area.
        ColumnLayout { anchors.fill: parent;
            Text {
                id: idMsgDisp; text: "No message";
                horizontalAlignment: Text.AlignHCenter;  verticalAlignment: Text.AlignVCenter;
                Layout.fillWidth: true; Layout.fillHeight: true; Layout.alignment: Qt.AlignCenter;
            }
            Item {
                id: idCtrlArea; Layout.fillWidth: true; Layout.preferredHeight: parent.height * 0.2;
                RowLayout { anchors.fill: parent;
                    Button {
                        id: idBtnYes; Layout.fillWidth: true; Layout.fillHeight: true;
                        Layout.alignment: Qt.AlignCenter; text: qsTr("Yes");
                        onClicked: { idPopup.isOK = true; idPopup.close(); }
                    }
                    Button {
                        id: idBtnNo;  Layout.fillWidth: true; Layout.fillHeight: true;
                        Layout.alignment: Qt.AlignCenter; text: qsTr("No");
                        onClicked: { idPopup.isOK = false; idPopup.close(); }
                    }
                }
            }
        }
    }
}
