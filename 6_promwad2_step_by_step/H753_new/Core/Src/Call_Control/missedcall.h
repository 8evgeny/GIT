#ifndef MISSEDCALL_H
#define MISSEDCALL_H

#include "cstdint"
#include "vector"
#include "main.h"
#include "cmsis_os.h"

struct MissedCallData {
    uint8_t key = 0;        /*!< The variable stores a missed call key value */
    uint16_t conter = 0;    /*!< The variable stores conter value to compare with MAX_COUNT and while conter is less than MAX_COUNT a key is staying into the missed container */
};

class MissedCall
{
public:

    /*!
     \brief Base constructor.

     \fn MissedCall
    */
    MissedCall();

    /*!
     \brief The function adds a missed call to the buffer and lights a missed call LED.

    \fn add
     \param Number of the key
            */
    void add(const uint8_t);
    /*!
     \brief The function removes a missed calls from the buffer.

    \fn remove
    \param Number of the key
     \return Status of the operation
        */
    bool remove(const uint8_t);

    /*!
     \brief The function looks for a missed calls in the buffer.

    \fn seek
    \param Number of the key
    \return Status of the operation
        */
    bool seek(const uint8_t);

    /*!
     \brief This method checks a counter value and if that is MAX_COUNT then missed call is removed from a missed call container.

     \fn handleAutoReset
    */
    void handleAutoReset();

    bool isMissedKey = false;     /*!< The flag of the missed call */
    std::vector <MissedCallData> missedCalls;   /*!< The container that stores missed calls */
    std::vector <MissedCallData>::iterator i;     /*!< The iterator of the missed call container */

    constexpr static uint16_t MAX_COUNT {60}; /*!< This conter stores a value of the delay in the milliseconds */


};

#ifdef __cplusplus
extern "C" {
#endif

/*!
 \brief This thread calls handleAutoReset() method once per second.

 \fn missedCallThread
 \param arg is not used
*/
[[noreturn]]
void missedCallThread(void const *arg);

#ifdef __cplusplus
}
#endif

#endif // MISSEDCALL_H
