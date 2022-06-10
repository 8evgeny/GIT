var componentChangeWindow;
var objectChangeWindow;

function createChangeWindowObject(root, flag) {
    componentChangeWindow = Qt.createComponent("change_window.qml")
    objectChangeWindow = componentChangeWindow.createObject(root)
    objectChangeWindow.flagCloseWindow = flag
}

function deleteChangeWindowObject() {
    objectChangeWindow.destroy()
}
