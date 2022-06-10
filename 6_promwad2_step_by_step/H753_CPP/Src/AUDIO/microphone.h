/*!
 \file microphone.h

*/

#ifndef MICROPHONE_H
#define MICROPHONE_H

#include "tlv320aic3254.h"

/*!
 \brief This class is used to control the microphone

 \class Microphone microphone.h "microphone.h"
*/
class Microphone
{
public:
    /*!
     \brief Base constructor. A default constructor

     \fn Microphone
    */
    Microphone() = default;
    /*!
     \brief The method is used to turn on the microphone

     \fn start
    */
    void start();
    /*!
     \brief The method id used to turn off the microphone

     \fn stop
    */
    void stop();

    /*!
     \brief The enumeration contains identifiers of the microphone state

     \enum micStatus
    */
    enum class micStatus {
        ON = 1,
        OFF = 2
    };

    micStatus status = micStatus::OFF; /*!< The variable stores the microphone state */

};

#endif // MICROPHONE_H
