import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

Item {
    width: parent.width
    height: parent.height

        property string subTabName: "АБОНЕНТ"
        property string progTabName: "ПРОГРАММАТОР"
        property string messTabName: "СООБЩЕНИЕ"

    TabBar {

        id: bar
         width: parent.width


         Repeater {
                model: [subTabName, progTabName, messTabName]

                TabButton {
                    text: modelData
                    background: Rectangle {
                        implicitHeight: 40
                                color: bar.currentIndex === index ? "#E0E0E0" : "grey"

                }
            }
        }
   }

    StackLayout {

        width: applicationWindow.width
        currentIndex: bar.currentIndex

        Item {
            id: tabSubscriber


            Component.onCompleted: {
                            var component = Qt.createComponent("subscriber_navigation.qml")
                            var object = component.createObject(tabSubscriber)
                        }
//            Rectangle {
//                 y: 40
//                color: "lightblue"
//                width: applicationWindow.width
//                height: applicationWindow.height-80
//            }

        }

        Item {
            id: tabProgrammer


            //anchors.top: bar.bottom
            Component.onCompleted: {
                            var component = Qt.createComponent("programmer.qml")
                            var object = component.createObject(tabProgrammer)
                        }

        }
        Item {
            id: tabMessages
            width: 1200
            height:780
            Component.onCompleted: {
                            var component = Qt.createComponent("messages.qml")
                            var object = component.createObject(tabMessages)
                        }
        }


    }



//    property string subTabName: "АБОНЕНТ"
//    property string progTabName: "ПРОГРАММАТОР"
//    property string messTabName: "СООБЩЕНИЕ"
//    property color backgroudColor: "#E0E0E0"
//    property color changeColor: "white"
//    property color changeBorderColor: "grey"

//    Rectangle {
//    id: place
//    width: applicationWindow.width
//    height: 40
//    color: backgroudColor
//    }

//    Rectangle {
//        anchors.bottom: place.bottom
//        width: place.width
//        height: 1
//        color: "black"
//    }

//    Row {

//        //property type name: value

//       Button {
//            id: subscriber
//            text: subTabName
//            height: place.height-1

//            background: Rectangle {
//            id: subBackground
//            implicitWidth: 100
//            implicitHeight: 40
//            opacity: enabled ? 1 : 0.3
//            border.color: backgroudColor
//            color: backgroudColor
//            border.width: 1
//            states: [
//                State {
//                    name: "Color"
//                    PropertyChanges { target: subBackground; color: changeColor }
//                    PropertyChanges { target: subBackground; border.color: changeBorderColor }
//                }
//            ]

//            MouseArea {
//                anchors.fill: parent
//                onClicked: {
//                    if (subBackground.state == "Color") subBackground.state = ""
//                        else subBackground.state = "Color"
//                                progBackground.state = ""
//                                    messBackground.state = ""
//                }
//            }
//          }
//        }



//        Button {
//            id: programmator
//            text: progTabName
//            height: place.height-1



//            background: Rectangle {
//                            id: progBackground
//                            implicitWidth: 100
//                            implicitHeight: 40
//                            opacity: enabled ? 1 : 0.3
//                            border.color: backgroudColor
//                            color: backgroudColor
//                            border.width: 1
//                            states: [
//                                State {
//                                    name: "Color"
//                                    PropertyChanges { target: progBackground; color: changeColor }
//                                    PropertyChanges { target: progBackground; border.color: changeBorderColor }
//                                }
//                            ]

//                            MouseArea {
//                                anchors.fill: parent
//                                onClicked: {

//                                    if (progBackground.state == "Color") {progBackground.state = ""
//                                    tabProgrammer.destroy()}
//                                        else {progBackground.state = "Color"
//                                                subBackground.state = ""
//                                                    messBackground.state = ""

//                                  var  component = Qt.createComponent("programmer.qml")
//                                 var object =    component.createObject(tabProgrammer)
//                                    }

//                                }
//                            }
//                        }
//            }

//        Button {
//            id: messages
//            text: messTabName
//            height: place.height-1

//            background: Rectangle {
//                            id: messBackground
//                            implicitWidth: 100
//                            implicitHeight: 40
//                            opacity: enabled ? 1 : 0.3
//                            border.color: backgroudColor
//                            color: backgroudColor
//                            border.width: 1
//                            states: [
//                                State {
//                                    name: "Color"
//                                    PropertyChanges { target: messBackground; color: changeColor }
//                                    PropertyChanges { target: messBackground; border.color: changeBorderColor }
//                                }
//                            ]

//                            MouseArea {
//                                anchors.fill: parent
//                                onClicked: {
//                                    if (messBackground.state == "Color") messBackground.state = ""
//                                        else messBackground.state = "Color"
//                                                subBackground.state = ""
//                                                    progBackground.state = ""
//                                }
//                            }
//                        }

//        }

//    }

}
