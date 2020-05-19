import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQml   2.2
import QtTest  1.0
import qxpack.indcom.ui.api  1.0
import qxpack.indcom.ui.base 1.0

TestCase {
    id: idRoot;

    function test_apiVersionCall()
    {
        verify( QxIcUiApi.apiVersion !== "", "could not read api version.");
    }

    Component {
        id: idWinCmpt;

        ApplicationWindow {
            id: idWin; width: 400; height: 400;
            Component {
                id: idPopupDialogCmpt;
                ModalPopupDialog {
                    id: idPopup; backgroundColor: "red";
                    contentItem: Rectangle {
                        implicitWidth: 100; implicitHeight: 100; color:"green";
                        onVisibleChanged: { if( visible ) { idTmr.start(); }}
                        Timer {
                            id: idTmr; interval: 1000;
                            onTriggered: { idPopup.close(); }
                        }
                    }
                }
            }
            Component.onCompleted: { QxIcUiApi.postMetaCall( idWin, "showPopup"); }

            function showPopup( ) {
                var p = idPopupDialogCmpt.createObject( idWin );
                p.destroyOnClose = true;
                p.open();
                p = null;
                QxIcUiApi.leaveEventLoop();
            }
        }
    }

    function test_showWin()
    {
        var obj = idWinCmpt.createObject( idRoot );
        obj.show();
        QxIcUiApi.enterEventLoop();
        obj.destroy();
    }
}
