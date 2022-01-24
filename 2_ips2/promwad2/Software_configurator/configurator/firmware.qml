import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2

Window {
    id: firmwareWindow
    width: 850
    height: 800
    visible: true
    title: qsTr("Software Update")

    property int spaceDefault: 5
    property int sizeDefault: 40
    property url urlBin: ""

    FileDialog {
        id: loadFileDialog
        title: qsTr("Select firmware file")
        nameFilters: [qsTr("Firmware file (* .bin)")]
        folder: shortcuts.home
        onAccepted: {
            console.log("Accepted: " + fileUrl)
            labelPath.text = fileUrl.toString()
            urlBin = fileUrl
        }
    }

    Button {
        id: buttonLoadFirmware
        text: qsTr("Load firmware file")
        anchors.top: parent.top
        anchors.topMargin: spaceDefault
        anchors.left: parent.left
        anchors.leftMargin: spaceDefault
        onClicked: {
            loadFileDialog.open()
        }
    }

    Rectangle {
        id: rectanglePathName
        height: sizeDefault
        color: "#ffffff"
        anchors.top: buttonLoadFirmware.bottom
        anchors.topMargin: spaceDefault
        anchors.right: parent.right
        anchors.rightMargin: spaceDefault
        anchors.left: parent.left
        anchors.leftMargin: spaceDefault

        Label {
            id: labelPathName
            text: qsTr("Path to the firmware file:")
            verticalAlignment: Text.AlignVCenter
            anchors.fill: parent
        }
    }

    Rectangle {
        id: rectangleNameKey
        height: sizeDefault
        color: "#ffffff"
        anchors.top: rectanglePath.bottom
        anchors.topMargin: spaceDefault
        anchors.right: parent.right
        anchors.rightMargin: spaceDefault
        anchors.left: parent.left
        anchors.leftMargin: spaceDefault

        Label {
            id: labelNameKey
            text: qsTr("Encryption Key (AES 128):")
            verticalAlignment: Text.AlignVCenter
            anchors.fill: parent
        }
    }

    Rectangle {
        id: rectanglePath
        height: sizeDefault
        color: "#ffffff"
        anchors.top: rectanglePathName.bottom
        anchors.topMargin: spaceDefault
        anchors.right: parent.right
        anchors.rightMargin: spaceDefault
        anchors.left: parent.left
        anchors.leftMargin: spaceDefault

        Label {
            id: labelPath
            text: ""
            verticalAlignment: Text.AlignVCenter
            anchors.fill: parent
        }
    }

    TextField {
        id: textFieldKey
        text: ""
        anchors.left: parent.left
        anchors.leftMargin: spaceDefault
        anchors.top: rectangleNameKey.bottom
        anchors.topMargin: spaceDefault
    }

    Button {
        id: buttonGenerate
        text: qsTr("Generate Encrypted Firmware")
        anchors.left: parent.left
        anchors.leftMargin: spaceDefault
        anchors.top: textFieldMainVersion.bottom
        anchors.topMargin: spaceDefault

        onClicked: {
            if ((textFieldKey.text.length === 16)
                    && (!isNaN(parseInt(textFieldMainVersion.text)))
                    && (!isNaN(parseInt(textFieldSubVersion.text)))) {
                labelDateTimeCurrent.text = Qt.formatDateTime(
                            new Date(), "dd.MM.yyyy hh:mm:ss")
                appCore.encryptionBinFile(labelPath.text, textFieldKey.text,
                                          labelDateTimeCurrent.text,
                                          parseInt(textFieldMainVersion.text),
                                          parseInt(textFieldSubVersion.text))
            }
        }
    }

    Rectangle {
        id: rectangleMainVersion
        height: sizeDefault
        color: "#ffffff"
        anchors.right: parent.right
        anchors.rightMargin: firmwareWindow.width / 2 - 2 * spaceDefault
        Label {
            id: labelMainVersion
            text: qsTr("New firmware version:")
            anchors.rightMargin: 0
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
        }
        anchors.top: textFieldKey.bottom
        anchors.topMargin: spaceDefault
        anchors.leftMargin: spaceDefault
        anchors.left: parent.left
    }

    TextField {
        id: textFieldMainVersion
        text: ""
        anchors.right: rectangleMainVersion.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: spaceDefault
        anchors.top: rectangleMainVersion.bottom
        anchors.topMargin: spaceDefault
    }

    Rectangle {
        id: rectangleSubVersion
        height: sizeDefault
        color: "#ffffff"
        anchors.right: parent.right
        anchors.rightMargin: spaceDefault
        Label {
            id: labelSubVersion
            text: qsTr("Update serial number:")
            anchors.rightMargin: 0
            verticalAlignment: Text.AlignVCenter
            anchors.fill: parent
        }
        anchors.top: textFieldKey.bottom
        anchors.topMargin: spaceDefault
        anchors.leftMargin: spaceDefault
        anchors.left: rectangleMainVersion.right
    }

    TextField {
        id: textFieldSubVersion
        text: ""
        anchors.right: rectangleSubVersion.right
        anchors.rightMargin: 0
        anchors.left: textFieldMainVersion.right
        anchors.leftMargin: spaceDefault
        anchors.top: rectangleSubVersion.bottom
        anchors.topMargin: spaceDefault
    }

    Rectangle {
        id: rectangleSizeFile
        height: sizeDefault
        color: "#ffffff"
        anchors.topMargin: spaceDefault
        anchors.right: parent.right
        anchors.rightMargin: spaceDefault
        Label {
            id: labelSizeFile
            text: qsTr("Size of encrypted firmware:")
            verticalAlignment: Text.AlignVCenter
            anchors.fill: parent
        }
        anchors.top: buttonGenerate.bottom
        anchors.leftMargin: spaceDefault
        anchors.left: parent.left
    }

    Rectangle {
        id: rectangleSizeFileCurrent
        height: sizeDefault
        color: "#ffffff"
        anchors.right: parent.right
        anchors.rightMargin: spaceDefault
        Label {
            id: labelSizeFileCurrent
            text: ""
            verticalAlignment: Text.AlignVCenter
            anchors.fill: parent
        }
        anchors.top: rectangleSizeFile.bottom
        anchors.topMargin: spaceDefault
        anchors.leftMargin: spaceDefault
        anchors.left: parent.left
    }

    Rectangle {
        id: rectangleMD5
        height: sizeDefault
        color: "#ffffff"
        anchors.right: parent.right
        anchors.rightMargin: spaceDefault
        Label {
            id: labelMD5
            text: "MD5:"
            verticalAlignment: Text.AlignVCenter
            anchors.fill: parent
        }
        anchors.top: rectangleSizeFileCurrent.bottom
        anchors.topMargin: spaceDefault
        anchors.leftMargin: spaceDefault
        anchors.left: parent.left
    }

    Rectangle {
        id: rectangleMD5Current
        height: sizeDefault
        color: "#ffffff"
        anchors.right: parent.right
        anchors.rightMargin: spaceDefault
        Label {
            id: labelMD5Current
            text: ""
            verticalAlignment: Text.AlignVCenter
            anchors.fill: parent
        }
        anchors.top: rectangleMD5.bottom
        anchors.topMargin: spaceDefault
        anchors.leftMargin: spaceDefault
        anchors.left: parent.left
    }

    Rectangle {
        id: rectangleDateTime
        height: sizeDefault
        color: "#ffffff"
        anchors.right: parent.right
        anchors.rightMargin: spaceDefault
        Label {
            id: labelDateTime
            text: qsTr("Time and date the firmware was created:")
            verticalAlignment: Text.AlignVCenter
            anchors.fill: parent
        }
        anchors.top: rectangleMD5Current.bottom
        anchors.topMargin: spaceDefault
        anchors.leftMargin: spaceDefault
        anchors.left: parent.left
    }

    Rectangle {
        id: rectangleDateTimeCurrent
        height: sizeDefault
        color: "#ffffff"
        anchors.right: parent.right
        anchors.rightMargin: spaceDefault
        Label {
            id: labelDateTimeCurrent
            text: ""
            verticalAlignment: Text.AlignVCenter
            anchors.fill: parent
        }
        anchors.top: rectangleDateTime.bottom
        anchors.topMargin: spaceDefault
        anchors.leftMargin: spaceDefault
        anchors.left: parent.left
    }

    Rectangle {
        id: rectangleEncrypted
        height: sizeDefault
        color: "#ffffff"
        anchors.right: parent.right
        anchors.rightMargin: spaceDefault
        Label {
            id: labelEncrypted
            text: qsTr("The path to the file with encrypted firmware:")
            verticalAlignment: Text.AlignVCenter
            anchors.fill: parent
        }
        anchors.top: rectangleDateTimeCurrent.bottom
        anchors.topMargin: spaceDefault
        anchors.leftMargin: spaceDefault
        anchors.left: parent.left
    }

    Rectangle {
        id: rectangleEncryptedPath
        height: sizeDefault
        color: "#ffffff"
        anchors.right: parent.right
        anchors.rightMargin: spaceDefault
        Label {
            id: labelEncryptedPath
            text: ""
            verticalAlignment: Text.AlignVCenter
            anchors.fill: parent
        }
        anchors.top: rectangleEncrypted.bottom
        anchors.topMargin: spaceDefault
        anchors.leftMargin: spaceDefault
        anchors.left: parent.left
    }

    Button {
        id: buttonUpload
        x: spaceDefault
        text: qsTr("Upload firmware")
        anchors.top: rectangleEncryptedPath.bottom
        anchors.topMargin: spaceDefault
        onClicked: {
            //if checkBoxSend.checkState == true sending data by multicast
            appCore.startLoadFirmware(textFieldIDStation.text,
                                      checkBoxSend.checkState)
        }
    }

    Connections {
        target: appCore

        onSendEncFileName: {
            labelEncryptedPath.text = encFileName
        }

        onSendMD5FileName: {
            labelMD5Current.text = MD5
        }

        onSendSizeFileName: {
            labelSizeFileCurrent.text = sizeFile
        }

        onSendToQmlSubscriberAssignmentFirmware: {
            textFieldIDStation.text = strAssignmentName
        }
    }

    Text {
        id: elementIDStation
        width: 250
        height: sizeDefault
        text: qsTr("Station ID for configuration")
        verticalAlignment: Text.AlignVCenter
        anchors.verticalCenter: textFieldIDStation.verticalCenter
        anchors.left: textFieldIDStation.right
        anchors.leftMargin: 10
        anchors.topMargin: 5
        font.pixelSize: 12
    }

    TextField {
        id: textFieldIDStation
        height: sizeDefault
        text: ""
        anchors.verticalCenter: buttonIdSelect.verticalCenter
        anchors.leftMargin: 10
        anchors.left: buttonIdSelect.right
    }

    Button {
        id: buttonIdSelect

        width: 45
        height: sizeDefault
        text: "..."
        anchors.verticalCenter: buttonUpload.verticalCenter
        anchors.left: buttonUpload.right
        anchors.leftMargin: 10
        anchors.rightMargin: 0

        onClicked: {
            var component = Qt.createComponent("assignment_list_firmware.qml")
            var windowSel = component.createObject(firmwareWindow)

            windowSel.show()
        }
    }

    CheckBox {
        id: checkBoxSend
        text: qsTr("Send to everyone")
        anchors.left: elementIDStation.right
        anchors.leftMargin: 0
        anchors.verticalCenter: textFieldIDStation.verticalCenter
        onClicked: {
            if (checkBoxSend.checkState) {
                elementIDStation.visible = false
                textFieldIDStation.visible = false
                buttonIdSelect.visible = false
            } else {
                elementIDStation.visible = true
                textFieldIDStation.visible = true
                buttonIdSelect.visible = true
            }
        }
    }
}
