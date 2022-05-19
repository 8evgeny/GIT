/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/containers/HomeUISplashContainerBase.hpp>
#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>

HomeUISplashContainerBase::HomeUISplashContainerBase()
{
    setWidth(480);
    setHeight(272);

    background.setXY(0, 0);
    background.setBitmap(touchgfx::Bitmap(BITMAP_HOMECONTROL_UI_BG_ID));

    textAreaTemp.setXY(190, 88);
    textAreaTemp.setColor(touchgfx::Color::getColorFrom24BitRGB(52, 75, 101));
    textAreaTemp.setLinespacing(0);
    Unicode::snprintf(textAreaTempBuffer, TEXTAREATEMP_SIZE, "%s", touchgfx::TypedText(T_SINGLEUSEID111).getText());
    textAreaTemp.setWildcard(textAreaTempBuffer);
    textAreaTemp.resizeToCurrentText();
    textAreaTemp.setTypedText(touchgfx::TypedText(T_SINGLEUSEID59));

    textAreaTempUnit.setXY(230, 78);
    textAreaTempUnit.setColor(touchgfx::Color::getColorFrom24BitRGB(52, 75, 101));
    textAreaTempUnit.setLinespacing(0);
    textAreaTempUnit.setTypedText(touchgfx::TypedText(T_SINGLEUSEID60));

    textAreaHeadline.setXY(173, 19);
    textAreaHeadline.setColor(touchgfx::Color::getColorFrom24BitRGB(52, 75, 101));
    textAreaHeadline.setLinespacing(0);
    textAreaHeadline.setTypedText(touchgfx::TypedText(T_HOMEUISPLASHBATHROOM));

    textAreaBottomline.setPosition(193, 188, 100, 19);
    textAreaBottomline.setColor(touchgfx::Color::getColorFrom24BitRGB(52, 75, 101));
    textAreaBottomline.setLinespacing(0);
    textAreaBottomline.setTypedText(touchgfx::TypedText(T_SINGLEUSEID110));

    textAreaWakeUp.setXY(120, 223);
    textAreaWakeUp.setColor(touchgfx::Color::getColorFrom24BitRGB(90, 118, 146));
    textAreaWakeUp.setLinespacing(0);
    textAreaWakeUp.setTypedText(touchgfx::TypedText(T_SINGLEUSEID62));

    add(background);
    add(textAreaTemp);
    add(textAreaTempUnit);
    add(textAreaHeadline);
    add(textAreaBottomline);
    add(textAreaWakeUp);
}

void HomeUISplashContainerBase::initialize()
{
	
}
