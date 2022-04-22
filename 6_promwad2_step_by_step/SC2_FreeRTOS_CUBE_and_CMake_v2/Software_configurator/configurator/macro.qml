import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtWebSockets 1.1

Rectangle {
	id: rectangleMacro

	property int baseHeight: 40
	property int baseSpacing: 10

	property int numberOfMacro: -1
	property string nameOfMacro: ""
	property int nameOfPriority: -1
	property string nameOfMsg: ""
	property int nameOfRepeat: -1
	property color errorColor: "red"
	property color defaultColor: "#26282a"

	width: 350
	height: 400

	Column {
		id: columnMacro
		width: parent.width
		height: parent.height
		spacing: rectangleMacro.baseSpacing

		Label {
			id: labelProperyMacro
			text: qsTr("Свойства Макроса:")
			verticalAlignment: Text.AlignVCenter
			height: rectangleMacro.baseHeight
		}

		Grid {
			id: gridMacro
			width: parent.width
			height: parent.height / 2
			spacing: rectangleMacro.baseSpacing
			rows: 4
			columns: 2

			Label {
				id: labelNameOfMacro
				text: qsTr("Имя макроса:")
				verticalAlignment: Text.AlignVCenter
				height: rectangleMacro.baseHeight
			}

			TextField {
				id: textFieldNameOfMacro
				text: qsTr("Макрос")
				height: rectangleMacro.baseHeight
			}

			Label {
				id: labelPriorityMacro
				text: qsTr("Приоритет:")
				verticalAlignment: Text.AlignVCenter
				height: rectangleMacro.baseHeight
			}

			TextField {
				id: textFieldPriorityMacro
				text: qsTr("1..8")
				height: rectangleMacro.baseHeight
			}

			Label {
				id: labelMsgMacro
				text: qsTr("Сообщение:")
				verticalAlignment: Text.AlignVCenter
				height: rectangleMacro.baseHeight
			}

			Row {
				id: rowMacro
				height: rectangleMacro.baseHeight
				spacing: rectangleMacro.baseSpacing

				TextField {
					id: textFieldMsgMacro
					text: qsTr("Track01")
					height: rectangleMacro.baseHeight
				}

				Button {
					id: buttonMsgMacro
					width: rectangleMacro.baseHeight
					text: qsTr("...")
					height: rectangleMacro.baseHeight
				}
			}

			Label {
				id: labelRepeatMacro
				text: qsTr("Повтор:")
				verticalAlignment: Text.AlignVCenter
				height: rectangleMacro.baseHeight
			}

			TextField {
				id: textFieldRepeatMacro
				text: qsTr("1...10")
				height: rectangleMacro.baseHeight
			}
		}

		Button {
			id: buttonAddMacro
			text: qsTr("Добавить макрос")
			height: rectangleMacro.baseHeight
		}

		Button {
			id: buttonSaveChanges
			text: qsTr("Сохранить изменения")
			height: rectangleMacro.baseHeight
		}

		Button {
			id: buttonListOfMacros
			text: qsTr("Список макросов")
			height: rectangleMacro.baseHeight
		}
	}


	/**
	 * Error of a macro
	 */
	function errorMacro(str) {
		var componentError = Qt.createComponent("error_macro.qml")
		var errorMacro = componentError.createObject(rectangleMacro)
		errorMacro.strError = str
		errorMacro.destroy(2500)
	}

	Connections {
		target: appCore
		onSendErrorMacroExists: {
			console.log("Макрос с таким именем существует" + " <1>")
			errorMacro("Макрос с таким именем существует")
		}
		onSendErrorMacroExistsChanges: {
			console.log("Макрос с таким именем не существует")
			errorMacro("Макрос с таким именем не существует")

			rectangleMacro.numberOfMacro = -1
			rectangleMacro.nameOfMacro = ""
			rectangleMacro.nameOfPriority = -1
			rectangleMacro.nameOfMsg = ""
			rectangleMacro.nameOfRepeat = -1
		}
		onSendSelectedMacro: {
			rectangleMacro.numberOfMacro = numberOfMacro
			rectangleMacro.nameOfMacro = nameOfMacro
			textFieldNameOfMacro.text = rectangleMacro.nameOfMacro
			rectangleMacro.nameOfPriority = nameOfPriority
			textFieldPriorityMacro.text = rectangleMacro.nameOfPriority
			rectangleMacro.nameOfMsg = nameOfMsg
			textFieldMsgMacro.text = rectangleMacro.nameOfMsg
			rectangleMacro.nameOfRepeat = nameOfRepeat
			textFieldRepeatMacro.text = rectangleMacro.nameOfRepeat
		}
		onSendSelectedTrack: {
			rectangleMacro.nameOfMsg = nameOfTrack
			textFieldMsgMacro.text = rectangleMacro.nameOfMsg
		}
	}

	function addMacroOrChange(add) {
		var intNameOfPriority = parseInt(textFieldPriorityMacro.text)
		if (isNaN(intNameOfPriority))
			intNameOfPriority = -1

		var intNameOfRepeat = parseInt(textFieldRepeatMacro.text)
		if (isNaN(intNameOfRepeat))
			intNameOfRepeat = -1

		var errorStr = ""
		if ((intNameOfPriority === -1) || (intNameOfRepeat === -1)) {

			if (intNameOfPriority === -1) {
				errorStr += "Неправильно задано поле приоритет\n"
				textFieldPriorityMacro.color = rectangleMacro.errorColor
			}

			if (intNameOfRepeat === -1) {
				errorStr += "Неправильно задано поле повторов\n"
				textFieldRepeatMacro.color = rectangleMacro.errorColor
			}

			console.log(errorStr)
			errorMacro(errorStr)
		} else {

			errorStr = ""

			if ((intNameOfPriority < 1) || (intNameOfPriority > 8)) {
				errorStr += "Неправильно задано поле приоритет\n"
				textFieldPriorityMacro.color = rectangleMacro.errorColor
			}

			if ((intNameOfRepeat < 1) || (intNameOfRepeat > 10)) {
				errorStr += "Неправильно задано поле повторов\n"
				textFieldRepeatMacro.color = rectangleMacro.errorColor
			}

			if (errorStr !== "") {
				console.log(errorStr)
				errorMacro(errorStr)
			}

			if ((intNameOfPriority >= 1) && (intNameOfPriority <= 8)
					&& (intNameOfRepeat >= 1) && (intNameOfRepeat <= 10)) {

				rectangleMacro.nameOfMacro = textFieldNameOfMacro.text
				rectangleMacro.nameOfMsg = textFieldMsgMacro.text

				rectangleMacro.nameOfPriority = intNameOfPriority
				rectangleMacro.nameOfRepeat = intNameOfRepeat

				if (add)
					appCore.receiveFromQmlMacro(rectangleMacro.nameOfMacro,
												rectangleMacro.nameOfPriority,
												rectangleMacro.nameOfMsg,
												rectangleMacro.nameOfRepeat)
				else
					appCore.changesOfMacro(rectangleMacro.nameOfMacro,
										   rectangleMacro.nameOfPriority,
										   rectangleMacro.nameOfMsg,
										   rectangleMacro.nameOfRepeat)
			}
		}
	}

	Connections {
		target: buttonAddMacro
		onClicked: {

			if ((rectangleMacro.nameOfMacro != textFieldNameOfMacro.text)
					&& (textFieldNameOfMacro.text != "")) {
				addMacroOrChange(true)
			} else {
				if (textFieldNameOfMacro.text != "") {
					if (rectangleMacro.nameOfMacro == "") {
						console.log("Макрос с таким именем не существует")
						errorMacro("Макрос с таким именем не существует")
					} else {
						console.log("Макрос с таким именем существует" + " <2>")
						errorMacro("Макрос с таким именем существует")
					}
				} else {
					console.log("Не задано имя макроса" + " <2>")
					errorMacro("Не задано имя макроса")
				}
			}
		}
	}

	Connections {
		target: buttonListOfMacros
		onClicked: {
			var componentListOfMacros = Qt.createComponent("macro_window.qml")
			var windowMacro = componentListOfMacros.createObject(rectangleMacro)
			windowMacro.titleText = buttonListOfMacros.text
			windowMacro.show()

			rectangleMacro.numberOfMacro = -1
			rectangleMacro.nameOfMacro = ""
			rectangleMacro.nameOfPriority = -1
			rectangleMacro.nameOfMsg = ""
			rectangleMacro.nameOfRepeat = -1
		}
	}

	Connections {
		target: buttonSaveChanges
		onClicked: {
			if (textFieldNameOfMacro.text != "") {
				addMacroOrChange(false)
			} else {
				errorMacro("Не задано имя макроса")
				console.log("Не задано имя макроса" + " <1>")
			}
		}
	}

	Connections {
		target: textFieldPriorityMacro
		onTextEdited: {
			textFieldPriorityMacro.color = rectangleMacro.defaultColor
		}
	}

	Connections {
		target: textFieldRepeatMacro
		onTextEdited: {
			textFieldRepeatMacro.color = rectangleMacro.defaultColor
		}
	}

	Connections {
		target: buttonMsgMacro
		onClicked: {
			var componentListOfTrack = Qt.createComponent("track_window.qml")
			var windowTrack = componentListOfTrack.createObject(rectangleMacro)
			windowTrack.titleText = buttonListOfMacros.text
			windowTrack.show()
		}
	}
}
