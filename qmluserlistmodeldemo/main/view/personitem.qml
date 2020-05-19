import QtQuick 2.7
import QtQml 2.2
import QtQuick.Controls 2.0
import QtQuick.Layouts  1.3

Item {
    id: idRoot;
    implicitWidth: 600; implicitHeight: 80;

    // ////////////////////////////////////////////////////////////////////////
    // layout
    // ////////////////////////////////////////////////////////////////////////
    RowLayout {
        id: idRow; anchors.fill: parent; spacing: 3;
        Label {
            id: idId;
            Layout.preferredWidth: idRoot.width * 0.3 ; Layout.fillHeight: true;
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter;
            verticalAlignment: Text.AlignVCenter; horizontalAlignment: Text.AlignHCenter;
            font.pixelSize: idRow.height * 0.3
            text: model.id;
        }
        Label {
            id: idName;
            Layout.preferredWidth: idRoot.width * 0.3; Layout.fillHeight: true;
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter;
            verticalAlignment: Text.AlignVCenter; horizontalAlignment: Text.AlignHCenter;
            font.pixelSize: idRow.height * 0.3;
            text: model.name;
        }
        Item {
            Layout.fillHeight: true; Layout.fillWidth: true;
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter;
        }

    }

}
