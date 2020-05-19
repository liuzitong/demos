import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQml   2.2
import qxpack.indcom.ui_qml_base  1.0
import qxpack.indcom.ui_qml_control 1.0

Item {
   id: idRoot; anchors.fill: parent;
   implicitWidth: parent.width; implicitHeight: parent.height;

    Row {
        anchors.fill: parent;
        Item {
            width: 400; height: 400;
            Rectangle { anchors.fill: idImItem; color: "#808080"; }
            IcQuickImageItem {
                id: idImItem; imageData: gQuickImageSrc;
                width: 400; height: 400;
            }
            Text {
                anchors.bottom: idImItem.bottom; color: "red";
                text : "fillModeStr: %1".arg( idImItem.fillModeStr );
            }
        }
        Item {
            width: 400; height: 400;
            Rectangle { anchors.fill: idImItemStrech; color:"#808080"; }
            IcQuickImageItem {
                id: idImItemStrech; imageData: gQuickImageSrc;
                width: 400; height: 200; fillMode: Qt.IgnoreAspectRatio;
            }
            Text {
                anchors.bottom: idImItemStrech.bottom; color: "red";
                text : "fillModeStr: %1".arg( idImItemStrech.fillModeStr );
            }
        }
    }
}
