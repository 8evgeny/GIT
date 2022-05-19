/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef EXTRACTORHOODSCREENVIEWBASE_HPP
#define EXTRACTORHOODSCREENVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/extractorhoodscreen_screen/ExtractorHoodScreenPresenter.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/containers/buttons/Buttons.hpp>
#include <gui/containers/FanKnob.hpp>
#include <gui/containers/FpsMcuLoad.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <touchgfx/mixins/MoveAnimator.hpp>

class ExtractorHoodScreenViewBase : public touchgfx::View<ExtractorHoodScreenPresenter>
{
public:
    ExtractorHoodScreenViewBase();
    virtual ~ExtractorHoodScreenViewBase() {}
    virtual void setupScreen();

    /*
     * Virtual Action Handlers
     */
    virtual void lightButtonClicked()
    {
        // Override and implement this function in ExtractorHoodScreen
    }

    virtual void fanButttonClicked()
    {
        // Override and implement this function in ExtractorHoodScreen
    }

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Image background;
    touchgfx::Button buttoKitchen;
    touchgfx::Image fanBg;
    touchgfx::Image gitter;
    touchgfx::MoveAnimator< touchgfx::Image > imageFanBtn;
    touchgfx::MoveAnimator< touchgfx::Image > imageLightBtn;
    touchgfx::ImageButtonStyle< touchgfx::ToggleButtonTrigger > flexButtonFan;
    touchgfx::ImageButtonStyle< touchgfx::ToggleButtonTrigger > flexButtonLight;
    FanKnob fanKnob;
    FpsMcuLoad fpsMcu;

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<ExtractorHoodScreenViewBase, const touchgfx::AbstractButton&> buttonCallback;
    touchgfx::Callback<ExtractorHoodScreenViewBase, const touchgfx::AbstractButtonContainer&> flexButtonCallback;

    /*
     * Callback Handler Declarations
     */
    void buttonCallbackHandler(const touchgfx::AbstractButton& src);
    void flexButtonCallbackHandler(const touchgfx::AbstractButtonContainer& src);

};

#endif // EXTRACTORHOODSCREENVIEWBASE_HPP
