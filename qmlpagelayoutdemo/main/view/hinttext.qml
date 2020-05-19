import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

import qxpack.indcom.ui_qml_control 1.0

IcModalPopupDialog {
    id: idRoot;
    property alias message : idMsgDisp.text;

    contentItem: Rectangle {
        color: "#60eec55a"; implicitWidth: 300; implicitHeight: 180;
        radius: width * 0.1; border.color: "#323232"; border.width: 5;
        Text {
           anchors.fill: parent; anchors.margins: parent.border.width * 3;
           font.pixelSize: parent.height * 0.1;
           id: idMsgDisp; text: "No message";
           wrapMode: Text.WrapAnywhere;
           horizontalAlignment: Text.AlignLeft;  verticalAlignment: Text.AlignVCenter;
       }
   }

}
