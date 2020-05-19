import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

import qxpack.indcom.ui_qml_base 1.0 // [HINT] this is the pre-registered module name.
import qmluserlistmodeldemo.main.view  1.0

ApplicationWindow {
    visible: true;
    width: 640;  height: 480;
    title: qsTr("Hello World")

    // ////////////////////////////////////////////////////////////////////////
    // layout
    // ////////////////////////////////////////////////////////////////////////
    ColumnLayout {
       anchors.fill: parent;

       // [HINT] use layout.xxxx to let ListView size
       ListView {
           id: idInfoList; clip: true;
           Layout.fillWidth: true; Layout.fillHeight: true; Layout.alignment: Qt.AlignCenter;
           delegate: PersonItem { width: parent.width; }
       }

       // [HINT] the area of input a person
       Rectangle {
           id: idPersonInfoArea
           Layout.fillWidth: true; Layout.preferredHeight: 50; Layout.alignment: Qt.AlignCenter;

           RowLayout {
               anchors.fill: parent; spacing: 6;

               TextField {
                   id: idPersonId;
                   Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter;
                   Layout.fillHeight: true; Layout.preferredWidth: parent.width * 0.5;
                   Component.onCompleted: { placeholderText = qsTr("input id here");  }
               }
               TextField {
                   id: idPersonName;
                   Layout.alignment: Qt.AlignRight | Qt.AlignVCenter;
                   Layout.fillHeight: true; Layout.fillWidth: true;
                   Component.onCompleted: { placeholderText = qsTr("input name here");  }
               }
           }
       }

       // [HINT] use Item as a container
       Item {
           id: idBtnArea;
           Layout.fillWidth: true; Layout.preferredHeight: 80;
           Layout.alignment: Qt.AlignBottom | Qt.AlignHCenter;

           // [HINT] here use RowLayout as a row container, let Buttons in a row
           RowLayout {
               anchors.fill: parent;
               Item   {
                   Layout.fillWidth: true;  Layout.fillHeight: true;
                   Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter;
               }
               Button {
                   text: qsTr("Rand.Gen.");
                   Layout.preferredWidth: 120; Layout.fillHeight: true;
                   Layout.alignment: Qt.AlignCenter;
                   onClicked: idPriv.m_people_mgr.randGenPeople();
               }
               Button {
                   text: qsTr("Clear");
                   Layout.preferredWidth: 120; Layout.fillHeight: true;
                   Layout.alignment: Qt.AlignCenter;
                   onClicked: idPriv.m_people_mgr.clearPeople();
               }
               Button {
                   text: qsTr("add Person");
                   Layout.preferredWidth: 120; Layout.fillHeight: true;
                   Layout.alignment: Qt.AlignCenter;
                   onClicked: idPriv.addPerson();
               }
               Item  {
                   Layout.fillWidth: true; Layout.fillHeight: true;
                   Layout.alignment: Qt.AlignRight | Qt.AlignVCenter;
               }
           }
       }
    }

    // ////////////////////////////////////////////////////////////////////////
    // logic
    // ////////////////////////////////////////////////////////////////////////
    Component.onCompleted:   { idPriv.init();  }
    Component.onDestruction: { idPriv.deinit();}
    QtObject {
        id: idPriv;
        property var m_people_mgr : null;

        // ====================================================================
        // init.
        // ====================================================================
        function init( )
        {
            m_people_mgr = IcUiQmlApi.appCtrl.objMgr.attachObj( "qmlUserListmodelDemo::PpPeopleMgrVm", false );
            idInfoList.model = m_people_mgr.peopleList;
        }

        // ====================================================================
        // deinit.
        // ====================================================================
        function deinit()
        {
            idInfoList.model = null;
            IcUiQmlApi.appCtrl.objMgr.detachObj( "qmlUserListmodelDemo::PpPeopleMgrVm", m_people_mgr );
        }

        // ====================================================================
        // add person into list
        // ====================================================================
        function addPerson( )
        {
            if ( idPersonId.text === "" ) { return; }
            var pi = {};
            pi["id"] = idPersonId.text;
            pi["name"] = idPersonName.text;
            m_people_mgr.addPerson( pi );
        }

    }
}
