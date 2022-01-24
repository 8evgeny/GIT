import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12

Rectangle {

	width: 800
	height: 80

	property int cellWidth: 200

	Rectangle {
		id: rectangle1_1
		width: grid.widthCell
		height: grid.heightCell
		color: grid.colorCellOfLabels
		border.color: grid.colorBorderOfLabels

		Label {
			id: label1
			width: parent.width
			height: parent.height
			color: "#3f4040"
			text: "Клавишный блок"
			horizontalAlignment: Text.AlignHCenter
			verticalAlignment: Text.AlignVCenter
		}
	}

	Rectangle {
		id: rectangle1_2
		width: grid.widthCell
		height: grid.heightCell
		color: grid.colorCellOfLabels
		border.color: grid.colorBorderOfLabels

		Label {
			id: label2
			width: parent.width
			height: parent.height
			text: "Название клавиши"
			horizontalAlignment: Text.AlignHCenter
			verticalAlignment: Text.AlignVCenter
		}
	}

	Rectangle {
		id: rectangle1_3
		width: grid.widthCell
		height: grid.heightCell
		color: grid.colorCellOfLabels
		border.color: grid.colorBorderOfLabels
		Label {
			id: label3
			width: parent.width
			height: parent.height
			text: "Функция клавиши"
			horizontalAlignment: Text.AlignHCenter
			verticalAlignment: Text.AlignVCenter
		}
	}

	Rectangle {
		id: rectangle1_4
		width: grid.widthCell
		height: grid.heightCell
		color: grid.colorCellOfLabels
		border.color: grid.colorBorderOfLabels
		Label {
			id: label4
			width: parent.width
			height: parent.height
			text: "Назначение"
			horizontalAlignment: Text.AlignHCenter
			verticalAlignment: Text.AlignVCenter
		}
	}

	Item {
		width: 800
		height: 40
		property int cellWidth: 200

		Row {
			id: rowMacroWindow
			width: parent.width
			height: parent.height

			Rectangle {
				id: rectangle1
				width: cellWidth
				height: parent.height
				TextField {
					readOnly: true
					width: parent.width
					height: parent.height
					text: name
					horizontalAlignment: Text.AlignHCenter
					id: textField1
				}
			}

			Rectangle {
				id: rectangle2
				width: cellWidth
				height: parent.height
				color: "#ffffff"
				TextField {
					width: parent.width
					height: parent.height
					horizontalAlignment: Text.AlignHCenter
					id: textField2
				}
			}

			Rectangle {
				id: rectangle3
				width: cellWidth
				height: parent.height
				color: "#ffffff"
				border.color: "#c54848"

				TextField {
					width: parent.width
					height: parent.height
					id: textField3
				}

				Button {
					id: button1
					width: parent.height - 10
					height: parent.height - 10
					text: qsTr("...")
					anchors.right: parent.right
					anchors.rightMargin: 10
					anchors.verticalCenter: parent.verticalCenter
					onClicked: {
						var component = Qt.createComponent(
									"subscriber_window.qml")
						var window = component.createObject(rootRectangle)
						window.mainText = textField3.text
						window.mainNumber = 1
						window.show()
					}
				}
			}

			Rectangle {
				id: rectangle4
				width: cellWidth
				height: parent.height
				color: "#ffffff"
				TextField {
					width: parent.width
					height: parent.height
					id: textField4
				}
				Button {
					id: button2
					width: parent.height - 10
					height: parent.height - 10
					text: qsTr("...")
					anchors.right: parent.right
					anchors.rightMargin: 10
					anchors.verticalCenter: parent.verticalCenter
				}
			}

			spacing: -1
		}
		MouseArea {
			id: mouseAreaMacroWindow
			anchors.fill: parent
			onClicked: listViewMacroWindow.currentIndex = index
		}
	}
}
