import QtQuick 2.12
import QtQuick.Controls 2.12

Rectangle {
	anchors.verticalCenter: parent.verticalCenter
	anchors.horizontalCenter: parent.horizontalCenter

	id: rectMacroError
	width: 200
	height: 100
	border.width: 1
	border.color: "#c70b0b"

	property string strError: ""

	Label {
		id: labelMacroError
		width: parent.width
		height: parent.height
		text: rectMacroError.strError
		verticalAlignment: Text.AlignVCenter
		horizontalAlignment: Text.AlignHCenter
		wrapMode: Text.WordWrap
	}
}
