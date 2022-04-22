#include "dtmf_generator.h"

DTMF::Status DTMF::getFrequency(uint8_t digit)
{
    if (status == Status::IDLE) {

        switch(digit) {
        case 1:
        case 2:
        case 3:
            lowToneFrequency =  STEP_697;
            break;
        case 4:
        case 5:
        case 6:
            lowToneFrequency =  STEP_770;
            break;
        case 7:
        case 8:
        case 9:
            lowToneFrequency =  STEP_852;
            break;
        case 0:
            lowToneFrequency =  STEP_941;
            break;
        }

        switch(digit) {
        case 1:
        case 4:
        case 7:
            highToneFrequency =  STEP_1209;
            break;
        case 2:
        case 5:
        case 8:
        case 0:
            highToneFrequency =  STEP_1336;
            break;
        case 3:
        case 6:
        case 9:
            highToneFrequency =  STEP_1477;
            break;
        }

        for (uint16_t i = 0; i < TABLESIZE; i++) {
            sineTable[i] = 32767 + (int16_t)(1000.0*sin(2.0*M_PI*i/ (TABLESIZE * 1.0)));
        }

        status = Status::READY;
    }
    return status;
}


uint16_t* DTMF::getData(Control control)
{

    uint16_t j = 0,
             size = 0;

    switch (control) {
    case Control::FIRST_HALF:
        j = 0;
        size = TABLESIZE/2;
        break;
    case Control::SECOND_HALF:
        j = TABLESIZE;
        size = TABLESIZE/2;
        break;
    case Control::FULL:
        j = 0;
        size = TABLESIZE;
        break;
    }

    for(uint32_t i = 0; i < size; i++) {

        buff[j+1] = buff[j] = (sineTable[static_cast<uint16_t>(loopIndexLow)] + sineTable[static_cast<uint16_t>(loopIndexHigh)]);
        j = j+2;

        loopIndexLow += lowToneFrequency;
        if (loopIndexLow > static_cast<float>(TABLESIZE))
            loopIndexLow -= static_cast<float>(TABLESIZE);

        loopIndexHigh += highToneFrequency;
        if (loopIndexHigh > static_cast<float>(TABLESIZE))
            loopIndexHigh -= static_cast<float>(TABLESIZE);
    }

    status = Status::START;
    return buff;
}
