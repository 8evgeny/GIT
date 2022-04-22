var componentMainMenu;
var objectMainMenu;

function createMainMenuObject(root, langState) {
    componentMainMenu = Qt.createComponent("main_menu.qml")
    objectMainMenu = componentMainMenu.createObject(root)
    objectMainMenu.langCurrentStatte = langState

}

function deleteMainMenuObject() {
    objectMainMenu.destroy()
}
