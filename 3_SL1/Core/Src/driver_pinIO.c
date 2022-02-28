/**
  ******************************************************************************
  * @file    driver_pinIO.c
  * @author  dmib@@ETS
  * @brief   This file contains pinouts I/O driver implementation
  ******************************************************************************
  */
#include "string.h"
#include "stm32h7xx_hal.h"
#include "main.h"		  // for port defines
#include "driver_pinIO.h"

/// current signal values
uint8_t valDET_48V, valDET_PHONE, valDET_MIC;

void pinio_init()
{ 
#ifdef SC4
  pinio_set_UPR_SP(UPR_STATE_OFF);
  pinio_set_UPR_FAN(UPR_STATE_OFF);
  pinio_set_UPR_RELE_SC4(UPR_STATE_OFF);
#endif
  valDET_48V = 0; 
  valDET_PHONE = 0;
  valDET_MIC = 0;
}

void pinio_task()
{
  // nothing to do yet
}

uint8_t pinio_get_CFGUI()
{
  uint8_t res = 0;
  #ifdef SC_4
  if (HAL_GPIO_ReadPin(CFG_UI0_GPIO_Port, CFG_UI0_Pin)==GPIO_PIN_SET)
	  res |= 1;
  if (HAL_GPIO_ReadPin(CFG_UI1_GPIO_Port, CFG_UI1_Pin)==GPIO_PIN_SET)
	  res |= 2;
  if (HAL_GPIO_ReadPin(CFG_UI2_GPIO_Port, CFG_UI2_Pin)==GPIO_PIN_SET)
	  res |= 4;
  #endif

  #ifdef SC_2
  res = 1;
  #endif
  return res;
}

void pinio_set_UPR_SP(uint8_t state)
{
  if (state == UPR_STATE_OFF)
   HAL_GPIO_WritePin(UPR1_SP_GPIO_Port, UPR1_SP_Pin, GPIO_PIN_SET);
  else
  if (state == UPR_STATE_ON)
   HAL_GPIO_WritePin(UPR1_SP_GPIO_Port, UPR1_SP_Pin, GPIO_PIN_RESET);
}

// only SC4 rev1
void pinio_set_UPR_FAN(uint8_t state)
{
  if (state == UPR_STATE_ON)
   HAL_GPIO_WritePin(UPR2_FAN_GPIO_Port, UPR2_FAN_Pin, GPIO_PIN_SET);
  else
  if (state == UPR_STATE_OFF)
   HAL_GPIO_WritePin(UPR2_FAN_GPIO_Port, UPR2_FAN_Pin, GPIO_PIN_RESET);
}

void pinio_set_UPR_RELE_SC4(uint8_t state)
{
  if (state == UPR_STATE_OFF)
   HAL_GPIO_WritePin(RELE_UPR_GPIO_Port, RELE_UPR_Pin, GPIO_PIN_SET);
  else
  if (state == UPR_STATE_ON)
   HAL_GPIO_WritePin(RELE_UPR_GPIO_Port, RELE_UPR_Pin, GPIO_PIN_RESET);
}

void pinio_set_UPR_RELE_SC2(uint8_t state)
{
  if (state == UPR_STATE_OFF)
   HAL_GPIO_WritePin(RELE1_UPR_Port, RELE1_UPR_Pin, GPIO_PIN_SET);
  else
  if (state == UPR_STATE_ON)
   HAL_GPIO_WritePin(RELE1_UPR_Port, RELE1_UPR_Pin, GPIO_PIN_RESET);
}

void pinio_set_AMP_UPR(uint8_t state)
{
  if (state == UPR_STATE_OFF)
   HAL_GPIO_WritePin(AMP_UPR_Port, AMP_UPR_Pin, GPIO_PIN_SET);
  else
  if (state == UPR_STATE_ON)
   HAL_GPIO_WritePin(AMP_UPR_Port, AMP_UPR_Pin, GPIO_PIN_RESET);
}

// only SC4 rev1
uint8_t pinio_getstate_DET_48V()
{ 
  uint8_t val = HAL_GPIO_ReadPin(DET_48V_GPIO_Port, DET_48V_Pin) ^ 1;
  if (val != valDET_48V) {
   valDET_48V = val; 
   if (val) return DET_STATE_ON;
   else return DET_STATE_OFF;
  }
  else return DET_STATE_IDLE;
}

// only SC4 rev1
uint8_t pinio_getval_DET_48V()
{ 
  return HAL_GPIO_ReadPin(DET_48V_GPIO_Port, DET_48V_Pin) ^ 1;
}

// only SC4 rev1, SC4 rev2 = DET_MIC

#if defined SC_2 || defined SC_4
uint8_t pinio_getstate_DET_PHONE()
{
  uint8_t val = HAL_GPIO_ReadPin(UPR_PHONE_GPIO_Port, UPR_PHONE_Pin) ^ 1;
  if (val != valDET_PHONE) {
   valDET_PHONE = val; 
   if (val) return DET_STATE_ON;
   else return DET_STATE_OFF;
  }
  else return DET_STATE_IDLE;
}
#endif

#if defined SC_2 || defined SC_4
// only SC4 rev1, SC4 rev2 = DET_MIC
uint8_t pinio_getval_DET_PHONE()
{
  return HAL_GPIO_ReadPin(UPR_PHONE_GPIO_Port, UPR_PHONE_Pin) ^ 1;
}
#endif

// SC4 rev1 from port, SC4 rev2 from AIC
uint8_t pinio_getstate_DET_MIC()
{
  uint8_t val = HAL_GPIO_ReadPin(UPR_MIC2_GPIO_Port, UPR_MIC2_Pin);
  if (val != valDET_MIC) {
   valDET_MIC = val; 
   if (val) return DET_STATE_ON;
   else return DET_STATE_OFF;
  }
  else return DET_STATE_IDLE;
}

// SC4 rev1 from port, SC4 rev2 from AIC
uint8_t pinio_getval_DET_MIC()
{
  return HAL_GPIO_ReadPin(UPR_MIC2_GPIO_Port, UPR_MIC2_Pin);
}
