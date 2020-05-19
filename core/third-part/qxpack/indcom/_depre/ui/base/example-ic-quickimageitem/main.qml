import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQml   2.2
import qxpack.indcom.ui.api  1.0

ApplicationWindow {
    id: idBoot; width: 640; height: 480; visible: true;

    Component.onCompleted: {
        console.info("[] initalize QxIcUiApi, version:" + QxIcUiApi.apiVersion );
        idContentLoader.source = "qrc:/mainview.qml"
    }
    Loader {
        id: idContentLoader; anchors.fill: parent;
    }
}

