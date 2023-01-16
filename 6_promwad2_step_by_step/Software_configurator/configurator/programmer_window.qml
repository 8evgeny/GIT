import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2

Window {
    id: programmerWindow
    width: 1350
    height: 800
    visible: true
    title: qsTr("Programmer")

    property color defaultColor: "#e1e1e2"

    property int defaultSizeOfHeight: 40
    property int defaultSizeOfSpace: 5
    property int cellWidth: 200
    property int baseHeight: 40
    property int typeOfMenu: 0

    Row
    {//Строка меню
        id: rowListOfSubscribersStatic
        width: parent.width
        height: defaultSizeOfHeight
        anchors.top: buttonReadConfiguration.bottom
        anchors.topMargin: 5

        //Rectangle of the checking device
        Rectangle {
            height: baseHeight
            width: baseHeight
            id: rectangleStatusCheckBoxStatic
            border.color: "#333"
            border.width: 1
        }

        //Rectangle of the device status
        Rectangle {
            height: baseHeight
            width: baseHeight
            id: rectangleStatusColorStatic
            border.color: "#333"
            border.width: 1
        }

        //number of the device
        Rectangle {
            height: baseHeight
            width: cellWidth - 50
            id: rectangleProgrammerIdStatic
            TextField {
                height: baseHeight
                width: cellWidth - 50
                readOnly: true
                font.family: "Arial"
                font.pointSize: 14
                text: qsTr("Device number")

                background: Rectangle {
                    border.color: "#333"
                    border.width: 1
                }
            }
        }

        //name of the device
        Rectangle {
            height: baseHeight
            width: cellWidth
            id: rectangleProgrammerNameStatic
            TextField {
                height: baseHeight
                width: cellWidth
                readOnly: true
                font.family: "Arial"
                font.pointSize: 14
                text: qsTr("Device name")

                background: Rectangle {
                    border.color: "#333"
                    border.width: 1
                }
            }
        }

        //version of the embedded software
        Rectangle {
            height: baseHeight
            width: cellWidth + 70
            id: rectangleProgrammerVersionStatic
            TextField {
                height: baseHeight
                width: cellWidth + 70
                readOnly: true
                font.family: "Arial"
                font.pointSize: 14
                text: qsTr("Firmware version")

                background: Rectangle {
                    border.color: "#333"
                    border.width: 1
                }
            }
        }

        //Name firmware file
        Rectangle
        {
            height: baseHeight
            width: cellWidth + 50
            id: rectangleProgrammerNameFware
            TextField {
                height: baseHeight
                width: cellWidth + 50
                readOnly: true
                font.family: "Arial"
                font.pointSize: 14
                text: qsTr("Firmware name")

                background: Rectangle {
                    border.color: "#333"
                    border.width: 1
                }
            }
        }


        //IP of the device
        Rectangle
        {
            height: baseHeight
            width: cellWidth
            id: rectangleProgrammerIpStatic
            TextField {
                height: baseHeight
                width: cellWidth
                readOnly: true
                font.family: "Arial"
                font.pointSize: 14
                text: qsTr("IP")

                background: Rectangle {
                    border.color: "#333"
                    border.width: 1
                }
            }
        }

        //MAC of the device
        Rectangle
        {
            height: baseHeight
            width: cellWidth
            id: rectangleProgrammerMacStatic
            TextField {
                height: baseHeight
                width: cellWidth
                readOnly: true
                font.family: "Arial"
                font.pointSize: 14
                text: qsTr("MAC")

                background: Rectangle {
                    border.color: "#333"
                    border.width: 1
                }
            }
        }
        spacing: -1
    } //End ROW - строка меню

    Flickable {
        id: flickableProgrammer
        width: parent.width + 5
        height: parent.height
        anchors.top: rowListOfSubscribersStatic.bottom
        anchors.topMargin: 0

        clip: true
        ScrollBar.vertical: ScrollBar {
            id: scrollBarProgrammer
        }
        Rectangle {
            id: rectangleProgrammer
            anchors.fill: parent

            Rectangle {
                id: rectangleInternalProgrammer
                anchors.fill: parent
                ListView {
                    id: listViewProgrammer
                    anchors.fill: parent
                    interactive: false

                    model: ListModel {
                        id: listModelProgrammer
                    }

                    delegate: Item {
                        width: flickableProgrammer.width
                        height: defaultSizeOfHeight

                        Row
                        {
                            id: rowListOfSubscribers
                            width: parent.width
                            height: parent.height

                            //Rectangle of the checking device
                            Rectangle {
                                height: baseHeight
                                width: baseHeight
                                id: rectangleStatusCheckBox
                                CheckBox {
                                    height: baseHeight
                                    width: baseHeight
                                    checked: checkedStatus
                                    background: Rectangle {
                                        border.color: "#333"
                                        border.width: borderSize
                                    }
                                    onClicked: {
                                        if (checked) {
                                            checkedStatus = true
                                        } else {
                                            checkedStatus = false
                                        }
                                    }
                                }
                            }

                            //Rectangle of the device status
                            Rectangle {
                                height: baseHeight
                                width: baseHeight
                                id: rectangleStatusColor
                                color: colorStatus
                                border.color: "#333"
                                border.width: borderSize
                            }

                            //number of the device
                            Rectangle {
                                height: baseHeight
                                width: cellWidth - 50
                                id: rectangleProgrammerId
                                TextField {
                                    height: baseHeight
                                    width: cellWidth - 50
                                    readOnly: true
                                    font.family: "Arial"
                                    font.pointSize: 12
                                    text: numberOfId

                                    background: Rectangle {
                                        border.color: "#333"
                                        border.width: borderSize
                                    }
                                }
                            }

                            //name of the device
                            Rectangle {
                                height: baseHeight
                                width: cellWidth
                                id: rectangleProgrammerName
                                TextField {
                                    height: baseHeight
                                    width: cellWidth
                                    readOnly: true
                                    font.family: "Arial"
                                    font.pointSize: 12
                                    text: nameOfId

                                    background: Rectangle {
                                        border.color: "#333"
                                        border.width: borderSize
                                    }
                                }
                            }

                            //version of the embedded software
                            Rectangle {
                                height: baseHeight
                                width: cellWidth + 70
                                id: rectangleProgrammerVersion
                                TextField {
                                    height: baseHeight
                                    width: cellWidth + 70
                                    readOnly: true
                                    font.family: "Arial"
                                    font.pointSize: 12
                                    text: versionOfSoftware

                                    background: Rectangle {
                                        border.color: "#333"
                                        border.width: borderSize
                                    }
                                }
                            }

                            //name fw file
                            Rectangle {
                                height: baseHeight
                                width: cellWidth + 50
                                id: rectangleProgrammerFwName
                                TextField {
                                    height: baseHeight
                                    width: cellWidth + 50
                                    readOnly: true
                                    font.family: "Arial"
                                    font.pointSize: 12
                                    text: nameOfSoftware

                                    background: Rectangle {
                                        border.color: "#333"
                                        border.width: borderSize
                                    }
                                }
                            }

                            //IP of the device
                            Rectangle {
                                height: baseHeight
                                width: cellWidth
                                id: rectangleProgrammerIp
                                TextField {
                                    height: baseHeight
                                    width: cellWidth
                                    readOnly: true
                                    font.family: "Arial"
                                    font.pointSize: 12
                                    text: ipOfDevice

                                    background: Rectangle {
                                        border.color: "#333"
                                        border.width: borderSize
                                    }
                                }
                            }

                            //MAC of the device
                            Rectangle {
                                height: baseHeight
                                width: cellWidth
                                id: rectangleProgrammerMac
                                TextField {
                                    height: baseHeight
                                    width: cellWidth
                                    readOnly: true
                                    font.family: "Arial"
                                    font.pointSize: 12
                                    text: macOfDevice

                                    background: Rectangle {
                                        border.color: "#333"
                                        border.width: borderSize
                                    }
                                }
                            }
                            spacing: -1
                        }//End Row Delagate
                    }
                    spacing: -1
                }
            }
        }
    }//End Flickable

    //Set the center of coordinates
//    function setCoordinates() {
//        x += Screen.width / 2 - width / 2
//        y += Screen.height / 2 - height / 2
//    }

    //Resize current list
    function resizeFlick() {
        flickableProgrammer.contentHeight = listModelProgrammer.count * baseHeight + 60
        rectangleInternalProgrammer.height = listModelProgrammer.count * baseHeight + 60

        rowListOfSubscribersStatic.spacing = -1
    }

    Button {
        id: buttonReadConfiguration
        y: 82
        text: qsTr("Read configuration")
        font.family: "Arial"
        font.pointSize: 14
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.top: parent.top
        anchors.topMargin: 5
        onClicked: {
            appCore.readStatusFromStations("")
        }
    }

    Button {
        id: buttonWriteConfiguration
        text: qsTr("Write configuration")
        font.family: "Arial"
        font.pointSize: 14
        anchors.left: buttonReadConfiguration.right
        anchors.leftMargin: 5
        anchors.verticalCenter: buttonReadConfiguration.verticalCenter
        onClicked: {
            var i = 0
            for (; i < listModelProgrammer.count; i++) {
                if (listModelProgrammer.get(i).checkedStatus)
                {
                    appCore.writeConfigNameByJson(listModelProgrammer.get(i).numberOfId)
                    appCore.writeConfigMcuByJson(listModelProgrammer.get(i).numberOfId)
                }
            }
        }
    }

    Button {
        id: buttonDeleteConfiguration
        text: qsTr("Delete configuration")
        font.family: "Arial"
        font.pointSize: 14
        anchors.left: buttonWriteConfiguration.right
        anchors.leftMargin: 5
        anchors.verticalCenter: buttonWriteConfiguration.verticalCenter
        onClicked: {
            var i = 0
            for (; i < listModelProgrammer.count; i++) {
                if (listModelProgrammer.get(i).checkedStatus) {
                    appCore.deleteConfigMcuByJson(
                                listModelProgrammer.get(i).numberOfId)
                }
            }
        }
    }

    Button {
        id: buttonReboot
        text: qsTr("Reboot")
        font.family: "Arial"
        font.pointSize: 14
        visible: true
        anchors.left: buttonDeleteConfiguration.right
        anchors.leftMargin: 5
        anchors.verticalCenter: buttonDeleteConfiguration.verticalCenter
        onClicked: {
            var i = 0
            for (; i < listModelProgrammer.count; i++) {
                if (listModelProgrammer.get(i).checkedStatus) {
                    appCore.rebootMcuByJson(
                                listModelProgrammer.get(i).numberOfId)
                }
            }
        }
    }

    Connections {
        target: appCore
        //(QString fillNumber, QString fillName, QString fillIp)
        onFillInfoForProgrammerWindow: {
            //Add a new item to the current list
            listModelProgrammer.append({
                                           "borderSize": 1,
                                           "checkedStatus": false,
                                           "colorStatus": "grey",
                                           "numberOfId": fillNumber,
                                           "nameOfId": fillName,
                                           "versionOfSoftware": "",
                                           "nameOfSoftware": "",
                                           "ipOfDevice": fillIp,
                                           "macOfDevice": ""
                                       })
            //Resize current list
            resizeFlick()
        }

        onFillInfoForProgrammerWindowByJson : {
            //Add a new item to the current list
            listModelProgrammer.append({
                                           "borderSize": 1,
                                           "checkedStatus": false,
                                           "colorStatus": fillColor,
                                           "numberOfId": fillNumber,
                                           "nameOfId": fillName,
                                           "versionOfSoftware": fillVersion,
                                           "nameOfSoftware": fillNameFware,
                                           "ipOfDevice": fillIp,
                                           "macOfDevice": fillMac
                                       })
            //Resize current list
            resizeFlick()
        }

        onClearListOfStationsStatus : {
            listModelProgrammer.clear()
        }

        onSendJsonUseSelectedBox: {
//            var i = 0
//            for (; i < listModelProgrammer.count; i++) {
//                if (listModelProgrammer.get(i).checkedStatus) {
//                    appCore.sendConfigJsonToStationByEthernet(
//                                listModelProgrammer.get(i).numberOfId)
//                }
//            }
        }
    }

    Connections {
        target: receiver
        onStatusChangedJson :{
//            void statusChangedJson(QString idJson, QString versionJson, QString macJson, QString ipJson);
            appCore.statusChangedJson(idJson, versionJson, fwNameJson, macJson, ipJson);
        }
    }

    Component.onCompleted: {
        //set current index -1
        listViewProgrammer.currentIndex = -1

        //Set the center of coordinates
//        setCoordinates()
        //Resize current list
        resizeFlick()

        appCore.getInformationForProgrammerWindow()

    }
}
