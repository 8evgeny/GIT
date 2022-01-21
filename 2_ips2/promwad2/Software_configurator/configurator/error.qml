import QtQuick 2.0
import QtQuick.Controls 2.12

Item {
    id: messageError
    height: 600
    width: 900
    anchors.verticalCenter: parent.verticalCenter
    anchors.horizontalCenter: parent.horizontalCenter
        Rectangle{
                color: "#a1a1f1"
                anchors.centerIn: parent.Center
                anchors.fill: parent

                Text {
                    id: nameText
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: qsTr("Ошибка!")
                    color: "red"
                    font.pointSize: 40
                }
                Button {
                    id: cancelButton
                    height: parent.height / 12
                    width: parent.width / 5
                    x: parent.x + parent.width / 2 - parent.width / 10
                    y: parent.y + 7 *parent.height / 8
                    text: "Ок"
                    onClicked: messageError.visible = false
                }
        }
}
