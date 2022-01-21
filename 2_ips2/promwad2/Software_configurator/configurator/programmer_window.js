var componentProgrammerWindow;
var objectProgrammerWindow;

function createProgrammerWindowObject(root) {
    componentProgrammerWindow = Qt.createComponent("programmer_window.qml")
    objectProgrammerWindow = componentProgrammerWindow.createObject(root)
}

function deleteProgrammerWindowObject() {
    objectProgrammerWindow.destroy()
}
