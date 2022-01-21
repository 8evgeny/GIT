import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2

Item {
    id: element
    width: parent.width
    height: parent.height
    y:40

    Rectangle {
    id: backgroundColor
    width: parent.width
    height: parent.height
    color: "#45637D"
    }

        property string recMessage: "Записать\nсообщение"
        property string saveMessage: "Сохранить\nсообщение"
        property string loadMessage: "Загрузить\nсообщение"
        property string setMessage: "Присвоить\nсообщение"
        property string stateRowTitle: "Состояние:"
        property string messageTitle: "СООБЩЕНИЕ"
        property string messageColumnNamber: "Номер"
        property string messageColumnName: "Имя"
        property string docimentingTitle: "ДОКУМЕНТИРОВАНИЕ"
        property string docimentingColumnNamber: "Номер"
        property string docimentingColumnName: "Имя"
        property string docimentingColumnType: "Тип"
        property string docimentingColumnButtons: "Кнопки"
        property string docimentingColumnRecord: "Запись"

        property var battonsColor: "#66A9EB"
        property var battonsColorDown: "#97C4F2"
        property var tableColor: "#79B1E2"
        property var titlesColor: "#427FAE"

        property var docColWidth: 200
        property var docColHeight: 30
        property var docHeaderWidth: 1000
        property var docHeaderHeight: 60

    Item {
        id: buttonsItem
        y: 30

    Row {

            id: buttonsRow
            spacing: 5


                Button {
                    id: butt1
                    height: 100
                    width: 200
                    text: recMessage
                    font.pointSize: 15
                    background: Rectangle {
                            id: bgButt1
                            opacity: enabled ? 1 : 0.3
                            color: battonsColor
                            radius: 10
                        }

                    MouseArea{
                        id: butt1MouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: bgButt1.color = battonsColorDown
                        onExited:  bgButt1.color = battonsColor
                        onClicked: {
                            var component = Qt.createComponent("audiorecorder.qml")
                            var object = component.createObject(element)
                        }

                    }

                }

                Button {
                    id: butt2
                    height: 100
                    width: 200
                    text: saveMessage
                    font.pointSize: 15
                    background: Rectangle {
                            id: bgButt2
                            opacity: enabled ? 1 : 0.3
                            color: battonsColor
                            radius: 10
                        }
                    MouseArea{
                        id: butt2MouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: bgButt2.color = battonsColorDown
                        onExited:  bgButt2.color = battonsColor
                        onClicked: {
                            fileDialog.selectExisting = false
                            fileDialog.visible = true
                        }

                    }

                }

                Button {
                    id: butt3
                    height: 100
                    width: 200
                    text: loadMessage
                    font.pointSize: 15
                    background: Rectangle {
                            id: bgButt3
                            opacity: enabled ? 1 : 0.3
                            color: battonsColor
                            //border.width: 1
                            radius: 10
                        }
                    MouseArea{
                        id: butt3MouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: bgButt3.color = battonsColorDown
                        onExited:  bgButt3.color = battonsColor
                        onClicked: {
                            fileDialog.selectExisting = true
                            fileDialog.visible = true
                        }

                    }
                }

                Button {
                    id: butt4
                    height: 100
                    width: 200
                    text: setMessage
                    font.pointSize: 15
                    background: Rectangle {
                            id: bgButt4
                            opacity: enabled ? 1 : 0.3
                            color: battonsColor
                            //border.width: 1
                            radius: 10
                        }
                    MouseArea{
                        id: butt4MouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: bgButt4.color = battonsColorDown
                        onExited:  bgButt4.color = battonsColor
                        onClicked: {
                           //
                        }

                    }
                }
            } //![buttonsRow]
    } //![buttonsItem]

    Item {
        id: fileDialogItem
        height: 200
        width: 200

        property var dialogHeight: 300
        property var dialogWidth: 300
        property string fileDialogTitle: "Загрузить файл"

        FileDialog {
            id: fileDialog
            title: fileDialogTitle
            folder: shortcuts.home
            nameFilters: [ "Sound files *.mp3 *.wav (*.mp3 *.wav)", "All files (*)" ]
            selectedNameFilter: "All files (*)"
            onAccepted: {
                if(fileDialog.selectExisting == false) {
                    appCore.saveFile(fileDialog.fileUrl, "saveAudio.mp3")
                } else {
                //Qt.quit()
                    console.log("You chose: " + fileDialog.fileUrls)
                }
            }
            onRejected: {
                console.log("Canceled")
                //Qt.quit()
            }
        }
    }   //![FileDialogItem]

    Item {
          id:message
            y:180
          width: parent.width
          height: parent.height

          Rectangle {
              id: messTitleName
              height: 30
              width: parent.width
             // y:120
              color: titlesColor
              border {
                  color: "black"
                  width: 1
              }
              Text {
                  anchors.centerIn: parent
                   text: messageTitle
               }
           }
          Loader {
              id: playerItem
              anchors.top: tableTitle.bottom
              source: "player.qml"
          }
          Row {
               id: tableTitle
               anchors.top: messTitleName.bottom
               Rectangle {
                   id: columnIndex
                   height: 30
                   width: 100
                   color: "#C8E2F9"
                   border {
                       color: "black"
                       width: 1
                   }
                   Text {
                       anchors.centerIn: parent
                        text: messageColumnNamber
                    }
               }

               Rectangle {
                   id: columnName
                   height: 30
                   width: 500
                   color: "#C8E2F9"
                   border {
                       color: "black"
                       width: 1
                   }
                   Text {
                       anchors.centerIn: parent
                       text: messageColumnName
                   }
               }

          }


          }
    Item {
          id:documents
            y:520
          width: parent.width
          height: 30

          Rectangle {
              id: docTitleName
              height: 30
              width: parent.width
             // y:120
              color: titlesColor
              border {
                  color: "black"
                  width: 1
              }
              Text {
                  anchors.centerIn: parent
                   text: "ДОКУМЕНТИРОВАНИЕ"
               }
           }
          Row {
              x: 0
              y: 30
              width: dcoHeaderWidth
              height: dcoHeaderHeight
              z: 1
              Repeater {
                     model: [   docimentingColumnNamber, docimentingColumnName,
                                docimentingColumnType, docimentingColumnButtons,
                                docimentingColumnRecord
                     ]

                     Rectangle {


                         height: 30
                         width: docColWidth
                         color: tableColor
                         border {
                             color: "black"
                             width: 1
                         }
                         Text {
                             anchors.centerIn: parent
                              text: modelData
                          }
                     }
                 }
           }

          Item {

              height: 100
              width: docColWidth*5
              y:60
          ListModel {
                            id: listModel
                            ListElement {text: "item 1"}
                            ListElement {text: "item 2"}
                            ListElement {text: "item 3"}
                            ListElement {text: "item 4"}
                             ListElement {text: "item 5"}
                              ListElement {text: "item 6"}
                               ListElement {text: "item 7"}
                                ListElement {text: "item 8"}
                                 ListElement {text: "item 9"}
                                  ListElement {text: "item 10"}
                        }

                        ListView {
                            z: -1

                            anchors.fill: parent
                            model: listModel
                            delegate: delegate
                        }


                        Component {

                            id: delegate


                            Row {
                                  anchors.top: docTitleName.bottom
                                Rectangle {

                                    height: docColHeight
                                    width: docColWidth
                                    color: tableColor
                                    //border.color: "black"
                                    //border.width: 1

                                    Text {
                                        anchors.centerIn: parent
                                        text: model.index
                                    }
                                }

                                Rectangle {

                                    height: docColHeight
                                    width: docColWidth
                                    color: tableColor
                                    //border.color: "black"
                                    //border.width: 1

                                    Text {
                                        anchors.centerIn: parent
                                        text: model.text
                                    }
                                }

                                Rectangle {

                                    height: docColHeight
                                    width: docColWidth
                                    color: tableColor
                                   // border.color: "black"
                                    //border.width: 1

                                    Text {
                                        anchors.centerIn: parent
                                        text: model.text
                                    }
                                }

                                Rectangle {

                                    height: docColHeight
                                    width: docColWidth
                                    color: tableColor
                                    //border.color: "black"
                                    //border.width: 1

                                    Text {
                                        anchors.centerIn: parent
                                        text: model.text
                                    }
                                }

                                Rectangle {

                                    height: docColHeight
                                    width: docColWidth
                                    color: tableColor
                                    //border.color: "black"
                                    //border.width: 1

                                    Text {
                                        anchors.centerIn: parent
                                        text: model.text
                                    }
                                }

                            }
                        }

          }


    }

    Item {
        id: stateRow
        x: 0
        //y: 300
        width: parent.width
        height: 30
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0



        Rectangle {
            id: rectangleStateRow
            width: parent.width
            height: 30
            border.width: 1
            border.color: "black"
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0

            Text {
                id: stateText
                text: qsTr(stateRowTitle)
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }
}


