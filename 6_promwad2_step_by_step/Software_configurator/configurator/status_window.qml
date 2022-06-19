import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2

ApplicationWindow {
    id: statusWindowsMsg
    width: 300

    height: 100
    visible: true

    property string nameId: ""
    property string statusStr: ""

    title: qsTr("Status window")

    maximumHeight: height
    maximumWidth: width

    minimumHeight: height
    minimumWidth: width

    Label {
        id: labelText
        text: qsTr("Uploaded ") + statusStr + ": " + nameId
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.fill: parent
    }
}
