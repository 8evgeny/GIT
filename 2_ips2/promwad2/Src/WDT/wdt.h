/*!
 * \file wdt.h
 *
 */

#pragma once

#include <atomic>
#ifdef STM32F777xx
#include "stm32f7xx_hal.h"
#else
#include "stm32f1xx_hal.h"
#endif
#include "cmsis_os.h"
#include "../Debug/debug.h"

/**
 * \brief Implementation: ensure that only one instance of the singleton class ever exists
        and provide global access to that instance.
 *
 * \class WDT wdt.h "wdt.h"
 */
class WDT
{
private:

    static WDT * pInstance; /**< The static pointer to the WDT class */

    /*!
     \brief Base constructor. A default constructor.

     \fn WDT
    */
    WDT();

    /*!
     \brief Copy constructor.

     \fn WDT
     \param
    */
    WDT(const WDT&);

    /*!
     \brief Copy assignment operator

     \fn operator =
     \param
     \return WDT &operator
    */
    WDT& operator=(WDT&);
public:


    /*!
     \brief It returns the sole instance of the class.

     \fn getInstance
     \return WDT. The public static operation can be accessed easily by using the class name and operation name.
    */
    static WDT * getInstance();

    /**
     * \brief The function sets time of restart timer
     *
     * \fn SetDelayTime
     * \param time
     */
    void SetDelayTime(uint32_t time);

    /**
     * \brief The function that starts up the WWDG
     *
     * \fn Start
     */
    void Start();

    /**
     * \brief The function that stops the WWDG
     *
     * \fn Stop
     */
    void Stop();

    /**
     * \brief The function that restarts the WWDG timer
     *
     * \fn refresh
     */
    void refresh();

    /**
     * \brief The function checks if the WWDG is ranning and
     *       restarts the WWDG timer
     *
     * \fn Loop
     */
    void Loop();

    std::atomic<uint32_t> time_; /**< The variable stores time of restart timer  */
    std::atomic<bool> running_; /**< The variable stores flag of the WWDG timer state */

    WWDG_HandleTypeDef *wwdgtHandle; /**< The variable is pointer to WWDG Handle Type */

};

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief The function initializes the WWDG
 *
 * \fn WDTInit
 */
void WDTInit(void);
void StartWdtThread(void const *argument);
#ifdef __cplusplus
}
#endif
