/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef WASHERSCREENVIEWBASE_HPP
#define WASHERSCREENVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/washerscreen_screen/WasherScreenPresenter.hpp>
#include <gui/containers/WasherContainer.hpp>
#include <gui/containers/WasherInstructionContainer.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/canvas_widget_renderer/CanvasWidgetRenderer.hpp>

class WasherScreenViewBase : public touchgfx::View<WasherScreenPresenter>
{
public:
    WasherScreenViewBase();
    virtual ~WasherScreenViewBase() {}
    virtual void setupScreen();

    /*
     * Virtual Action Handlers
     */
    virtual void buttonInstructionsClicked()
    {
        // Override and implement this function in WasherScreen
    }

    virtual void backButtonClicked()
    {
        // Override and implement this function in WasherScreen
    }

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    WasherContainer washerContainer;
    WasherInstructionContainer washerInstructionContainer;
    touchgfx::Button buttonInstructions;
    touchgfx::Button buttonBack;
    touchgfx::Button buttonBath;

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<WasherScreenViewBase, const touchgfx::AbstractButton&> buttonCallback;

    /*
     * Callback Handler Declarations
     */
    void buttonCallbackHandler(const touchgfx::AbstractButton& src);

    /*
     * Canvas Buffer Size
     */
    static const uint16_t CANVAS_BUFFER_SIZE = 14000;
    uint8_t canvasBuffer[CANVAS_BUFFER_SIZE];

};

#endif // WASHERSCREENVIEWBASE_HPP
