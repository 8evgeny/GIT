import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2
import audioRec 1.0



Item {

    id: record
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.verticalCenter: parent.verticalCenter
    height: parent.height
    width: parent.width

    property var dialogHeight: 300
    property var dialogWidth: 300
    property string dialogTitle: "Запись сообщения"

    Dialog {
        id: dialog
        height: dialogHeight
        width: dialogWidth
        title: dialogTitle
        visible: true
        standardButtons: Dialog.Cancel
        Button {
            id: recButton
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            text: (( audioRecorder_Sample.recording ) ? "Stop" : "Record" )
            onClicked:  (( audioRecorder_Sample.recording ) ? audioRecorder_Sample.stop ( ) : audioRecorder_Sample.record ( ))
           // DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole
        }

    }

        AudioRecorder {
            id: audioRecorder_Sample
            onRecordingChanged: {console.log("File is recording:")}
        }



}
