var componentUpdateRs232;
var objectUpdateRs232;

function createUpdateRs232Object(root) {
    componentUpdateRs232 = Qt.createComponent("update_rs232.qml")
    objectUpdateRs232 = componentUpdateRs232.createObject(root)
}

function deleteUpdateRs232Object() {
    objectUpdateRs232.destroy()
}
