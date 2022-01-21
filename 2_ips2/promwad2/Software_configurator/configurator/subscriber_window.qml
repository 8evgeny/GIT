import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Window 2.12

ApplicationWindow {
    id: subscriberRoot

    width: 320
    height: 420
    title: nameTitle

    maximumHeight: height
    maximumWidth: width

    minimumHeight: height
    minimumWidth: width

    property color defaultColor: "#00000000"
    property int defaultHeight: 40

    property string nameTitle: qsTr("Key function")
    property color selectedColor: "#e1e1e2"
    property string tempText: mainText
    property string mainText: ""
    property int mainNumber: 0

    property int xScreen: 0
    property int yScreen: 0


    function keyFunction(){
        mainText = tempText
        if (mainNumber >= 1) {

            if (mainText == labelDirectConnectionDuplex.text)
                appCore.receiveFromQmlSubscriber("1", mainNumber)

            if (mainText == labelGroupCommunication.text)
                appCore.receiveFromQmlSubscriber("2", mainNumber)

            if (mainText == labelCircularLink.text)
                appCore.receiveFromQmlSubscriber("3", mainNumber)

            if (mainText == labelConferenceCommunication.text)
                appCore.receiveFromQmlSubscriber("4", mainNumber)

            if (mainText == labelTelephoneCommunications.text)
                appCore.receiveFromQmlSubscriber("5", mainNumber)

            if (mainText == labelDirectConnectionSimplex.text)
                appCore.receiveFromQmlSubscriber("6", mainNumber)

            if (mainText == labelHungUp.text)
                appCore.receiveFromQmlSubscriber("7", mainNumber)

            subscriberRoot.close()
        }
    }

    Grid {
        id: gridConnection
        width: parent.width
        height: 350
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 2
        rows: 7
        columns: 1

        MouseArea {
            id: mouseAreaDirectConnectionDuplex
            width: parent.width
            height: subscriberRoot.defaultHeight
            Rectangle {
                id: rectangleDirectConnectionDuplex
                width: parent.width
                height: parent.height
                color: subscriberRoot.defaultColor
                Label {
                    id: labelDirectConnectionDuplex
                    text: qsTr("Direct connection: duplex")
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }

        MouseArea {
            id: mouseAreaDirectConnectionSimplex
            width: parent.width
            height: subscriberRoot.defaultHeight
            Rectangle {
                id: rectangleDirectConnectionSimplex
                width: parent.width
                height: parent.height
                color: subscriberRoot.defaultColor
                Label {
                    id: labelDirectConnectionSimplex
                    text: qsTr("Direct connection: simplex")
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }

        MouseArea {
            id: mouseAreaGroupCommunication
            width: parent.width
            height: subscriberRoot.defaultHeight
            Rectangle {
                id: rectangleGroupCommunication
                width: parent.width
                height: parent.height
                color: subscriberRoot.defaultColor

                Label {
                    id: labelGroupCommunication
                    text: qsTr("Group communication")
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }

        MouseArea {
            id: mouseAreaCircularLink
            width: parent.width
            height: subscriberRoot.defaultHeight
            Rectangle {
                id: rectangleCircularLink
                width: parent.width
                height: parent.height
                color: subscriberRoot.defaultColor

                Label {
                    id: labelCircularLink
                    text: qsTr("Circular bond")
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }

        MouseArea {
            id: mouseAreaConferenceCommunication
            width: parent.width
            height: subscriberRoot.defaultHeight
            Rectangle {
                id: rectangleConferenceCommunication
                width: parent.width
                height: parent.height
                color: subscriberRoot.defaultColor

                Label {
                    id: labelConferenceCommunication
                    text: qsTr("Conference call")
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }

        MouseArea {
            id: mouseAreaTelephoneCommunications
            width: parent.width
            height: subscriberRoot.defaultHeight
            Rectangle {
                id: rectangleTelephoneCommunications
                width: parent.width
                height: parent.height
                color: subscriberRoot.defaultColor
                Label {
                    id: labelTelephoneCommunications
                    text: qsTr("Telephone communications")
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }

        MouseArea {
            id: mouseAreaHungUp
            width: parent.width
            height: subscriberRoot.defaultHeight
            Rectangle {
                id: rectangleHungUp
                width: parent.width
                height: parent.height
                color: subscriberRoot.defaultColor
                Label {
                    id: labelHungUp
                    text: qsTr("Hang up")
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }
    }

    Button {
        id: buttonCancel
        text: qsTr("Cancel")
        anchors.left: parent.left
        anchors.leftMargin: 25
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 15
        onClicked: {

        }
    }

    Button {
        id: buttonOk
        text: qsTr("Ok")
        anchors.right: parent.right
        anchors.rightMargin: 25
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 15
        onClicked: {
            mainText = tempText
            if (mainNumber >= 1) {

                if (mainText == labelDirectConnectionDuplex.text)
                    appCore.receiveFromQmlSubscriber("1", mainNumber)

                if (mainText == labelGroupCommunication.text)
                    appCore.receiveFromQmlSubscriber("2", mainNumber)

                if (mainText == labelCircularLink.text)
                    appCore.receiveFromQmlSubscriber("3", mainNumber)

                if (mainText == labelConferenceCommunication.text)
                    appCore.receiveFromQmlSubscriber("4", mainNumber)

                if (mainText == labelTelephoneCommunications.text)
                    appCore.receiveFromQmlSubscriber("5", mainNumber)

                if (mainText == labelDirectConnectionSimplex.text)
                    appCore.receiveFromQmlSubscriber("6", mainNumber)

                if (mainText == labelHungUp.text)
                    appCore.receiveFromQmlSubscriber("7", mainNumber)

                subscriberRoot.close()
            }
        }
    }

    Connections {
        target: mouseAreaDirectConnectionDuplex
        onClicked: {
            rectangleDirectConnectionDuplex.color = subscriberRoot.selectedColor
            rectangleDirectConnectionSimplex.color = subscriberRoot.defaultColor
            rectangleGroupCommunication.color = subscriberRoot.defaultColor
            rectangleCircularLink.color = subscriberRoot.defaultColor
            rectangleConferenceCommunication.color = subscriberRoot.defaultColor
            rectangleTelephoneCommunications.color = subscriberRoot.defaultColor
            rectangleHungUp.color = subscriberRoot.defaultColor

            subscriberRoot.title = subscriberRoot.nameTitle + " ("
                    + labelDirectConnectionDuplex.text + ")"
            tempText = labelDirectConnectionDuplex.text
        }
        onDoubleClicked: {
            rectangleDirectConnectionDuplex.color = subscriberRoot.selectedColor
            rectangleDirectConnectionSimplex.color = subscriberRoot.defaultColor
            rectangleGroupCommunication.color = subscriberRoot.defaultColor
            rectangleCircularLink.color = subscriberRoot.defaultColor
            rectangleConferenceCommunication.color = subscriberRoot.defaultColor
            rectangleTelephoneCommunications.color = subscriberRoot.defaultColor
            rectangleHungUp.color = subscriberRoot.defaultColor

            subscriberRoot.title = subscriberRoot.nameTitle + " ("
                    + labelDirectConnectionDuplex.text + ")"
            tempText = labelDirectConnectionDuplex.text
            keyFunction()
        }
    }

    Connections {
        target: mouseAreaDirectConnectionSimplex
        onClicked: {
            rectangleDirectConnectionDuplex.color = subscriberRoot.defaultColor
            rectangleDirectConnectionSimplex.color = subscriberRoot.selectedColor
            rectangleGroupCommunication.color = subscriberRoot.defaultColor
            rectangleCircularLink.color = subscriberRoot.defaultColor
            rectangleConferenceCommunication.color = subscriberRoot.defaultColor
            rectangleTelephoneCommunications.color = subscriberRoot.defaultColor
            rectangleHungUp.color = subscriberRoot.defaultColor

            subscriberRoot.title = subscriberRoot.nameTitle + " ("
                    + labelDirectConnectionSimplex.text + ")"
            tempText = labelDirectConnectionSimplex.text
        }
        onDoubleClicked: {
            rectangleDirectConnectionDuplex.color = subscriberRoot.defaultColor
            rectangleDirectConnectionSimplex.color = subscriberRoot.selectedColor
            rectangleGroupCommunication.color = subscriberRoot.defaultColor
            rectangleCircularLink.color = subscriberRoot.defaultColor
            rectangleConferenceCommunication.color = subscriberRoot.defaultColor
            rectangleTelephoneCommunications.color = subscriberRoot.defaultColor
            rectangleHungUp.color = subscriberRoot.defaultColor

            subscriberRoot.title = subscriberRoot.nameTitle + " ("
                    + labelDirectConnectionSimplex.text + ")"
            tempText = labelDirectConnectionSimplex.text
            keyFunction()
        }
    }

    Connections {
        target: mouseAreaGroupCommunication
        onClicked: {
            rectangleDirectConnectionDuplex.color = subscriberRoot.defaultColor
            rectangleDirectConnectionSimplex.color = subscriberRoot.defaultColor
            rectangleGroupCommunication.color = subscriberRoot.selectedColor
            rectangleCircularLink.color = subscriberRoot.defaultColor
            rectangleConferenceCommunication.color = subscriberRoot.defaultColor
            rectangleTelephoneCommunications.color = subscriberRoot.defaultColor
            rectangleHungUp.color = subscriberRoot.defaultColor

            subscriberRoot.title = subscriberRoot.nameTitle + " ("
                    + labelGroupCommunication.text + ")"
            tempText = labelGroupCommunication.text
        }
        onDoubleClicked: {
            rectangleDirectConnectionDuplex.color = subscriberRoot.defaultColor
            rectangleDirectConnectionSimplex.color = subscriberRoot.defaultColor
            rectangleGroupCommunication.color = subscriberRoot.selectedColor
            rectangleCircularLink.color = subscriberRoot.defaultColor
            rectangleConferenceCommunication.color = subscriberRoot.defaultColor
            rectangleTelephoneCommunications.color = subscriberRoot.defaultColor
            rectangleHungUp.color = subscriberRoot.defaultColor

            subscriberRoot.title = subscriberRoot.nameTitle + " ("
                    + labelGroupCommunication.text + ")"
            tempText = labelGroupCommunication.text
            keyFunction()
        }
    }

    Connections {
        target: mouseAreaCircularLink
        onClicked: {
            rectangleDirectConnectionDuplex.color = subscriberRoot.defaultColor
            rectangleDirectConnectionSimplex.color = subscriberRoot.defaultColor
            rectangleGroupCommunication.color = subscriberRoot.defaultColor
            rectangleCircularLink.color = subscriberRoot.selectedColor
            rectangleConferenceCommunication.color = subscriberRoot.defaultColor
            rectangleTelephoneCommunications.color = subscriberRoot.defaultColor
            rectangleHungUp.color = subscriberRoot.defaultColor

            subscriberRoot.title = subscriberRoot.nameTitle + " (" + labelCircularLink.text + ")"
            tempText = labelCircularLink.text
        }
        onDoubleClicked: {
            rectangleDirectConnectionDuplex.color = subscriberRoot.defaultColor
            rectangleDirectConnectionSimplex.color = subscriberRoot.defaultColor
            rectangleGroupCommunication.color = subscriberRoot.defaultColor
            rectangleCircularLink.color = subscriberRoot.selectedColor
            rectangleConferenceCommunication.color = subscriberRoot.defaultColor
            rectangleTelephoneCommunications.color = subscriberRoot.defaultColor
            rectangleHungUp.color = subscriberRoot.defaultColor

            subscriberRoot.title = subscriberRoot.nameTitle + " (" + labelCircularLink.text + ")"
            tempText = labelCircularLink.text
            keyFunction()
        }
    }

    Connections {
        target: mouseAreaConferenceCommunication
        onClicked: {

            rectangleDirectConnectionDuplex.color = subscriberRoot.defaultColor
            rectangleDirectConnectionSimplex.color = subscriberRoot.defaultColor
            rectangleGroupCommunication.color = subscriberRoot.defaultColor
            rectangleCircularLink.color = subscriberRoot.defaultColor
            rectangleConferenceCommunication.color = subscriberRoot.selectedColor
            rectangleTelephoneCommunications.color = subscriberRoot.defaultColor
            rectangleHungUp.color = subscriberRoot.defaultColor

            subscriberRoot.title = subscriberRoot.nameTitle + " ("
                    + labelConferenceCommunication.text + ")"
            tempText = labelConferenceCommunication.text
        }
         onDoubleClicked: {
             rectangleDirectConnectionDuplex.color = subscriberRoot.defaultColor
             rectangleDirectConnectionSimplex.color = subscriberRoot.defaultColor
             rectangleGroupCommunication.color = subscriberRoot.defaultColor
             rectangleCircularLink.color = subscriberRoot.defaultColor
             rectangleConferenceCommunication.color = subscriberRoot.selectedColor
             rectangleTelephoneCommunications.color = subscriberRoot.defaultColor
             rectangleHungUp.color = subscriberRoot.defaultColor

             subscriberRoot.title = subscriberRoot.nameTitle + " ("
                     + labelConferenceCommunication.text + ")"
             tempText = labelConferenceCommunication.text
             keyFunction()
         }
    }

    Connections {
        target: mouseAreaTelephoneCommunications
        onClicked: {
            rectangleDirectConnectionDuplex.color = subscriberRoot.defaultColor
            rectangleDirectConnectionSimplex.color = subscriberRoot.defaultColor
            rectangleGroupCommunication.color = subscriberRoot.defaultColor
            rectangleCircularLink.color = subscriberRoot.defaultColor
            rectangleConferenceCommunication.color = subscriberRoot.defaultColor
            rectangleTelephoneCommunications.color = subscriberRoot.selectedColor
            rectangleHungUp.color = subscriberRoot.defaultColor

            subscriberRoot.title = subscriberRoot.nameTitle + " ("
                    + labelTelephoneCommunications.text + ")"
            tempText = labelTelephoneCommunications.text
        }
        onDoubleClicked: {
            rectangleDirectConnectionDuplex.color = subscriberRoot.defaultColor
            rectangleDirectConnectionSimplex.color = subscriberRoot.defaultColor
            rectangleGroupCommunication.color = subscriberRoot.defaultColor
            rectangleCircularLink.color = subscriberRoot.defaultColor
            rectangleConferenceCommunication.color = subscriberRoot.defaultColor
            rectangleTelephoneCommunications.color = subscriberRoot.selectedColor
            rectangleHungUp.color = subscriberRoot.defaultColor

            subscriberRoot.title = subscriberRoot.nameTitle + " ("
                    + labelTelephoneCommunications.text + ")"
            tempText = labelTelephoneCommunications.text
            keyFunction()
        }
    }

    Connections {
        target: mouseAreaHungUp
        onClicked: {
            rectangleDirectConnectionDuplex.color = subscriberRoot.defaultColor
            rectangleDirectConnectionSimplex.color = subscriberRoot.defaultColor
            rectangleGroupCommunication.color = subscriberRoot.defaultColor
            rectangleCircularLink.color = subscriberRoot.defaultColor
            rectangleConferenceCommunication.color = subscriberRoot.defaultColor
            rectangleTelephoneCommunications.color = subscriberRoot.defaultColor
            rectangleHungUp.color = subscriberRoot.selectedColor

            subscriberRoot.title = subscriberRoot.nameTitle + " ("
                    + labelHungUp.text + ")"
            tempText = labelHungUp.text
        }
        onDoubleClicked: {
            rectangleDirectConnectionDuplex.color = subscriberRoot.defaultColor
            rectangleDirectConnectionSimplex.color = subscriberRoot.defaultColor
            rectangleGroupCommunication.color = subscriberRoot.defaultColor
            rectangleCircularLink.color = subscriberRoot.defaultColor
            rectangleConferenceCommunication.color = subscriberRoot.defaultColor
            rectangleTelephoneCommunications.color = subscriberRoot.defaultColor
            rectangleHungUp.color = subscriberRoot.selectedColor

            subscriberRoot.title = subscriberRoot.nameTitle + " ("
                    + labelHungUp.text + ")"
            tempText = labelHungUp.text
            keyFunction()
        }

    }

    Connections {
        target: buttonCancel
        onClicked: {
            rectangleDirectConnectionDuplex.color = subscriberRoot.defaultColor
            rectangleDirectConnectionSimplex.color = subscriberRoot.defaultColor
            rectangleGroupCommunication.color = subscriberRoot.defaultColor
            rectangleCircularLink.color = subscriberRoot.defaultColor
            rectangleConferenceCommunication.color = subscriberRoot.defaultColor
            rectangleTelephoneCommunications.color = subscriberRoot.defaultColor
            rectangleHungUp.color = subscriberRoot.defaultColor

            subscriberRoot.title = subscriberRoot.nameTitle
            tempText = mainText
        }
        onDoubleClicked: {
            rectangleDirectConnectionDuplex.color = subscriberRoot.defaultColor
            rectangleDirectConnectionSimplex.color = subscriberRoot.defaultColor
            rectangleGroupCommunication.color = subscriberRoot.defaultColor
            rectangleCircularLink.color = subscriberRoot.defaultColor
            rectangleConferenceCommunication.color = subscriberRoot.defaultColor
            rectangleTelephoneCommunications.color = subscriberRoot.defaultColor
            rectangleHungUp.color = subscriberRoot.defaultColor

            subscriberRoot.title = subscriberRoot.nameTitle
            tempText = mainText
            keyFunction()
        }
    }

    onClosing: {

    }

    Connections {
        target: appCore
    }

//    function setCoordinates() {
//        x += Screen.width / 2 - width / 2
//        y += Screen.height / 2 - height / 2
//    }

    Component.onCompleted: {
//        setCoordinates()
    }
}
