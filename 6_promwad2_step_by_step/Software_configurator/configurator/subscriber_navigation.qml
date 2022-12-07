import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4

Rectangle //id: rectangleMain
{
    color: canvasColor
    id: rectangleMain
    height: parent.height
    width: 600
    //counter - [100-65000]
    property int counterDigitalStation: 100
    property int counterIpStation: 1
    property int counterPDO16: 1
    property int counterPDO16N: 1
    property int counterPDO32: 1
    property int counterSL1: 1
//    property int counterBoardSl1: 100
property color canvasColor: "#F8FACF"
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

//        if (listBoardSl1) {
//            flickLists.contentHeight += listModelBoardSl1.count * baseHeight
//            rectListBoardSl1.height = listModelBoardSl1.count * baseHeight
//        }

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

    }
    function resizeFlickWithoutGroup()
    {
        flickListsWithoutGroup.contentHeight = listModelStationWithoutGroup.count * baseHeight
        rectListStationWithoutGroup.height = listModelStationWithoutGroup.count * baseHeight
    }

    Button //Кнопка Add device
    {
        id: addDevice
        width: 160
        height: baseHeight
        text: qsTr("Add device")
        font.family: "Arial"
        font.pointSize: 14
        onClicked:
        {
//            appCore.ipPlus()
            var stationName = ""
            var id = "CID " + counterDigitalStation
            var ip = counterIpStation
            if (comboBoxListOfDevices.currentIndex == 0)
            {
                stationName = qsTr("PDO16") + "-" + counterPDO16
                listModelDigitalStation.append({"name": stationName, "cidName": id })
                listModelStationWithoutGroup.append({"name": stationName, "cidName": id })
                counterPDO16++
            }
            else if (comboBoxListOfDevices.currentIndex == 1)
            {
                stationName = qsTr("PDO16N") + "-" + counterPDO16N
                listModelDigitalStation.append({ "name": stationName, "cidName": id })
                listModelStationWithoutGroup.append({"name": stationName, "cidName": id })
                counterPDO16N++
            }
            else if (comboBoxListOfDevices.currentIndex == 2)
            {
                stationName = qsTr("PDO32") + "-" + counterPDO32
                listModelDigitalStation.append({ "name": stationName, "cidName": id })
                listModelStationWithoutGroup.append({"name": stationName, "cidName": id })
                counterPDO32++
            }
            else if (comboBoxListOfDevices.currentIndex == 3)
            {
                stationName = qsTr("SL1") + "-" + counterSL1
                listModelDigitalStation.append({ "name": stationName, "cidName": id })
                listModelStationWithoutGroup.append({"name": stationName, "cidName": id })
                counterSL1++
            }
            counterDigitalStation++
            counterIpStation++
            appCore.saveStation(stationName, id, ip)

            //Добавил чтобы не ломалось отображение - иммитация клика мышки по текущей строчке
            appCore.sendCurrentIndexOfDigitalStation(
            listViewStationWithoutGroup.currentIndex,
            listModelDigitalStation.get(listViewStationWithoutGroup.currentIndex).cidName)

            resizeFlick()
            resizeFlickWithoutGroup()
        }
    }
    ComboBox //Выбор типа станции
    {
        id: comboBoxListOfDevices
        width: 140
        height: baseHeight
        anchors.left: addDevice.right
        anchors.leftMargin: 5
        font.family: "Arial"
        font.pointSize: 14
        model: ListModel
        {
            id: modelListOfDevices
            ListElement {
                text: qsTr("PDO16")
            }
            ListElement {
                text: qsTr("PDO16N")
            }
            ListElement {
                text: qsTr("PDO32")
            }
            ListElement {
                text: "SL1"
            }
        }
    }
    Button //Кнопка Remove device
    {
        id: deleteDevice
        width: 180
        height: baseHeight
        text: qsTr("Remove device")
        anchors.left: comboBoxListOfDevices.right
        anchors.leftMargin: 5
        font.family: "Arial"
        font.pointSize: 14
        onClicked:
        {
            var currentIndex = -1
            currentIndex = listViewDigitalStation.currentIndex

            if (currentIndex >= 0)
            {
                appCore.deleteStation(listModelDigitalStation.get(
                                          currentIndex).cidName)
            }
            if (listModelDigitalStation.count > 0)
            {
                if (listViewDigitalStation.currentIndex >= 0) {
                    listModelDigitalStation.remove(
                                listViewDigitalStation.currentIndex)

                    listModelStationWithoutGroup.remove(
                                listViewStationWithoutGroup.currentIndex)
                }
            }
//            if (listModelBoardSl1.count > 0)
//            {
//                if (listViewBoardSl1.currentIndex >= 0)
//                    listModelBoardSl1.remove(listViewBoardSl1.currentIndex)
//            }

            resizeFlick()
            resizeFlickWithoutGroup()

            if (listModelDigitalStation.count > 0)
            {
                if (listViewDigitalStation.currentIndex >= 0)
                {
                    appCore.sendCurrentIndexOfDigitalStation(
                                listViewDigitalStation.currentIndex, listModelDigitalStation.get(
                                    listViewDigitalStation.currentIndex).cidName)
                }
            }
        }//onClicked
    }
//    CheckBox  //Чек бокс Группы
//    {
//        id: checkBoxActGroup
//        width: 150
//        height: 40
//        text: qsTr("Group")
//        checked: true
//        anchors.left: deleteDevice.right
//        anchors.leftMargin: 5
//        onClicked: {
//            if (checkBoxActGroup.checked) {
//                flickLists.visible = true
//                flickListsWithoutGroup.visible = false
//                listViewDigitalStation.currentIndex = listViewStationWithoutGroup.currentIndex
//            } else {
//                flickLists.visible = false
//                flickListsWithoutGroup.visible = true
//                listViewStationWithoutGroup.currentIndex = listViewDigitalStation.currentIndex
//            }
//        }
//    }

    Rectangle //id: rectangleGroups
    {
        id: rectangleGroups
        x: 0
        y: 45
        height: 40
        color: canvasColor//"#ffffff"
        visible: true
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: addDevice.bottom
        anchors.topMargin: 0

        Rectangle  //Надпись Groups
        {
            id: rectangleGroupsColor
            color: canvasColor//"#ffffff"
            anchors.rightMargin: 5
            anchors.leftMargin: 5
            anchors.bottomMargin: 5
            anchors.topMargin: 5
            anchors.fill: parent

            Label //Надпись Groups
            {
                id: labelGroups
                text: qsTr("Groups")
                color:  "blue"
                font.bold: true
                font.family: "Arial"
                font.pointSize: 14
                verticalAlignment: Text.AlignVCenter
                anchors.fill: parent
            }
            MouseArea
            {
                id: mouseAreaGroups
                anchors.fill: parent
                onClicked:
                {
                    rectangleGroupsColor.color = defaultColor
                    listViewDigitalStation.currentIndex = -1
                    listViewStationWithoutGroup.currentIndex = -1
                    appCore.enableVisibleGroups()
                }

            }
        }
    }

    Flickable
    {
        id: flickLists
        width: parent.width
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: rectangleGroups.bottom
        anchors.topMargin: 0
        clip: true
visible: false
        ScrollBar.vertical: ScrollBar {}

        Rectangle //Список станций + список SL1
        {
            id: rectLists
            anchors.fill: parent
            color: canvasColor
            Rectangle  //id: rectListDigitalStation
            {
                id: rectListDigitalStation
                color: canvasColor
                anchors.left: buttonListDigitalStation.right
                anchors.leftMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.top: buttonListDigitalStation.bottom
                anchors.topMargin: 0
                ListView //listViewDigitalStation
                {
                    id: listViewDigitalStation
                    anchors.fill: parent
                    currentIndex: -1
                    interactive: false
                    model: ListModel
                    {
                        id: listModelDigitalStation
                    }
                    delegate: Item
                    {
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
                        MouseArea
                        {
                            anchors.fill: parent
                            onClicked:
                            {
                                rectangleGroupsColor.color = canvasColor//"#ffffff"
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
            Button //Кнопка скрыть показать список станций
            {
                width: baseHeight
                height: baseHeight
                id: buttonListDigitalStation
                x: 0
                text: "-"
                anchors.top: rectangleGroups.bottom
                anchors.topMargin: 0
            }
            Connections //Действие кнопки
            {
                target: buttonListDigitalStation
                onClicked:
                {
                    if (listDigitalStation) {
                        buttonListDigitalStation.text = "+"
                    } else {
                        buttonListDigitalStation.text = "-"
                    }

                    listDigitalStation = !listDigitalStation

                    resizeFlick()
                }
            }
            Label //Надпись Digital stations:
            {
                id: labelDigitalStation
                width: 300
                height: baseHeight
                text: qsTr("Digital stations:")
                anchors.left: buttonListDigitalStation.right
                anchors.leftMargin: 5
                //                anchors.top: rectangleGroups.bottom
                //                anchors.topMargin: 0
                font.bold: true
                color: "blue"
                //                anchors.left: buttonListDigitalStation.right
                //                anchors.leftMargin: 5
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
            }

//            Connections //Недоработано ?
//            {
//                target: listViewDigitalStation
//            }

            Rectangle //id: rectListBoardSl1
            {
                id: rectListBoardSl1
                color: canvasColor
                anchors.left: buttonListBoardSl1.right
                anchors.leftMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.top: buttonListBoardSl1.bottom
                anchors.topMargin: 0
                ListView // listViewBoardSl1
                {
                    id: listViewBoardSl1
                    anchors.fill: parent
                    currentIndex: -1
                    interactive: false
                    model: ListModel
                    {
                        id: listModelBoardSl1
                    }
                    delegate: Item {
                        x: 5
                        width: 300
                        height: baseHeight
                        Row {
                            id: rowBoardSl1
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
                            onClicked:
                            {
//                                listViewBoardSl1.currentIndex = index
//                                listViewDigitalStation.currentIndex = -1

//                                //send current index of the station
//                                appCore.sendCurrentIndexOfDigitalStation(
//                                            index, listModelBoardSl1.get(
//                                                index).cidName)
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
            Button //Кнопка скрыть показать список SL1
            {
                id: buttonListBoardSl1
                width: baseHeight
                height: baseHeight
                text: "-"
                anchors.top: rectListDigitalStation.bottom
                anchors.topMargin: 0
            }
            Connections //Действие кнопки
            {
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
            Label //Надпись Boards SL1:
            {
                id: labelBoardSl1
                width: 300
                height: baseHeight
                text: qsTr("Boards SL1:")
                color: "blue"
                font.bold: true
                anchors.top: rectListDigitalStation.bottom
                anchors.topMargin: 0
                anchors.left: buttonListBoardSl1.right
                anchors.leftMargin: 5
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
            }

        }//id: rectLists
    }//Flickable

    Connections  //Обработка сигналов
    {
        target: appCore
    }

    Flickable
    {
        id: flickListsWithoutGroup
        anchors.top: rectangleGroups.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.topMargin: 0
//        visible: false
visible: true
        clip: true
        ScrollBar.vertical: ScrollBar {}

        Rectangle //rectListsWithoutGroup
        {
            id: rectListsWithoutGroup
            anchors.fill: parent
            color: canvasColor
            Rectangle {
                id: rectListStationWithoutGroup
                width: 0
                color: canvasColor
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
                                text: name
                                anchors.verticalCenter: parent.verticalCenter
                                font.family: "Arial"
                                font.pointSize: 14
                            }
                            Text {
                                text: ".  .  .  .  ."
                                anchors.verticalCenter: parent.verticalCenter
                                font.family: "Arial"
                                font.pointSize: 14
                            }
                            Text {
                                text: cidName
                                anchors.verticalCenter: parent.verticalCenter
                                font.family: "Arial"
                                font.pointSize: 14
                            }
                            spacing: 10
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                rectangleGroupsColor.color = canvasColor//"#ffffff"

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


//    Button //Скрытая кнопка
//    {
//        id: buttonLoadStations
//        text: qsTr("Save project")
//        anchors.left: checkBoxActGroup.right
//        visible: false
//        anchors.leftMargin: 5
//        onClicked: {
//            appCore.saveListOfStationAndGroupsQJson()
//        }
//    }

//    Button //Скрытая кнопка
//    {
//        id: buttonLoadGroups
//        text: qsTr("Load project")
//        anchors.left: buttonLoadStations.right
//        visible: false
//        anchors.leftMargin: 5
//        onClicked: {
//            appCore.loadListOfStationAndGroupsQJson()
//        }
//    }

    Connections //Сигналы
    {
        target: appCore

        onSendUpdateListOfStationsFromFile://Сигеал
//                                           void sendUpdateListOfStationsFromFile(
//                                               QString nameOfStation,
//                                               QString nameOfId);
        {

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

        onClearListOfStations :
        {
            listModelDigitalStation.clear()
            listModelStationWithoutGroup.clear()
//            listModelBoardSl1.clear()

            resizeFlick()
            resizeFlickWithoutGroup()
        }

        onSendClearAllListOfStations:
        {
            listModelDigitalStation.clear()
            listModelStationWithoutGroup.clear()
//            listModelBoardSl1.clear().clear()
        }

        onGetCurrentPositionListOfStation:
        {
            currentPosition = listViewDigitalStation.currentIndex
            currentPositionWithoutGroup = listViewStationWithoutGroup.currentIndex
        }

        onRestoreCurrentPositionListOfStation:
        {
            listViewDigitalStation.currentIndex = currentPosition
            listViewStationWithoutGroup.currentIndex = currentPosition
        }

        onGetStartVerificationStation :
        {
            if((listViewStationWithoutGroup.currentIndex >= 0) || (listViewDigitalStation.currentIndex >= 0)){
//                console.log(listModelDigitalStation.get(listViewStationWithoutGroup.currentIndex).name)
//                console.log(listModelDigitalStation.get(listViewDigitalStation.currentIndex).name)
                appCore.startVerificationStation(listModelDigitalStation.get(listViewDigitalStation.currentIndex).cidName)
            }
        }

        onSetCounterDigitalStation :
        {
            counterDigitalStation = idSetStaion
            counterDigitalStation++
//            counterBoardSl1 = idSetStaion
//            counterBoardSl1++
        }
    }
}
