/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/containers/forecastBarContainerBase.hpp>

forecastBarContainerBase::forecastBarContainerBase()
{
    setWidth(800);
    setHeight(54);

    forecastDay1.setXY(58, 0);

    forecastDay2.setXY(203, 0);

    forecastDay3.setXY(350, 0);

    forecastDay4.setXY(496, 0);

    forecastDay5.setXY(641, 0);

    add(forecastDay1);
    add(forecastDay2);
    add(forecastDay3);
    add(forecastDay4);
    add(forecastDay5);
}

void forecastBarContainerBase::initialize()
{
    forecastDay1.initialize();
    forecastDay2.initialize();
    forecastDay3.initialize();
    forecastDay4.initialize();
    forecastDay5.initialize();	
}
