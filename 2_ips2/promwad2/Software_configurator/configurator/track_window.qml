import QtQuick 2.0

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

ApplicationWindow {

	id: trackWindow

	width: 320
	height: 320

	title: titleText

	property string titleText: ""
	property int baseHeight: 40
	property color defaultColor: "#e1e1e2"

	Rectangle {
		id: rectangleTrackWindow
		width: parent.width
		height: parent.height - 2 * trackWindow.baseHeight

		ListView {
			id: listViewTrackWindow
			anchors.fill: parent
			model: ListModel {
				id: listModelTrack
			}
			delegate: Item {
				width: 200
				height: 40
				Row {
					id: rowTrackWindow
					Text {
						text: name
						anchors.verticalCenter: parent.verticalCenter
					}
					spacing: 10
				}
				MouseArea {
					id: mouseAreaTrackWindow
					anchors.fill: parent
					onClicked: listViewTrackWindow.currentIndex = index
					onDoubleClicked: {
						if (listModelTrack.count > 0) {
							var nameTmp = listModelTrack.get(
										listViewTrackWindow.currentIndex).name
							appCore.fillTrack(nameTmp)
							console.log(nameTmp)
							trackWindow.close()
						}
					}
				}
			}
			highlight: Rectangle {
				id: highlightMainTrack
				color: defaultColor
			}
			highlightMoveDuration: 100
			ScrollBar.vertical: ScrollBar {
			}
		}
	}

	Rectangle {
		id: rectButtonsTrackWindow
		width: parent.width
		height: trackWindow.baseHeight
		anchors.bottom: parent.bottom
		anchors.bottomMargin: 0

		Rectangle {
			id: rectangleTrackLeft
			height: trackWindow.baseHeight
			anchors.left: parent.left
			anchors.leftMargin: 0
			width: parent.width
			Button {
				id: buttonSelect
				text: qsTr("Выбрать")
				anchors.horizontalCenter: parent.horizontalCenter
				height: trackWindow.baseHeight
				onClicked: {
					if (listModelTrack.count > 0) {
						var nameTmp = listModelTrack.get(
									listViewTrackWindow.currentIndex).name
						appCore.fillTrack(nameTmp)
						console.log(nameTmp)
					}
				}
			}
		}
	}

	Connections {
		target: appCore
		onSendListNameOfTrack: {
			listModelTrack.append({
									  "name": name
								  })
		}
	}

	function setCoordinates() {
		x += screen.width / 2 - width / 2
		y += screen.height / 2 - height / 2
	}

	Component.onCompleted: {
		appCore.getListOfTracks()
		setCoordinates()
	}
}
