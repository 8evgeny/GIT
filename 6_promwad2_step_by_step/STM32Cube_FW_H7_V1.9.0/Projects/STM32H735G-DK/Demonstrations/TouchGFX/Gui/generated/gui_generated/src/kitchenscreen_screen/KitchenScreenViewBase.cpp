/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/kitchenscreen_screen/KitchenScreenViewBase.hpp>
#include "BitmapDatabase.hpp"

KitchenScreenViewBase::KitchenScreenViewBase() :
    buttonCallback(this, &KitchenScreenViewBase::buttonCallbackHandler),
    flexButtonCallback(this, &KitchenScreenViewBase::flexButtonCallbackHandler)
{
    background.setXY(0, 0);
    background.setBitmap(touchgfx::Bitmap(BITMAP_KITCHEN_UI_BG_ID));

    buttoHallway.setXY(2, 2);
    buttoHallway.setBitmaps(touchgfx::Bitmap(BITMAP_GO_BACK_BUTTON_ID), touchgfx::Bitmap(BITMAP_GO_BACK_BUTTON_ID));
    buttoHallway.setAction(buttonCallback);

    imageLight.setXY(0, 0);
    imageLight.setVisible(false);
    imageLight.setBitmap(touchgfx::Bitmap(BITMAP_LIGHTS_ON_ID));

    buttonExtractorHood.setXY(77, 0);
    buttonExtractorHood.setBitmaps(touchgfx::Bitmap(BITMAP_AIR_SELECT_ID), touchgfx::Bitmap(BITMAP_AIR_SELECT_ID));
    buttonExtractorHood.setAction(buttonCallback);
    buttonExtractorHood.setAlpha(0);

    buttonThermo.setXY(22, 68);
    buttonThermo.setBitmaps(touchgfx::Bitmap(BITMAP_TERMO_SELECT_ID), touchgfx::Bitmap(BITMAP_TERMO_SELECT_ID));
    buttonThermo.setAction(buttonCallback);
    buttonThermo.setAlpha(0);

    buttonOven.setIconBitmaps(Bitmap(BITMAP_OVEN_SELECT_ID), Bitmap(BITMAP_OVEN_SELECT_ID));
    buttonOven.setIconXY(0, 20);
    buttonOven.setPosition(351, 89, 129, 183);
    buttonOven.setAlpha(0);
    buttonOven.setAction(flexButtonCallback);

    imageCookpot.setXY(49, 40);
    imageCookpot.setVisible(false);
    imageCookpot.setBitmap(touchgfx::Bitmap(BITMAP_COOKPOT_ON_ID));

    imageOvenOn.setXY(375, 137);
    imageOvenOn.setVisible(false);
    imageOvenOn.setBitmap(touchgfx::Bitmap(BITMAP_OVEN_ON_ID));

    imageHoodUI.setXY(173, 8);
    imageHoodUI.setVisible(false);
    imageHoodUI.setBitmap(touchgfx::Bitmap(BITMAP_FAN_DISPLAY_ON_ID));

    imageHomeUI.setXY(43, 98);
    imageHomeUI.setVisible(false);
    imageHomeUI.setBitmap(touchgfx::Bitmap(BITMAP_RECIPIES_DISPLAY_ON_ID));

    imageOvenUI.setXY(415, 96);
    imageOvenUI.setVisible(false);
    imageOvenUI.setBitmap(touchgfx::Bitmap(BITMAP_OVEN_DISPLAY_ON_ID));

    mcuLoad.setXY(351, 251);

    add(background);
    add(buttoHallway);
    add(imageLight);
    add(buttonExtractorHood);
    add(buttonThermo);
    add(buttonOven);
    add(imageCookpot);
    add(imageOvenOn);
    add(imageHoodUI);
    add(imageHomeUI);
    add(imageOvenUI);
    add(mcuLoad);
}

void KitchenScreenViewBase::setupScreen()
{
    mcuLoad.initialize();
}

//Called when the screen is done with transition/load
void KitchenScreenViewBase::afterTransition()
{
    //InteractionEnterScreen
    //When screen is entered call virtual function
    //Call startGlowAnimaton
    startGlowAnimaton();
}

void KitchenScreenViewBase::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &buttoHallway)
    {
        //GoToHallway
        //When buttoHallway clicked change screen to HallwayScreen
        //Go to HallwayScreen with screen transition towards West
        application().gotoHallwayScreenScreenSlideTransitionWest();
    }
    else if (&src == &buttonExtractorHood)
    {
        //InteractionEnterExtractorHood
        //When buttonExtractorHood clicked change screen to ExtractorHoodScreen
        //Go to ExtractorHoodScreen with no screen transition
        application().gotoExtractorHoodScreenScreenNoTransition();
    }
    else if (&src == &buttonThermo)
    {
        //InteractionGoToHomeControl
        //When buttonThermo clicked change screen to HomeControlScreen
        //Go to HomeControlScreen with no screen transition
        application().gotoHomeControlScreenScreenNoTransition();
    }
}

void KitchenScreenViewBase::flexButtonCallbackHandler(const touchgfx::AbstractButtonContainer& src)
{
    if (&src == &buttonOven)
    {
        //InteractionGoToOven
        //When buttonOven clicked change screen to OvenScreen
        //Go to OvenScreen with no screen transition
        application().gotoOvenScreenScreenNoTransition();
    }
}
