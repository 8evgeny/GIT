import QtQuick 2.0
import QtQuick.Controls 2.12

Item {
    id: messageProgram
    height: 600
    width: 900
    anchors.verticalCenter: parent.verticalCenter
    anchors.horizontalCenter: parent.horizontalCenter
        Rectangle{
                color: "#e1e1e1"
                anchors.centerIn: parent.Center
                anchors.fill: parent

                Text {
                    id: nameText
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: qsTr("Будет произведена перезапись конфигурации на всех устройствах!")
                    font.pointSize: 40
                }


                    Button {
                     //   x: parent.height
                        id: writeButton
                        height: parent.height / 12
                        width: parent.width / 5
                        x: parent.x + parent.width / 2 - parent.width / 4 - parent.width / 10
                        y: parent.y + 7 *parent.height / 8

                        text: "Записать"
                        onClicked: {
                            messageProgram.visible = false
                            appCore.writeProject();
                            var component = Qt.createComponent("done.qml")
                            var window    = component.createObject(rectangleList.parent)
                            window.show()

                        }
                    }
                    Button {
                        id: cancelButton
                        height: parent.height / 12
                        width: parent.width / 5
                        x: parent.x + parent.width / 2 + parent.width / 4 - parent.width / 10
                        y: parent.y + 7 *parent.height / 8
                        text: "Отмена"
                        onClicked: messageProgram.visible = false
                    }

        }
}
