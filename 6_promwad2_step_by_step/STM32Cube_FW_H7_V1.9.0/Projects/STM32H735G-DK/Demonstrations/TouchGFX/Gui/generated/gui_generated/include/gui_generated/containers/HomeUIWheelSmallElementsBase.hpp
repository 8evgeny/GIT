/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef HOMEUIWHEELSMALLELEMENTSBASE_HPP
#define HOMEUIWHEELSMALLELEMENTSBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <touchgfx/mixins/FadeAnimator.hpp>

class HomeUIWheelSmallElementsBase : public touchgfx::Container
{
public:
    HomeUIWheelSmallElementsBase();
    virtual ~HomeUIWheelSmallElementsBase() {}
    virtual void initialize();

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::FadeAnimator< touchgfx::TextArea > textAreaBottomline;
    touchgfx::FadeAnimator< touchgfx::TextAreaWithOneWildcard > textAreaDevices;
    touchgfx::FadeAnimator< touchgfx::TextArea > textAreaRoom;
    touchgfx::FadeAnimator< touchgfx::Image > imageSlider;
    touchgfx::FadeAnimator< touchgfx::TextArea > textAreaDegree;

    /*
     * Wildcard Buffers
     */
    static const uint16_t TEXTAREADEVICES_SIZE = 3;
    touchgfx::Unicode::UnicodeChar textAreaDevicesBuffer[TEXTAREADEVICES_SIZE];

private:

};

#endif // HOMEUIWHEELSMALLELEMENTSBASE_HPP
