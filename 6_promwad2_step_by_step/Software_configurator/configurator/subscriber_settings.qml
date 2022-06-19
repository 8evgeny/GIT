import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

Rectangle {
	id: rectangle
	height: parent.height
	property color defaultColor: "#e1e1e2" //"#c2bdbd"
	width: 450

	Column {
		id: column
		width: parent.width
		height: parent.height
		spacing: 10

		Rectangle {
			color: rectangle.defaultColor
			anchors.right: parent.right
			anchors.rightMargin: 0
			anchors.left: parent.left
			anchors.leftMargin: 0
			height: 40
			Text {
				id: label10
				width: parent.width
				height: parent.height
				text: qsTr("Сведения о станции")
				verticalAlignment: Text.AlignVCenter
				horizontalAlignment: Text.AlignHCenter
			}
		}

		Rectangle {
			id: rectangle1
			height: 259
			color: "#ffffff"
			anchors.right: parent.right
			anchors.rightMargin: 0
			anchors.left: parent.left
			anchors.leftMargin: 0

			Grid {
				id: grid
				anchors.rightMargin: 0
				anchors.bottomMargin: 0
				anchors.leftMargin: 0
				anchors.topMargin: 0
				anchors.fill: parent
				flow: Grid.LeftToRight
				spacing: 10
				rows: 5
				columns: 2

				Label {
					id: label
					text: qsTr("Номер станции:")
					horizontalAlignment: Text.AlignHCenter
					verticalAlignment: Text.AlignVCenter
				}

				TextField {
					id: textField
					text: qsTr("")
				}

				Label {
					id: label1
					text: qsTr("Имя станции:")
				}

				TextField {
					id: textField1
					text: qsTr("")
				}

				Label {
					id: label2
					text: qsTr("IP адрес:")
				}

				TextField {
					id: textField2
					text: qsTr("")
				}

				Label {
					id: label3
					text: qsTr("Маска сети:")
				}

				TextField {
					id: textField3
					text: qsTr("")
				}

				Label {
					id: label4
					text: qsTr("Шлюз:")
				}

				TextField {
					id: textField4
					text: qsTr("")
				}
			}
		}

		Rectangle {
			color: rectangle.defaultColor
			anchors.right: parent.right
			anchors.rightMargin: 0
			anchors.left: parent.left
			anchors.leftMargin: 0
			height: 40
			Text {
				id: label11
				width: parent.width
				height: parent.height
				text: qsTr("Свойство станции")
				verticalAlignment: Text.AlignVCenter
				horizontalAlignment: Text.AlignHCenter
			}
		}

		Rectangle {
			id: rectangle2
			height: 150
			color: "#ffffff"
			anchors.right: parent.right
			anchors.rightMargin: 0
			anchors.left: parent.left
			anchors.leftMargin: 0

			Grid {
				id: grid1
				y: 0
				height: 142
				anchors.right: parent.right
				anchors.rightMargin: 0
				anchors.left: parent.left
				anchors.leftMargin: 0
				spacing: 10
				rows: 3
				columns: 1

				CheckBox {
					id: checkBox
					text: qsTr("Сигнализация пропущенного вызова")
				}

				CheckBox {
					id: checkBox1
					text: qsTr("Гарнитура/трубка")
				}

				CheckBox {
					id: checkBox2
					text: qsTr("Дополнительный усилитель")
				}
			}
		}
		Rectangle {
			color: rectangle.defaultColor
			height: 40
			width: parent.width
			Text {
				id: label12
				width: parent.width
				height: parent.height
				text: qsTr("Канал записи")
				verticalAlignment: Text.AlignVCenter
				horizontalAlignment: Text.AlignHCenter
			}
		}

		ComboBox {
			id: comboBox
			y: 208
			height: 40
			anchors.right: parent.right
			anchors.rightMargin: 0
			anchors.left: parent.left
			anchors.leftMargin: 0
			textRole: ""
			currentIndex: -1
			model: ["Канал записи 1", "Канал записи 2", "Канал записи 3"]
		}

		Rectangle {
			id: rectangle3
			width: parent.width
			height: 40
			color: rectangle.defaultColor
			anchors.right: parent.right
			anchors.rightMargin: 0
			anchors.left: parent.left
			anchors.leftMargin: 0

			Text {
				id: label6
				width: parent.width
				height: parent.height
				text: qsTr("Линии управления")
				verticalAlignment: Text.AlignVCenter
				horizontalAlignment: Text.AlignHCenter
			}
		}

		Rectangle {
			id: rectangle4
			height: 149
			width: parent.width
			Grid {
				id: grid2
				anchors.left: parent.left
				anchors.leftMargin: 0
				anchors.right: parent.right
				anchors.rightMargin: 0
				spacing: 10
				rows: 3
				columns: 3

				Label {
					id: label7
					text: qsTr("Состояние прослушивания: ")
				}

				TextField {
					id: textField5
					text: qsTr("")
				}

				Button {
					id: button
					width: 40
					text: qsTr("...")
				}

				Label {
					id: label8
					text: qsTr("Состояние разговора: ")
				}

				TextField {
					id: textField6
					text: qsTr("")
				}

				Button {
					id: button1
					width: 40
					text: qsTr("...")
				}

				Label {
					id: label9
					text: qsTr("Состояние вызова:")
				}

				TextField {
					id: textField7
					text: qsTr("")
				}

				Button {
					id: button2
					width: 40
					text: qsTr("...")
				}
			}
		}
	}
}
