var componentGroups;
var objectGroups;

function createGroupsObject(root) {
    componentGroups = Qt.createComponent("groups.qml")
    objectGroups = componentGroups.createObject(root)
}

function deleteGroupsObject() {
    objectGroups.destroy()
}
