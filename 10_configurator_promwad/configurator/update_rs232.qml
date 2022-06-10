import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2

ApplicationWindow {
    id: updateRS232Window
    width: 270
    height: 400
    visible: true
    title: qsTr("Download configuration via RS232")

    property string portOpen: qsTr("Open port: ")

    ComboBox {
        id: comboBoxSelectUsb
        width: 250
        height: 35
        anchors.top: textSelectUsb.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        model: ListModel {
            id: modelUsbTTY
        }
    }

    Text {
        id: textSelectUsb
        width: 250
        height: 15
        text: qsTr("Choosing ttyUSBx/COMx port")
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 12
    }

    Button {
        id: buttonOpenPort
        width: 120
        height: 40
        text: qsTr("Open port")
        anchors.top: comboBoxSpeed.bottom
        anchors.topMargin: 5
        anchors.left: comboBoxSpeed.left
        anchors.leftMargin: 0
    }

    ComboBox {
        id: comboBoxSpeed
        width: 250
        height: 40
        anchors.top: textSelectUsbSpeed.bottom
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        currentIndex: 7
        model: ListModel {
            id: portSpeed
            ListElement {
                text: "1200"
            }

            ListElement {
                text: "2400"
            }

            ListElement {
                text: "4800"
            }

            ListElement {
                text: "9600"
            }

            ListElement {
                text: "19200"
            }

            ListElement {
                text: "38400"
            }

            ListElement {
                text: "57600"
            }

            ListElement {
                text: "115200"
            }
        }
    }

    Text {
        id: textSelectUsbSpeed
        width: 250
        height: 15
        text: qsTr("Speed")
        anchors.top: comboBoxSelectUsb.bottom
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 12
    }

    Text {
        id: textInfoPort
        width: 250
        height: 65
        text: qsTr("* 8 data bits, no flow control, no parity bit, two-way data transfer, 1 stop bit, the structure is speed-8-N-1")
        anchors.top: buttonClosePort.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        horizontalAlignment: Text.AlignHCenter
        fontSizeMode: Text.FixedSize
        font.pixelSize: 12
        textFormat: Text.AutoText
        wrapMode: Text.WordWrap
    }

    Button {
        id: buttonClosePort
        width: 120
        height: 40
        text: qsTr("Close port")
        anchors.top: comboBoxSpeed.bottom
        anchors.topMargin: 5
        anchors.right: comboBoxSpeed.right
        anchors.rightMargin: 0
    }

    Text {
        id: textOpenedPort
        width: 250
        height: 15
        text: qsTr("Open port:")
        anchors.top: textInfoPort.bottom
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 12
    }

    //Close terminal
    Connections {
        target: buttonClosePort
        onClicked: {
            appCore.receiveFromQmlClosePort()
        }
    }

    //Open terminal
    Connections {
        target: buttonOpenPort
        onClicked: {
            var emptyString = ""
            var dev = comboBoxSelectUsb.currentText
            var speed = parseInt(comboBoxSpeed.currentText)
            if (!emptyString.localeCompare(dev)) {
                console.log(new Date().toLocaleTimeString(
                                Qt.locale("ru_RU"),
                                "hh:mm:ss") + " No port selected for connection")
            } else {
                appCore.receiveFromQmlOpenPort(dev, speed)
            }
        }
    }

    Connections {
        target: appCore

        onSendToQmlPortOpenedPort: {
            textOpenedPort.text = portOpen + openedPort
        }

        onSendToQmlPortSerial: {
            var txt = portSerialTxt

            modelUsbTTY.append({
                                   "text": txt
                               })
        }

        onSendToQmlPortSerialDeleteIndex: {
            modelUsbTTY.get(deleteIndex).selected
            modelUsbTTY.remove(deleteIndex)
        }

        onSendToQmlPortSerialDeleteAll: {
            modelUsbTTY.clear()
        }

        onSendToQmlSubscriberAssignmentRs232: {
            textFieldIDStation.text = strAssignmentName
        }
    }

    Button {
        id: buttonSendConfiguration
        text: qsTr("Send configuration")
        anchors.top: textFieldIDStation.bottom
        anchors.topMargin: 10
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: parent.horizontalCenter
        onClicked: {
            appCore.sendConfigJsonToStation(textFieldIDStation.text)
        }
    }

    Text {
        id: elementIDStation
        width: 250
        height: 15
        text: qsTr("Station ID for configuration")
        anchors.top: textOpenedPort.bottom
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 12
    }

    TextField {
        id: textFieldIDStation
        text: ""
        anchors.top: elementIDStation.bottom
        anchors.topMargin: 5
        anchors.left: comboBoxSpeed.left
        anchors.leftMargin: 0
    }

    Button {
        id: button
        width: 45
        height: 40
        text: "..."
        anchors.top: elementIDStation.bottom
        anchors.topMargin: 5
        anchors.right: comboBoxSpeed.right
        anchors.rightMargin: 0
        onClicked: {
            var component = Qt.createComponent("assignment_list_rs232.qml")
            var windowSel = component.createObject(updateRS232Window)

            windowSel.show()
        }
    }

    Component.onCompleted: {
        appCore.receiveInformationAboutPort()
    }

    Timer {
        id: uartTimer
        interval: 1000
        repeat: true
        running: true
        triggeredOnStart: true
        onTriggered: {
            appCore.updateListOfUsbUarts()
        }
    }
}
