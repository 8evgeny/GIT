/**
  ******************************************************************************
  * @file    driver_ui.c
  * @author  dmib@@ETS
  * @brief   This file contains UI driver implementation
  ******************************************************************************
  */
#include "string.h"
#include "stm32h7xx_hal.h"
#include "main.h"		// for port defines
#include "driver_pinIO.h"
#include "CLI_io.h"
#include "driver_UI.h"

/// UI driver error print level
#define UI_CLI_ERROR_LEVEL	CLI_PRINTLEVEL_WORK
/// UI driver debug print level
#define UI_CLI_DEBUG_LEVEL	CLI_PRINTLEVEL_TEST

/// UI board mode
uint8_t ui_mode;

/// max address number of TLC59116F chips
uint8_t TLC59116F_max_address;
/// TLC59116F chips addresses
const uint8_t TLC59116F_address[] 	= {0xC0,0xC2,0xC4,0xC6,0xC8,0xCA};
/// TLC59116F chip button registers
const uint8_t TLC59116F_register[] 	= {0x14,0x15,0x16,0x17};

/// max address number of MCP23017 chips
uint8_t MCP23017_max_address;
/// MCP23017 chips addresses
const uint8_t MCP23017_address[] = {0x40,0x42,0x44,0x4E};
/// MCP23017 chip LEDs registers
const uint8_t MCP23017_register[] = {0x12,0x13};

/// TLC59116F chip setup registers values
const uint8_t TLC59116F_Init_Val[] = {
			//  Reg    Data
                0x00 , 0x00,	// MODE1
                0x14 , 0x00,    // LEDOUT0
                0x15 , 0x00,	// LEDOUT1
                0x16 , 0x00,	// LEDOUT2
                0x17 , 0x00		// LEDOUT3
             };

/// MCP23017 chip setup registers values
const uint8_t MCP23017_Init_Val[] = {
			//  Reg    Data
                0x04 , 0xFF,	// GPINTENA
                0x05 , 0xFF,	// GPINTENB
                0x0A , 0x42,	// IOCON
                0x0C , 0xFF,	// GPPUA
                0x0D , 0xFF,	// GPPUB
             };

/// LEDs current values
uint8_t LED_val[LED_NUM];
/// LEDs current states
uint8_t LED_state[LED_NUM]; 

/// LEDs FL1 type flash current time
uint32_t LED_time_FL1;
/// LEDs FL2 type flash current time
uint32_t LED_time_FL2;
/// LEDs FL3 type flash current time
uint32_t LED_time_FL3;
/// LEDs FL1 type flash current value
uint8_t LED_val_FL1;
/// LEDs FL2 type flash current value
uint8_t LED_val_FL2;
/// LEDs FL3 type flash current value
uint8_t LED_val_FL3;

/// Buttons current values
uint8_t BTN_val[BTN_NUM]; 
/// Buttons current states
uint8_t BTN_state[BTN_NUM];

/// MCP buttons read interrupt flag
uint8_t MCP_needtoread;
/// block keyboard flag
uint8_t ui_block_kbd;

/// link to I2C3 hardware module control/status structure
extern I2C_HandleTypeDef hi2c3;

/**
  * @brief setup buttons hardware interrupt
  */
void SC_2_4_BTN_EXTI_IRQHandler_Config() {
  GPIO_InitTypeDef   GPIO_InitStructure;

  //button TEST interrupt pin setup
  GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Pin = TEST_BUT_Pin;
  HAL_GPIO_Init(TEST_BUT_GPIO_Port, &GPIO_InitStructure);
  //set interrupt for EXTI5
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 8, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  #ifdef SC_4
  //button UI board interrupt pin setup
  GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Pin = INT_BUT_Pin;
  HAL_GPIO_Init(INT_BUT_GPIO_Port, &GPIO_InitStructure);
  #endif

  //set interrupt for EXTI12
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 8, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/**
  * @brief TEST button interrupt handler
  */
void EXTI9_5_IRQHandler() {
  BTN_state[BTN_TEST] = BTN_STATE_TOGGLE;
  BTN_val[BTN_TEST] = HAL_GPIO_ReadPin(TEST_BUT_GPIO_Port, TEST_BUT_Pin) ^ 1;
  HAL_GPIO_EXTI_IRQHandler(TEST_BUT_Pin);
}

/**
  * @brief UI board buttons interrupt handler
  */
void EXTI15_10_IRQHandler() {
//  MCP23017_readbuttons();
  #ifdef SC_4
  MCP_needtoread = 1;
  HAL_GPIO_EXTI_IRQHandler(INT_BUT_Pin);
  #endif
}

/**
  * @brief setup TLC59116F chips
  * @retval result - 1 - ok, <0 - HAL IO error
  */
int8_t TLC59116F_Init()
{
  uint8_t i,j;
  for (i = 0; i < TLC59116F_max_address; i++)
   for (j = 0; j < sizeof(TLC59116F_Init_Val); j+=2) {
    if (HAL_I2C_Mem_Write(&hi2c3, TLC59116F_address[i], TLC59116F_Init_Val[j] ,
    		              I2C_MEMADD_SIZE_8BIT,(uint8_t *)&TLC59116F_Init_Val[j+1], 1, 100)!=HAL_OK) return -i;
   }

  return 1;
}

/**
  * @brief setup MCP23017 chips
  * @retval result - 1 - ok, <0 - HAL IO error
  */
int8_t MCP23017_Init()
{
  uint8_t i,j;
  for (i = 0; i < MCP23017_max_address; i++)
   for (j = 0; j < sizeof(MCP23017_Init_Val); j+=2) {
    if (HAL_I2C_Mem_Write(&hi2c3, MCP23017_address[i], MCP23017_Init_Val[j] ,
    		              I2C_MEMADD_SIZE_8BIT,(uint8_t *)&MCP23017_Init_Val[j+1], 1, 100)!=HAL_OK) return -i;
   }

  if ((ui_mode == UI_mode_UI16N)||(ui_mode == UI_mode_UI32N)) { // tel number btn reg
   for (j = 0; j < sizeof(MCP23017_Init_Val); j+=2) {
    if (HAL_I2C_Mem_Write(&hi2c3, MCP23017_address[3], MCP23017_Init_Val[j] ,
    		              I2C_MEMADD_SIZE_8BIT,(uint8_t *)&MCP23017_Init_Val[j+1], 1, 100)!=HAL_OK) return -3;
   }
  }
  return 1;
}

int8_t TLC59116F_Write(uint8_t idx, uint8_t reg, uint8_t data)
{
  if (HAL_I2C_Mem_Write(&hi2c3, TLC59116F_address[idx], reg , I2C_MEMADD_SIZE_8BIT,(uint8_t *)&data, 1, 100)!=HAL_OK)
	return -1;
  else 
	return 0;						  
}

int8_t TLC59116F_Read(uint8_t idx, uint8_t reg, uint8_t *data)
{
  if (HAL_I2C_Mem_Read(&hi2c3, TLC59116F_address[idx], reg , I2C_MEMADD_SIZE_8BIT, data, 1, 100)!=HAL_OK) 
	return -1;
  else
	return 0;						  
}

int8_t MCP23017_Write(uint8_t idx, uint8_t reg, uint8_t data)
{
  if (HAL_I2C_Mem_Write(&hi2c3, MCP23017_address[idx], reg , I2C_MEMADD_SIZE_8BIT,(uint8_t *)&data, 1, 100)!=HAL_OK)
	return -1;
  else 
	return 0;						  
}

int8_t MCP23017_Read(uint8_t idx, uint8_t reg, uint8_t *data)
{
  if (HAL_I2C_Mem_Read(&hi2c3, MCP23017_address[idx], reg ,I2C_MEMADD_SIZE_8BIT, data, 1, 100)!=HAL_OK)
	return -1;
  else 
	return 0;						  
}

int8_t ui_test_TLC(uint8_t idx, uint8_t printout)
{
	uint8_t j, reg, data;
	
	if (TLC59116F_Write(idx, 0x00, 0x00)<0) {
	  if (printout) 
	   CLI_print("Error write TLC%i reg 00\r\n",idx);
	  return -1;			
	}
	HAL_Delay(10);
		
	reg = 0x14;
		
	for (j = 0 ; j < 4; j++)
	{
		if (TLC59116F_Write(idx, reg, 0x55)<0) {
		  if (printout) 
		   CLI_print("Error write TLC%i reg .2X\r\n",idx, reg);
		  return -1;			
		}
		HAL_Delay(10);
		if (TLC59116F_Read(idx, reg, &data)<0) {
		  if (printout) 
		   CLI_print("Error read TLC%i reg .2X",idx, reg);
		  return -2;			
		  
		  if (data != 0x55) {
		   if (printout) 
		    CLI_print("Error read TLC%i data .2X",idx, reg);
		   return -3;			
		  }
		}		
		reg++;
	}

	return 0;
}

int8_t ui_test_MCP(uint8_t idx, uint8_t printout)
{
	uint8_t j, reg, wdata, rdata;
	
	for (j = 0; j < sizeof(MCP23017_Init_Val); j+=2) {
		reg = MCP23017_Init_Val[j];
		wdata = MCP23017_Init_Val[j+1];
		if (MCP23017_Write(idx, reg, wdata) < 0)
		{
			if (printout) 
			   CLI_print("Error write MCP%i reg .2X", idx, reg);
			return -1;
		}
		HAL_Delay(10);
		if (MCP23017_Read(idx, reg, &rdata) < 0)
		{
			if (printout) 
			   CLI_print("Error read MCP%i reg .2X\r\n", idx, reg);
			return -2;				   
		}
	   
		if (wdata != rdata) {
		   if (printout) 
		    CLI_print("Error read TLC%i reg .2X wdata=.2X rdata=.2X\r\n", idx, reg, wdata, rdata);
		   return -3;			
		}
	}
	return 0;
}

int8_t ui_test_board(uint8_t printout)
{
	uint8_t i, res;

	res=0;
	
	for (i = 0 ; i < TLC59116F_max_address; i++) {
		if (ui_test_TLC(i, printout)<0) res = -1;
		else if (printout) CLI_print("Test TLC%i ok\r\n", i);
	}
	TLC59116F_Init();
	
	for (i = 0 ; i < MCP23017_max_address; i++)
	{
		if (ui_test_MCP(i, printout)) res = -1;
		else if (printout) CLI_print("Test MCP%i ok\r\n", i);
	}
    if ((ui_mode == UI_mode_UI16N)||(ui_mode == UI_mode_UI32N)) // tel number btn reg
	{ 
		if (ui_test_MCP(3, printout)) res = -1;
		else if (printout) CLI_print("Test MCP3 ok\r\n");
    }   
	
	return res;
}

/**
  * @brief calculate TLC59116F LED register value
  * @retval result - calculated register value
  */
uint8_t TLC59116F_makeledval(uint8_t led) 
{
   uint8_t i,res;
   res = 0;
   for (i=0; i<8; i+=2) {
     res |= (LED_val[led++] & 0x03) << i;
   }

   return res;
}

/**
  * @brief write LED value to TLC59116F
  * @param  [in] led - LED index
  * @retval result - calculated register value
  */
void TLC59116F_writeled(uint8_t led)
{
  led &= 0xFC;
  uint8_t data = TLC59116F_makeledval(led);
  led -= LED_AB1R;
  HAL_I2C_Mem_Write(&hi2c3, TLC59116F_address[led >> 4], TLC59116F_register[(led & 0x0F) >> 2] ,I2C_MEMADD_SIZE_8BIT,&data, 1, 100);
}

/**
  * @brief read all buttons current values from MCP23017 chips
  * @retval none
  */
void MCP23017_readbuttons()
{
  uint16_t data;
  uint8_t i,j,btn_idx,res;

  btn_idx = BTN_AB1;
  for (i = 0; i < MCP23017_max_address; i++) {
    if ((res=HAL_I2C_Mem_Read(&hi2c3, MCP23017_address[i], 0x12,I2C_MEMADD_SIZE_8BIT,(uint8_t*)&data, 2, 100))==HAL_OK) {
      if (ui_block_kbd) { btn_idx+=16; continue;}
	  data = ~data;
	  for (j = 0 ; j < 16 ; j++) {
	    if (BTN_val[btn_idx] != (data & 1)) {
		  BTN_val[btn_idx] = data & 1;
		  BTN_state[btn_idx] = BTN_STATE_TOGGLE;
		}
		data >>= 1;
		btn_idx++;
	  }
	} else {
		//CLI_print_lev(UI_CLI_DEBUG_LEVEL, "Error: UI read MCP%.2X\r\n",i);
	};
  }
  
  if ((ui_mode == UI_mode_UI16N)||(ui_mode == UI_mode_UI32N)) { // tel number btn
    if (HAL_I2C_Mem_Read(&hi2c3, MCP23017_address[3], 0x12,I2C_MEMADD_SIZE_8BIT,(uint8_t*)&data, 2, 100)==HAL_OK) {
     if (!ui_block_kbd) {
      btn_idx = BTN_NN1;
	  data = ~data;
	  for (j = 0 ; j < 12 ; j++) {
	    if (BTN_val[btn_idx] != (data & 1)) {
		  BTN_val[btn_idx] = data & 1;
		  BTN_state[btn_idx] = BTN_STATE_TOGGLE;
		}
		data >>= 1;
		btn_idx++;
	  }
     }
	} else {
		//CLI_print_lev(UI_CLI_DEBUG_LEVEL, "Error: UI read MCP NN\r\n");
	}
  }
  
}

/**
  * @brief read all button captured values after interrupt from MCP23017 chips
  * @retval none
  */
void MCP23017_readbuttonsCapture()
{
  uint16_t data;
  uint8_t i,j,btn_idx,res;

  btn_idx = BTN_AB1;
  for (i = 0; i < MCP23017_max_address; i++) {
    if ((res=HAL_I2C_Mem_Read(&hi2c3, MCP23017_address[i], 0x10,I2C_MEMADD_SIZE_8BIT,(uint8_t*)&data, 2, 100))==HAL_OK) {
      if (ui_block_kbd) { btn_idx+=16; continue;}
	  data = ~data;
	  for (j = 0 ; j < 16 ; j++) {
	    if (BTN_val[btn_idx] != (data & 1)) {
		  BTN_val[btn_idx] = data & 1;
		  BTN_state[btn_idx] = BTN_STATE_TOGGLE;
		}
		data >>= 1;
		btn_idx++;
	  }
	} else {
		//CLI_print_lev(UI_CLI_DEBUG_LEVEL, "Error: UI read MCP%.2X\r\n",i);
	};
  }

  if ((ui_mode == UI_mode_UI16N)||(ui_mode == UI_mode_UI32N)) { // tel number btn
    if (HAL_I2C_Mem_Read(&hi2c3, MCP23017_address[3], 0x10,I2C_MEMADD_SIZE_8BIT,(uint8_t*)&data, 2, 100)==HAL_OK) {
     if (!ui_block_kbd) {
 	  btn_idx = BTN_NN1;
	  data = ~data;
	  for (j = 0 ; j < 12 ; j++) {
	    if (BTN_val[btn_idx] != (data & 1)) {
		  BTN_val[btn_idx] = data & 1;
		  BTN_state[btn_idx] = BTN_STATE_TOGGLE;
		}
		data >>= 1;
		btn_idx++;
	  }
     }
	} else {
		//CLI_print_lev(UI_CLI_DEBUG_LEVEL, "Error: UI read MCP NN\r\n");
	}
  }

}

void ui_setledstate(uint8_t led, uint8_t state)
{
    LED_state[led] = state;
	switch (state) {
		case LED_STATE_OFF:
			LED_val[led] = LED_STATE_OFF;
			break;
		case LED_STATE_ON:
			LED_val[led] = LED_STATE_ON;
			break;
		case LED_STATE_FL1:
			LED_val[led] = LED_val_FL1;
			break;
		case LED_STATE_FL2:
			LED_val[led] = LED_val_FL2;
			break;
		case LED_STATE_FL3:
			LED_val[led] = LED_val_FL3;
			break;
		default: ;
	};
	// обновить состояние	
	if (led == LED_TEST)
		HAL_GPIO_WritePin(TEST_LED_GPIO_Port, TEST_LED_Pin, LED_val[led]);
#ifdef SC_2
    else
    if (led == LED_L1)
        HAL_GPIO_WritePin(GPIOC, LED1, LED_val[led]);
    else
    if (led == LED_L2)
        HAL_GPIO_WritePin(GPIOC, LED2, LED_val[led]);
    else
    if (led == LED_L3)
        HAL_GPIO_WritePin(GPIOC, LED3, LED_val[led]);
    else
    if (led == LED_L4)
        HAL_GPIO_WritePin(GPIOG, LED4, LED_val[led]);
    else
    if (led == LED_L5)
        HAL_GPIO_WritePin(GPIOG, LED5, LED_val[led]);
    else
    if (led == LED_L6)
        HAL_GPIO_WritePin(GPIOG, LED6, LED_val[led]);
#endif
#ifdef SC_4
    else
    if (led == LED_NORMA)
        HAL_GPIO_WritePin(NORMA_UPR_GPIO_Port, NORMA_UPR_Pin, LED_val[led]);
    else
    if (led == LED_MKVKL)
        HAL_GPIO_WritePin(MKVKL_UPR_GPIO_Port, MKVKL_UPR_Pin, LED_val[led]);
	else 
	    TLC59116F_writeled(led);
#endif
}

uint8_t ui_getledstate(uint8_t led)
{
   return LED_state[led];
}

uint8_t ui_getbtnval(uint8_t btn) 
{
   return BTN_val[btn];
}

uint8_t ui_getbtnstate(uint8_t btn) 
{
   return BTN_state[btn];
}

void ui_resetbtnstate(uint8_t btn) 
{
   BTN_state[btn] = BTN_STATE_IDLE;
}

uint8_t ui_getactivebtn() 
{
   uint8_t idx;
   
   for (idx = 0; idx < BTN_NUM; idx++)
    if (BTN_state[idx] != BTN_STATE_IDLE) {
	 BTN_state[idx] = BTN_STATE_IDLE;
	 return idx;
	}
    
   return BTN_NONE;   
}

void ui_init()
{
   int8_t res;

   ui_mode = pinio_get_CFGUI();
   switch (ui_mode) {	
		case UI_mode_UI16: TLC59116F_max_address = 2; MCP23017_max_address = 1; break;	//УИ-16-1
		case UI_mode_UI32: TLC59116F_max_address = 4; MCP23017_max_address = 2; break;	//УИ-32-1
		case UI_mode_UI48: TLC59116F_max_address = 6; MCP23017_max_address = 3; break;	//УИ-48-1
		case UI_mode_UI16N: TLC59116F_max_address = 2; MCP23017_max_address = 1; break;	//УИ-16Н-1
		case UI_mode_UI32N: TLC59116F_max_address = 4; MCP23017_max_address = 2; break;	//УИ-32Н-1
		default: TLC59116F_max_address = 6; MCP23017_max_address = 3; break;
   };   
   res = TLC59116F_Init();
   if (res<0) CLI_print_lev(UI_CLI_ERROR_LEVEL, "Error : UI init TLC%i fail\r\n",-res);
   res = MCP23017_Init();
   if (res<0) CLI_print_lev(UI_CLI_ERROR_LEVEL, "Error : UI init MCP%i fail\r\n",-res);

   SC_2_4_BTN_EXTI_IRQHandler_Config();
   
   LED_time_FL1 = HAL_GetTick(); 
   LED_time_FL2 = HAL_GetTick(); 
   LED_time_FL3 = HAL_GetTick();
   LED_val_FL1 = 0; 
   LED_val_FL2 = 0; 
   LED_val_FL3 = 0;
   memset(LED_val,0, LED_NUM * sizeof(uint8_t)); 	  
   memset(LED_state,0, LED_NUM * sizeof(uint8_t)); 	  
   memset(BTN_val,0, BTN_NUM * sizeof(uint8_t)); 	  
   memset(BTN_state,0, BTN_NUM * sizeof(uint8_t)); 	 
   
   ui_setledstate(LED_TEST, LED_STATE_OFF);
   ui_setledstate(LED_NORMA, LED_STATE_OFF);
   ui_setledstate(LED_MKVKL, LED_STATE_OFF);

   MCP_needtoread = 0;
   MCP23017_readbuttons();
}

/**
  * @brief update state of all LEDs
  */
void ui_updateLED()
{
  if (LED_state[LED_TEST] > LED_STATE_ON) 
	HAL_GPIO_WritePin(TEST_LED_GPIO_Port, TEST_LED_Pin, LED_val[LED_TEST]);

    #ifdef SC_2
      if (LED_state[LED_L1] > LED_STATE_ON)
        HAL_GPIO_WritePin(GPIOC, LED1, LED_val[LED_L1]);
      if (LED_state[LED_L2] > LED_STATE_ON)
        HAL_GPIO_WritePin(GPIOC, LED2, LED_val[LED_L2]);
      if (LED_state[LED_L3] > LED_STATE_ON)
        HAL_GPIO_WritePin(GPIOC, LED3, LED_val[LED_L3]);
      if (LED_state[LED_L4] > LED_STATE_ON)
        HAL_GPIO_WritePin(GPIOG, LED4, LED_val[LED_L4]);
      if (LED_state[LED_L5] > LED_STATE_ON)
        HAL_GPIO_WritePin(GPIOG, LED5, LED_val[LED_L5]);
      if (LED_state[LED_L6] > LED_STATE_ON)
        HAL_GPIO_WritePin(GPIOG, LED6, LED_val[LED_L6]);
    #endif

    #ifdef SC_4
      if (LED_state[LED_NORMA] > LED_STATE_ON)
        HAL_GPIO_WritePin(NORMA_UPR_GPIO_Port, NORMA_UPR_Pin, LED_val[LED_NORMA]);
      if (LED_state[LED_MKVKL] > LED_STATE_ON)
        HAL_GPIO_WritePin(MKVKL_UPR_GPIO_Port, MKVKL_UPR_Pin, LED_val[LED_MKVKL]);

      uint8_t i = LED_AB1R;
      while	(i<LED_NUM) {
       if (LED_state[i] > LED_STATE_ON) {
         TLC59116F_writeled(i);
         i = (i & 0xFC) + 4; // jump to led in next register
       } else i++;
      }
    #endif

}

/**
  * @brief update state of SC4 board buttons
  */
void ui_updateBtn() {
  uint8_t val;
  
  if (ui_block_kbd) return;

  val = HAL_GPIO_ReadPin(VOL_UP_GPIO_Port, VOL_UP_Pin) ^ 1;
  if (BTN_val[BTN_VOLUP] != val) {
	  BTN_val[BTN_VOLUP] = val;
	  BTN_state[BTN_VOLUP] = BTN_STATE_TOGGLE;
  }

  val = HAL_GPIO_ReadPin(VOL_DOWN_GPIO_Port, VOL_DOWN_Pin) ^ 1;
  if (BTN_val[BTN_VOLDN] != val) {
	  BTN_val[BTN_VOLDN] = val;
	  BTN_state[BTN_VOLDN] = BTN_STATE_TOGGLE;
  }

  val = HAL_GPIO_ReadPin(SENS_UP_GPIO_Port, SENS_UP_Pin) ^ 1;
  if (BTN_val[BTN_MICUP] != val) {
	  BTN_val[BTN_MICUP] = val;
	  BTN_state[BTN_MICUP] = BTN_STATE_TOGGLE;
  }

  val = HAL_GPIO_ReadPin(SENS_DOWN_GPIO_Port, SENS_DOWN_Pin) ^ 1;
  if (BTN_val[BTN_MICDN] != val) {
	  BTN_val[BTN_MICDN] = val;
	  BTN_state[BTN_MICDN] = BTN_STATE_TOGGLE;
  }

  val = HAL_GPIO_ReadPin(GPIOG, K1) ^ 1;
  if (BTN_val[BTN_K1] != val) {
      BTN_val[BTN_K1] = val;
      BTN_state[BTN_K1] = BTN_STATE_TOGGLE;
  }
  val = HAL_GPIO_ReadPin(GPIOG, K2) ^ 1;
  if (BTN_val[BTN_K2] != val) {
      BTN_val[BTN_K2] = val;
      BTN_state[BTN_K2] = BTN_STATE_TOGGLE;
  }
  val = HAL_GPIO_ReadPin(GPIOG, K3) ^ 1;
  if (BTN_val[BTN_K3] != val) {
      BTN_val[BTN_K3] = val;
      BTN_state[BTN_K3] = BTN_STATE_TOGGLE;
  }
  val = HAL_GPIO_ReadPin(GPIOG, K4) ^ 1;
  if (BTN_val[BTN_K4] != val) {
      BTN_val[BTN_K4] = val;
      BTN_state[BTN_K4] = BTN_STATE_TOGGLE;
  }
  val = HAL_GPIO_ReadPin(GPIOG, K5) ^ 1;
  if (BTN_val[BTN_K5] != val) {
      BTN_val[BTN_K5] = val;
      BTN_state[BTN_K5] = BTN_STATE_TOGGLE;
  }
  val = HAL_GPIO_ReadPin(GPIOG, K6) ^ 1;
  if (BTN_val[BTN_K6] != val) {
      BTN_val[BTN_K6] = val;
      BTN_state[BTN_K6] = BTN_STATE_TOGGLE;
  }
}

void ui_task()
{
  uint8_t i, need_update_led;
  
  // update led state
  need_update_led = 0;

  if ((HAL_GetTick() - LED_time_FL1) > LED_PERIOD_FL1) {
   LED_val_FL1 ^= 1;
   LED_time_FL1 = HAL_GetTick(); 
   for (i = 0; i < LED_NUM; i++) if (LED_state[i] == LED_STATE_FL1) { LED_val[i] = LED_val_FL1; need_update_led = 1;}
   if (!MCP_needtoread) MCP23017_readbuttons();
  }

  if ((HAL_GetTick() - LED_time_FL2) > LED_PERIOD_FL2) {
   LED_val_FL2 ^= 1;
   LED_time_FL2 = HAL_GetTick(); 
   for (i = 0; i < LED_NUM; i++) if (LED_state[i] == LED_STATE_FL2) { LED_val[i] = LED_val_FL2; need_update_led = 1;}
  }

  if (LED_val_FL3) {
	  if ((HAL_GetTick() - LED_time_FL3) > LED_PERIOD_FL3_1) {
		  LED_val_FL3 = 0;
		  LED_time_FL3 = HAL_GetTick();
		  for (i = 0; i < LED_NUM; i++) if (LED_state[i] == LED_STATE_FL3) { LED_val[i] = LED_val_FL3; need_update_led = 1;}
	  }
  } else {
	  if ((HAL_GetTick() - LED_time_FL3) > LED_PERIOD_FL3_0) {
		  LED_val_FL3 = 1;
		  LED_time_FL3 = HAL_GetTick();
		  for (i = 0; i < LED_NUM; i++) if (LED_state[i] == LED_STATE_FL3) { LED_val[i] = LED_val_FL3; need_update_led = 1;}
	  }
  }

  // update button state
  ui_updateBtn();

  if (MCP_needtoread) { // was btn int
	  MCP23017_readbuttonsCapture();
	  MCP_needtoread = 0;
  }
  
  HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
  if (need_update_led) ui_updateLED();  // write updated leds
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

uint8_t ui_get_uimode()
{
	return ui_mode;
}

void ui_set_block_kbd(uint8_t isblock)
{
	ui_block_kbd = isblock;
}
