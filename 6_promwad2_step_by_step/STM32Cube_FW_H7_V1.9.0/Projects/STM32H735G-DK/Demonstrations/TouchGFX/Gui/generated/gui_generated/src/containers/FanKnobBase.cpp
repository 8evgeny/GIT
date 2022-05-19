/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/containers/FanKnobBase.hpp>
#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>

FanKnobBase::FanKnobBase()
{
    setWidth(480);
    setHeight(80);

    containerKnob.setPosition(61, 0, 80, 80);

    imageKnob.setXY(0, 0);
    imageKnob.setBitmap(touchgfx::Bitmap(BITMAP_SPPED_KNOB_ID));
    containerKnob.add(imageKnob);

    textAreaKnob.setPosition(10, 17, 60, 45);
    textAreaKnob.setColor(touchgfx::Color::getColorFrom24BitRGB(57, 57, 57));
    textAreaKnob.setLinespacing(0);
    Unicode::snprintf(textAreaKnobBuffer, TEXTAREAKNOB_SIZE, "%s", touchgfx::TypedText(T_SINGLEUSEID46).getText());
    textAreaKnob.setWildcard(textAreaKnobBuffer);
    textAreaKnob.setTypedText(touchgfx::TypedText(T_SINGLEUSEID45));
    containerKnob.add(textAreaKnob);

    add(containerKnob);
}

void FanKnobBase::initialize()
{
	
}
