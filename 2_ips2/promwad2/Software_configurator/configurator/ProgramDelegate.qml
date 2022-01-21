import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

ItemDelegate {
    id: delegate
    width: parent.width;
    height: rectangleList.delegateProgramHeight
    transformOrigin: Item.Center
    highlighted: ListView.isCurrentItem
    property int delegateChecked: headCheckBox.checkState
    property int number : index
    onClicked:{
        programListView.currentIndex = index
        console.log(index);
    }
    clip: true
    anchors.margins: 4
        Row {
            height: parent.height
            id: delegateRow
            clip: true
            opacity: 1
            anchors.fill: parent
            Rectangle {
                width: parent.height
                height: parent.height
                id: rectangleCheckBox
                border.color: "black"
                CheckBox {
                    id : delegateCheckBox
                    anchors.centerIn: parent
                    tristate: false
                    wheelEnabled: false
                    bottomPadding: 8
                    autoExclusive: false
                    checkState: headCheckBox.checkState
                    onClicked: delegateChecked = checkState
                   // visible: delegateChecked ? false : true
                }
            }

            Rectangle {
                width: (delegateRow.width-parent.height) / 10
                height: parent.height
                id: rectangleNumber
                border.color: "black"

                color: delegateCheckBox.checkState ? defaultColor : "transparent"
                TextArea {
                    text: number
                    anchors.centerIn: parent
                    font.pixelSize: rectangleList.pixelSize
                    font.pointSize: rectangleList.pointSize
                    font.bold: delegateCheckBox.checkState ? true : false
                    width: parent.textWidth
                }
            }

            Rectangle {
                width: (delegateRow.width-parent.height) / 5
                height: parent.height
                id: rectangleName
                border.color: "black"
                color: delegateCheckBox.checkState ? defaultColor : "transparent"
                Text {
                    text: name
                    anchors.centerIn: parent
                    font.pixelSize: rectangleList.pixelSize
                    font.pointSize: rectangleList.pointSize
                    font.bold: delegateCheckBox.checkState ? true : false
                    width: parent.textWidth
                }
            }

            Rectangle {
                width: (delegateRow.width-parent.height) / 10
                height: parent.height
                id: rectangleType
                border.color: "black"
                color: delegateCheckBox.checkState ? defaultColor : "transparent"
                Text {
                    text: type
                    anchors.centerIn: parent
                    font.pixelSize: rectangleList.pixelSize
                    font.pointSize: rectangleList.pointSize
                    font.bold: delegateCheckBox.checkState ? true : false
                    width: parent.textWidth
                }
            }

            Rectangle {
                width: (delegateRow.width-parent.height) / 10
                height: parent.height
                id: rectangleButtons
                border.color: "black"
                color: delegateCheckBox.checkState ? defaultColor : "transparent"
                Text {
                    text: buttons
                   anchors.centerIn: parent
                   font.pixelSize: rectangleList.pixelSize
                   font.pointSize: rectangleList.pointSize
                   font.bold: delegateCheckBox.checkState ? true : false
                    width: parent.textWidth
                }
            }

            Rectangle {
                width: (delegateRow.width-parent.height) / 10
                height: parent.height
                id: rectangleGroupNumber
                border.color: "black"
                color: delegateCheckBox.checkState ? defaultColor : "transparent"
                Text {
                    text: groupNumber
                    anchors.centerIn: parent
                    font.pixelSize: rectangleList.pixelSize
                    font.pointSize: rectangleList.pointSize
                    font.bold: delegateCheckBox.checkState ? true : false
                    width: parent.textWidth
                }
            }

            Rectangle {
                width: (delegateRow.width-parent.height) / 5
                height: parent.height
                id: rectangleIp
                border.color: "black"
                color: delegateCheckBox.checkState ? defaultColor : "transparent"
                Text {
                    text: ip
                    anchors.centerIn: parent
                    font.pixelSize: rectangleList.pixelSize
                    font.pointSize: rectangleList.pointSize
                    font.bold: delegateCheckBox.checkState ? true : false
                    width: parent.textWidth
                }
            }

            Rectangle {
                width: (delegateRow.width-parent.height) / 5
                height: parent.height
                id: rectangleMac
                border.color: "black"
                color: delegateCheckBox.checkState ? defaultColor : "transparent"
                Text {
                    text: mac
                    anchors.centerIn: parent
                    font.pixelSize: rectangleList.pixelSize
                    font.pointSize: rectangleList.pointSize
                    font.bold: delegateCheckBox.checkState ? true : false
                    width: parent.textWidth
                }
            }
        }
        background: Rectangle {
            anchors.fill: delegate
            color: highlighted ? "#c1e1e2" : "transparent"
        }
}
