/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/containers/trackSelectorBase.hpp>
#include "BitmapDatabase.hpp"

trackSelectorBase::trackSelectorBase() :
    buttonCallback(this, &trackSelectorBase::buttonCallbackHandler),
    updateItemCallback(this, &trackSelectorBase::updateItemCallbackHandler),
    shuffelPressedCallback(0),
    repeatPressedCallback(0),
    trackPlayPausedPressedCallback(0)
{
    setWidth(250);
    setHeight(190);

    albumBG.setXY(0, 0);
    albumBG.setBitmap(touchgfx::Bitmap(BITMAP_TRACK_LIST_CONTAINER_ID));

    trackList.setPosition(21, 40, 202, 132);
    trackList.setHorizontal(false);
    trackList.setCircular(false);
    trackList.setEasingEquation(touchgfx::EasingEquations::backEaseOut);
    trackList.setSwipeAcceleration(10);
    trackList.setDragAcceleration(10);
    trackList.setNumberOfItems(10);
    trackList.setPadding(7, 0);
    trackList.setSnapping(false);
    trackList.setDrawableSize(36, 0);
    trackList.setDrawables(trackListListItems, updateItemCallback);

    gradientTop.setXY(20, 40);
    gradientTop.setBitmap(touchgfx::Bitmap(BITMAP_SHADOW_LIST_TOP_ID));

    gradientButtom.setXY(20, 163);
    gradientButtom.setBitmap(touchgfx::Bitmap(BITMAP_SHADOW_LIST_BOTTOM_ID));

    shuffelButton.setXY(106, 11);
    shuffelButton.setBitmaps(touchgfx::Bitmap(BITMAP_ICON_SHUFFLE_OFF_ID), touchgfx::Bitmap(BITMAP_ICON_SHUFFLE_ON_ID));
    shuffelButton.setAction(buttonCallback);

    repeatButton.setXY(150, 11);
    repeatButton.setBitmaps(touchgfx::Bitmap(BITMAP_ICON_REPEAT_OFF_ID), touchgfx::Bitmap(BITMAP_ICON_REPEAT_ON_ID));
    repeatButton.setAction(buttonCallback);

    add(albumBG);
    add(trackList);
    add(gradientTop);
    add(gradientButtom);
    add(shuffelButton);
    add(repeatButton);
}

void trackSelectorBase::initialize()
{
    trackList.initialize();
    for (int i = 0; i < trackListListItems.getNumberOfDrawables(); i++)
    {
        trackListListItems[i].initialize();
    }	
}

void trackSelectorBase::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &shuffelButton)
    {
        //shufflePressed
        //When shuffelButton clicked emit shuffelPressed callback
        //Emit callback
        emitShuffelPressedCallback(shuffelButton.getState());
    }
    else if (&src == &repeatButton)
    {
        //repeatPressed
        //When repeatButton clicked emit repeatPressed callback
        //Emit callback
        emitRepeatPressedCallback(repeatButton.getState());
    }
}

void trackSelectorBase::updateItemCallbackHandler(touchgfx::DrawableListItemsInterface* items, int16_t containerIndex, int16_t itemIndex)
{
    if (items == &trackListListItems)
    {
        touchgfx::Drawable* d = items->getDrawable(containerIndex);
        track* cc = (track*)d;
        trackListUpdateItem(*cc, itemIndex);
    }
}
