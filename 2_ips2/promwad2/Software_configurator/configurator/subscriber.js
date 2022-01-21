var componentSubscriber;
var objectSubscriber;

function createSubscriberObject(root) {
    componentSubscriber = Qt.createComponent("subscriber.qml")
    objectSubscriber = componentSubscriber.createObject(root)
}

function deleteSubscriberObject() {
    objectSubscriber.destroy()
}
