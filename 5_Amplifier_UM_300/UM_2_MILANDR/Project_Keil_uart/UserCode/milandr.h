#pragma once
#include "main.h"
#include "inputSig.h"


//Команды Milandr -> Stm
#define cmdToStm_OVERHEAT_MC_ON      'x'
#define cmdToStm_OVERHEAT_MC_OFF     'c'
#define cmdToStm_BOARD_OK_ON         'v'
#define cmdToStm_BOARD_OK_OFF        'b'
#define cmdToStm_ERROR_MC_ON         'n'
#define cmdToStm_ERROR_MC_OFF        'm'
#define toStm_BlankCommand           '&'

//Stm -> Milandr (подтверждение получения команды)
#define fromStm_OVERHEAT_MC_ON_OK        'X'
#define fromStm_OVERHEAT_MC_OFF_OK       'C'
#define fromStm_BOARD_OK_ON_OK           'V'
#define fromStm_BOARD_OK_OFF_OK          'B'
#define fromStm_ERROR_MC_ON_OK           'N'
#define fromStm_ERROR_MC_OFF_OK          'M'


class Milandr
{
public:
    Milandr(std::shared_ptr<InputSig>);
    ~Milandr();

    void initGPIO();
    void initUART();

    bool getMAINPOWER();
    bool getRESPOWER();
    bool getFAULT();
    bool getCLIP_OTW();
    bool getPOW_READY();
    void setGAIN_UPR (bool set);
    void setCOMP_UPR (bool set);
    void setFAN_ON (bool set);
    void setRESET_UPR (bool set);

    bool getToStmCmdSend() const;
    void setToStmCmdSend(bool newToStmCmdSend);
    char getToStmCmd() const;

    bool isPowerOk();

    bool getOVERHEAT_MC() const;
    void setOVERHEAT_MC(bool newOVERHEAT_MC);

    bool getERROR_MC() const;
    void setERROR_MC(bool newERROR_MC);

    bool getBOARD_OK() const;
    void setBOARD_OK(bool newBOARD_OK);

private:
    bool StmNoReceiveCmd = 0;
    bool UncnownCmdFromStm = 0;


    //Milandr -> Stm
    bool OVERHEAT_MC = 0;
    bool BOARD_OK = 0;
    bool ERROR_MC = 0;

    bool tmpOVERHEAT_MC = 0;
    bool tmpBOARD_OK = 0;
    bool tmpERROR_MC = 0;

    char toStmCmd = 0x00;
    bool toStmCmdSend = true;

    std::shared_ptr<InputSig> _inp;

};
