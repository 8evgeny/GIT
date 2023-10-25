import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 1.4
import Qt.labs.folderlistmodel 1.0

Window {
    visible: true
    width: 800
    height: 600
    title: qsTr("QML PatchView delegate Demo")

    Rectangle {
        width: 800
        height: 600

        FolderListModel {
            id: dataModel

            showDirs: false
            nameFilters: [
                "*.PDF"
            ]
            folder: "file:///home/evg/SOFT/Github/GIT/18_create_soft_links/Ниокр-Документы_по_обозначениям"
        }

        TableView {
            id: view

            anchors.margins: 10
            anchors.fill: parent
            model: dataModel
            clip: true

            TableViewColumn {
                width: 400
                title: "Name"
                role: "fileName"
            }
            TableViewColumn {
                width: 100
                title: "Size"
                role: "fileSize"
            }
            TableViewColumn {
                width: 100
                title: "Modified"
                role: "fileModified"
            }

            itemDelegate: Item {
                Text {
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    renderType: Text.NativeRendering
                    text: styleData.value
                }
            }
        }
    }
}
