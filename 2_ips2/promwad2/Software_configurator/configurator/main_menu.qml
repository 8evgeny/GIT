import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2
import RuLang 1.0

import "update_rs232.js" as UpdateRs232
import "network.js" as UpdateNetwork
import "programmer_window.js" as UpdateProgrammerWindow

MenuBar {
    id: menuBar
    contentHeight: 40
    contentWidth: parent.width

    background: Rectangle {
        border.color: "#000000"
        color: "#e1e1e2"
    }

    //property bool closeWindow: false

    property bool rs232State: false
    property bool networkState: false
    property bool programmerWindowState: false

    property int langCurrentStatte: 0
    property string softwareVersionConfiguration: "1.0"

    property string newPrj: qsTr("New project")
    property string openPrj: qsTr("Load project")
    property string savePrj: qsTr("Save project")
    property string saveAsPrj: qsTr("Save project as")
    property string closePrj: qsTr("Close project")
    property string recentPrj: qsTr("Recent projects")
    property string network: qsTr("Network")
    property string exit: qsTr("Exit")
    property string rs232: qsTr("RS232")
    property string firmware: qsTr("Firmware")
    property string checkPrj: qsTr("Check project")
    property string createPrj: qsTr("Generate project")
    property string checkDevice: qsTr("Check device")
    property string readConf: qsTr("Read configuration")
    property string writeConf: qsTr("Write configuration")
    property string delConf: qsTr("Delete configuration")
    property string writePrj: qsTr("Record project")
    property string rusMenu: qsTr("Russian")
    property string engMenu: qsTr("English")
    property string programmer: qsTr("Programmer")

    property string ctrlN: "Ctrl+N"
    property string ctrlO: "Ctrl+O"
    property string ctrlS: "Ctrl+S"
    property string ctrlA: "Ctrl+A"
    property string ctrlC: "Ctrl+C"
    property string altF4: "Alt+F4"

    property string f3: "F3"
    property string f4: "F4"
    property string f5: "F5"
    property string f6: "F6"
    property string f7: "F7"
    property string f8: "F8"
    property string f9: "F9"

    property string link: '<html><style type="text/css"></style><a href="https://www.git-holding.ru/">www.git-holding.ru</a></html>'
    property string companyName: qsTr("GIT Group Industrial Technology")
    property string progName: qsTr("Software \"Configurator\" for digital system\nmultifunctional dispatch communication\nand alerts GIT-Comm IPS")
    property string phone: qsTr("Phone: +7 (495) 223-0725")

   function resizeMenuFiles(){
        var widthFiles = 0
        var heightFiles = 0

        if (onepRecent1Action.text != ""){
               onepRecent1Action.visible = true
               heightFiles += 40
               widthFiles = 400
        }
        if (onepRecent2Action.text != ""){
               onepRecent2Action.visible = true
               heightFiles += 40
               widthFiles = 400
        }
        if (onepRecent3Action.text != ""){
               onepRecent3Action.visible = true
               heightFiles += 40
               widthFiles = 400
        }

        menuFiles.width = widthFiles
        menuFiles.height = heightFiles
   }

    //Get the path from a QML url
    function urlToPath(urlString) {
        var s
        if (urlString.startsWith("file:///")) {
            var k = urlString.charAt(9) === ':' ? 8 : 7
            s = urlString.substring(k)
        } else {
            s = urlString
        }
        return decodeURIComponent(s)
    }

    function functChooseLogo(){
        if(langCurrentStatte == 0){
            return "icons/GIT_en.png"
        }
        else {
            return "icons/git-logo.png"
        }
    }

    Menu {

        title: qsTr("&File")

        background: Rectangle {
            implicitWidth: 300
            implicitHeight: 40
            border.color: "#000000"
        }

        Action {
            id: newAction
            text: "%2\t%1".arg(newPrj).arg(ctrlN)
            shortcut: ctrlN
            onTriggered: {
                appCore.resetCurrentStateForProject()
            }
        }

        Action {
            id: openAction
            text: "%2\t%1".arg(openPrj).arg(ctrlO)
            shortcut: ctrlO
            onTriggered: {
                loadFileDialog.open()
            }
        }

        Action {
            id: saveAction
            text: "%2\t%1".arg(savePrj).arg(ctrlS)
            shortcut: ctrlS
            onTriggered: {
                appCore.saveListOfStationAndGroupsQJson()
            }
        }

        FileDialog {
            id: saveFileDialog
            title: qsTr("Save configuration")
            selectExisting: false
            folder: shortcuts.home
            onAccepted: {
                console.log("Accepted: " + fileUrl)
                appCore.saveListOfStationAndGroupsQJson(urlToPath(fileUrl))
            }
        }

        FileDialog {
            id: loadFileDialog
            title: qsTr("Load configuration")
            folder: shortcuts.home
            onAccepted: {
                console.log("Accepted: " + fileUrl)
                appCore.loadListOfStationAndGroupsQJson(urlToPath(fileUrl))
            }
        }

        Action {
            id: saveAsAction
            text: "%2\t%1".arg(saveAsPrj).arg(ctrlA)
            shortcut: ctrlA
            onTriggered: {
                saveFileDialog.open()
            }
        }

//        Action {
//            id: closeAction
//            text: "%2\t%1".arg(closePrj).arg(ctrlC)

//            shortcut: ctrlC
//            onTriggered: {
//                //close project
//                appCore.saveListOfStationAndGroupsQJson()
//                appCore.resetCurrentStateForProject()
//            }
//        }

        Menu {
            id: menuFiles
            title: recentPrj
            height: 0
            width: 0
            MenuItem {
                id: onepRecent1Action
                text: ""
                onTriggered: {
                    if (text != "") {
                        appCore.loadListOfStationAndGroupsQJson(text)
                    }
                }
            }
            MenuItem {
                id: onepRecent2Action
                visible: false
                text: ""
                onTriggered: {
                    if (text != "") {
                        appCore.loadListOfStationAndGroupsQJson(text)
                    }
                }
            }
            MenuItem {
                id: onepRecent3Action
                visible: false
                text: ""
                onTriggered: {
                    if (text != "") {
                        appCore.loadListOfStationAndGroupsQJson(text)
                    }
                }
            }
        }

        Action {
            id: networkAction
            text: network
            onTriggered: {
                if (networkState == false) {
                    UpdateNetwork.createNetworkObject(menuBar)
                    networkState = true
                } else {
                    UpdateNetwork.deleteNetworkObject()
                    UpdateNetwork.createNetworkObject(menuBar)
                }
                appCore.getNetworkList()
            }
        }

        Action {
            id: exitAction
            text: "%2\t%1".arg(exit).arg(altF4)
            shortcut: altF4
            onTriggered: {
                 appCore.exitActionFromMenu()
            }
        }
    }

    Menu {
        title: qsTr("&Instruments")

        background: Rectangle {
            implicitWidth: 360
            implicitHeight: 40
            border.color: "black"
        }

        Action {
            id: checkAction
            text: "%2\t%1".arg(checkPrj).arg(f3)
            shortcut: f3
            onTriggered: {
                var component = Qt.createComponent("verification.qml")
                var object = component.createObject(menuBar)
                appCore.startVerificationStations()
            }
        }

//        Action {
//            id: cteateAction
//            text: "%2\t%1".arg(createPrj).arg(f4)
//            shortcut: f4
//            onTriggered: {
//            }
//        }

        Action {
            id: checkDeviceAction
            text: "%2\t%1".arg(checkDevice).arg(f5)
            shortcut: f5
            onTriggered: {                
                var component = Qt.createComponent("verification.qml")
                var object = component.createObject(menuBar)
                appCore.startVerificationStation()
            }
        }

//        Action {
//            id: readConfigAction
//            text: "%2\t%1".arg(readConf).arg(f6)
//            shortcut: f6
//            onTriggered: {

//            }
//        }

//        Action {
//            id: writeConfigAction
//            text: "%2\t%1".arg(writeConf).arg(f7)
//            shortcut: f7
//            onTriggered: {
//                appCore.requestSendJson()
//            }
//        }

//        Action {
//            id: deleteConfigAction
//            text: "%2\t%1".arg(delConf).arg(f8)
//            shortcut: f8
//            onTriggered: {

//            }
//        }

//        Action {
//            id: writeProjAction
//            text: "%2\t%1".arg(writePrj).arg(f9)
//            shortcut: f9
//            onTriggered: {
//            }
//        }

        Action {
            id: rs232Action
            text: "%1".arg(rs232)
            onTriggered: {
                if (rs232State == false) {
                    UpdateRs232.createUpdateRs232Object(menuBar)
                    rs232State = true
                } else {
                    UpdateRs232.deleteUpdateRs232Object()
                    UpdateRs232.createUpdateRs232Object(menuBar)
                }
            }
        }

        Action {
            id: firmwareAction
            text: "%1".arg(firmware)
            onTriggered: {
                var component = Qt.createComponent("firmware.qml")
                var object = component.createObject(menuBar)
            }
        }

        Action {
            id: programmerAction
            text: "%1".arg(programmer)
            onTriggered: {
                if (programmerWindowState == false) {
                    UpdateProgrammerWindow.createProgrammerWindowObject(menuBar)
                    programmerWindowState = true
                } else {
                    UpdateProgrammerWindow.deleteProgrammerWindowObject()
                    UpdateProgrammerWindow.createProgrammerWindowObject(menuBar)
                }
            }
        }
    }

    Connections {
        target: appCore
        onHistoryOfFiles: {
            onepRecent1Action.text = file1
            onepRecent2Action.text = file2
            onepRecent3Action.text = file3
            resizeMenuFiles()
        }

       onNeedSelectNetworkCard: {
           if (networkState == false) {
               UpdateNetwork.createNetworkObject(menuBar)
               networkState = true
           } else {
               UpdateNetwork.deleteNetworkObject()
               UpdateNetwork.createNetworkObject(menuBar)
           }
           appCore.getNetworkList()
       }
    }

    Connections {
        target: rusTrans
    }

    Component.onCompleted: {
        appCore.updateInfoAboutListFiles()
        resizeMenuFiles()
    }

    Menu {

        title: qsTr("&Languages")
        Action {
            id: rusId
            text: "%1".arg(rusMenu)
            onTriggered: {
                rusTrans.updateLanguage(RuLang.RUS)
                appCore.setCurrentLanguage(RuLang.RUS)
                langCurrentStatte = 1
            }
        }

        Action {
            id: engId
            text: "%1".arg(engMenu)
            onTriggered: {
                rusTrans.updateLanguage(RuLang.ENG)
                appCore.setCurrentLanguage(RuLang.ENG)
                langCurrentStatte = 0
            }
        }
    }

    Menu {
        title: qsTr("&About the program")

        background: Rectangle {
            id: place
            implicitWidth: if(langCurrentStatte == 0){
                               900
                           }
                           else{
                               580
                           }
            implicitHeight: 140
            border.color: "black"

            Image {
                id: logo
                width: if(langCurrentStatte == 0){
                           600
                       }
                       else{
                           220
                       }
                height: 128
                fillMode: Image.Stretch
                anchors.left: parent.left
                anchors.leftMargin: 8
                anchors.verticalCenter: parent.verticalCenter

                source: functChooseLogo()
            }

            Text {
                id: textProgName
                text: progName
                anchors.left: logo.right
                anchors.leftMargin: 5
                anchors.top: parent.top
                anchors.topMargin: 8
            }

            Text {
                id: textCompanyName
                text: companyName
                anchors.left: logo.right
                anchors.leftMargin: 5
                anchors.top: textProgName.bottom
                anchors.topMargin: 8
            }

            Text {
                id: textPhone
                text: phone
                anchors.left: logo.right
                anchors.leftMargin: 5
                anchors.top: textCompanyName.bottom
                anchors.topMargin: 5
            }

            Text {
                id: eMail
                text: "E-mail: " + link
                anchors.left: textPhone.left
                anchors.top: textPhone.bottom
                anchors.topMargin: 5
                onLinkActivated: Qt.openUrlExternally(link)
            }

            Text {
                id: versionSoftware
                text: qsTr("Version: ") + softwareVersionConfiguration
                anchors.left: eMail.left
                anchors.top: eMail.bottom
            }
        }
    }
//    Component.onCompleted: {
//        rusTrans.getCurrentLanguage()
//    }
}
