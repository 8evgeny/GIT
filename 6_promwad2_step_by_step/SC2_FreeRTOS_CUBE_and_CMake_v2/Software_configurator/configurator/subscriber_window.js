var componentSubscriberWindow;
var objectSubscriberWindow;

function createSubscriberWindowObject(root) {
    componentSubscriberWindow = Qt.createComponent("subscriber_window.qml")
    objectSubscriberWindow = componentSubscriberWindow.createObject(root)
}

function deleteSubscriberWindowObject() {
    objectSubscriberWindow.destroy()
}
