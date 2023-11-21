/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   volume_control.c
 * Author: roman.ganaza
 * 
 * Created on August 4, 2023, 3:24 PM
 */

#include <stdio.h>
#include "main.h"
#include "volume_control.h"
#include "potentiometer.h"
#include "codec_TLV320AIC3100.h"

void volumeControlInit(void) {
  potInit();
}

void doVolumeControl(void) {
  static uint32_t tmr_vol = 0;

  if (HAL_GetTick() - tmr_vol > 100) {
    tmr_vol = HAL_GetTick();
    
    setSpeakerVol(getPotValue(POT_VOL));
    setHltVol(getPotValue(POT_HLT));
    
    if (selected_mic == HANDSET_MIC) {
      setMicVol(getPotValue(POT_HMIC), selected_mic);
    } else if (selected_mic == LOUDSPEAKER_MIC) {
      setMicVol(getPotValue(POT_MIC), selected_mic);    
    }
  }
}
