import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.2



Item {
    id: dialog
    height: 200
    width: 200

    property var dialogHeight: 300
    property var dialogWidth: 300
    property string fileDialogTitle: "Загрузить файл"

    FileDialog {
        id: fileDialog
        title: fileDialogTitle
        folder: shortcuts.home
        nameFilters: [ "Sound files *.mp3 *.wav (*.mp3 *.wav)", "All files (*)" ]
        selectedNameFilter: "All files (*)"
        onAccepted: {
            console.log("You chose: " + fileDialog.fileUrls)

            //Qt.quit()
        }
        onRejected: {
            console.log("Canceled")
            //Qt.quit()
        }
        Component.onCompleted: visible = true
    }
}
