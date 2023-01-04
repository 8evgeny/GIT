import QtQuick 2.12
import QtQuick.Controls 2.12

Rectangle {
    id: subsribersWindow
color: colorSubsribersWindow
    width: cellWidth * 4 - 50
    height: 742
    visible: false

    property color defaultColor: "#e1e1e2"
property color colorSubsribersWindow: "#F8FACF"
property color colorKeyProperty: "#F8FACF"
    property int defaultSizeOfHeight: 40
    property int defaultSizeOfSpace: 5
    property int cellWidth: 200
    property int cellWidth50: 50

    property string nameError: qsTr("Error")

    property string nameOfStation: ""

    property string ipOfStation: ""
    property int indexOfStation: -1

    property color ipBorderColor: "#333"
    property color maskBorderColor: "#333"
    property color gatewayBorderColor: "#333"
    property color keyPriorityBorderColor: "#333"
    property color keyModeBorderColor: "#333"

    property string nameDirectConnectionDuplex: qsTr("Direct connection: duplex")
    property string nameDirectConnectionSimplex: qsTr("Direct connection: simplex")
    property string nameGroupCommunication: qsTr("Group communication")
    property string nameCircularBond: qsTr("Circular bond")
    property string nameConferenceCall: qsTr("Conference call")
    property string nameTelephoneCommunications: qsTr("Telephone communications")
    property string nameHungUp: qsTr("Hung up")

    function getNameOfKeyFunction(str)
    {
        var nameStr = ""
        if (str === "1")
            nameStr = nameDirectConnectionDuplex

        if (str === "2")
            nameStr = nameGroupCommunication

        if (str === "3")
            nameStr = nameCircularBond

        if (str === "4")
            nameStr = nameConferenceCall

        if (str === "5")
            nameStr = nameTelephoneCommunications

        if (str === "6")
            nameStr = nameDirectConnectionSimplex

        if (str === "7")
            nameStr = nameHungUp

        return nameStr
    }

    function checkAssignment(str1, str2)
    {
        var colorStr = "#333"

        if(str1 === ""){
            colorStr = "red"
        }

        //can't be assignment
        if((getNameOfKeyFunctionNumber(str2) === "3")
                ||(getNameOfKeyFunctionNumber(str2) === "4")
                ||(getNameOfKeyFunctionNumber(str2) === "5")
                ||(getNameOfKeyFunctionNumber(str2) === "7")){
            colorStr = "#333"
        }
        else {
            colorStr = "red"
        }
        return colorStr
    }

    function getNameOfKeyFunctionNumber(str)
    {
        var nameStr = ""
        if (str === nameDirectConnectionDuplex)
            nameStr = "1"

        if (str === nameGroupCommunication)
            nameStr = "2"

        if (str === nameCircularBond)
            nameStr = "3"

        if (str === nameConferenceCall)
            nameStr = "4"

        if (str === nameTelephoneCommunications)
            nameStr = "5"

        if (str === nameDirectConnectionSimplex)
            nameStr = "6"

        if (str === nameHungUp)
            nameStr = "7"

        return nameStr
    }

    function resizeFlickableList()
    {
        flickableListOfSubscribers.contentHeight
                = listModelListOfSubscribers.count * (defaultSizeOfHeight - 1) + 1
    }

    Button //Клавиша +
    {
        id: buttonAddSubscriber
        height: defaultSizeOfHeight
        width: defaultSizeOfHeight
        text: "+"
        anchors.bottom: rectangleWithMargins.top
        anchors.bottomMargin: defaultSizeOfSpace
        onClicked: {
            //it is depricated
            //appCore.setListOfElements()
            //add a new key block
            appCore.addNewKeyBlock(nameOfStation)
        }
    }
    Button //Клавиша -
    {
        id: buttonDeleteSubscriber
        width: defaultSizeOfHeight
        height: defaultSizeOfHeight
        text: "-"
        anchors.bottom: rectangleWithMargins.top
        anchors.bottomMargin: defaultSizeOfSpace
        anchors.left: buttonAddSubscriber.right
        anchors.leftMargin: defaultSizeOfSpace
        onClicked: {
            if (listModelListOfSubscribers.count > 0) {
                if (listViewListOfSubscribers.currentIndex >= 0) {
                    var index = listViewListOfSubscribers.currentIndex
                    listModelListOfSubscribers.get(
                                listViewListOfSubscribers.currentIndex).borderSize = 1

                    appCore.deleteBlock(
                                nameOfStation, listModelListOfSubscribers.get(
                                    listViewListOfSubscribers.currentIndex).uid)

                    listModelListOfSubscribers.remove(
                                listViewListOfSubscribers.currentIndex)
                    resizeFlickableList()
                    index--
                    if (index < 0)
                        index = 0
                    listViewListOfSubscribers.currentIndex = index

                    if (listModelListOfSubscribers.count > 0)
                        listModelListOfSubscribers.get(index).borderSize = 2
                }
            }
        }
    }
    Rectangle // Шапка таблицы
    {
        id: rectangleWithMargins
        width: cellWidth * 4 + defaultSizeOfSpace
        height: defaultSizeOfHeight
        color: "#ffffff"
        anchors.bottom: flickableListOfSubscribers.top
        anchors.bottomMargin: -1

        Row {
            id: rowWithMargins
            width: parent.width
            height: parent.height

            Rectangle {
                id: rectangleKeyBlock
                width: cellWidth50 * 2
                height: parent.height
                TextField {
                    readOnly: true
                    width: parent.width
                    height: parent.height
                    text: qsTr("Key block")
                    horizontalAlignment: Text.AlignHCenter
                    id: textFieldKeyBlock
                    background: Rectangle {
                        border.color: "#333"
                        border.width: 1
                    }
                }
            }

            Rectangle {
                id: rectangleKeyName
                width: cellWidth
                height: parent.height
                color: "#ffffff"
                TextField {
                    width: parent.width
                    height: parent.height
                    text: qsTr("Key Name")
                    horizontalAlignment: Text.AlignHCenter
                    id: textFieldKeyName
                    readOnly: true
                    background: Rectangle {
                        border.color: "#333"
                        border.width: 1
                    }
                }
            }

            Rectangle {
                id: rectangleKeyFunction
                width: cellWidth + 2 * cellWidth50
                height: parent.height
                color: "#ffffff"
                border.color: "#c54848"
                TextField {
                    width: parent.width
                    height: parent.height
                    id: textFieldKeyFunction
                    text: qsTr("Key function")
                    horizontalAlignment: Text.AlignHCenter
                    readOnly: true
                    background: Rectangle {
                        border.color: "#333"
                        border.width: 1
                    }
                }
            }

            Rectangle {
                id: rectangleAppointment
                width: cellWidth50 * 3
                height: parent.height
                color: "#ffffff"
                TextField {
                    width: parent.width
                    height: parent.height
                    text: qsTr("Appointment")
                    horizontalAlignment: Text.AlignHCenter
                    id: textFieldAppointment
                    readOnly: true
                    background: Rectangle {
                        border.color: "#333"
                        border.width: 1
                    }
                }
            }
            spacing: -1
        }
    }

    Flickable
    {
        id: flickableListOfSubscribers
        width: parent.width + 5
        height: parent.height
        clip: true
//        ScrollBar.vertical: ScrollBar
        Rectangle //Основное поле абонентов
        {
            anchors.fill: parent
            ListView
            {
                id: listViewListOfSubscribers
                anchors.fill: parent
                interactive: false

                model: ListModel {
                    id: listModelListOfSubscribers
                }

                delegate: Item
                {
                    width: flickableListOfSubscribers.width
                    height: defaultSizeOfHeight

                    property string uidOfKey: ""

                    Row
                    {
                        id: rowListOfSubscribers
                        width: parent.width
                        height: parent.height

                        Rectangle //rectangleNewKeyboardUnit
                        {
                            id: rectangleNewKeyboardUnit
                            width: 2 * cellWidth50
                            height: parent.height
                            TextField
                            {
                                //                                    readOnly: true
                                width: parent.width
                                height: parent.height
                                text: number
                                horizontalAlignment: Text.AlignHCenter
                                id: textFieldNewKeyboardUnit
                                background: Rectangle {
                                    border.color: borderNumberColor
                                    border.width: borderSize
                                }

                                color: numberColor
                                onPressed:
                                {
                                    if (listViewListOfSubscribers.currentIndex >= 0) {
                                        listModelListOfSubscribers.get(
                                                    listViewListOfSubscribers.currentIndex).borderSize = 1
                                    }

                                    listViewListOfSubscribers.currentIndex = index
                                    listModelListOfSubscribers.get(
                                                index).borderSize = 2

                                    appCore.getSubscriberListPriorityOfKeys(
                                                nameOfStation,
                                                listModelListOfSubscribers.get(
                                                    listViewListOfSubscribers.currentIndex).uid)
                                }
                                onTextChanged: {
                                    borderNumberColor = "#333"
                                    numberColor = "#333"

                                    if (listViewListOfSubscribers.currentIndex >= 0) {
                                        appCore.updateKeySubscriberNumberOfKey(
                                                    nameOfStation,
                                                    listModelListOfSubscribers.get(
                                                        listViewListOfSubscribers.currentIndex).uid,
                                                    textFieldNewKeyboardUnit.text)
                                    }

                                    if (!appCore.keyVerification(
                                                nameOfStation,
                                                textFieldNewKeyboardUnit.text)) {
                                        borderNumberColor = "red"
                                        numberColor = "red"
                                    }
                                }
                                onEditingFinished: {
                                    focus = false

                                    var val = parseInt(
                                                textFieldNewKeyboardUnit.text)
                                    if (!isNaN(val)) {
                                        if (!appCore.keyVerification(
                                                    nameOfStation,
                                                    textFieldNewKeyboardUnit.text)) {
                                            borderNumberColor = "red"
                                            numberColor = "red"
                                        }
                                    } else {
                                        borderNumberColor = "red"
                                        numberColor = "red"
                                    }
                                }
                            }
                        }

                        Rectangle //rectangleNewKeyName
                        {
                            id: rectangleNewKeyName
                            width: cellWidth
                            height: parent.height
                            color: "#ffffff"
                            TextField {
                                width: parent.width
                                height: parent.height
                                horizontalAlignment: Text.AlignHCenter
                                id: textFieldNewKeyName
                                text: name
                                background: Rectangle {
                                    border.color: borderNameColor
                                    border.width: borderSize
                                }
                                color: nameColor
                                onPressed: {
                                    if (listViewListOfSubscribers.currentIndex >= 0) {
                                        listModelListOfSubscribers.get(
                                                    listViewListOfSubscribers.currentIndex).borderSize = 1
                                    }

                                    listViewListOfSubscribers.currentIndex = index
                                    listModelListOfSubscribers.get(
                                                index).borderSize = 2

                                    appCore.getSubscriberListPriorityOfKeys(
                                                nameOfStation,
                                                listModelListOfSubscribers.get(
                                                    listViewListOfSubscribers.currentIndex).uid)
                                }
                                onTextChanged: {
                                    borderNameColor = "#333"
                                    nameColor = "#333"

                                    if (listViewListOfSubscribers.currentIndex >= 0) {
                                        appCore.updateKeySubscriberNameOfKey(
                                                    nameOfStation,
                                                    listModelListOfSubscribers.get(
                                                        listViewListOfSubscribers.currentIndex).uid,
                                                    textFieldNewKeyName.text)
                                    }
                                }
                                onEditingFinished: {
                                    focus = false

                                    var val = textFieldNewKeyName.text
                                    if (val !== "") {

                                    } else {
                                        borderNameColor = "red"
                                        nameColor = "red"
                                    }
                                }
                            }
                        }

                        Rectangle //rectangleNewKeyFunction
                        {
                            id: rectangleNewKeyFunction
                            width: cellWidth + cellWidth50 * 2
                            height: parent.height
                            color: "#ffffff"
                            border.color: "#c54848"

                            TextField {
                                width: parent.width
                                height: parent.height
                                id: textFieldNewKeyFunction
                                text: functionName
                                background: Rectangle {
                                    border.color: borderFunctionNameColor
                                    border.width: borderSize
                                }

                                color: functionNameColor

                                onPressed: {
                                    if (listViewListOfSubscribers.currentIndex >= 0) {
                                        listModelListOfSubscribers.get(
                                                    listViewListOfSubscribers.currentIndex).borderSize = 1
                                    }

                                    listViewListOfSubscribers.currentIndex = index
                                    listModelListOfSubscribers.get(
                                                index).borderSize = 2

                                    appCore.getSubscriberListPriorityOfKeys(
                                                nameOfStation,
                                                listModelListOfSubscribers.get(
                                                    listViewListOfSubscribers.currentIndex).uid)
                                }
                                onTextChanged: {

                                    functionNameColor = "#333"
                                    borderFunctionNameColor = "#333"

                                    var nameNumber = getNameOfKeyFunctionNumber(textFieldNewKeyFunction.text)

                                    if (listViewListOfSubscribers.currentIndex >= 0) {
                                        appCore.updateKeySubscriberFunctionOfKey(
                                                    nameOfStation,
                                                    listModelListOfSubscribers.get(
                                                        listViewListOfSubscribers.currentIndex).uid,
                                                    nameNumber)

                                        if (!appCore.functionVerification(
                                                    nameNumber)) {
                                            functionNameColor = "red"
                                            borderFunctionNameColor = "red"
                                        }
                                    }

                                    //can't be assignment
                                    if((getNameOfKeyFunctionNumber(textFieldNewKeyFunction.text) === "4")
                                            ||(getNameOfKeyFunctionNumber(textFieldNewKeyFunction.text) === "3")
                                            ||(getNameOfKeyFunctionNumber(textFieldNewKeyFunction.text) === "5")
                                            ||(getNameOfKeyFunctionNumber(textFieldNewKeyFunction.text) === "7")){
                                        textFieldNewAppointment.text = ""
                                        buttonNewAppointment.visible = false
                                        textFieldNewAppointment.readOnly = true
                                        assignedNameColor = "#333"
                                        borderAssignedNameColor = "#333"
                                    }
                                    else{
                                        buttonNewAppointment.visible = true
                                        textFieldNewAppointment.readOnly = false
                                    }
                                }
                                onEditingFinished: {
                                    focus = false
                                    var nameNumber = getNameOfKeyFunctionNumber(textFieldNewKeyFunction.text)

                                    var val = textFieldNewKeyFunction.text
                                    if (val !== "") {
                                        if (!appCore.functionVerification(
                                                    nameNumber)) {
                                            functionNameColor = "red"
                                            borderFunctionNameColor = "red"
                                        }
                                    } else {
                                        functionNameColor = "red"
                                        borderFunctionNameColor = "red"
                                    }
                                }
                            }

                            Button //buttonNewKeyFunction
                            {
                                id: buttonNewKeyFunction
                                width: parent.height - 10
                                height: parent.height - 10
                                text: "..."
                                anchors.right: parent.right
                                anchors.rightMargin: 10
                                anchors.verticalCenter: parent.verticalCenter
                                onClicked: {

                                    if (listViewListOfSubscribers.currentIndex >= 0) {
                                        listModelListOfSubscribers.get(
                                                    listViewListOfSubscribers.currentIndex).borderSize = 1
                                    }

                                    var component = Qt.createComponent(
                                                "subscriber_window.qml")
                                    var windowSel = component.createObject(
                                                rectangleNewKeyFunction)
                                    windowSel.mainText = textFieldNewKeyFunction.text
                                    windowSel.mainNumber = index + 1
                                    listViewListOfSubscribers.currentIndex = index

                                    windowSel.show()
                                }
                            }
                        }//Rectangle //rectangleNewKeyFunction

                        Rectangle //rectangleNewAppointment
                        {
                            id: rectangleNewAppointment
                            width: 3 * cellWidth50
                            height: parent.height
                            color: "#ffffff"
                            TextField
                            {
                                width: parent.width
                                height: parent.height
                                id: textFieldNewAppointment
                                text: assignedName

                                background: Rectangle {
                                    border.color: borderAssignedNameColor
                                    border.width: borderSize
                                }

                                color: assignedNameColor

                                onPressed: {
                                    if (listViewListOfSubscribers.currentIndex >= 0) {
                                        listModelListOfSubscribers.get(
                                                    listViewListOfSubscribers.currentIndex).borderSize = 1
                                    }

                                    listViewListOfSubscribers.currentIndex = index
                                    listModelListOfSubscribers.get(
                                                index).borderSize = 2

                                    appCore.getSubscriberListPriorityOfKeys(
                                                nameOfStation,
                                                listModelListOfSubscribers.get(
                                                    listViewListOfSubscribers.currentIndex).uid)
                                }
                                onTextChanged:
                                {

                                    assignedNameColor = "#333"
                                    borderAssignedNameColor = "#333"

                                    if (listViewListOfSubscribers.currentIndex >= 0) {
                                        appCore.updateKeySubscriberAssignedOfKey(
                                                    nameOfStation,
                                                    listModelListOfSubscribers.get(
                                                        listViewListOfSubscribers.currentIndex).uid,

                                                    textFieldNewAppointment.text)
                                    }
                                    //can't be assignment
                                    if((getNameOfKeyFunctionNumber(textFieldNewKeyFunction.text) === "4")
                                            ||(getNameOfKeyFunctionNumber(textFieldNewKeyFunction.text) === "3")
                                            ||(getNameOfKeyFunctionNumber(textFieldNewKeyFunction.text) === "5")
                                            ||(getNameOfKeyFunctionNumber(textFieldNewKeyFunction.text) === "7")){
                                        assignedNameColor = "#333"
                                        borderAssignedNameColor = "#333"
                                    }
                                }
                                onEditingFinished: {
                                    focus = false

                                    var val = textFieldNewAppointment.text
                                    if (val !== "") {

                                    } else {
                                        assignedNameColor = "red"
                                        borderAssignedNameColor = "red"
                                    }

                                    //can't be assignment
                                    if((getNameOfKeyFunctionNumber(textFieldNewKeyFunction.text) === "4")
                                            ||(getNameOfKeyFunctionNumber(textFieldNewKeyFunction.text) === "3")
                                            ||(getNameOfKeyFunctionNumber(textFieldNewKeyFunction.text) === "5")
                                            ||(getNameOfKeyFunctionNumber(textFieldNewKeyFunction.text) === "7")){
                                        assignedNameColor = "#333"
                                        borderAssignedNameColor = "#333"
                                    }
                                }
                            }
                            Button //buttonNewAppointment
                            {
                                id: buttonNewAppointment
                                width: parent.height - 10
                                height: parent.height - 10
                                text: "..."
                                anchors.right: parent.right
                                anchors.rightMargin: 10
                                anchors.verticalCenter: parent.verticalCenter

                                onClicked:
                                {

                                    if (listViewListOfSubscribers.currentIndex >= 0) {
                                        listModelListOfSubscribers.get(
                                                    listViewListOfSubscribers.currentIndex).borderSize = 1
                                    }

                                    var component = Qt.createComponent(
                                                "assignment_list.qml")
                                    var windowSel = component.createObject(
                                                rectangleNewAppointment)
                                    listViewListOfSubscribers.currentIndex = index

                                    windowSel.show()
                                }
                            }//Button //buttonNewAppointment
                        }    //Rectangle //rectangleNewAppointment
                    spacing: -1
                    }       //Row
                }           //delegate: Item
                    spacing: -1
            }               //ListView
        }                   //Rectangle
    }                       //Flickable

    Connections //Отработка сигналов
    {
        target: appCore
        onSendListNameOfElements:
//                                     void sendListNameOfElements(QString uidOfKeyAdd,
//                                                                 QString numberOfKeyAdd,
//                                                                 QString nameOfKeyAdd,
//                                                                 QString functionOfKeyAdd,
//                                                                 QString assignedKeyAdd);
        {
            var nameStr = getNameOfKeyFunction(functionOfKeyAdd)

            listModelListOfSubscribers.append //Обновляется одна строка таблицы
                    ({
                    "uid": uidOfKeyAdd,
                    "borderSize": 1,
                    "number": numberOfKeyAdd,
                    "numberColor": appCore.keyVerification(nameOfStation, numberOfKeyAdd) ? "#333" : "red",
                    "borderNumberColor": appCore.keyVerification(nameOfStation, numberOfKeyAdd) ? "#333" : "red",
                    "name": nameOfKeyAdd,
                    "nameColor": nameOfKeyAdd != "" ? "#333" : "red",
                    "borderNameColor": nameOfKeyAdd != "" ? "#333" : "red",
                    "functionName": nameStr,
                    "functionNameColor": nameStr !== "" ? "#333" : "red",
                    "borderFunctionNameColor": nameStr !== "" ? "#333" : "red",
                    "assignedName": assignedKeyAdd,
                    "assignedNameColor": checkAssignment(assignedKeyAdd, nameStr),
                     //!= "" ? "#333" : "red",
                    "borderAssignedNameColor": checkAssignment(assignedKeyAdd, nameStr)//assignedKeyAdd != "" ? "#333" : "red"
                    })

            resizeFlickableList()
        }

        onSendVisibleSubsriber:  //Это сигнал из C++ кода
//                                 void sendVisibleSubsriber(bool stateOnVisible)
        {
            subsribersWindow.visible = stateOnVisible
            listModelListOfSubscribers.clear()

            listViewListOfSubscribers.currentIndex = -1
        }
        onClearVisibleSubsriber:  //Это сигнал из C++ кода
        {
            subsribersWindow.visible = false
            listModelListOfSubscribers.clear()
            listViewListOfSubscribers.currentIndex = -1
        }
        onSendNameOfStation: //Это сигнал из C++ кода   void sendNameOfStation(
//                                 qint32 indexOfStationFromList,
//                                 QString nameOfStationList);
        {
            nameOfStation = nameOfStationList
            indexOfStation = indexOfStationFromList
        }
        onSendUpdateListOfStationResize: //Это сигнал из C++ кода
        {
            resizeFlickableList()
        }

//        onNextIp:
//        {
//            textFieldIPStation.text = ipNext
//        }

        onSendInfoNetworkAboutTheStation: //Это сигнал из C++ кода
//                          void sendInfoNetworkAboutTheStation(
//                                            QString nameOfStationMain,
//                                            QString nameOfId,
//                                            QString nameIP,
//                                            QString nameMaskNetwork,
//                                            QString nameGateway);
        {
            textFieldNameStation.text = nameOfStationMain
            textFieldNumberStation.text = nameOfId
            //check ip
            textFieldIPStation.text = nameIP
            if (!appCore.ipVerification(nameIP)) {
                textFieldIPStation.color = "red"
                ipBorderColor = "red"
            } else {
                textFieldIPStation.color = "#333"
                ipBorderColor = "#333"
            }

            //check mask
            textFieldMaskNetwork.text = nameMaskNetwork
            if (!appCore.ipVerification(nameMaskNetwork)) {
                textFieldMaskNetwork.color = "red"
                maskBorderColor = "red"
            } else {
                textFieldMaskNetwork.color = "#333"
                maskBorderColor = "#333"
            }

            //check gateway
            textFieldGateway.text = nameGateway
            if (!appCore.ipVerification(nameGateway)) {
                textFieldGateway.color = "red"
                gatewayBorderColor = "red"
            } else {
                textFieldGateway.color = "#333"
                gatewayBorderColor = "#333"
            }
        }
        onSendInfoPrioriryKeysAboutTheStation: //Это сигнал из C++ кода
//            void sendInfoPrioriryKeysAboutTheStation(
//                QString modeKeyStation,
//                QString prioriryKeyStation);
        {
            textFieldKeyPriority.text = prioriryKeyStation

            if (textFieldKeyPriority.text == "") {
                keyPriorityBorderColor = "red"
            } else {
                keyPriorityBorderColor = "#333"
            }

            if (modeKeyStation == "1") {
                comboBoxModeOfKey.currentIndex = 0
                keyModeBorderColor = "#333"
            } else if (modeKeyStation == "2") {
                comboBoxModeOfKey.currentIndex = 1
                keyModeBorderColor = "#333"
            } else {
                comboBoxModeOfKey.currentIndex = -1
                keyModeBorderColor = "red"
            }
        }
        onSendToQmlSubscriber: //Это сигнал из C++ кода
//            void sendToQmlSubscriber(QString str,
//                                     qint32 number);
        {
            var nameStr = getNameOfKeyFunction(str)
            listModelListOfSubscribers.get(
                        listViewListOfSubscribers.currentIndex).functionName = nameStr
        }
        onSendToQmlSubscriberAssignment: //Это сигнал из C++ кода
        {
            listModelListOfSubscribers.get(
                        listViewListOfSubscribers.currentIndex).assignedName = strAssignmentName
        }
    }

    Rectangle //Текст Station Number:
    {
        id: rectangleNumberStation
        width: 200
        height: 40
        color: "#ffffff"
        anchors.top: rectangleStationInformation.bottom
        anchors.topMargin: 5
        anchors.left: flickableListOfSubscribers.right
        anchors.leftMargin: 5

        Label {
            id: labelNumberStation
            text: qsTr("Station Number:")
            verticalAlignment: Text.AlignVCenter
            anchors.fill: parent
        }
    }
    Rectangle //Текст Station Information
    {
        id: rectangleStationInformation
        width: 405
        height: defaultSizeOfHeight
        color: "#ffffff"
        anchors.verticalCenter: rectangleWithMargins.verticalCenter
        anchors.left: flickableListOfSubscribers.right
        anchors.leftMargin: defaultSizeOfSpace

        Label {
            id: labelStationInformation
            text: qsTr("Station Information")
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.fill: parent
            font.bold: true
            color: "red"
            font.pointSize: 12
        }
    }
    Rectangle //Текст Station Name:
    {
        id: rectangleNameStation
        width: 200
        height: defaultSizeOfHeight
        color: "#ffffff"
        anchors.left: flickableListOfSubscribers.right
        anchors.topMargin: defaultSizeOfSpace
        anchors.top: textFieldNumberStation.bottom
        Label {
            id: labelNameStation
            text: qsTr("Station Name:")
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
        }
        anchors.leftMargin: defaultSizeOfSpace
    }
    Rectangle //Текст IP address:
    {
        id: rectangleIPStation
        width: 200
        height: defaultSizeOfHeight
        color: "#ffffff"
        anchors.left: flickableListOfSubscribers.right
        anchors.topMargin: defaultSizeOfSpace
        anchors.top: rectangleNameStation.bottom
        Label {
            id: labelIPStation
            text: qsTr("IP address:")
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
        }
        anchors.leftMargin: defaultSizeOfSpace
    }
    Rectangle //Текст Network mask:
    {
        id: rectangleMaskNetwork
        width: 200
        height: defaultSizeOfHeight
        color: "#ffffff"
        anchors.left: flickableListOfSubscribers.right
        anchors.topMargin: defaultSizeOfSpace
        anchors.top: rectangleIPStation.bottom
        Label {
            id: labelMaskNetwork
            text: qsTr("Network mask:")
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
        }
        anchors.leftMargin: defaultSizeOfSpace
    }
    Rectangle //Текст Gateway:
    {
        id: rectangleGateway
        width: 200
        height: defaultSizeOfHeight
        color: "#ffffff"
        anchors.left: flickableListOfSubscribers.right
        anchors.topMargin: defaultSizeOfSpace
        anchors.top: rectangleMaskNetwork.bottom
        Label {
            id: labelGateway
            text: qsTr("Gateway:")
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
        }
        anchors.leftMargin: defaultSizeOfSpace
    }

    TextField //Номер станции
    {
        width: cellWidth
//        readOnly: true
        height: defaultSizeOfHeight
        id: textFieldNumberStation
        text: ""
        font.bold: true
        font.pointSize: 12
//        color: "grey"

        anchors.top: rectangleStationInformation.bottom
        anchors.topMargin: defaultSizeOfSpace
        anchors.left: rectangleNumberStation.right
        anchors.leftMargin: defaultSizeOfSpace
        onTextChanged: {
            appCore.sendPositionInListOfStation()
        }
        onEditingFinished:
        {

            focus = false
            appCore.updateStationId(nameOfStation, textFieldNumberStation.text)
            appCore.updateAllViewsWithPostion()
        }
        background: Rectangle
        {
            border.color: "#333"
            border.width: 1
        }
    }
    TextField //Имя станции
    {
        property string startEdit: ""
//        readOnly: true
        id: textFieldNameStation
        text: ""
        font.bold: true
        font.pointSize: 12

        anchors.left: rectangleNameStation.right
        anchors.topMargin: defaultSizeOfSpace
        anchors.top: textFieldNumberStation.bottom
        anchors.leftMargin: defaultSizeOfSpace


        width: cellWidth
        height: defaultSizeOfHeight

        onTextChanged: {
            appCore.sendPositionInListOfStation()
            appCore.updateStationName(nameOfStation, textFieldNameStation.text)
            appCore.updateAllViewsWithPostion()
        }
        onEditingFinished:
        {
            focus = false
        }
        background: Rectangle
        {
            border.color: "#333"
            border.width: 1
        }
    }
    TextField //IP станции
    {
        id: textFieldIPStation
        text: ipOfStation
        font.bold: true
        font.pointSize: 12
        anchors.left: rectangleIPStation.right
        anchors.topMargin: defaultSizeOfSpace
        anchors.top: textFieldNameStation.bottom
        anchors.leftMargin: defaultSizeOfSpace

        width: cellWidth
        height: defaultSizeOfHeight

        background: Rectangle {
            border.color: ipBorderColor
            border.width: 1
        }

        //IP Address Validation
        validator: RegExpValidator {
            regExp: /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/
        }

        onTextChanged: {
            ipBorderColor = color = "#333"
            appCore.updateStationIp(nameOfStation, textFieldIPStation.text)
        }

        onEditingFinished: {
            appCore.saveIP(textFieldIPStation.text)
            focus = false
        }
    }
    TextField //Маска станции
    {
        id: textFieldMaskNetwork
        text: ""
        font.bold: true
        font.pointSize: 12
        anchors.left: rectangleMaskNetwork.right
        anchors.topMargin: defaultSizeOfSpace
        anchors.top: textFieldIPStation.bottom
        anchors.leftMargin: defaultSizeOfSpace

        width: cellWidth
        height: defaultSizeOfHeight

        background: Rectangle {
            border.color: maskBorderColor
            border.width: 1
        }

        //Mask Address Validation
        validator: RegExpValidator {
            regExp: /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/
        }

        onTextChanged: {
            maskBorderColor = color = "#333"
            appCore.updateStationMaskNetwork(nameOfStation,
                                             textFieldMaskNetwork.text)
        }
        onEditingFinished: {
            focus = false
        }
    }
    TextField //Gateway станции
    {
        id: textFieldGateway
        text: ""
        font.bold: true
        font.pointSize: 12
        anchors.left: rectangleGateway.right
        anchors.topMargin: defaultSizeOfSpace
        anchors.top: textFieldMaskNetwork.bottom
        anchors.leftMargin: defaultSizeOfSpace

        width: cellWidth
        height: defaultSizeOfHeight

        background: Rectangle {
            border.color: gatewayBorderColor
            border.width: 1
        }

        //Gateway Address Validation
        validator: RegExpValidator {
            regExp: /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/
        }

        onTextChanged: {
            gatewayBorderColor = color = "#333"
            appCore.updateStationGateway(nameOfStation, textFieldGateway.text)
        }

        onEditingFinished: {
            focus = false
        }
    }

    Rectangle //Area for property of keys
    {
        id: rectangleKeyProperty
        width: 400
        height: defaultSizeOfHeight * 2
        color: colorKeyProperty
        anchors.left: rectangleWithMargins.right
        anchors.leftMargin: defaultSizeOfSpace
        anchors.top: textFieldGateway.bottom
        anchors.topMargin: 50
        visible: listViewListOfSubscribers.currentIndex >= 0 ? true : false

        Rectangle  //Text Key Mode:
        {
            id: rectKeyMode
            color: colorKeyProperty
//            anchors.top: rectKeyProperty.bottom
            anchors.topMargin: defaultSizeOfSpace
            width: cellWidth
            height: defaultSizeOfHeight
            Label {
                verticalAlignment: Text.AlignVCenter
                anchors.fill: parent
                id: labelKeyMode
                text: qsTr("Key Mode:")
            }
        }
        Rectangle //Text Key Priority:
        {
            id: rectKeyPriority
            color: colorKeyProperty
            anchors.top: rectKeyMode.bottom
            anchors.topMargin: defaultSizeOfSpace
            width: cellWidth
            height: defaultSizeOfHeight
            Label {
                verticalAlignment: Text.AlignVCenter
                anchors.fill: parent
                id: labelKeyPriority
                text: qsTr("Key Priority:")
            }
        }
        ComboBox //Выбор режима клавиши без/c фиксацией
        {
            width: cellWidth
            id: comboBoxModeOfKey
            anchors.verticalCenter: rectKeyMode.verticalCenter
            anchors.left: rectKeyMode.right
            anchors.leftMargin: defaultSizeOfSpace
            currentIndex: -1

            height: defaultSizeOfHeight
            font.bold: true
            font.pointSize: 12
            background: Rectangle {
                border.color: keyModeBorderColor
                border.width: 1
            }

            model: ListModel {
                id: cbItemsModeOfKey
                ListElement {
                    text: qsTr("fixation")

                }
                ListElement {
                    text: qsTr("no fixation")

                }
            }

            onCurrentIndexChanged: {
                if (currentIndex == 0) {
                    if (listViewListOfSubscribers.currentIndex >= 0) {
                        appCore.updateKeySubscriberModeOfKey(
                                    nameOfStation,
                                    listModelListOfSubscribers.get(
                                        listViewListOfSubscribers.currentIndex).uid,
                                    "1")
                        keyModeBorderColor = "#333"
                    }
                }
                if (currentIndex == 1) {
                    if (listViewListOfSubscribers.currentIndex >= 0) {
                        appCore.updateKeySubscriberModeOfKey(
                                    nameOfStation,
                                    listModelListOfSubscribers.get(
                                        listViewListOfSubscribers.currentIndex).uid,
                                    "2")
                        keyModeBorderColor = "#333"
                    }
                }
            }
        }
        TextField //Выбор приоритета клавиши
        {
            width: cellWidth
            id: textFieldKeyPriority
            text: ""
            font.bold: true
            font.pointSize: 12
            anchors.left: rectKeyPriority.right
            anchors.leftMargin: defaultSizeOfSpace
            anchors.verticalCenter: rectKeyPriority.verticalCenter
            height: defaultSizeOfHeight

            background: Rectangle {
                border.color: keyPriorityBorderColor
                border.width: 1
            }

            onEditingFinished: {
                focus = false
            }

            onTextChanged: {

                keyPriorityBorderColor = color = "#333"

                if (listViewListOfSubscribers.currentIndex >= 0) {
                    appCore.updateKeySubscriberPriorityOfKey(
                                nameOfStation, listModelListOfSubscribers.get(
                                    listViewListOfSubscribers.currentIndex).uid,
                                textFieldKeyPriority.text)
                }
            }
        }
    }
}
