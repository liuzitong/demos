import QtQuick 2.7
import QtQml   2.2
import QtQuick.Controls 2.0
import qxpack.indcom.ui.api  1.0

Item {
    id: idRoot; anchors.fill: parent;

    // //////////////////////////////////////////////////////////
    //  export
    // //////////////////////////////////////////////////////////
    function open ( ) { if ( backgroundVisible ) { idPopupBg.open(); } idPopup.open (); QxIcUiApi.enterEventLoop(); }
    function close( ) { idPopup.close(); idPopupBg.close(); }

    property bool  destroyOnClose : false;
    property alias contentItem    : idPopup.contentItem;
    property alias modal          : idPopup.modal;
    property alias closePolicy    : idPopup.closePolicy;
    property alias backgroundVisible: idPopupBg.visible;
    property alias backgroundOpacity: idBgRect.opacity;
    property alias backgroundColor  : idBgRect.color;
    property alias popupX : idPopup.x;
    property alias popupY : idPopup.y;
    readonly property alias popupWidth : idPopup.width;
    readonly property alias popupHeight: idPopup.height;

    signal opened ( );
    signal closed ( );
    signal aboutToDestroy ( var obj );

    // ///////////////////////////////////////////////////////////
    // layout
    // //////////////////////////////////////////////////////////
    Popup {
        id: idPopupBg; x: 0; y: 0; width: parent.width; height: parent.height; closePolicy: Popup.NoAutoClose;
        dim: false; modal: idPopup.modal; padding: 0; background: Item { }
        contentItem: Rectangle { id: idBgRect; opacity: 0.6; color: "#000000"; anchors.fill: parent; }
    }

    Popup {
        // ====================================================
        // about the width & height.
        // normally, user item must provide the implicitWidth and implictHeight
        // Popup always use them to calc. the popup area size.
        // ====================================================
        id: idPopup; modal: true; dim: false; background: Item { }
        padding: 0; margins: 0; closePolicy: Popup.NoAutoClose;
        x : ( parent.width - width ) / 2; y: ( parent.height - height) / 2;

        onClosed: {
            idRoot.closed();
            if ( idRoot.destroyOnClose ) {
                idRoot.aboutToDestroy( idRoot );
                idRoot.destroy();
            }
            QxIcUiApi.leaveEventLoop();
        }

        onOpened: { idRoot.opened(); }
    }

    Component.onDestruction: {
        //console.debug("qxpack.indcom.ui.base.ModalPopupDialog.destruction()");
    }

}
