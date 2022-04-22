var componentStatusWindow;
var objectStatusWindow;

function createStatusWindowObject(root, idStr, str) {
    componentStatusWindow = Qt.createComponent("status_window.qml")
    objectStatusWindow = componentStatusWindow.createObject(root)

    objectStatusWindow.nameId = idStr
    objectStatusWindow.statusStr = str
}

function deleteStatusWindowObject() {
//    objectStatusWindow.destroy(2000)
}
