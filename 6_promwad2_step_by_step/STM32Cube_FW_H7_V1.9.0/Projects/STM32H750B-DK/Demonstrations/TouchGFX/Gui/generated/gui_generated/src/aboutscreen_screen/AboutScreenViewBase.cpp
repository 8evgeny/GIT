/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/aboutscreen_screen/AboutScreenViewBase.hpp>
#include <touchgfx/Color.hpp>
#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>

AboutScreenViewBase::AboutScreenViewBase() :
    buttonCallback(this, &AboutScreenViewBase::buttonCallbackHandler),
    updateItemCallback(this, &AboutScreenViewBase::updateItemCallbackHandler)
{
    scrollableContainerAbout.setPosition(0, 0, 480, 272);
    scrollableContainerAbout.enableHorizontalScroll(false);
    scrollableContainerAbout.setScrollbarsColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));

    backgroundBox.setPosition(0, 0, 480, 1485);
    backgroundBox.setColor(touchgfx::Color::getColorFrom24BitRGB(218, 212, 212));
    scrollableContainerAbout.add(backgroundBox);

    backgroudImage.setXY(0, 0);
    backgroudImage.setBitmap(touchgfx::Bitmap(BITMAP_BGTOP_ID));
    scrollableContainerAbout.add(backgroudImage);

    buttonHallway.setXY(0, 0);
    buttonHallway.setBitmaps(touchgfx::Bitmap(BITMAP_INFOBACKBUTTON_ID), touchgfx::Bitmap(BITMAP_INFOBACKBUTTON_ID));
    buttonHallway.setAction(buttonCallback);
    scrollableContainerAbout.add(buttonHallway);

    boxVideoSeperator2.setPosition(0, 543, 480, 1);
    boxVideoSeperator2.setColor(touchgfx::Color::getColorFrom24BitRGB(163, 159, 158));
    scrollableContainerAbout.add(boxVideoSeperator2);

    boxVideoSeperator1.setPosition(0, 272, 480, 1);
    boxVideoSeperator1.setColor(touchgfx::Color::getColorFrom24BitRGB(163, 159, 158));
    scrollableContainerAbout.add(boxVideoSeperator1);

    imageVideoBackground.setXY(0, 273);
    imageVideoBackground.setBitmap(touchgfx::Bitmap(BITMAP_VIDEOBG_ID));
    scrollableContainerAbout.add(imageVideoBackground);

    imageLaptopCreative.setXY(28, 671);
    imageLaptopCreative.setBitmap(touchgfx::Bitmap(BITMAP_LAPTOP_ID));
    scrollableContainerAbout.add(imageLaptopCreative);

    buttonPlayVideo.setXY(198, 374);
    buttonPlayVideo.setBitmaps(touchgfx::Bitmap(BITMAP_PLAY_ABOUT_VIDEO_ICON_ID), touchgfx::Bitmap(BITMAP_PLAY_ABOUT_VIDEO_ICON_ID));
    buttonPlayVideo.setAction(buttonCallback);
    scrollableContainerAbout.add(buttonPlayVideo);

    imageLaptopRocket.setXY(28, 1125);
    imageLaptopRocket.setBitmap(touchgfx::Bitmap(BITMAP_LAPTOPROCKET_ID));
    scrollableContainerAbout.add(imageLaptopRocket);

    imageLaptopMjolner.setXY(28, 1348);
    imageLaptopMjolner.setBitmap(touchgfx::Bitmap(BITMAP_LAPTOPMJOLNER_ID));
    scrollableContainerAbout.add(imageLaptopMjolner);

    imageSliderBg.setXY(0, 911);
    imageSliderBg.setBitmap(touchgfx::Bitmap(BITMAP_ABOUTSLIDER_ID));
    scrollableContainerAbout.add(imageSliderBg);

    textAreaGetStartedHeadline.setPosition(0, 562, 480, 45);
    textAreaGetStartedHeadline.setColor(touchgfx::Color::getColorFrom24BitRGB(54, 54, 54));
    textAreaGetStartedHeadline.setLinespacing(0);
    textAreaGetStartedHeadline.setTypedText(touchgfx::TypedText(T_SINGLEUSEID131));
    scrollableContainerAbout.add(textAreaGetStartedHeadline);

    textAreaGettingStarted.setPosition(0, 604, 480, 49);
    textAreaGettingStarted.setColor(touchgfx::Color::getColorFrom24BitRGB(54, 54, 54));
    textAreaGettingStarted.setLinespacing(0);
    textAreaGettingStarted.setTypedText(touchgfx::TypedText(T_SINGLEUSEID132));
    scrollableContainerAbout.add(textAreaGettingStarted);

    textAreaDesigerHead.setPosition(158, 665, 322, 27);
    textAreaDesigerHead.setColor(touchgfx::Color::getColorFrom24BitRGB(54, 54, 54));
    textAreaDesigerHead.setLinespacing(0);
    textAreaDesigerHead.setTypedText(touchgfx::TypedText(T_SINGLEUSEID134));
    scrollableContainerAbout.add(textAreaDesigerHead);

    textAreaDesigner1.setPosition(158, 692, 322, 101);
    textAreaDesigner1.setColor(touchgfx::Color::getColorFrom24BitRGB(54, 54, 54));
    textAreaDesigner1.setLinespacing(0);
    textAreaDesigner1.setTypedText(touchgfx::TypedText(T_SINGLEUSEID133));
    scrollableContainerAbout.add(textAreaDesigner1);

    textAreaDesigner2.setPosition(28, 795, 452, 101);
    textAreaDesigner2.setColor(touchgfx::Color::getColorFrom24BitRGB(54, 54, 54));
    textAreaDesigner2.setLinespacing(0);
    textAreaDesigner2.setTypedText(touchgfx::TypedText(T_SINGLEUSEID135));
    scrollableContainerAbout.add(textAreaDesigner2);

    designerAboutScrollWheel.setPosition(0, 922, 480, 38);
    designerAboutScrollWheel.setHorizontal(true);
    designerAboutScrollWheel.setCircular(true);
    designerAboutScrollWheel.setEasingEquation(touchgfx::EasingEquations::backEaseOut);
    designerAboutScrollWheel.setSwipeAcceleration(10);
    designerAboutScrollWheel.setDragAcceleration(10);
    designerAboutScrollWheel.setNumberOfItems(9);
    designerAboutScrollWheel.setSelectedItemOffset(150);
    designerAboutScrollWheel.setSelectedItemExtraSize(0, 0);
    designerAboutScrollWheel.setSelectedItemMargin(0, 0);
    designerAboutScrollWheel.setDrawableSize(180, 0);
    designerAboutScrollWheel.setDrawables(designerAboutScrollWheelListItems, updateItemCallback,
                              designerAboutScrollWheelSelectedListItems, updateItemCallback);
    designerAboutScrollWheel.animateToItem(0, 0);
    scrollableContainerAbout.add(designerAboutScrollWheel);

    boxSeperator1.setPosition(30, 1100, 420, 1);
    boxSeperator1.setColor(touchgfx::Color::getColorFrom24BitRGB(163, 159, 158));
    scrollableContainerAbout.add(boxSeperator1);

    textAreaAboutInfo.setPosition(30, 984, 420, 116);
    textAreaAboutInfo.setColor(touchgfx::Color::getColorFrom24BitRGB(54, 54, 54));
    textAreaAboutInfo.setLinespacing(0);
    textAreaAboutInfo.setTypedText(touchgfx::TypedText(T_ABOUTSTRUCTURETEXT));
    scrollableContainerAbout.add(textAreaAboutInfo);

    textAreaGetStartedHead.setPosition(158, 1119, 322, 27);
    textAreaGetStartedHead.setColor(touchgfx::Color::getColorFrom24BitRGB(54, 54, 54));
    textAreaGetStartedHead.setLinespacing(0);
    textAreaGetStartedHead.setTypedText(touchgfx::TypedText(T_SINGLEUSEID136));
    scrollableContainerAbout.add(textAreaGetStartedHead);

    textAreaGetStarted1.setPosition(158, 1151, 322, 108);
    textAreaGetStarted1.setColor(touchgfx::Color::getColorFrom24BitRGB(54, 54, 54));
    textAreaGetStarted1.setLinespacing(0);
    textAreaGetStarted1.setTypedText(touchgfx::TypedText(T_SINGLEUSEID137));
    scrollableContainerAbout.add(textAreaGetStarted1);

    textAreaGetStarted2.setPosition(28, 1250, 452, 51);
    textAreaGetStarted2.setColor(touchgfx::Color::getColorFrom24BitRGB(54, 54, 54));
    textAreaGetStarted2.setLinespacing(0);
    textAreaGetStarted2.setTypedText(touchgfx::TypedText(T_SINGLEUSEID139));
    scrollableContainerAbout.add(textAreaGetStarted2);

    boxSeperator2.setPosition(30, 1321, 420, 1);
    boxSeperator2.setColor(touchgfx::Color::getColorFrom24BitRGB(163, 159, 158));
    scrollableContainerAbout.add(boxSeperator2);

    textAreaGetAssistanceHead.setPosition(158, 1343, 322, 27);
    textAreaGetAssistanceHead.setColor(touchgfx::Color::getColorFrom24BitRGB(54, 54, 54));
    textAreaGetAssistanceHead.setLinespacing(0);
    textAreaGetAssistanceHead.setTypedText(touchgfx::TypedText(T_SINGLEUSEID140));
    scrollableContainerAbout.add(textAreaGetAssistanceHead);

    textAreaAssistance.setPosition(158, 1375, 322, 105);
    textAreaAssistance.setColor(touchgfx::Color::getColorFrom24BitRGB(54, 54, 54));
    textAreaAssistance.setLinespacing(0);
    textAreaAssistance.setTypedText(touchgfx::TypedText(T_SINGLEUSEID141));
    scrollableContainerAbout.add(textAreaAssistance);

    textAreaHyperlinkMjolner.setPosition(238, 1444, 322, 26);
    textAreaHyperlinkMjolner.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 114, 185));
    textAreaHyperlinkMjolner.setLinespacing(0);
    textAreaHyperlinkMjolner.setTypedText(touchgfx::TypedText(T_SINGLEUSEID142));
    scrollableContainerAbout.add(textAreaHyperlinkMjolner);

    MissingUSBMessage.setPosition(40, 419, 400, 35);
    MissingUSBMessage.setVisible(false);
    MissingUSBMessage.setColor(touchgfx::Color::getColorFrom24BitRGB(54, 54, 54));
    MissingUSBMessage.setLinespacing(0);
    MissingUSBMessage.setTypedText(touchgfx::TypedText(T_ABOUTINSERTUSB));
    scrollableContainerAbout.add(MissingUSBMessage);

    textAreaSTHyperlink.setPosition(350, 1273, 45, 25);
    textAreaSTHyperlink.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 114, 185));
    textAreaSTHyperlink.setLinespacing(0);
    textAreaSTHyperlink.setTypedText(touchgfx::TypedText(T_SINGLEUSEID150));
    scrollableContainerAbout.add(textAreaSTHyperlink);
    scrollableContainerAbout.setScrollbarsVisible(false);

    add(scrollableContainerAbout);
}

void AboutScreenViewBase::setupScreen()
{
    designerAboutScrollWheel.initialize();
    for (int i = 0; i < designerAboutScrollWheelListItems.getNumberOfDrawables(); i++)
    {
        designerAboutScrollWheelListItems[i].initialize();
    }
    for (int i = 0; i < designerAboutScrollWheelSelectedListItems.getNumberOfDrawables(); i++)
    {
        designerAboutScrollWheelSelectedListItems[i].initialize();
    }
}

void AboutScreenViewBase::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &buttonHallway)
    {
        //GoToHallway
        //When buttonHallway clicked change screen to HallwayScreen
        //Go to HallwayScreen with screen transition towards South
        application().gotoHallwayScreenScreenCoverTransitionSouth();
    }
    else if (&src == &buttonPlayVideo)
    {
        //PlayAboutVideo
        //When buttonPlayVideo clicked call virtual function
        //Call playAboutVideo
        playAboutVideo();
    }
}

void AboutScreenViewBase::updateItemCallbackHandler(touchgfx::DrawableListItemsInterface* items, int16_t containerIndex, int16_t itemIndex)
{
    if (items == &designerAboutScrollWheelListItems)
    {
        touchgfx::Drawable* d = items->getDrawable(containerIndex);
        AboutWheelElement* cc = (AboutWheelElement*)d;
        designerAboutScrollWheelUpdateItem(*cc, itemIndex);
    }
    else if (items == &designerAboutScrollWheelSelectedListItems)
    {
        touchgfx::Drawable* d = items->getDrawable(containerIndex);
        AboutWheelElement* cc = (AboutWheelElement*)d;
        designerAboutScrollWheelUpdateCenterItem(*cc, itemIndex);
    }
}
