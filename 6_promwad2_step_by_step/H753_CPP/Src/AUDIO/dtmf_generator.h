/*!
 \file dtmf_generator.h

*/

#ifndef DTMF_GENERATOR_H
#define DTMF_GENERATOR_H
#include <cstdint>
#include  <cmath>
#include <cstdlib>

/*!
 \brief This class is used to generate a DTMF tone

 \class DTMF dtmf_generator.h "dtmf_generator.h"
*/
class DTMF
{
public:
    /*!
     \brief Base constructor. A default constructor.

     \fn DTMF
    */
    DTMF() = default;

    /*!
     \brief Copy constructor. A default constructor.

    \fn DTMF
     \param
            */
    DTMF(const DTMF&) = default;

    /*!
     \brief Copy assignment operator.

    \fn operator =
     \param
     \return DTMF &operator
        */
    DTMF& operator=(DTMF&) = default;

public:

    /*!
     \brief Enumeration that stores a list of identifiers to control updating data in a buff.

     \enum Control
    */
    enum class Control {
        FIRST_HALF,
        SECOND_HALF,
        FULL
    };

    /*!
     \brief Enumeration that stores a list of identifiers to get states about the DTMF generator.

     \enum Status
    */
    enum class Status {
        IDLE         = 0x00U,
        READY        = 0x01U,
        START        = 0x02U,
        ERROR        = 0x04U,
        STOPPED      = 0x05U,
    };

    /*!
     \brief The function creates samples of a signal and puts those in a buff.

     \fn getData
     \param Control is the argument to control creating process
     \return Buff is the pointer to the signal data buff.
    */
    uint16_t* getData(Control);
    /*!
     \brief The function is used to get frequency values depending on the key number.

     \fn getFrequency
     \param digit is the argument to send a key number to the function
     \return Status is the status about the DTMF generator
    */
    Status getFrequency(uint8_t digit);

    constexpr static int TABLESIZE {640};   /*!< size of look up table */
    Status status = Status::IDLE;           /*!< The variable stores a status about the DTMF generator */

private:

    constexpr static float M_PI       {3.14159265358979323846}, /*!< The identifier stores the PI number value */
              SAMPLING_FREQ {16000},                             /*!< The identifier stores the sampling frequency */
              STEP_770 {(float)(770 * TABLESIZE)/SAMPLING_FREQ}, /*!< The identifier stores the sampling step from the look up table */
              STEP_1336 {(float)(1336 * TABLESIZE)/SAMPLING_FREQ}, /*!< The identifier stores the sampling step from the look up table */
              STEP_697 {(float)(697 * TABLESIZE)/SAMPLING_FREQ}, /*!< The identifier stores the sampling step from the look up table */
              STEP_852 {(float)(852 * TABLESIZE)/SAMPLING_FREQ}, /*!< The identifier stores the sampling step from the look up table */
              STEP_941 {(float)(941 * TABLESIZE)/SAMPLING_FREQ}, /*!< The identifier stores the sampling step from the look up table */
              STEP_1209 {(float)(1209 * TABLESIZE)/SAMPLING_FREQ}, /*!< The identifier stores the sampling step from the look up table */
              STEP_1477 {(float)(1477 * TABLESIZE)/SAMPLING_FREQ}, /*!< The identifier stores the sampling step from the look up table */
              STEP_1633 {(float)(1633 * TABLESIZE)/SAMPLING_FREQ};/*!< The identifier stores the sampling step from the look up table */

    uint16_t sineTable[TABLESIZE] = {0}; /*!< look up table */
    uint16_t buff[TABLESIZE*2] = {0}; /*!< The buffer is used to stores generated signal samples */
    float loopIndexLow = 0.0, /*!< The variable stores an low frequency index which is used to create a signal sample */
          loopIndexHigh = 0.0, /*!< The variable stores an high frequency index which is used to create a signal sample */
          lowToneFrequency = 0.0, /*!< The variable stores an sampling step identifier for low frequency  */
          highToneFrequency = 0.0; /*!< The variable stores an sampling step identifier for high frequency */

};


#endif // DTMF_GENERATOR_H
