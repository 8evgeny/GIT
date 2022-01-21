var componentNetwork;
var objectNetwork;

function createNetworkObject(root) {
    componentNetwork = Qt.createComponent("network.qml")
    objectNetwork = componentNetwork.createObject(root)
}

function deleteNetworkObject() {
    objectNetwork.destroy()
}
