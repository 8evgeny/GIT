import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQuick.Controls.Styles 1.4
import RuLang 1.0

import "subscriber.js" as Subscriber
import "main_menu.js" as MainMenu
import "subscriber_navigation.js" as SubscriberNavigation
import "groups.js" as Groups
import "change_window.js" as UpdateChangeWindow
import "status_window.js" as UpdateStatusWindow

ApplicationWindow {
    id: applicationWindow

    property int widthDefault: 1920
    property int heightDefault: 1000
    property bool changeWindowState: false
    property bool closeWindow: false

    width: widthDefault
    height: heightDefault

    visible: true

    title: "GIT"
    visibility: Window.Maximized

    Flickable {

        clip: true
        anchors.fill: parent

        contentWidth: widthDefault
        contentHeight: heightDefault

        ScrollBar.horizontal: ScrollBar {
            id: hbar
            active: vbar.active
        }
        ScrollBar.vertical: ScrollBar {
            id: vbar
            active: hbar.active
        }

        Item {
            id: itemSubscriber
            x: 610
            y: 230
            width: 1000
            height: 1000
            anchors.left: itemSubscriberNavigation.right
            anchors.leftMargin: 5

            Component.onCompleted: {
                Subscriber.createSubscriberObject(itemSubscriber)
            }

            Connections {
                target: rusTrans
                onLanguageChanged: {
                    Subscriber.deleteSubscriberObject()
                    Subscriber.createSubscriberObject(itemSubscriber)
                }
            }
        }

        Item {
            id: itemGroups
            x: 610
            y: 145
            width: 1000
            height: 1000
            anchors.left: itemSubscriberNavigation.right
            anchors.leftMargin: 5
            Component.onCompleted: {
                Groups.createGroupsObject(itemGroups)
            }

            Connections {
                target: rusTrans
                onLanguageChanged: {
                    Groups.deleteGroupsObject()
                    Groups.createGroupsObject(itemGroups)
                    appCore.getCurrentIndexInList()
                }
            }
        }

        Item {
            id: itemSubscriberNavigation
            x: 5
            y: 45
            width: 600
            height: 1000

            Component.onCompleted: {
                SubscriberNavigation.createSubscriberWindowObject(
                            itemSubscriberNavigation)                
            }

            Connections {
                target: rusTrans
                onLanguageChanged: {
                    SubscriberNavigation.deleteSubscriberNavigationObject()
                    SubscriberNavigation.createSubscriberWindowObject(
                                itemSubscriberNavigation)
                    appCore.updateAllViews()
                    appCore.getCurrentIndexInList()
                }
            }
        }

        Item {
            id: itemMainMenu
            width: parent.width
            height: parent.heigth

            Component.onCompleted: {
                MainMenu.createMainMenuObject(itemMainMenu, 0)
            }

            Connections {
                target: rusTrans
                onLanguageChanged: {
                    MainMenu.deleteMainMenuObject()
                    MainMenu.createMainMenuObject(itemMainMenu, langState)
                    //langState
                }
            }
        }
    }

    Connections {
        target: receiver
        onStatusFirmwareLoad: {
            //UpdateStatusWindow.createStatusWindowObject(itemMainMenu, "CID " + idDevice, qsTr("Firmware"))
            //UpdateStatusWindow.deleteStatusWindowObject()
            appCore.getStatusMsg("CID " + idDevice, qsTr("Firmware"))
        }
        onStatusConfigurationLoad : {
            //UpdateStatusWindow.createStatusWindowObject(itemMainMenu, "CID " + idDevice, qsTr("Configuration"))
            //UpdateStatusWindow.deleteStatusWindowObject()
            appCore.getStatusMsg("CID " + idDevice, qsTr("Configuration"))
        }
    }

    Connections {
        target: appCore
        onWindowChangesBeforeSave : {
            closeWindow = false
            if (changeWindowState == false) {
                UpdateChangeWindow.createChangeWindowObject(applicationWindow, true)
                changeWindowState = true
            } else {
                UpdateChangeWindow.deleteChangeWindowObject()
                UpdateChangeWindow.createChangeWindowObject(applicationWindow, true)
            }
        }

        onWindowChangesMenuSave: {
            if (changeWindowState == false) {
                UpdateChangeWindow.createChangeWindowObject(applicationWindow, false)
                changeWindowState = true
            } else {
                UpdateChangeWindow.deleteChangeWindowObject()
                UpdateChangeWindow.createChangeWindowObject(applicationWindow, false)
            }
        }

        onCreateWindowStatus : {
            UpdateStatusWindow.createStatusWindowObject(applicationWindow, idDevice, msgDevice)
            UpdateStatusWindow.deleteStatusWindowObject()
        }

        onCloseMainWindow : {
            closeWindow = true
            applicationWindow.close()
        }

        onExitFromMenu : {
            if(!closeWindow){
                appCore.checkChanges(true)
                close.accepted = false
            }
        }
    }

    onClosing: {
        if(!closeWindow){
            appCore.checkChanges(true)
            close.accepted = false
        }
    }

}
