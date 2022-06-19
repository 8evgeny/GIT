import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQuick.Controls.Styles 1.4

Rectangle {
    id: rectangleMain

    width: 1260
    height: 600
    visible: false

    property int defaultSizeOfHeight: 40
    property int defaultIdent: 1
    property int defaultIdentForButtons: 10
    property color defaultColor: "#e1e1e2"

    property int counter: 0

    function resizeFlickableGroupCallOverviewInter() {
        flickableGroupCallOverviewInter.contentHeight
                = listModelGroupCallOverviewInterList.count * (defaultSizeOfHeight - 1) + 1
    }

    function resizeFlickableIncludedParticipantsList() {
        flickableIncludedParticipantsList.contentHeight
                = listModelIncludedParticipantsList.count * (defaultSizeOfHeight - 1) + 1
    }

    function resizeFlickableParticipantsList() {
        flickablePossibleParticipantsList.contentHeight
                = listModelParticipantsList.count * (defaultSizeOfHeight - 1) + 1
    }

    Rectangle {
        id: rectangleGroupCall
        height: defaultSizeOfHeight * 2
        color: "#ffffff"
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0

        Label {
            id: labelGroupCall
            text: qsTr("Group calls")
            verticalAlignment: Text.AlignVCenter
            anchors.fill: parent
        }

        Button {
            id: buttonSaveGroups
            text: qsTr("Save groups")
            visible: false
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.verticalCenter: parent.verticalCenter
            onClicked: {
                appCore.saveListOfGroupsQJson()
            }
        }

        Button {
            id: buttonLoadGroups
            text: qsTr("Load groups")
            visible: false
            anchors.right: buttonSaveGroups.left
            anchors.rightMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            onClicked: {
                appCore.loadListOfGroupsQJson()
            }
        }
    }

    Rectangle {
        id: rectangleGroupCallOverview
        width: 400
        height: 400
        color: "#ffffff"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: rectangleGroupCall.bottom
        anchors.topMargin: 0

        Label {
            id: labelGroupCallOverviewInter
            height: defaultSizeOfHeight
            text: qsTr("Group Call Overview")
            verticalAlignment: Text.AlignVCenter
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
        }

        Button {
            id: buttonAddGroup
            width: defaultSizeOfHeight
            height: defaultSizeOfHeight
            text: "+"
            anchors.right: buttonDeleteGroup.left
            anchors.rightMargin: defaultIdent
            anchors.top: parent.top
            anchors.topMargin: 0
            onClicked: {
                listModelGroupCallOverviewInterList.append({
                                                               "name": qsTr("Group call") + " "
                                                                       + counter,
                                                               "ident": "GR " + counter,
                                                               "borderSize": 1
                                                           })
                resizeFlickableGroupCallOverviewInter()
                appCore.createGroup(qsTr("Group call") + " " + counter,
                                    "GR " + counter)
                counter++
            }
        }

        Button {
            id: buttonDeleteGroup
            width: defaultSizeOfHeight
            height: defaultSizeOfHeight
            text: "-"
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            onClicked: {
                if (listModelGroupCallOverviewInterList.count > 0) {
                    if (listViewGroupCallOverviewInterList.currentIndex >= 0) {
                        var index = listViewGroupCallOverviewInterList.currentIndex
                        listModelGroupCallOverviewInterList.get(
                                    listViewGroupCallOverviewInterList.currentIndex).borderSize = 1

                        appCore.deleteGroup(
                                    listModelGroupCallOverviewInterList.get(
                                        index).name)

                        listModelGroupCallOverviewInterList.remove(
                                    listViewGroupCallOverviewInterList.currentIndex)
                        resizeFlickableGroupCallOverviewInter()
                        index--
                        if (index < 0)
                            index = 0
                        listViewGroupCallOverviewInterList.currentIndex = index

                        if (listModelGroupCallOverviewInterList.count > 0)
                            listModelGroupCallOverviewInterList.get(
                                        index).borderSize = 2


                        if (listModelGroupCallOverviewInterList.count > 0) {
//                                appCore.sendCurrentIndexOfDigitalStation(
//                                            listViewDigitalStation.currentIndex, listModelDigitalStation.get(
//                                                listViewDigitalStation.currentIndex).cidName)
                            appCore.updateViewListOfGroups(
                                        listModelGroupCallOverviewInterList.get(
                                            index).name)
                        }
                    }
                }
            }
        }

        Flickable {
            id: flickableGroupCallOverviewInter
            width: parent.width - defaultSizeOfHeight
            anchors.bottomMargin: defaultIdentForButtons
            anchors.bottom: parent.bottom
            anchors.top: rectangleGroupCallIDNames.bottom
            anchors.topMargin: -1
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter

            clip: true
            ScrollBar.vertical: ScrollBar {
                id: scrollBarGroupCallOverviewInter
            }

            Rectangle {
                id: rectangleGroupCallOverviewInter
                color: "#ffffff"
                anchors.fill: parent

                Rectangle {
                    id: rectangleGroupCallOverviewInterList
                    anchors.fill: parent
                    ListView {
                        id: listViewGroupCallOverviewInterList
                        anchors.topMargin: 0
                        anchors.fill: parent
                        currentIndex: -1
                        spacing: -1

                        interactive: false
                        model: ListModel {
                            id: listModelGroupCallOverviewInterList
                        }

                        delegate: Item {
                            width: rectangleGroupCallOverviewInter.width
                            height: defaultSizeOfHeight
                            Row {
                                id: rowGroupCallOverviewInterList

                                TextField {
                                    id: testFieldGroupCallOverviewInterIdent
                                    height: defaultSizeOfHeight
                                    text: ident
                                    width: rectangleGroupCallOverviewInter.width
                                           / 2
                                    horizontalAlignment: Text.AlignHCenter
                                    background: Rectangle {
                                        border.color: "#333"
                                        border.width: borderSize
                                    }
                                    onPressed: {
                                        if (listViewGroupCallOverviewInterList.currentIndex >= 0) {
                                            listModelGroupCallOverviewInterList.get(
                                                        listViewGroupCallOverviewInterList.currentIndex).borderSize = 1
                                        }

                                        appCore.updateViewListOfGroups(
                                                    listModelGroupCallOverviewInterList.get(
                                                        index).name)

                                        listViewGroupCallOverviewInterList.currentIndex = index
                                        listModelGroupCallOverviewInterList.get(
                                                    index).borderSize = 2
                                    }
                                    onTextChanged: {
                                        appCore.upgradeListOfGroupsIdent(
                                                    index,
                                                    testFieldGroupCallOverviewInterIdent.text)
                                    }
                                }

                                TextField {
                                    id: testFieldGroupCallOverviewInterName
                                    height: defaultSizeOfHeight
                                    width: rectangleGroupCallOverviewInter.width / 2  - scrollBarGroupCallOverviewInter.width
                                    text: name
                                    horizontalAlignment: Text.AlignHCenter
                                    background: Rectangle {
                                        border.color: "#333"
                                        border.width: borderSize
                                    }
                                    onPressed: {
                                        if (listViewGroupCallOverviewInterList.currentIndex >= 0) {
                                            listModelGroupCallOverviewInterList.get(
                                                        listViewGroupCallOverviewInterList.currentIndex).borderSize = 1
                                        }

                                        appCore.updateViewListOfGroups(
                                                    listModelGroupCallOverviewInterList.get(
                                                        index).name)

                                        listViewGroupCallOverviewInterList.currentIndex = index
                                        listModelGroupCallOverviewInterList.get(
                                                    index).borderSize = 2
                                    }
                                    onTextChanged: {
                                        appCore.upgradeListOfGroupsName(
                                                    index,
                                                    testFieldGroupCallOverviewInterName.text)
                                    }
                                }

                                spacing: -1
                            }
                        }
                        highlightMoveDuration: 100
                    }
                }
            }
        }

        Rectangle {
            id: rectangleGroupCallIDNames
            color: "#ffffff"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: buttonAddGroup.bottom
            anchors.topMargin: 1
            width: parent.width - defaultSizeOfHeight
            height: defaultSizeOfHeight
            Row {
                id: rowGroupCallOverviewInterListGroupCallIDNames

                TextField {
                    id: testFieldGroupCallOverviewInterIdentGroupCallIDNames
                    height: defaultSizeOfHeight
                    text: qsTr("Identifier")
                    width: rectangleGroupCallOverviewInter.width / 2

                    horizontalAlignment: Text.AlignHCenter
                    readOnly: true
                    background: Rectangle {
                        border.color: "#333"
                        border.width: 1
                    }
                }
                TextField {
                    id: testFieldGroupCallOverviewInterNameGroupCallIDNames
                    height: defaultSizeOfHeight
                    width: rectangleGroupCallOverviewInter.width / 2 - scrollBarGroupCallOverviewInter.width
                    text: qsTr("Name")
                    horizontalAlignment: Text.AlignHCenter
                    readOnly: true
                    background: Rectangle {
                        border.color: "#333"
                        border.width: 1
                    }
                }


                spacing: -1
            }
        }

        Button {
            id: buttonUpdateList
            y: -120
            text: qsTr("Refresh List")
            anchors.verticalCenter: buttonDeleteGroup.verticalCenter
            anchors.left: buttonDeleteGroup.right
            anchors.leftMargin: 1
            visible: false

            onClicked: {
                listModelIncludedParticipantsList.clear()
                listModelParticipantsList.clear()
                appCore.getListOfStations()
            }
        }
    }

    Rectangle {
        id: rectangleParticipants
        height: 400
        color: "#ffffff"
        anchors.left: rectangleGroupCallOverview.right
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0

        Label {
            id: labeleParticipants
            height: defaultSizeOfHeight
            text: qsTr("Members")
            verticalAlignment: Text.AlignVCenter
        }

        Rectangle {
            id: rectangleIncludedParticipants
            width: 360
            color: "#ffffff"
            anchors.left: parent.left
            anchors.leftMargin: defaultIdentForButtons
            anchors.top: labeleParticipants.bottom
            anchors.topMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: defaultIdentForButtons

            Label {
                id: labelIncludedParticipants
                text: qsTr("Participants Included")
                verticalAlignment: Text.AlignVCenter
                anchors.top: parent.top
                anchors.topMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                height: defaultSizeOfHeight
            }

            Flickable {
                id: flickableIncludedParticipantsList
                anchors.bottomMargin: -3
                anchors.top: rectanglePartList.bottom
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.topMargin: -1

                clip: true
                ScrollBar.vertical: ScrollBar {
                    id: scrollBarIncludedParticipantsList
                }

                Rectangle {
                    id: rectangleIncludedParticipantsListMain
                    color: "#ffffff"
                    anchors.fill: parent

                    Rectangle {
                        id: rectangleIncludedParticipantsList
                        anchors.fill: parent
                        ListView {
                            id: listViewIncludedParticipantsList
                            anchors.rightMargin: 0
                            anchors.bottomMargin: 0
                            anchors.leftMargin: 0
                            anchors.topMargin: 0
                            anchors.fill: parent
                            currentIndex: -1
                            spacing: -1

                            interactive: false
                            model: ListModel {
                                id: listModelIncludedParticipantsList
                            }

                            delegate: Item {
                                width: rectangleIncludedParticipantsListMain.width
                                height: defaultSizeOfHeight
                                Row {
                                    id: rowIncludedParticipantsList
                                    TextField {
                                        id: testFieldIncludedParticipantsListID
                                        height: defaultSizeOfHeight
                                        text: identOfStation
                                        width: rectangleIncludedParticipantsListMain.width / 2

                                        horizontalAlignment: Text.AlignHCenter
                                        background: Rectangle {
                                            border.color: "#333"
                                            border.width: borderSize
                                        }
                                        color: isColor
                                    }
                                    TextField {
                                        id: testFieldIncludedParticipantsListName
                                        height: defaultSizeOfHeight
                                        width: rectangleIncludedParticipantsListMain.width / 2 - scrollBarIncludedParticipantsList.width
                                        text: nameOfStation
                                        horizontalAlignment: Text.AlignHCenter
                                        background: Rectangle {
                                            border.color: "#333"
                                            border.width: borderSize
                                        }
                                        color: isColor
                                    }

                                    spacing: -1
                                }
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        if (listViewIncludedParticipantsList.currentIndex >= 0) {
                                            listModelIncludedParticipantsList.get(
                                                        listViewIncludedParticipantsList.currentIndex).borderSize = 1
                                        }
                                        listViewIncludedParticipantsList.currentIndex = index
                                        listModelIncludedParticipantsList.get(
                                                    index).borderSize = 2
                                    }
                                }
                            }
                            highlightMoveDuration: 100
                        }
                    }
                }
            }

            Rectangle {
                id: rectanglePartList
                height: defaultSizeOfHeight
                color: "#ffffff"
                anchors.top: labelIncludedParticipants.bottom
                anchors.topMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0

                Row {
                    id: rowGroupCallOverviewInterListGroupCallIDNamesPart
                    TextField {
                        id: testFieldGroupCallOverviewInterIdentGroupCallIDNamespart
                        height: defaultSizeOfHeight
                        text: qsTr("Identifier")
                        width: rectangleIncludedParticipantsListMain.width / 2

                        horizontalAlignment: Text.AlignHCenter
                        readOnly: true
                        background: Rectangle {
                            border.color: "#333"
                            border.width: 1
                        }
                    }
                    TextField {
                        id: testFieldGroupCallOverviewInterNameGroupCallIDNamesPart
                        height: defaultSizeOfHeight
                        width: rectangleIncludedParticipantsListMain.width / 2 - scrollBarIncludedParticipantsList.width
                        text: qsTr("Name")
                        horizontalAlignment: Text.AlignHCenter
                        readOnly: true
                        background: Rectangle {
                            border.color: "#333"
                            border.width: 1
                        }
                    }

                    spacing: -1
                }
            }
        }

        Button {
            id: buttonParticipantsAdd
            text: qsTr("<-- Add")
            anchors.left: rectangleIncludedParticipants.right
            height: defaultSizeOfHeight
            anchors.leftMargin: defaultIdentForButtons
            anchors.verticalCenter: rectangleIncludedParticipants.verticalCenter

            onClicked: {
                if ((listViewParticipantsList.currentIndex >= 0)
                        && (listViewGroupCallOverviewInterList.currentIndex >= 0)) {

                    if (listModelParticipantsList.count > 0) {

                        var index = listViewParticipantsList.currentIndex
                        listModelParticipantsList.get(
                                    listViewParticipantsList.currentIndex).borderSize = 1

                        appCore.addNewStationToGroup(
                                    listModelGroupCallOverviewInterList.get(
                                        listViewGroupCallOverviewInterList.currentIndex).name,
                                    listModelParticipantsList.get(
                                        listViewParticipantsList.currentIndex).identOfStation)

                        index--
                        if (index < 0)
                            index = 0

                        listViewParticipantsList.currentIndex = index

                        if (listModelParticipantsList.count > 0)
                            listModelParticipantsList.get(index).borderSize = 2
                    }
                }
            }
        }

        Button {
            id: buttonParticipantsDelete
            text: qsTr("--> Delete")
            height: defaultSizeOfHeight
            anchors.horizontalCenter: buttonParticipantsAdd.horizontalCenter
            anchors.top: buttonParticipantsAdd.bottom
            anchors.topMargin: defaultIdentForButtons

            onClicked: {
                if ((listViewIncludedParticipantsList.currentIndex >= 0)
                        && (listViewGroupCallOverviewInterList.currentIndex >= 0)) {

                    if (listModelIncludedParticipantsList.count > 0) {

                        var index = listViewIncludedParticipantsList.currentIndex
                        listModelIncludedParticipantsList.get(
                                    listViewIncludedParticipantsList.currentIndex).borderSize = 1

                        appCore.deleteStationFromGroup(
                                    listModelGroupCallOverviewInterList.get(
                                        listViewGroupCallOverviewInterList.currentIndex).name,
                                    listModelIncludedParticipantsList.get(
                                        listViewIncludedParticipantsList.currentIndex).identOfStation)

                        index--
                        if (index < 0)
                            index = 0

                        listViewIncludedParticipantsList.currentIndex = index

                        if (listModelIncludedParticipantsList.count > 0)
                            listModelIncludedParticipantsList.get(
                                        index).borderSize = 2
                    }
                }
            }
        }

        Rectangle {
            id: rectanglePossibleParticipants
            color: "#ffffff"
            anchors.top: labeleParticipants.bottom
            anchors.topMargin: 0
            anchors.left: buttonParticipantsAdd.right
            anchors.leftMargin: defaultIdentForButtons
            anchors.bottom: parent.bottom
            anchors.bottomMargin: defaultIdentForButtons
            anchors.right: parent.right
            anchors.rightMargin: defaultIdentForButtons

            Label {
                id: labelPossibleParticipants
                text: qsTr("Possible participants")
                verticalAlignment: Text.AlignVCenter
                anchors.top: parent.top
                anchors.topMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                height: defaultSizeOfHeight
            }

            Flickable {
                id: flickablePossibleParticipantsList
                anchors.rightMargin: 0
                anchors.bottomMargin: -3
                anchors.leftMargin: 0
                anchors.top: rectangleParticipantsListLabel.bottom
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.topMargin: -1

                clip: true
                ScrollBar.vertical: ScrollBar {
                    id: scrollBarParticipantsList
                }

                Rectangle {
                    id: rectangleParticipantsListMain
                    color: "#ffffff"
                    anchors.fill: parent

                    Rectangle {
                        id: rectangleParticipantsList
                        anchors.fill: parent
                        ListView {
                            id: listViewParticipantsList
                            anchors.rightMargin: 0
                            anchors.bottomMargin: 0
                            anchors.leftMargin: 0
                            anchors.topMargin: 0
                            anchors.fill: parent
                            currentIndex: -1
                            spacing: -1

                            interactive: false
                            model: ListModel {
                                id: listModelParticipantsList
                            }

                            delegate: Item {
                                width: rectangleParticipantsListMain.width
                                height: defaultSizeOfHeight
                                Row {
                                    id: rowParticipantsList
                                    TextField {
                                        id: testFieldParticipantsListID
                                        height: defaultSizeOfHeight
                                        text: identOfStation
                                        width: rectangleParticipantsListMain.width / 2

                                        horizontalAlignment: Text.AlignHCenter
                                        background: Rectangle {
                                            border.color: "#333"
                                            border.width: borderSize
                                        }
                                    }
                                    TextField {
                                        id: testFieldParticipantsListName
                                        height: defaultSizeOfHeight
                                        width: rectangleParticipantsListMain.width / 2  - scrollBarParticipantsList.width
                                        text: nameOfStation
                                        horizontalAlignment: Text.AlignHCenter
                                        background: Rectangle {
                                            border.color: "#333"
                                            border.width: borderSize
                                        }
                                    }
                                    spacing: -1
                                }
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        if (listViewParticipantsList.currentIndex >= 0) {
                                            listModelParticipantsList.get(
                                                        listViewParticipantsList.currentIndex).borderSize = 1
                                        }
                                        listViewParticipantsList.currentIndex = index
                                        listModelParticipantsList.get(
                                                    index).borderSize = 2
                                    }
                                }
                            }
                            highlightMoveDuration: 100
                        }
                    }
                }
            }

            Rectangle {
                id: rectangleParticipantsListLabel
                height: defaultSizeOfHeight
                color: "#ffffff"
                anchors.top: labelPossibleParticipants.bottom
                anchors.topMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0

                Row {
                    id: rowParticipantsListLabel
                    TextField {
                        id: testFieldParticipantsListLabelID
                        height: defaultSizeOfHeight
                        text: qsTr("Identifier")
                        width: rectangleParticipantsListMain.width / 2

                        horizontalAlignment: Text.AlignHCenter
                        readOnly: true
                        background: Rectangle {
                            border.color: "#333"
                            border.width: 1
                        }
                    }
                    TextField {
                        id: testFieldParticipantsListLabelName
                        height: defaultSizeOfHeight
                        width: rectangleParticipantsListMain.width / 2 - scrollBarParticipantsList.width
                        text: qsTr("Name")
                        horizontalAlignment: Text.AlignHCenter
                        readOnly: true
                        background: Rectangle {
                            border.color: "#333"
                            border.width: 1
                        }
                    }

                    spacing: -1
                }
            }
        }
    }
    Connections {
        target: appCore
        onSendListOfStations: {

            listModelParticipantsList.append({
                                                 "nameOfStation": nameOfStation,
                                                 "identOfStation": nameOfId,
                                                 "borderSize": 1
                                             })

            resizeFlickableIncludedParticipantsList()
            resizeFlickableParticipantsList()
        }

        onSendUpdateListOfStations: {
            listModelIncludedParticipantsList.append({
                                                         "nameOfStation": nameOfStation,
                                                         "identOfStation": nameOfId,
                                                         "borderSize": 1,
                                                         "isColor": "#333"
                                                     })

            resizeFlickableIncludedParticipantsList()
            resizeFlickableParticipantsList()
        }

        onSendUpdateListOfGroupsFromFile: {
            listModelGroupCallOverviewInterList.append({
                                                           "name": nameOfGroup,
                                                           "ident": nameOfId,
                                                           "borderSize": 1
                                                       })
            resizeFlickableGroupCallOverviewInter()
        }

        onSendClearListOfStations: {
            listModelIncludedParticipantsList.clear()
        }

        onSendClearListOfParts: {
            listModelParticipantsList.clear()
        }

        onSendNumberOfErrorIndex: {
            listModelIncludedParticipantsList.get(
                        numberOfErrorIndex).isColor = "red"
        }

        onSendNumberOfCorrectIndex: {
            listModelIncludedParticipantsList.get(
                        numberOfCorrectIndex).isColor = "#333"
        }

        onSendIndexCorrectDataIdentListOfGroups: {
            listModelGroupCallOverviewInterList.get(index).ident = dataIdent
        }

        onSendIndexCorrectDataNameListOfGroups: {
            listModelGroupCallOverviewInterList.get(index).name = dataName
        }

        onSendVisibleGroups: {
            rectangleMain.visible = stateOnVisible
        }

        onSendClearAllListOfGroups: {
            listModelGroupCallOverviewInterList.clear()
        }

        onClearListOfGroups :{
            listModelIncludedParticipantsList.clear()
            listModelParticipantsList.clear()
            listModelGroupCallOverviewInterList.clear()
            resizeFlickableIncludedParticipantsList()
            resizeFlickableParticipantsList()
            resizeFlickableGroupCallOverviewInter()
        }

        onSetCounterGroup : {
            counter = idSetStaion
            counter++
        }
    }
}
