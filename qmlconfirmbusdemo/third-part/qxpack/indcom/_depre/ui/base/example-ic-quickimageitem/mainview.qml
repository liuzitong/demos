import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQml   2.2
import qxpack.indcom.ui.base  1.0

Item {
   id: idRoot; anchors.fill: parent;
   implicitWidth: parent.width; implicitHeight: parent.height;

    Row {
        anchors.fill: parent;
        IcQuickImageItem {
            imageSource: gQuickImageSrc;
            width: 400; height: 400;
        }
    }
}
