var componentSubscriberNavigation;
var objectSubscriberNavigation;

function createSubscriberWindowObject(root) {
    componentSubscriberNavigation = Qt.createComponent("subscriber_navigation.qml")
    objectSubscriberNavigation = componentSubscriberNavigation.createObject(root)
}

function deleteSubscriberNavigationObject() {
    objectSubscriberNavigation.destroy()
}
