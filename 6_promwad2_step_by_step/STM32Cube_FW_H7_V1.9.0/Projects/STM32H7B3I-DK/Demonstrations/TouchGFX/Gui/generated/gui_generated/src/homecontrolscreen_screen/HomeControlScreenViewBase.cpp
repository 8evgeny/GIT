/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/homecontrolscreen_screen/HomeControlScreenViewBase.hpp>
#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>

HomeControlScreenViewBase::HomeControlScreenViewBase() :
    buttonCallback(this, &HomeControlScreenViewBase::buttonCallbackHandler),
    flexButtonCallback(this, &HomeControlScreenViewBase::flexButtonCallbackHandler),
    radioButtonSelectedCallback(this, &HomeControlScreenViewBase::radioButtonSelectedCallbackHandler),
    updateItemCallback(this, &HomeControlScreenViewBase::updateItemCallbackHandler)
{
    background.setXY(0, 0);
    background.setBitmap(touchgfx::Bitmap(BITMAP_MAIN_BG_HOME_CONTROL_ID));

    scrollWheelRoomSelector.setPosition(0, 60, 480, 135);
    scrollWheelRoomSelector.setHorizontal(true);
    scrollWheelRoomSelector.setCircular(true);
    scrollWheelRoomSelector.setEasingEquation(touchgfx::EasingEquations::backEaseOut);
    scrollWheelRoomSelector.setSwipeAcceleration(30);
    scrollWheelRoomSelector.setDragAcceleration(10);
    scrollWheelRoomSelector.setNumberOfItems(5);
    scrollWheelRoomSelector.setSelectedItemOffset(163);
    scrollWheelRoomSelector.setSelectedItemExtraSize(0, 0);
    scrollWheelRoomSelector.setSelectedItemMargin(0, 0);
    scrollWheelRoomSelector.setDrawableSize(120, 15);
    scrollWheelRoomSelector.setDrawables(scrollWheelRoomSelectorListItems, updateItemCallback,
                              scrollWheelRoomSelectorSelectedListItems, updateItemCallback);
    scrollWheelRoomSelector.animateToItem(1, 0);

    scrollWheelRoomInfo.setPosition(43, 53, 390, 146);
    scrollWheelRoomInfo.setHorizontal(true);
    scrollWheelRoomInfo.setCircular(true);
    scrollWheelRoomInfo.setEasingEquation(touchgfx::EasingEquations::cubicEaseOut);
    scrollWheelRoomInfo.setSwipeAcceleration(10);
    scrollWheelRoomInfo.setDragAcceleration(10);
    scrollWheelRoomInfo.setNumberOfItems(2);
    scrollWheelRoomInfo.setSelectedItemOffset(0);
    scrollWheelRoomInfo.setDrawableSize(390, 0);
    scrollWheelRoomInfo.setDrawables(scrollWheelRoomInfoListItems, updateItemCallback);
    scrollWheelRoomInfo.animateToItem(0, 0);
    scrollWheelRoomInfo.setVisible(false);

    containerDevicesWheel.setPosition(51, 0, 378, 272);
    containerDevicesWheel.setVisible(false);

    scrollListKitchenDevices.setPosition(1, 0, 380, 272);
    scrollListKitchenDevices.setHorizontal(false);
    scrollListKitchenDevices.setCircular(false);
    scrollListKitchenDevices.setEasingEquation(touchgfx::EasingEquations::backEaseOut);
    scrollListKitchenDevices.setSwipeAcceleration(17);
    scrollListKitchenDevices.setDragAcceleration(10);
    scrollListKitchenDevices.setNumberOfItems(4);
    scrollListKitchenDevices.setPadding(63, 0);
    scrollListKitchenDevices.setSnapping(false);
    scrollListKitchenDevices.setDrawableSize(124, 0);
    scrollListKitchenDevices.setDrawables(scrollListKitchenDevicesListItems, updateItemCallback);
    containerDevicesWheel.add(scrollListKitchenDevices);

    scrollListBathroomDevices.setPosition(429, 0, 380, 272);
    scrollListBathroomDevices.setHorizontal(false);
    scrollListBathroomDevices.setCircular(false);
    scrollListBathroomDevices.setEasingEquation(touchgfx::EasingEquations::backEaseOut);
    scrollListBathroomDevices.setSwipeAcceleration(17);
    scrollListBathroomDevices.setDragAcceleration(10);
    scrollListBathroomDevices.setNumberOfItems(3);
    scrollListBathroomDevices.setPadding(63, 0);
    scrollListBathroomDevices.setSnapping(false);
    scrollListBathroomDevices.setDrawableSize(124, 0);
    scrollListBathroomDevices.setDrawables(scrollListBathroomDevicesListItems, updateItemCallback);
    containerDevicesWheel.add(scrollListBathroomDevices);

    switchRoomLeft.setIconBitmaps(Bitmap(BITMAP_ARROW_LEFT_ID), Bitmap(BITMAP_ARROW_LEFT_PRESSED_ID));
    switchRoomLeft.setIconXY(15, 56);
    switchRoomLeft.setPosition(0, 81, 52, 133);
    switchRoomLeft.setVisible(false);
    switchRoomLeft.setAction(flexButtonCallback);

    switchRoomRight.setIconBitmaps(Bitmap(BITMAP_ARROW_RIGHT_ID), Bitmap(BITMAP_ARROW_RIGHT_PRESSED_ID));
    switchRoomRight.setIconXY(15, 56);
    switchRoomRight.setPosition(430, 81, 52, 133);
    switchRoomRight.setVisible(false);
    switchRoomRight.setAction(flexButtonCallback);

    radioButtonTemperature.setXY(0, 193);
    radioButtonTemperature.setBitmaps(touchgfx::Bitmap(BITMAP_TEMPERATURE_NORMAL_ID), touchgfx::Bitmap(BITMAP_TEMPERATURE_NORMAL_ID), touchgfx::Bitmap(BITMAP_TEMPERATURE_SELECTED_ID), touchgfx::Bitmap(BITMAP_TEMPERATURE_SELECTED_ID));
    radioButtonTemperature.setSelected(true);
    radioButtonTemperature.setDeselectionEnabled(false);
    radioButtonTemperature.setAlpha(0);

    radioButtonDevices.setXY(120, 193);
    radioButtonDevices.setBitmaps(touchgfx::Bitmap(BITMAP_DEVICES_NORMAL_ID), touchgfx::Bitmap(BITMAP_DEVICES_NORMAL_ID), touchgfx::Bitmap(BITMAP_DEVICES_SELECTED_ID), touchgfx::Bitmap(BITMAP_DEVICES_SELECTED_ID));
    radioButtonDevices.setSelected(false);
    radioButtonDevices.setDeselectionEnabled(false);
    radioButtonDevices.setAlpha(0);

    buttonPersonalize.setXY(360, 193);
    buttonPersonalize.setBitmaps(touchgfx::Bitmap(BITMAP_PESONALIZE_ID), touchgfx::Bitmap(BITMAP_PESONALIZE_ID));
    buttonPersonalize.setAction(buttonCallback);
    buttonPersonalize.setAlpha(0);

    buttonMusicRecipes.setXY(240, 193);
    buttonMusicRecipes.setBitmaps(touchgfx::Bitmap(BITMAP_MUSIC_ID), touchgfx::Bitmap(BITMAP_MUSIC_ID));
    buttonMusicRecipes.setAction(buttonCallback);
    buttonMusicRecipes.setAlpha(0);

    imageTopGradient.setXY(0, 0);
    imageTopGradient.setVisible(false);
    imageTopGradient.setBitmap(touchgfx::Bitmap(BITMAP_TOP_GRADIENT_OVERLAY_ID));

    buttonPreviousRoom.setXY(0, 0);
    buttonPreviousRoom.setBitmaps(touchgfx::Bitmap(BITMAP_GO_BACK_BUTTON_BATH_ID), touchgfx::Bitmap(BITMAP_GO_BACK_BUTTON_BATH_ID));
    buttonPreviousRoom.setAction(buttonCallback);
    buttonPreviousRoom.setAlpha(0);

    buttonBack.setXY(0, 0);
    buttonBack.setVisible(false);
    buttonBack.setBitmaps(touchgfx::Bitmap(BITMAP_GO_BACK_BUTTON_PRESSED_ID), touchgfx::Bitmap(BITMAP_GO_BACK_BUTTON_PRESSED_ID));
    buttonBack.setAction(buttonCallback);
    buttonBack.setAlpha(0);

    textAreaHomeUIHeadline.setPosition(79, 15, 323, 30);
    textAreaHomeUIHeadline.setColor(touchgfx::Color::getColorFrom24BitRGB(30, 125, 200));
    textAreaHomeUIHeadline.setLinespacing(0);
    textAreaHomeUIHeadline.setAlpha(0);
    textAreaHomeUIHeadline.setTypedText(touchgfx::TypedText(T_HOMEUIHEADLINEROOMTEMPERATURES));

    textAreaDate.setPosition(392, 11, 75, 14);
    textAreaDate.setColor(touchgfx::Color::getColorFrom24BitRGB(58, 80, 110));
    textAreaDate.setLinespacing(0);
    textAreaDate.setAlpha(0);
    Unicode::snprintf(textAreaDateBuffer, TEXTAREADATE_SIZE, "%s", touchgfx::TypedText(T_SINGLEUSEID71).getText());
    textAreaDate.setWildcard(textAreaDateBuffer);
    textAreaDate.setTypedText(touchgfx::TypedText(T_SINGLEUSEID69));

    textAreaTime.setPosition(392, 25, 75, 14);
    textAreaTime.setColor(touchgfx::Color::getColorFrom24BitRGB(58, 80, 110));
    textAreaTime.setLinespacing(0);
    textAreaTime.setAlpha(0);
    Unicode::snprintf(textAreaTimeBuffer, TEXTAREATIME_SIZE, "%s", touchgfx::TypedText(T_SINGLEUSEID72).getText());
    textAreaTime.setWildcard(textAreaTimeBuffer);
    textAreaTime.setTypedText(touchgfx::TypedText(T_SINGLEUSEID70));

    glowTemp.setXY(20, 187);
    glowTemp.setBitmap(touchgfx::Bitmap(BITMAP_SELECT_GLOW_ID));
    glowTemp.setAlpha(0);

    glowDevices.setXY(140, 187);
    glowDevices.setBitmap(touchgfx::Bitmap(BITMAP_SELECT_GLOW_ID));
    glowDevices.setAlpha(0);

    glowRecipe.setXY(260, 187);
    glowRecipe.setBitmap(touchgfx::Bitmap(BITMAP_SELECT_GLOW_ID));
    glowRecipe.setAlpha(0);

    homeUISplashContainer.setXY(0, 0);

    mcuLoad.setXY(353, 42);

    add(background);
    add(scrollWheelRoomSelector);
    add(scrollWheelRoomInfo);
    add(containerDevicesWheel);
    add(switchRoomLeft);
    add(switchRoomRight);
    add(radioButtonTemperature);
    add(radioButtonDevices);
    add(buttonPersonalize);
    add(buttonMusicRecipes);
    add(imageTopGradient);
    add(buttonPreviousRoom);
    add(buttonBack);
    add(textAreaHomeUIHeadline);
    add(textAreaDate);
    add(textAreaTime);
    add(glowTemp);
    add(glowDevices);
    add(glowRecipe);
    add(homeUISplashContainer);
    add(mcuLoad);
    radioButtonGroup.add(radioButtonTemperature);
    radioButtonGroup.add(radioButtonDevices);
    radioButtonGroup.setRadioButtonSelectedHandler(radioButtonSelectedCallback);
}

void HomeControlScreenViewBase::setupScreen()
{
    scrollWheelRoomSelector.initialize();
    for (int i = 0; i < scrollWheelRoomSelectorListItems.getNumberOfDrawables(); i++)
    {
        scrollWheelRoomSelectorListItems[i].initialize();
    }
    for (int i = 0; i < scrollWheelRoomSelectorSelectedListItems.getNumberOfDrawables(); i++)
    {
        scrollWheelRoomSelectorSelectedListItems[i].initialize();
    }
    scrollWheelRoomInfo.initialize();
    for (int i = 0; i < scrollWheelRoomInfoListItems.getNumberOfDrawables(); i++)
    {
        scrollWheelRoomInfoListItems[i].initialize();
    }
    scrollListKitchenDevices.initialize();
    for (int i = 0; i < scrollListKitchenDevicesListItems.getNumberOfDrawables(); i++)
    {
        scrollListKitchenDevicesListItems[i].initialize();
    }
    scrollListBathroomDevices.initialize();
    for (int i = 0; i < scrollListBathroomDevicesListItems.getNumberOfDrawables(); i++)
    {
        scrollListBathroomDevicesListItems[i].initialize();
    }
    homeUISplashContainer.initialize();
    mcuLoad.initialize();
}

void HomeControlScreenViewBase::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &buttonPersonalize)
    {
        //buttonPersonalizePressed
        //When buttonPersonalize clicked call virtual function
        //Call buttonPersPressed
        buttonPersPressed();
    }
    else if (&src == &buttonMusicRecipes)
    {
        //MusicRecipesPressed
        //When buttonMusicRecipes clicked call virtual function
        //Call buttonMuiscRecipesPressed
        buttonMuiscRecipesPressed();
    }
    else if (&src == &buttonPreviousRoom)
    {
        //GoToPrevRoom
        //When buttonPreviousRoom clicked call virtual function
        //Call goToPrevRoom
        goToPrevRoom();
    }
    else if (&src == &buttonBack)
    {
        //InteractionBackButtonPressed
        //When buttonBack clicked call virtual function
        //Call backButtonPressed
        backButtonPressed();
    }
}

void HomeControlScreenViewBase::flexButtonCallbackHandler(const touchgfx::AbstractButtonContainer& src)
{
    if (&src == &switchRoomLeft)
    {
        //InteractionRoomInfoLeftClicked
        //When switchRoomLeft clicked call virtual function
        //Call roomInfoLeftClicked
        roomInfoLeftClicked();
    }
    else if (&src == &switchRoomRight)
    {
        //InteractionRoomInfoRightClicked
        //When switchRoomRight clicked call virtual function
        //Call roomInfoRightClicked
        roomInfoRightClicked();
    }
}

void HomeControlScreenViewBase::radioButtonSelectedCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &radioButtonDevices)
    {
        //InteractionRadioDevicesSelected
        //When radioButtonDevices selected call virtual function
        //Call radioDevicesSelected
        radioDevicesSelected();
    }
    else if (&src == &radioButtonTemperature)
    {
        //InteractionRadioTemperatureSelected
        //When radioButtonTemperature selected call virtual function
        //Call radioTemperatureSelected
        radioTemperatureSelected();
    }
}
void HomeControlScreenViewBase::updateItemCallbackHandler(touchgfx::DrawableListItemsInterface* items, int16_t containerIndex, int16_t itemIndex)
{
    if (items == &scrollListKitchenDevicesListItems)
    {
        touchgfx::Drawable* d = items->getDrawable(containerIndex);
        DeviceInfoWheelElements* cc = (DeviceInfoWheelElements*)d;
        scrollListKitchenDevicesUpdateItem(*cc, itemIndex);
    }
    else if (items == &scrollListBathroomDevicesListItems)
    {
        touchgfx::Drawable* d = items->getDrawable(containerIndex);
        DeviceInfoWheelElements* cc = (DeviceInfoWheelElements*)d;
        scrollListBathroomDevicesUpdateItem(*cc, itemIndex);
    }
    else if (items == &scrollWheelRoomSelectorListItems)
    {
        touchgfx::Drawable* d = items->getDrawable(containerIndex);
        HomeUIWheelElement* cc = (HomeUIWheelElement*)d;
        scrollWheelRoomSelectorUpdateItem(*cc, itemIndex);
    }
    else if (items == &scrollWheelRoomSelectorSelectedListItems)
    {
        touchgfx::Drawable* d = items->getDrawable(containerIndex);
        HomeUIWheelElement* cc = (HomeUIWheelElement*)d;
        scrollWheelRoomSelectorUpdateCenterItem(*cc, itemIndex);
    }
    else if (items == &scrollWheelRoomInfoListItems)
    {
        touchgfx::Drawable* d = items->getDrawable(containerIndex);
        RoomInfoWheelElements* cc = (RoomInfoWheelElements*)d;
        scrollWheelRoomInfoUpdateItem(*cc, itemIndex);
    }
}
