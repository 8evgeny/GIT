var componentFirmware;
var objectFirmware;

function createFirmwareObject(root) {
    componentFirmware = Qt.createComponent("firmware.qml")
    objectFirmware = componentFirmware.createObject(root)
}

function deleteFirmwareObject() {
    objectFirmware.destroy()
}
