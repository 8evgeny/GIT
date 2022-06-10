import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2

ApplicationWindow {
    id: changeWindow
    width: 400
    height: 100
    visible: true
    title: qsTr("Сhanges")
    property bool flagCloseWindow: true

    FileDialog {
        id: saveFileDialog
        title: qsTr("Save configuration")
        selectExisting: false
        folder: shortcuts.home
        onAccepted: {
            console.log("Accepted: " + fileUrl)
            appCore.saveListOfStationAndGroupsQJson(urlToPath(fileUrl))
            if(flagCloseWindow)
                appCore.needCloseMainWindow()
        }
    }

    Label {
        id: labelCanges
        height: 40
        text: qsTr("The project contains changes")
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
    }

    Button {
        id: buttonSaveAs
        text: qsTr("Save as...")
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 5
        onClicked: {
            saveFileDialog.open()
        }
    }

    Button {
        id: buttonCancel
        text: qsTr("Cancel")
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 5
        onClicked: {
            if(flagCloseWindow)
                appCore.needCloseMainWindow()
            else
                changeWindow.close()
        }
    }
    onClosing: {
        if(flagCloseWindow)
            appCore.needCloseMainWindow()
    }
}
