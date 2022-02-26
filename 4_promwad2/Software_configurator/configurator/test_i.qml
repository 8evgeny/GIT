import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQuick.Controls.Styles 1.4

Rectangle {
    Item {

                                width: 300
                                height: 200
                                Row {
                                    width: 300
                                    height: 200
                                    id: rowGroupCallOverviewInterList

                                    TextField {
                                        height: 50
                                        text: "asfasdf"
                                        horizontalAlignment: Text.AlignHCenter
                                        width: 120
                                        id: textField1
                                        background: Rectangle {
                                            color: "yellow"
                                            border.color: "#333"
                                            border.width: 1
                                        }
                                    }
                                    TextField {
                                        id: textField2
                                        height: 50
                                        text: "adsfasdf"
                                        width: 120
                                    }
                                    spacing: 1
                                }

                                MouseArea {
                                    anchors.top: parent.top

                                    TextEdit {
                                        id: textEdit
                                        x: 158
                                        y: 144
                                        width: 80
                                        height: 20
                                        text: qsTr("Text Edit")
                                        font.pixelSize: 12
                                    }

                                    TextField {
                                        id: textField
                                        x: 125
                                        y: 180
                                        text: qsTr("Text Field")
                                    }


                                }
                            }


}







/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
