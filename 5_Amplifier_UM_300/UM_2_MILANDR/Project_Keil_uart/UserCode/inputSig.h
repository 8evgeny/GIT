#pragma once
#include "main.h"
#include <memory>

//Команды  Stm -> Milandr
#define fromStm_BtnImpedance_On     'Q'
#define fromStm_BtnImpedance_Off    'W'
#define fromStm_BtnCalibr_On        'E'
#define fromStm_BtnCalibr_Off       'R'
#define fromStm_BtnReset_On         'T'
#define fromStm_BtnReset_Off        'Y'
#define fromStm_SignalPowerOn_ON    'U'
#define fromStm_SignalPowerOn_OFF   'I'
#define fromStm_SignalImpedanse_ON  'O'
#define fromStm_SignalImpedanse_OFF 'P'
#define fromStm_SignalTranslate_ON  'A'
#define fromStm_SignalTranslate_OFF 'S'
#define fromStm_SignalFromOut_ON    'D'
#define fromStm_SignalFromOut_OFF   'F'
#define fromStm_SignalMic_ON        'G'
#define fromStm_SignalMic_OFF       'H'
#define fromStm_BlankCommand        '&'

//Milandr -> Stm (подтверждение получения команды)
#define toStm_BtnImpedance_On_OK     'q'
#define toStm_BtnImpedance_Off_OK    'w'
#define toStm_BtnCalibr_On_OK        'e'
#define toStm_BtnCalibr_Off_OK       'r'
#define toStm_BtnReset_On_OK         't'
#define toStm_BtnReset_Off_OK        'y'
#define toStm_SignalPowerOn_ON_OK    'u'
#define toStm_SignalPowerOn_OFF_OK   'i'
#define toStm_SignalImpedanse_ON_OK  'o'
#define toStm_SignalImpedanse_OFF_OK 'p'
#define toStm_SignalTranslate_ON_OK  'a'
#define toStm_SignalTranslate_OFF_OK 's'
#define toStm_SignalFromOut_ON_OK    'd'
#define toStm_SignalFromOut_OFF_OK   'f'
#define toStm_SignalMic_ON_OK        'g'
#define toStm_SignalMic_OFF_OK       'h'
#define toStm_SignalMic_OFF_OK       'h'
#define UncnownCmd                   '#'


class InputSig
{
public:
    InputSig();
    ~InputSig();

    char checkReceivedByte(uint8_t data);

    bool getSignalPowerOn() const;
    bool getBtnImpedance() const;
    bool getBtnCalibrovka() const;
    bool getBtnReset() const;
    bool getSignalImpedance() const;
    bool getSignalTranslate() const;
    bool getSignalVnesh() const;
    bool getSignalMic() const;

private:
    //Stm -> Milandr
    bool btnImpedance = 0;
    bool btnCalibrovka = 0;
    bool btnReset = 0;
    bool signalPowerOn = 0;
    bool signalImpedance = 0;
    bool signalTranslate = 0;
    bool input_IMP_UPR = 0;
    bool input_VOLT_UPR = 0;
    bool input_CUR_UPR1 = 0;
    bool input_CUR_UPR2 = 0;
    bool signalVnesh = 0;
    bool signalMic = 0;

};
