import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4

Rectangle {
    id: rectangleMain
    height: parent.height

    //counter - [100-65000]
    property int counterDigitalStation: 100
    property int counterBoardSl1: 100
    property int counterBoardSl2: 100

    property color defaultColor: "#e1e1e2"

    property bool listDigitalStation: true
    property bool listBoardSl1: true
    property bool listBoardSl2: true

    property int baseHeight: 40

    property int currentPositionWithoutGroup: -1
    property int currentPosition: -1

    width: 600

    function resizeFlick() {
        rectListDigitalStation.height = 0
        rectListBoardSl1.height = 0
        rectListBoardSl2.height = 0

        flickLists.contentHeight = baseHeight * 3

        if (listDigitalStation) {
            flickLists.contentHeight += listModelDigitalStation.count * baseHeight
            rectListDigitalStation.height = listModelDigitalStation.count * baseHeight
        }

        if (listBoardSl1) {
            flickLists.contentHeight += listModelBoardSl1.count * baseHeight
            rectListBoardSl1.height = listModelBoardSl1.count * baseHeight
        }

        if (listBoardSl2) {
            flickLists.contentHeight += listModelBoardSl2.count * baseHeight
            rectListBoardSl2.height = listModelBoardSl2.count * baseHeight
        }

        if (listDigitalStation) {
            rectListDigitalStation.visible = true
        } else {
            rectListDigitalStation.visible = false
        }

        if (listBoardSl1) {
            rectListBoardSl1.visible = true
        } else {
            rectListBoardSl1.visible = false
        }

        if (listBoardSl2) {
            rectListBoardSl2.visible = true
        } else {
            rectListBoardSl2.visible = false
        }
    }

    function resizeFlickWithoutGroup() {
        flickListsWithoutGroup.contentHeight = listModelStationWithoutGroup.count * baseHeight
        rectListStationWithoutGroup.height = listModelStationWithoutGroup.count * baseHeight
    }

    Flickable {
        id: flickLists
        width: parent.width
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: rectangleGroups.bottom
        anchors.topMargin: 0
        clip: true
        ScrollBar.vertical: ScrollBar {}

        Rectangle {
            id: rectLists
            anchors.fill: parent

            Rectangle {
                id: rectListDigitalStation
                anchors.left: buttonListDigitalStation.right
                anchors.leftMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.top: buttonListDigitalStation.bottom
                anchors.topMargin: 0
                ListView {
                    id: listViewDigitalStation
                    anchors.fill: parent
                    currentIndex: -1

                    interactive: false
                    model: ListModel {
                        id: listModelDigitalStation
                    }
                    delegate: Item {
                        x: 5
                        width: 300
                        height: baseHeight
                        Row {
                            id: rowDigitalStation
                            Text {
                                text: cidName
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            Text {
                                text: name
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            spacing: 10
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {

                                rectangleGroupsColor.color = "#ffffff"

                                listViewDigitalStation.currentIndex = index
                                listViewStationWithoutGroup.currentIndex = index

                                listViewBoardSl1.currentIndex = -1
                                listViewBoardSl2.currentIndex = -1

                                //send current index of the station
                                appCore.sendCurrentIndexOfDigitalStation(
                                            index, listModelDigitalStation.get(
                                                index).cidName)


                            }
                        }
                    }
                    highlight: Rectangle {
                        id: highlightDigitalStation
                        color: defaultColor
                    }
                    highlightMoveDuration: 100
                }
            }

            Rectangle {
                id: rectListBoardSl1
                anchors.left: buttonListBoardSl1.right
                anchors.leftMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.top: buttonListBoardSl1.bottom
                anchors.topMargin: 0
                ListView {
                    id: listViewBoardSl1
                    anchors.fill: parent

                    currentIndex: -1

                    interactive: false
                    model: ListModel {
                        id: listModelBoardSl1
                    }
                    delegate: Item {
                        x: 5
                        width: 300
                        height: baseHeight
                        Row {
                            id: rowBoardSl1
                            Text {
                                text: name
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            spacing: 10
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                listViewDigitalStation.currentIndex = -1
                                listViewBoardSl1.currentIndex = index
                                listViewBoardSl2.currentIndex = -1
                            }
                        }
                    }
                    highlight: Rectangle {
                        id: highlightBoardSl1
                        color: defaultColor
                    }
                    highlightMoveDuration: 100
                }
            }

            Rectangle {
                id: rectListBoardSl2
                anchors.left: buttonListBoardSl2.right
                anchors.leftMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.top: buttonListBoardSl2.bottom
                anchors.topMargin: 0
                ListView {
                    id: listViewBoardSl2
                    anchors.fill: parent

                    currentIndex: -1

                    interactive: false
                    model: ListModel {
                        id: listModelBoardSl2
                    }
                    delegate: Item {
                        x: 5
                        width: 300
                        height: baseHeight
                        Row {
                            id: rowBoardSl2
                            Text {
                                text: name
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            spacing: 10
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                listViewDigitalStation.currentIndex = -1
                                listViewBoardSl1.currentIndex = -1
                                listViewBoardSl2.currentIndex = index
                            }
                        }
                    }
                    highlight: Rectangle {
                        id: highlightBoardSl2
                        color: defaultColor
                    }
                    highlightMoveDuration: 100
                }
            }

            Label {
                id: labelDigitalStation
                width: 300
                height: baseHeight
                text: qsTr("Digital stations:")
                anchors.left: buttonListDigitalStation.right
                anchors.leftMargin: 5
                //                anchors.top: rectangleGroups.bottom
                //                anchors.topMargin: 0
                font.bold: true
                //                anchors.left: buttonListDigitalStation.right
                //                anchors.leftMargin: 5
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
            }

            Connections {
                target: listViewDigitalStation
            }

            Button {
                width: baseHeight
                height: baseHeight
                id: buttonListDigitalStation
                x: 0
                text: "-"
                //                anchors.top: rectangleGroups.bottom
                //                anchors.topMargin: 0
            }

            Connections {
                target: buttonListDigitalStation
                onClicked: {
                    if (listDigitalStation) {
                        buttonListDigitalStation.text = "+"
                    } else {
                        buttonListDigitalStation.text = "-"
                    }

                    listDigitalStation = !listDigitalStation

                    resizeFlick()
                }
            }

            Button {
                id: buttonListBoardSl1
                width: baseHeight
                height: baseHeight
                text: "-"
                anchors.top: rectListDigitalStation.bottom
                anchors.topMargin: 0
            }

            Connections {
                target: buttonListBoardSl1
                onClicked: {
                    if (listBoardSl1) {
                        buttonListBoardSl1.text = "+"
                    } else {
                        buttonListBoardSl1.text = "-"
                    }

                    listBoardSl1 = !listBoardSl1

                    resizeFlick()
                }
            }

            Button {
                id: buttonListBoardSl2
                width: baseHeight
                height: baseHeight
                text: "-"
                visible: true
                anchors.topMargin: 0
                anchors.top: rectListBoardSl1.bottom
            }

            Connections {
                target: buttonListBoardSl2
                onClicked: {
                    if (listBoardSl2) {
                        buttonListBoardSl2.text = "+"
                    } else {
                        buttonListBoardSl2.text = "-"
                    }

                    listBoardSl2 = !listBoardSl2

                    resizeFlick()
                }
            }

            Label {
                id: labelBoardSl2
                width: 300
                height: baseHeight
                text: qsTr("Boards SL2:")
                anchors.top: rectListBoardSl1.bottom
                anchors.topMargin: 0
                font.bold: true
                visible: true
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                anchors.left: buttonListBoardSl2.right
                anchors.leftMargin: 5
            }

            Label {
                id: labelBoardSl1
                width: 300
                height: baseHeight
                text: qsTr("Boards SL1:")
                font.bold: true
                anchors.top: rectListDigitalStation.bottom
                anchors.topMargin: 0
                anchors.left: buttonListBoardSl1.right
                anchors.leftMargin: 5
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
            }
        }
    }

    Button {
        id: deleteDevice
        width: 120
        height: baseHeight
        text: qsTr("Remove device")
        anchors.left: comboBoxListOfDevices.right
        anchors.leftMargin: 5
        onClicked: {
            var currentIndex = -1
            currentIndex = listViewDigitalStation.currentIndex

            if (currentIndex >= 0) {
                appCore.deleteStation(listModelDigitalStation.get(
                                          currentIndex).cidName)
            }

            if (listModelDigitalStation.count > 0) {
                if (listViewDigitalStation.currentIndex >= 0) {
                    listModelDigitalStation.remove(
                                listViewDigitalStation.currentIndex)

                    listModelStationWithoutGroup.remove(
                                listViewStationWithoutGroup.currentIndex)
                }
            }            

            //for future works

            //            if (listModelBoardSl1.count > 0) {
            //                if (listViewBoardSl1.currentIndex >= 0)
            //                    listModelBoardSl1.remove(listViewBoardSl1.currentIndex)
            //            }

            //            if (listModelBoardSl2.count > 0) {
            //                if (listViewBoardSl2.currentIndex >= 0)
            //                    listModelBoardSl2.remove(listViewBoardSl2.currentIndex)
            //            }
            resizeFlick()
            resizeFlickWithoutGroup()


            if (listModelDigitalStation.count > 0) {
                if (listViewDigitalStation.currentIndex >= 0) {
                    appCore.sendCurrentIndexOfDigitalStation(
                                listViewDigitalStation.currentIndex, listModelDigitalStation.get(
                                    listViewDigitalStation.currentIndex).cidName)
                }
            }

        }
    }

    Button {
        id: addDevice
        width: 130
        height: baseHeight
        text: qsTr("Add device")
        onClicked: {

            var stationName = ""
            var id = "CID "

            if (comboBoxListOfDevices.currentIndex == 0) {
                //counterDigitalStation = 0// TODO
                id += counterDigitalStation

                stationName = qsTr(
                            "Digital station") + " " + counterDigitalStation
                listModelDigitalStation.append({
                                                   "name": stationName,
                                                   "cidName": id
                                               })
                listModelStationWithoutGroup.append({
                                                        "name": stationName,
                                                        "cidName": id
                                                    })
                counterDigitalStation++
            }
            //for future work
            //            else if (comboBoxListOfDevices.currentIndex == 1) {

            //                id += counterBoardSl1

            //                stationName = "Плата СЛ1 " + counterBoardSl1
            //                listModelBoardSl1.append({
            //                                             "name": stationName
            //                                         })

            //                listModelStationWithoutGroup.append({
            //                                                        "name": stationName
            //                                                    })

            //                counterBoardSl1++
            //            } else if (comboBoxListOfDevices.currentIndex == 2) {

            //                id += counterBoardSl2

            //                stationName = "Плата СЛ2 " + counterBoardSl2
            //                listModelBoardSl2.append({
            //                                             "name": stationName
            //                                         })
            //                listModelStationWithoutGroup.append({
            //                                                        "name": stationName
            //                                                    })
            //                counterBoardSl2++
            //            }

            //            appCore.saveStation(stationName, id)
            appCore.saveStation(stationName, id)

            resizeFlick()
            resizeFlickWithoutGroup()
        }
    }

    ComboBox {
        id: comboBoxListOfDevices
        width: 175
        height: baseHeight
        anchors.left: addDevice.right
        anchors.leftMargin: 5
        model: ListModel {
            id: modelListOfDevices
            ListElement {
                text: qsTr("Digital station")
            }
            //for future works

            //            ListElement {
            //                text: "Плата СЛ1"
            //            }
            //            ListElement {
            //                text: "Плата СЛ2"
            //            }
        }
    }

    Connections {
        target: appCore
    }

    Flickable {
        id: flickListsWithoutGroup
        anchors.top: rectangleGroups.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.topMargin: 0
        visible: false
        clip: true
        ScrollBar.vertical: ScrollBar {}

        Rectangle {
            id: rectListsWithoutGroup
            anchors.fill: parent

            Rectangle {
                id: rectListStationWithoutGroup
                width: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.topMargin: 0
                ListView {
                    id: listViewStationWithoutGroup
                    anchors.fill: parent
                    currentIndex: -1

                    interactive: false
                    model: ListModel {
                        id: listModelStationWithoutGroup
                    }
                    delegate: Item {
                        x: 5
                        width: 300
                        height: baseHeight
                        Row {
                            id: rowStationWithoutGroup
                            Text {
                                text: cidName
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            Text {
                                text: name
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            spacing: 10
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                rectangleGroupsColor.color = "#ffffff"

                                listViewDigitalStation.currentIndex = index
                                listViewStationWithoutGroup.currentIndex = index
                                appCore.sendCurrentIndexOfDigitalStation(
                                            index, listModelDigitalStation.get(
                                                index).cidName)

                            }
                        }
                    }
                    highlight: Rectangle {
                        id: highlightStationWithoutGroup
                        color: defaultColor
                    }
                    highlightMoveDuration: 100
                }
            }
        }
    }

    Rectangle {
        id: rectangleGroups
        x: 0
        y: 45
        height: 40
        color: "#ffffff"
        visible: true
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: addDevice.bottom
        anchors.topMargin: 0

        Rectangle {
            id: rectangleGroupsColor
            color: "#ffffff"
            anchors.rightMargin: 5
            anchors.leftMargin: 5
            anchors.bottomMargin: 5
            anchors.topMargin: 5
            anchors.fill: parent

            Label {
                id: labelGroups
                text: qsTr("Groups")
                font.bold: true
                verticalAlignment: Text.AlignVCenter
                anchors.fill: parent
            }
            MouseArea {
                id: mouseAreaGroups
                anchors.fill: parent
                onClicked: {
                    rectangleGroupsColor.color = defaultColor
                    listViewDigitalStation.currentIndex = -1
                    listViewStationWithoutGroup.currentIndex = -1

                    appCore.enableVisibleGroups()
                }
            }
        }
    }

    Button {
        id: buttonLoadStations
        text: qsTr("Save project")
        anchors.left: checkBoxActGroup.right
        visible: false
        anchors.leftMargin: 5
        onClicked: {
            appCore.saveListOfStationAndGroupsQJson()
        }
    }

    Button {
        id: buttonLoadGroups
        text: qsTr("Load project")
        anchors.left: buttonLoadStations.right
        visible: false
        anchors.leftMargin: 5
        onClicked: {
            appCore.loadListOfStationAndGroupsQJson()
        }
    }

    CheckBox {
        id: checkBoxActGroup
        width: 150
        height: 40
        text: qsTr("Group")
        checked: true
        anchors.left: deleteDevice.right
        anchors.leftMargin: 5
        onClicked: {
            if (checkBoxActGroup.checked) {
                flickLists.visible = true
                flickListsWithoutGroup.visible = false
                listViewDigitalStation.currentIndex = listViewStationWithoutGroup.currentIndex
            } else {
                flickLists.visible = false
                flickListsWithoutGroup.visible = true
                listViewStationWithoutGroup.currentIndex = listViewDigitalStation.currentIndex
            }
        }
    }

    Connections {
        target: appCore

        onSendUpdateListOfStationsFromFile: {

            listModelDigitalStation.append({
                                               "name": nameOfStation,
                                               "cidName": nameOfId
                                           })
            listModelStationWithoutGroup.append({
                                                    "name": nameOfStation,
                                                    "cidName": nameOfId
                                                })

            resizeFlick()
            resizeFlickWithoutGroup()
        }

        onClearListOfStations : {
            listModelDigitalStation.clear()
            listModelStationWithoutGroup.clear()

            resizeFlick()
            resizeFlickWithoutGroup()
        }

        onSendClearAllListOfStations: {
            listModelDigitalStation.clear()
            listModelStationWithoutGroup.clear()
        }

        onGetCurrentPositionListOfStation: {
            currentPosition = listViewDigitalStation.currentIndex
            currentPositionWithoutGroup = listViewStationWithoutGroup.currentIndex
        }

        onRestoreCurrentPositionListOfStation: {
            listViewDigitalStation.currentIndex = currentPosition
            listViewStationWithoutGroup.currentIndex = currentPosition
        }

        onGetStartVerificationStation :{
            if((listViewStationWithoutGroup.currentIndex >= 0) || (listViewDigitalStation.currentIndex >= 0)){
//                console.log(listModelDigitalStation.get(listViewStationWithoutGroup.currentIndex).name)
//                console.log(listModelDigitalStation.get(listViewDigitalStation.currentIndex).name)
                appCore.startVerificationStation(listModelDigitalStation.get(listViewDigitalStation.currentIndex).cidName)
            }
        }

        onSetCounterDigitalStation : {
            counterDigitalStation = idSetStaion
            counterDigitalStation++
        }
    }    
}
