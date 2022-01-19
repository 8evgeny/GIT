import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2

ApplicationWindow {
    id: verificationId

    width: 400
    height: 400
    visible: true

    title: qsTr("Verification")

    property string okGeneric: qsTr("The project does not contain errors")
    property string wrongGeneric: qsTr("Something went wrong")
    property string wrongNameId: qsTr("name identifier for stations")
    property string wrongNameInGroupsId: qsTr("name identifier for groups")
    property string wrongName: qsTr("name for stations")
    property string wrongTypeAssign: qsTr("type of assignment")
    property string wrongIdKeyNotValid: qsTr("the key id is invalid")
    property string wrongIp: qsTr("IP is not valid")
    property string wrongMask: qsTr("mask is not valid")
    property string wrongGateway: qsTr("gateway is not valid")
    property string wrongNumberKeyNotValid: qsTr("this key number is not valid")
    property string wrongFunctionKeys: qsTr("function for keys")
    property string wrongCidId: qsTr("CID ID for stations")
    property string wrongMode: qsTr("a mode of this key is not valid")
    property string wrongPriority: qsTr("a priority of this key is not valid")

    ScrollView {
        id: verificationScrollView
        anchors.fill: parent

        TextArea {
            id: verificationTextArea
        }
    }

    Connections {
        target: appCore

        onSendWrongGeneric: {
            verificationTextArea.append(wrongGeneric)
        }

        onSendWrongNameId: {
            verificationTextArea.append(nameOfStation + " " + wrongNameId)
        }

        onSendWrongName: {
            verificationTextArea.append(nameOfStation + " " + wrongName)
        }

        onSendWrongTypeAssign: {
            verificationTextArea.append(nameOfStation + " " + wrongTypeAssign)
        }

        onSendWrongIdKeyNotValid: {
            verificationTextArea.append(
                        nameOfStation + " " + wrongIdKeyNotValid)
        }

        onSendWrongIp: {
            verificationTextArea.append(nameOfStation + " " + wrongIp)
        }

        onSendWrongMask: {
            verificationTextArea.append(nameOfStation + " " + wrongMask)
        }

        onSendWrongGateway: {
            verificationTextArea.append(nameOfStation + " " + wrongGateway)
        }

        onSendWrongNumberKeyNotValid: {
            verificationTextArea.append(
                        nameOfStation + " " + wrongNumberKeyNotValid)
        }

        onSendWrongFunctionKeys: {
            verificationTextArea.append(nameOfStation + " " + wrongFunctionKeys)
        }

        onSendWrongCidId: {
            verificationTextArea.append(nameOfStation + " " + wrongCidId)
        }

        onSendWrongMode: {
            verificationTextArea.append(nameOfStation + " " + wrongMode)
        }

        onSendWrongNameInGroupsId: {
            verificationTextArea.append(nameOfGroup + " " + wrongNameInGroupsId)
        }

        onSendWrongPriority: {
            verificationTextArea.append(nameOfStation + " " + wrongPriority)
        }

        onSendOkGeneric: {
            verificationTextArea.append(okGeneric)
        }
    }
}
