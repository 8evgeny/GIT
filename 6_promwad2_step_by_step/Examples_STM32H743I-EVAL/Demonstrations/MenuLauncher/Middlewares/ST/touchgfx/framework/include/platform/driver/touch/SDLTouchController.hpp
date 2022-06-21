/**
  ******************************************************************************
  * This file is part of the TouchGFX 4.12.3 distribution.
  *
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#ifndef SDLTOUCHCONTROLLER_HPP
#define SDLTOUCHCONTROLLER_HPP

#include <platform/driver/touch/TouchController.hpp>

namespace touchgfx
{
/**
 * @class SDLTouchController SDLTouchController.hpp platform/driver/touch/SDLTouchController.hpp
 *
 * @brief TouchController for the simulator.
 *
 *        TouchController for the simulator.
 *
 * @sa TouchController
 */
class SDLTouchController : public TouchController
{
public:
    /**
     * @fn virtual void SDLTouchController::init();
     *
     * @brief Initializes touch controller.
     *
     *        Initializes touch controller.
     */
    virtual void init();

    /**
     * @fn virtual bool SDLTouchController::sampleTouch(int32_t& x, int32_t& y);
     *
     * @brief Checks whether the touch screen is being touched, and if so, what coordinates.
     *
     *        Checks whether the touch screen is being touched, and if so, what coordinates.
     *
     * @param [out] x The x position of the touch
     * @param [out] y The y position of the touch
     *
     * @return True if a touch has been detected, otherwise false.
     */
    virtual bool sampleTouch(int32_t& x, int32_t& y);
};

} // namespace touchgfx

#endif // SDLTOUCHCONTROLLER_HPP
