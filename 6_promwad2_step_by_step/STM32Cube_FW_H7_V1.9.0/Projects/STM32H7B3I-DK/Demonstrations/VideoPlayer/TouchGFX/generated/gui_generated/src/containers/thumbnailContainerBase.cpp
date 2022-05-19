/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/containers/thumbnailContainerBase.hpp>
#include "BitmapDatabase.hpp"

thumbnailContainerBase::thumbnailContainerBase() :
    buttonCallback(this, &thumbnailContainerBase::buttonCallbackHandler),
    flexButtonCallback(this, &thumbnailContainerBase::flexButtonCallbackHandler),
    itemSelectedCallback(0),
    playPausePressedCallback(0)
{
    setWidth(150);
    setHeight(91);

    thumbnailBitmap.setXY(0, 0);

    selectItemOverlay.setXY(0, 0);
    selectItemOverlay.setBitmaps(touchgfx::Bitmap(BITMAP_VIDEO_CLIP_OVERLAY_ID), touchgfx::Bitmap(BITMAP_VIDEO_CLIP_OVERLAY_ID));
    selectItemOverlay.setAction(buttonCallback);

    playPauseOverlay.setBitmaps(Bitmap(BITMAP_OVERLAY_NO_ICON_ID), Bitmap(BITMAP_OVERLAY_NO_ICON_ID));
    playPauseOverlay.setBitmapXY(0, 0);
    playPauseOverlay.setIconBitmaps(Bitmap(BITMAP_PLAY_ID), Bitmap(BITMAP_PAUSE_ID));
    playPauseOverlay.setIconXY(125, 68);
    playPauseOverlay.setPosition(0, 0, 150, 91);
    playPauseOverlay.setVisible(false);
    playPauseOverlay.setAction(flexButtonCallback);

    add(thumbnailBitmap);
    add(selectItemOverlay);
    add(playPauseOverlay);
}

void thumbnailContainerBase::initialize()
{
	
}

void thumbnailContainerBase::itemSelectedPressed(const touchgfx::AbstractButton& value)
{

}

void thumbnailContainerBase::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &selectItemOverlay)
    {
        //itemSelected
        //When selectItemOverlay clicked call itemSelectedPressed on thumbnailContainer
        //Call itemSelectedPressed
        itemSelectedPressed(src);
    }
}

void thumbnailContainerBase::flexButtonCallbackHandler(const touchgfx::AbstractButtonContainer& src)
{
    if (&src == &playPauseOverlay)
    {
        //playPausePressed
        //When playPauseOverlay clicked emit playPausePressed callback
        //Emit callback
        emitPlayPausePressedCallback(playPauseOverlay.getPressed());
    }
}
