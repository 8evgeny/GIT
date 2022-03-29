#pragma once
#include "main.h"
#include "milandr.h"
#include <memory>

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


class OutputSig
{
public:
    OutputSig(std::shared_ptr<Milandr>);
    ~OutputSig();

    bool getBOARD_OK() const;
    void setBOARD_OK(bool newBOARD_OK);
    bool getOVERHEAT_MC() const;
    void setOVERHEAT_MC(bool newOVERHEAT_MC);
    bool getERROR_MC() const;
    void setERROR_MC(bool newERROR_MC);
    bool getTmpOVERHEAT_MC() const;
    void setTmpOVERHEAT_MC(bool newTmpOVERHEAT_MC);
    bool getTmpBOARD_OK() const;
    void setTmpBOARD_OK(bool newTmpBOARD_OK);
    bool getTmpERROR_MC() const;
    void setTmpERROR_MC(bool newTmpERROR_MC);

    bool getToStmCmdSend() const;
    void setToStmCmdSend(bool newToStmCmdSend);
    char getToStmCmd() const;

    //Проверка переменных и если не совпадает с temp - сигнал отправка
    void checkOVERHEAT_MC();
    void checkBOARD_OK();
    void checkERROR_MC();

private:
    //Milandr -> Stm
    bool OVERHEAT_MC = 0;
    bool BOARD_OK = 0;
    bool ERROR_MC = 0;
    bool tmpOVERHEAT_MC = 0;
    bool tmpBOARD_OK = 0;
    bool tmpERROR_MC = 0;

    char toStmCmd = 0x00;
    bool toStmCmdSend = true;

    std::shared_ptr<Milandr> milandr;
};
