import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

ApplicationWindow {

	id: macroWindow

	width: 320
	height: 320

	title: titleText

	property string titleText: ""
	property int baseHeight: 40
	property color defaultColor: "#e1e1e2"

	Rectangle {
		id: rectangleMacroWindow
		width: parent.width
		height: parent.height - 2 * macroWindow.baseHeight

		ListView {
			id: listViewMacroWindow
			anchors.fill: parent
			model: ListModel {
				id: listModelMacro
			}
			delegate: Item {
				width: 200
				height: 40
				Row {
					id: rowMacroWindow
					Text {
						text: name
						anchors.verticalCenter: parent.verticalCenter
					}
					spacing: 10
				}
				MouseArea {
					id: mouseAreaMacroWindow
					anchors.fill: parent
					onClicked: listViewMacroWindow.currentIndex = index
					onDoubleClicked: {
						if (listModelMacro.count > 0) {
							var nameTmp = listModelMacro.get(
										listViewMacroWindow.currentIndex).name
							appCore.fillMacro(nameTmp)
							console.log(nameTmp)
							macroWindow.close()
						}
					}
				}
			}
			highlight: Rectangle {
				id: highlightMainMacro
				color: defaultColor
			}
			highlightMoveDuration: 100
			ScrollBar.vertical: ScrollBar {
			}
		}
	}

	Rectangle {
		id: rectButtonsMacroWindow
		width: parent.width
		height: macroWindow.baseHeight
		anchors.bottom: parent.bottom
		anchors.bottomMargin: 0

		Rectangle {
			id: rectangleMacroLeft
			height: macroWindow.baseHeight
			anchors.left: parent.left
			anchors.leftMargin: 0
			width: parent.width / 2
			Button {
				id: buttonSelect
				text: qsTr("Выбрать")
				anchors.horizontalCenter: parent.horizontalCenter
				height: macroWindow.baseHeight
				onClicked: {
					if (listModelMacro.count > 0) {
						var nameTmp = listModelMacro.get(
									listViewMacroWindow.currentIndex).name
						appCore.fillMacro(nameTmp)
						console.log(nameTmp)
					}
				}
			}
		}

		Rectangle {
			id: rectangleMacroRight
			height: macroWindow.baseHeight
			anchors.right: parent.right
			anchors.rightMargin: 0
			width: parent.width / 2
			Button {
				height: macroWindow.baseHeight
				id: buttonDelete
				text: qsTr("Удалить")
				anchors.horizontalCenter: parent.horizontalCenter
				onClicked: {
					if (listModelMacro.count > 0) {
						appCore.removeMacro(listViewMacroWindow.currentIndex)
						listModelMacro.remove(listViewMacroWindow.currentIndex)
					}
				}
			}
		}
	}

	Connections {
		target: appCore
		onSendListNameOfMacro: {
			listModelMacro.append({
									  "name": name
								  })
		}
	}


	/**
	*/
	function setCoordinates() {
		x += screen.width / 2 - width / 2
		y += screen.height / 2 - height / 2
	}

	Component.onCompleted: {
		appCore.getListOfMacros()
		setCoordinates()
	}
}
