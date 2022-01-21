import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Window 2.12

ApplicationWindow {
    id: assignmentListRoot

    width: 520
    height: 320
    title: nameTitle
    property string nameTitle: qsTr("List of Appointments")

    maximumHeight: height
    maximumWidth: width

    minimumHeight: height
    minimumWidth: width

    property color defaultColor: "#e1e1e2"

    property int defaultSizeOfHeight: 40
    property int defaultSizeOfSpace: 5
    property int cellWidth: 200
    property int baseHeight: 40
    property int typeOfMenu: 0


    Flickable {
        id: flickableAssignmentList
        anchors.bottomMargin: 55
        anchors.fill: parent

        clip: true
        ScrollBar.vertical: ScrollBar {
            id: scrollBarAssignmentList
        }

        Rectangle {
            id: rectangleAssignmentList
            anchors.fill: parent

            Rectangle {
                id: rectangleInternalAssignmentList
                anchors.fill: parent
                ListView {
                    id: listViewAssignmentList
                    anchors.fill: parent
                    interactive: false

                    model: ListModel {
                        id: listModelAssignmentList
                    }

                    delegate: Item {
                        width: flickableAssignmentList.width
                        height: defaultSizeOfHeight

                        Row {
                            id: rowListOfSubscribers
                            width: parent.width
                            height: parent.height
                            Rectangle {
                                height: baseHeight
                                width: parent.width
                                id: rectangleAssignmentListName
                                TextField {
                                    //selectByMouse: true
                                    id: textFieldAssignmentListName
                                    height: baseHeight
                                    width: parent.width / 2
                                    readOnly: true
                                    text: nameOfAssignment

                                    background: Rectangle {
                                        border.color: "#333"
                                        border.width: borderSize
                                    }

                                    MouseArea {
                                        anchors.fill: parent

                                        onDoubleClicked: {
                                            if (listViewAssignmentList.currentIndex >= 0) {
                                                listModelAssignmentList.get(
                                                            listViewAssignmentList.currentIndex).borderSize = 1
                                            }

                                            listViewAssignmentList.currentIndex = index
                                            listModelAssignmentList.get(
                                                        index).borderSize = 2

                                            title = listModelAssignmentList.get(
                                                        index).nameOfAssignment + " " + listModelAssignmentList.get(
                                                        index).nameStation

                                            if (listViewAssignmentList.currentIndex >= 0) {
                                                appCore.getCurrentNameOfStationsAndGroups(
                                                            listModelAssignmentList.get(
                                                                listViewAssignmentList.currentIndex).nameOfAssignment)
                                            }
                                            assignmentListRoot.close()
                                        }

                                        onClicked: {
                                            if (listViewAssignmentList.currentIndex >= 0) {
                                                listModelAssignmentList.get(
                                                            listViewAssignmentList.currentIndex).borderSize = 1
                                            }

                                            listViewAssignmentList.currentIndex = index
                                            listModelAssignmentList.get(
                                                        index).borderSize = 2

                                            title = listModelAssignmentList.get(
                                                        index).nameOfAssignment + " " + listModelAssignmentList.get(
                                                        index).nameStation
                                        }
                                    }


                                }
                                TextField {
                                    anchors.left: textFieldAssignmentListName.right
                                    anchors.leftMargin: -1

                                    height: baseHeight
                                    width: parent.width / 2
                                    readOnly: true
                                    text: nameStation

                                    background: Rectangle {
                                        border.color: "#333"
                                        border.width: borderSize
                                    }
                                    MouseArea {
                                        anchors.fill: parent

                                        onDoubleClicked: {
                                            if (listViewAssignmentList.currentIndex >= 0) {
                                                listModelAssignmentList.get(
                                                            listViewAssignmentList.currentIndex).borderSize = 1
                                            }

                                            listViewAssignmentList.currentIndex = index
                                            listModelAssignmentList.get(
                                                        index).borderSize = 2

                                            title = listModelAssignmentList.get(
                                                        index).nameOfAssignment + " " + listModelAssignmentList.get(
                                                        index).nameStation

                                            if (listViewAssignmentList.currentIndex >= 0) {
                                                appCore.getCurrentNameOfStationsAndGroups(
                                                            listModelAssignmentList.get(
                                                                listViewAssignmentList.currentIndex).nameOfAssignment)
                                            }
                                            assignmentListRoot.close()
                                        }

                                        onClicked: {
                                            if (listViewAssignmentList.currentIndex >= 0) {
                                                listModelAssignmentList.get(
                                                            listViewAssignmentList.currentIndex).borderSize = 1
                                            }

                                            listViewAssignmentList.currentIndex = index
                                            listModelAssignmentList.get(
                                                        index).borderSize = 2

                                            title = listModelAssignmentList.get(
                                                        index).nameOfAssignment + " " + listModelAssignmentList.get(
                                                        index).nameStation
                                        }
                                    }
                                }

                            }
                            spacing: -1
                        }
                    }
                    spacing: -1
                }
            }
        }
    }

    //Set the center of coordinates
//    function setCoordinates() {
//        x += Screen.width / 2 - width / 2
//        y += Screen.height / 2 - height / 2
//    }

    //Resize current list
    function resizeFlick() {
        flickableAssignmentList.contentHeight = listModelAssignmentList.count * baseHeight
        rectangleInternalAssignmentList.height = listModelAssignmentList.count * baseHeight
    }

    Connections {
        target: appCore
        onListOfStationsAndGroups: {
            //Add a new item to the current list
            listModelAssignmentList.append({
                                               "nameOfAssignment": itemForAdding,
                                               "nameStation" : nameStationAdding,
                                               "borderSize": 1
                                           })
            //Resize current list
            resizeFlick()
        }
    }

    Button {
        id: buttonOk
        text: qsTr("Ok")
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        onClicked: {
            if (listViewAssignmentList.currentIndex >= 0) {
                appCore.getCurrentNameOfStationsAndGroups(
                            listModelAssignmentList.get(
                                listViewAssignmentList.currentIndex).nameOfAssignment)
            }
            assignmentListRoot.close()
        }
    }

    Button {
        id: buttonCancel
        text: qsTr("Cancel")
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        onClicked: {
            if (listViewAssignmentList.currentIndex >= 0) {
                listModelAssignmentList.get(
                            listViewAssignmentList.currentIndex).borderSize = 1
            }
            listViewAssignmentList.currentIndex = -1
            assignmentListRoot.title = nameTitle
        }
    }

//    onClosing: {

//    }

    Component.onCompleted: {
        //set current index -1
        listViewAssignmentList.currentIndex = -1

        //Set the center of coordinates
//        setCoordinates()

        //Get the list of stations and groups
        appCore.getListOfStationsAndGroups()
    }
}
