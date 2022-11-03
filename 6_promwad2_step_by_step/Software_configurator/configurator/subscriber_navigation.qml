import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4

Rectangle //Список всех станций
{
    color: canvasColor1
    id: rectangleMain
    height: parent.height
    width: 600
    //counter - [100-65000]
    property int counterDigitalStation: 100
    property int counterBoardSl1: 200
    property color canvasColor1: "#FAFCD4"
    property color defaultColor: "#D7C6A7"//"#e1e1e2" //Цвет выделения строки с номером
    property bool listDigitalStation: true
    property bool listBoardSl1: true
    property int baseHeight: 40
    property int currentPositionWithoutGroup: -1
    property int currentPosition: -1

    function resizeFlick()
    {
        rectListDigitalStation.height = 0
        rectListBoardSl1.height = 0

        flickLists.contentHeight = baseHeight * 3

        if (listDigitalStation) {
            flickLists.contentHeight += listModelDigitalStation.count * baseHeight
            rectListDigitalStation.height = listModelDigitalStation.count * baseHeight
        }

        if (listBoardSl1) {
            flickLists.contentHeight += listModelBoardSl1.count * baseHeight
            rectListBoardSl1.height = listModelBoardSl1.count * baseHeight
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

    }//resizeFlick()

    function resizeFlickWithoutGroup()
    {
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

            if (listModelBoardSl1.count > 0) {
                if (listViewBoardSl1.currentIndex >= 0)
                    listModelBoardSl1.remove(listViewBoardSl1.currentIndex)
            }

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
            else if (comboBoxListOfDevices.currentIndex == 1) {
                id += counterBoardSl1
                stationName = "board SL1 " + counterBoardSl1
                listModelBoardSl1.append({
                                             "name": stationName
                                         })
                listModelStationWithoutGroup.append({
                                                        "name": stationName
                                                     })
                counterBoardSl1++
            }

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

            ListElement {
                text: "board SL1"
            }
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
        color: canvasColor1//"#ffffff"
        visible: true
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: addDevice.bottom
        anchors.topMargin: 0

        Rectangle {
            id: rectangleGroupsColor
            color: canvasColor1//"#ffffff"
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
