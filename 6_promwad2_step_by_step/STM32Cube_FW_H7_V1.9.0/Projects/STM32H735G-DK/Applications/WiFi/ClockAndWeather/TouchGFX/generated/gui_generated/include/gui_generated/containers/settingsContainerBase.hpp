/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef SETTINGSCONTAINERBASE_HPP
#define SETTINGSCONTAINERBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/ToggleButton.hpp>
#include <touchgfx/containers/ListLayout.hpp>

#include <touchgfx/containers/buttons/Buttons.hpp>
class settingsContainerBase : public touchgfx::Container
{
public:
    settingsContainerBase();
    virtual ~settingsContainerBase() {}
    virtual void initialize();

    /*
     * Custom Trigger Callback Setters
     */
    void setClockModeButtonClickedCallback(touchgfx::GenericCallback<bool>& callback)
    {
        this->clockModeButtonClickedCallback = &callback;
    }
    void setTemperatureUnitButtonClickedCallback(touchgfx::GenericCallback<bool>& callback)
    {
        this->temperatureUnitButtonClickedCallback = &callback;
    }
    void setBackButonClickedCallback(touchgfx::GenericCallback<>& callback)
    {
        this->backButonClickedCallback = &callback;
    }
    void setChangeCityCallback(touchgfx::GenericCallback<>& callback)
    {
        this->changeCityCallback = &callback;
    }
    void setWifiMenuSelectedCallback(touchgfx::GenericCallback<>& callback)
    {
        this->wifiMenuSelectedCallback = &callback;
    }

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Custom Trigger Emitters
     */
    virtual void emitClockModeButtonClickedCallback(bool value)
    {
        if (clockModeButtonClickedCallback && clockModeButtonClickedCallback->isValid())
        {
            this->clockModeButtonClickedCallback->execute(value);
        }
    }
    virtual void emitTemperatureUnitButtonClickedCallback(bool value)
    {
        if (temperatureUnitButtonClickedCallback && temperatureUnitButtonClickedCallback->isValid())
        {
            this->temperatureUnitButtonClickedCallback->execute(value);
        }
    }
    virtual void emitBackButonClickedCallback()
    {
        if (backButonClickedCallback && backButonClickedCallback->isValid())
        {
            this->backButonClickedCallback->execute();
        }
    }
    virtual void emitChangeCityCallback()
    {
        if (changeCityCallback && changeCityCallback->isValid())
        {
            this->changeCityCallback->execute();
        }
    }
    virtual void emitWifiMenuSelectedCallback()
    {
        if (wifiMenuSelectedCallback && wifiMenuSelectedCallback->isValid())
        {
            this->wifiMenuSelectedCallback->execute();
        }
    }

    /*
     * Member Declarations
     */
    touchgfx::Image bgGradient;
    touchgfx::Image bgGradient_1;
    touchgfx::TextArea preferencesHeadline;
    touchgfx::TextArea units;
    touchgfx::TextArea CelFah;
    touchgfx::ToggleButton temperatureUnitButton;
    touchgfx::ToggleButton clockModeButton;
    touchgfx::ListLayout wifiList;
    touchgfx::TextArea timeSettings;
    touchgfx::TextArea timeSettingsSmall;
    touchgfx::TextArea wifiHeadline;
    touchgfx::TextButtonStyle< touchgfx::ClickButtonTrigger > moreNetworksButton;

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<settingsContainerBase, const touchgfx::AbstractButton&> buttonCallback;
    touchgfx::Callback<settingsContainerBase, const touchgfx::AbstractButtonContainer&> flexButtonCallback;

    /*
     * Custom Trigger Callback Declarations
     */
    touchgfx::GenericCallback<bool>* clockModeButtonClickedCallback;
    touchgfx::GenericCallback<bool>* temperatureUnitButtonClickedCallback;
    touchgfx::GenericCallback<>* backButonClickedCallback;
    touchgfx::GenericCallback<>* changeCityCallback;
    touchgfx::GenericCallback<>* wifiMenuSelectedCallback;

    /*
     * Callback Handler Declarations
     */
    void buttonCallbackHandler(const touchgfx::AbstractButton& src);
    void flexButtonCallbackHandler(const touchgfx::AbstractButtonContainer& src);

};

#endif // SETTINGSCONTAINERBASE_HPP
