import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2

ApplicationWindow {
    id: networkId

    width: 300
    height: 300
    visible: true

    property string defaultTitle: qsTr("Network")
    property string currentCardNetwork: ""
    title: defaultTitle

    ButtonGroup {
        id: buttonGroupNetwork
        buttons: columnNetwork.children
        onClicked: {
            console.log("clicked:", button.text)
            currentCardNetwork = button.text
            networkId.title = defaultTitle + " " + button.text
        }
    }

    ScrollView {
        id: scrollViewNetwork
        anchors.bottomMargin: 60
        anchors.fill: parent

        Column {
            id: columnNetwork
            anchors.fill: parent
        }
    }

    Connections {
        target: receiver
    }

    Connections {
        target: appCore

        onAddNetworkListGroups: {
            var button = Qt.createQmlObject(
                        "import QtQuick 2.12; import QtQuick.Controls 2.12; RadioButton { text: '"
                        + nameOfNetworkList + "' }", columnNetwork)
        }

        onAddNetworkListGroupsOn: {
            var button = Qt.createQmlObject(
                        "import QtQuick 2.12; import QtQuick.Controls 2.12; RadioButton { checked: true; text: '" + nameOfNetworkList + "' }",
                        columnNetwork)
        }
    }

    Button {
        id: buttonOk
        text: qsTr("Ok")
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 5
        onClicked: {
            appCore.saveCurrentNetworkCard(currentCardNetwork)
            receiver.setCard(currentCardNetwork)
            networkId.close()
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
            networkId.title = defaultTitle
            currentCardNetwork =  ""
            buttonGroupNetwork.checkState = Qt.Unchecked
        }
    }

//    function setCoordinates() {
//        x += Screen.width / 2 - width / 2
//        y += Screen.height / 2 - height / 2
//    }

    Component.onCompleted: {
//        setCoordinates()
    }
}

/*##^##
Designer {
    D{i:6;anchors_x:8}
}
##^##*/
