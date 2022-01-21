import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

Rectangle {
	id: rect
	width: 400
	height: 400

	property int baseHeight: 40
	property int listHeight: 150

	ButtonGroup {
		id: buttonGroup
	}

	ListView {
		width: parent.width
		height: listHeight
		layoutDirection: Qt.LeftToRight
		model: ["2x проводная с управлением переменным током", "2x проводная с управлением постоянным током", "4х проводная 0,78 В"]
		delegate: RadioButton {
			text: modelData
			checked: index == 0
			ButtonGroup.group: buttonGroup
		}
	}

	Text {
		id: element
		text: qsTr("Транслировать на усилитель")
		horizontalAlignment: Text.AlignHCenter
		verticalAlignment: Text.AlignVCenter
		anchors.top: parent.top
		anchors.topMargin: listHeight
		width: parent.width
		height: baseHeight
		font.pixelSize: 12
	}

	Grid {
		id: grid
		x: 0
		anchors.top: parent.top
		width: parent.width
		height: baseHeight * 3
		anchors.topMargin: 189
		rows: 2
		columns: 3

		Label {
			id: label
			text: qsTr("Label")
		}

		Label {
			id: label1
			text: qsTr("Label")
		}

		Label {
			id: label2
			text: qsTr("Label")
		}
	}
}
