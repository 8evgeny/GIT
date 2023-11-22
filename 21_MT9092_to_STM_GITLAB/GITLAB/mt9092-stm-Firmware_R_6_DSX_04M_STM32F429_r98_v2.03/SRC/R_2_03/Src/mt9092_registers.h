/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   mt9092_registers.h
 * Author: roman.ganaza
 *
 * Created on November 5, 2020, 12:14 PM
 */

#ifndef MT9092_REGISTERS_H
#define MT9092_REGISTERS_H

extern unsigned char mt9092_reg[];
extern unsigned char C_channel_out;

#define MT9092_MAX_REG_ADDR 0x3F

// адреса регистров имитируемой MT9092
// не все регистры имеют какое-то реальное воплощение
// регистры есть 3-х типов:
// 1 - можно записать и потом считать то, что записали (write/verify)
// 2 - можно только считать - статусные регистры (read only)
// 3 - записваем одно, а считываем другое - FIFO, C-channel (read/write)
// 4 - можно только писать - WDT (write only)
#define HDLC_ADDRESS_RECOGNITION_REGISTER_1         0x00 // write/verify
#define HDLC_ADDRESS_RECOGNITION_REGISTER_2         0x01 // write/verify
#define HDLC_TX_RX_FIFO                             0x02 // read/write
#define HDLC_CONTROL_REGISTER_1                     0x03 // write/verify
#define HDLC_STATUS_REGISTER                        0x04 // read only
#define HDLC_CONTROL_REGISTER_2                     0x05 // write/verify
#define HDLC_INTERRUPT_ENABLE_REGISTER              0x06 // write/verify
#define HDLC_INTERRUPT_STATUS_REGISTER              0x07 // read only
// 0x08-0x09 RESERVED
#define FCODEC_GAIN_CONTROL_REGISTER_1              0x0A // write/verify
#define FCODEC_GAIN_CONTROL_REGISTER_2              0x0B // write/verify
// 0x0C-0x0D RESERVED
#define TRANSDUCER_CONTROL_REGISTER                 0x0E // write/verify
#define GENERAL_CONTROL_REGISTER                    0x0F // write/verify
// 0x10 RESERVED
#define WATCHDOG_REGISTER                           0x11 // write only
#define LCD_SEGMENT_ENABLE_REGISTER_1               0x12 // write/verify
#define LCD_SEGMENT_ENABLE_REGISTER_2               0x13 // write/verify
#define C_CHANNEL_REGISTER                          0x14 // read/write
#define TIMING_CONTROL_REGISTER                     0x15 // write/verify
#define LOOP_BACK_REGISTER_VERIFY                   0x16 // write/verify
// 0x17-0x1C RESERVED
#define RECEIVE_GAIN_CONTROL_REGISTER               0x1D // write/verify
#define DSP_CONTROL_REGISTER                        0x1E // write/verify
// 0x1F RESERVED
#define TRANSMIT_AUDIO_GAIN_REGISTER                0x20 // write/verify
#define TRANSMIT_DTMF_GAIN_REGISTER                 0x21 // write/verify
// 0x22 RESERVED
#define TONE_COEFFICIENT_REGISTER_1                 0x23 // write/verify
#define TONE_COEFFICIENT_REGISTER_2                 0x24 // write/verify
// 0x25 RESERVED
#define TONE_RINGER_WARBLE_RATE_REGISTER            0x26 // write/verify
// 0x27-0x3F RESERVED

// Биты HDLC CONTROL REGISTER 1
#define ADREC               (1 << 7)
#define HRXEN               (1 << 6)
#define HTXEN               (1 << 5)
#define EOP                 (1 << 4)
#define FA                  (1 << 3)
#define MARK_IDLE           (1 << 2)
#define TRANS               (1 << 1)
//--                        (1 << 0)

// Биты HDLC CONTROL REGISTER 2
#define INTSEL              (1 << 7)
//--                        (1 << 6)
#define TCRCI               (1 << 5)
#define SEVEN               (1 << 4)
#define FLRX                (1 << 3)
#define FLTX                (1 << 2)
#define RXFRST              (1 << 1)
#define TXFRST              (1 << 0)

// Биты HDLC STATUS REGISTER
#define INTGEN              (1 << 7)
#define IDLE_CHAN           (1 << 6)
#define RXBS2               (1 << 5)
#define RXBS1               (1 << 4)
#define TXSTAT2             (1 << 3)
#define TXSTAT1             (1 << 2)
#define RXSTAT2             (1 << 1)
#define RXSTAT1             (1 << 0)

// Биты HDLC INTERRUPT STATUS REGISTER и HDLC INTERRUPT ENABLE REGISTER (биты те же)
#define GA                  (1 << 7)
#define EOPD                (1 << 6)
#define TEOP                (1 << 5)
#define EOPR                (1 << 4)
#define TX_FL               (1 << 3)
#define FA_TX_UNDER         (1 << 2)
#define RX_FF               (1 << 1)
#define RX_OVFL             (1 << 0)

// Биты TRANSDUCER_CONTROL_REGISTER
#define PUFC                (1 << 7)
#define TFHP                (1 << 6)
#define DIAL_EN             (1 << 5)
#define SIDE_EN             (1 << 4)
#define MIC_EN              (1 << 3)
#define MIC_nHNSTMIC        (1 << 2)
#define SPKR_EN             (1 << 1)
#define HSSPKR_EN           (1 << 0)

// Биты TIMING CONTROL REGISTER
//#define --                (1 << 7)
//#define --                (1 << 6)
//#define --                (1 << 5)
//#define --                (1 << 4)
#define CH3_EN              (1 << 3) // B2-channel EN
#define CH2_EN              (1 << 2) // B1-channel EN (если установлены CH3_EN и CH2_EN, приоритет у CH2)
#define CH1_EN              (1 << 1) // C-channel - исходящий (входящий С-канал всегда работает, не смотря на этот бит)
#define CH0_EN              (1 << 0) // D-channel


void mt9092RegistersInit(void);

#endif /* MT9092_REGISTERS_H */
