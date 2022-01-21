/**
  ******************************************************************************
  * @file    mdr32f8_ebc.c
  * @author  Milandr Application Team
  * @version V1.4.0
  * @date    20/07/2015
  * @brief   This file provides all the ADC firmware functions.
  ******************************************************************************
  * <br><br>
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, MILANDR SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
  * OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2021 Milandr</center></h2>
  ******************************************************************************
  * FILE mdr32f8_ebc.c
  */

/* Includes ------------------------------------------------------------------*/
#include "mdr32f8_config.h"
#include "mdr32f8_ebc.h"

#define ASSERT_INFO_FILE_ID FILEID__MDR32F8_EBC_C

/** @addtogroup __MDR32F8_StdPeriph_Driver MDR32F8 Standard Peripherial Driver
  * @{
  */

/** @defgroup EBC EBC
  * @{
  */

/** @defgroup EBC_Private_Functions EBC Private Functions
  * @{
  */


/**
  * @brief  Initializes the EBC peripheral according to
  *         the specified parameters in the EBC_RGN_InitStruct.
  * @param  EBC_RGN_InitStruct: pointer to a EBC_RGN_InitTypeDef structure
  *         that contains the configuration information for the specified EBC_RGN.
  * @param 	RGNx: specifies the region number.
  *         @arg RGN0: region 0 (0x1000_0000 - 0x17FF_FFFF)
  *         @arg RGN1: region 1 (0x1800_0000 - 0x1FFF_FFFF)
  *         @arg RGN2: region 2 (0x5000_0000 - 0x57FF_FFFF)
  *         @arg RGN3: region 3 (0x5800_0000 - 0x5FFF_FFFF)
  *         @arg RGN4: region 4 (0x6000_0000 - 0x67FF_FFFF)
  *         @arg RGN5: region 5 (0x6800_0000 - 0x6FFF_FFFF)
  *         @arg RGN6: region 6 (0x7000_0000 - 0x7FFF_FFFF)
  *         @arg RGN7: region 7 (0x8000_0000 - 0xDFFF_FFFF)
  * @retval None
  */
void EBC_RGNx_Init(uint32_t RGNx, const EBC_RGN_InitTypeDef* EBC_RGN_InitStruct)
{
	uint32_t tmpreg_RGNx_CNTRL;
	/* Check the parameters */
	assert_param(IS_EBC_RGNx(RGNx));

	tmpreg_RGNx_CNTRL =               (EBC_RGN_InitStruct->RGN_ECCEN 		<< EBC_ECCEN_pos)
									| (EBC_RGN_InitStruct->RGN_ECCMODE 		<< EBC_ECCMODE_pos)
									| (EBC_RGN_InitStruct->RGN_RDYWAIT 		<< EBC_RDYWAIT_pos)
									| (EBC_RGN_InitStruct->RGN_MODE 	 	<< EBC_MODE_pos)
									| (EBC_RGN_InitStruct->RGN_ROM 		 	<< EBC_ROM_pos)
									| (EBC_RGN_InitStruct->RGN_CPOL 	 	<< EBC_CPOL_pos)
									| (EBC_RGN_InitStruct->RGN_WS_ACTIVE 	<< EBC_WS_ACTIVE_pos)
									| (EBC_RGN_InitStruct->RGN_WS_SETUP 	<< EBC_WS_SETUP_pos)
									| (EBC_RGN_InitStruct->RGN_WS_HOLD 		<< EBC_WS_HOLD_pos)
									| (EBC_RGN_InitStruct->RGN_USESEPAR 	<< EBC_USESEPAR_pos)
									| (EBC_RGN_InitStruct->RGN_CACHEBLE 	<< EBC_CACHEBLE_pos)
									| (EBC_RGN_InitStruct->RGN_READ32 		<< EBC_READ32_pos)
									| (EBC_RGN_InitStruct->RGN_ENOCLK 		<< EBC_ENOCLK_pos)
									| (EBC_RGN_InitStruct->RGN_DIVOCLK 		<< EBC_DIVOCLK_pos);									
	switch (RGNx)
	{
		case RGN0: EXT_BUS_CNTR->RGN0_CNTRL = tmpreg_RGNx_CNTRL;break;
		case RGN1: EXT_BUS_CNTR->RGN1_CNTRL = tmpreg_RGNx_CNTRL;break;
		case RGN2: EXT_BUS_CNTR->RGN2_CNTRL = tmpreg_RGNx_CNTRL;break;
		case RGN3: EXT_BUS_CNTR->RGN3_CNTRL = tmpreg_RGNx_CNTRL;break;
		case RGN4: EXT_BUS_CNTR->RGN4_CNTRL = tmpreg_RGNx_CNTRL;break;
		case RGN5: EXT_BUS_CNTR->RGN5_CNTRL = tmpreg_RGNx_CNTRL;break;
		case RGN6: EXT_BUS_CNTR->RGN6_CNTRL = tmpreg_RGNx_CNTRL;break;
		case RGN7: EXT_BUS_CNTR->RGN7_CNTRL = tmpreg_RGNx_CNTRL;break;			
	}

	
}

/**
  * @brief  Fills each EBC_RGN_InitStruct member with its default value.
  * @param  EBC_RGN_InitStruct: pointer to a EBC_RGN_InitTypeDef structure
  *         which will be initialized.
  * @retval None
  */
void EBC_RGNx_StructInit(EBC_RGN_InitTypeDef* EBC_RGN_InitStruct)
{
	EBC_RGN_InitStruct->RGN_ECCEN = DISABLE;
	EBC_RGN_InitStruct->RGN_ECCMODE = 0;
	EBC_RGN_InitStruct->RGN_RDYWAIT = 0;
	EBC_RGN_InitStruct->RGN_MODE = 0;
	EBC_RGN_InitStruct->RGN_ROM = 0;
	EBC_RGN_InitStruct->RGN_CPOL = 0;
	EBC_RGN_InitStruct->RGN_WS_ACTIVE = 0;
	EBC_RGN_InitStruct->RGN_WS_SETUP = 0;
	EBC_RGN_InitStruct->RGN_WS_HOLD = 0;
	EBC_RGN_InitStruct->RGN_USESEPAR = 0;
	EBC_RGN_InitStruct->RGN_CACHEBLE = DISABLE;
	EBC_RGN_InitStruct->RGN_READ32 = 0;
	EBC_RGN_InitStruct->RGN_ENOCLK = DISABLE;
	EBC_RGN_InitStruct->RGN_DIVOCLK = 0;		
}

/**
  * @brief  Enables or disables the EBC_RGNx.
  * @param 	RGNx: specifies the region number.
  *         @arg RGN0: region 0 (0x1000_0000 - 0x17FF_FFFF)
  *         @arg RGN1: region 1 (0x1800_0000 - 0x1FFF_FFFF)
  *         @arg RGN2: region 2 (0x5000_0000 - 0x57FF_FFFF)
  *         @arg RGN3: region 3 (0x5800_0000 - 0x5FFF_FFFF)
  *         @arg RGN4: region 4 (0x6000_0000 - 0x67FF_FFFF)
  *         @arg RGN5: region 5 (0x6800_0000 - 0x6FFF_FFFF)
  *         @arg RGN6: region 6 (0x7000_0000 - 0x7FFF_FFFF)
  *         @arg RGN7: region 7 (0x8000_0000 - 0xDFFF_FFFF)
  * @retval None
  */
void EBC_RGNx_Cmd(uint32_t RGNx, FunctionalState NewState)
{
	uint32_t tmpreg_CNTRL;

	/* Check the parameters */
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	assert_param(IS_EBC_RGNx(RGNx));

	switch (RGNx)
	{
		case RGN0: tmpreg_CNTRL = EXT_BUS_CNTR->RGN0_CNTRL;break;
		case RGN1: tmpreg_CNTRL = EXT_BUS_CNTR->RGN1_CNTRL;break;
		case RGN2: tmpreg_CNTRL = EXT_BUS_CNTR->RGN2_CNTRL;break;
		case RGN3: tmpreg_CNTRL = EXT_BUS_CNTR->RGN3_CNTRL;break;
		case RGN4: tmpreg_CNTRL = EXT_BUS_CNTR->RGN4_CNTRL;break;
		case RGN5: tmpreg_CNTRL = EXT_BUS_CNTR->RGN5_CNTRL;break;
		case RGN6: tmpreg_CNTRL = EXT_BUS_CNTR->RGN6_CNTRL;break;
		case RGN7: tmpreg_CNTRL = EXT_BUS_CNTR->RGN7_CNTRL;break;			
	}

	/* Form new value */
	if (NewState != DISABLE)
	{
		/* Enable EBC_RGNx by setting the EBC_RGNx_CNTRL_EN bit in the CNTRL register */
		tmpreg_CNTRL |= EBC_RGNx_EN;
	}
	else
	{
		/* Disable EBC_RGNx by resetting the EBC_RGNx_CNTRL_EN bit in the CNTRL register */
		tmpreg_CNTRL &= ~EBC_RGNx_EN;
	}

  /* Configure CNTRL register with new value */
	switch (RGNx)
	{
		case RGN0: EXT_BUS_CNTR->RGN0_CNTRL |= tmpreg_CNTRL;break;
		case RGN1: EXT_BUS_CNTR->RGN1_CNTRL |= tmpreg_CNTRL;break;
		case RGN2: EXT_BUS_CNTR->RGN2_CNTRL |= tmpreg_CNTRL;break;
		case RGN3: EXT_BUS_CNTR->RGN3_CNTRL |= tmpreg_CNTRL;break;
		case RGN4: EXT_BUS_CNTR->RGN4_CNTRL |= tmpreg_CNTRL;break;
		case RGN5: EXT_BUS_CNTR->RGN5_CNTRL |= tmpreg_CNTRL;break;
		case RGN6: EXT_BUS_CNTR->RGN6_CNTRL |= tmpreg_CNTRL;break;
		case RGN7: EXT_BUS_CNTR->RGN7_CNTRL |= tmpreg_CNTRL;break;			
	}
}

/**
  * @brief Set adddres to serial ECC
  * @param 	RGNx: specifies the region number.
  *         @arg RGN0: region 0 (0x1000_0000 - 0x17FF_FFFF)
  *         @arg RGN1: region 1 (0x1800_0000 - 0x1FFF_FFFF)
  *         @arg RGN2: region 2 (0x5000_0000 - 0x57FF_FFFF)
  *         @arg RGN3: region 3 (0x5800_0000 - 0x5FFF_FFFF)
  *         @arg RGN4: region 4 (0x6000_0000 - 0x67FF_FFFF)
  *         @arg RGN5: region 5 (0x6800_0000 - 0x6FFF_FFFF)
  *         @arg RGN6: region 6 (0x7000_0000 - 0x7FFF_FFFF)
  *         @arg RGN7: region 7 (0x8000_0000 - 0xDFFF_FFFF)
  * @param addr: value address 
  * @retval None
  */
void EBC_RGNx_Addr_serial_ECC (uint32_t RGNx, uint32_t addr)
{
	/* Check the parameters */
  assert_param(IS_EBC_RGNx(RGNx));
  
	switch (RGNx)
	{
		case RGN0: EXT_BUS_CNTR->RGN0_ECCBASE = addr;break;
		case RGN1: EXT_BUS_CNTR->RGN1_ECCBASE = addr;break;
		case RGN2: EXT_BUS_CNTR->RGN2_ECCBASE = addr;break;
		case RGN3: EXT_BUS_CNTR->RGN3_ECCBASE = addr;break;
		case RGN4: EXT_BUS_CNTR->RGN4_ECCBASE = addr;break;
		case RGN5: EXT_BUS_CNTR->RGN5_ECCBASE = addr;break;
		case RGN6: EXT_BUS_CNTR->RGN6_ECCBASE = addr;break;
		case RGN7: EXT_BUS_CNTR->RGN7_ECCBASE = addr;break;
	}
}

/**
  * @brief  Configurate the interruptions of EBC
  * @param 	RGNx: specifies the region number.
  *         @arg RGN0: region 0 (0x1000_0000 - 0x17FF_FFFF)
  *         @arg RGN1: region 1 (0x1800_0000 - 0x1FFF_FFFF)
  *         @arg RGN2: region 2 (0x5000_0000 - 0x57FF_FFFF)
  *         @arg RGN3: region 3 (0x5800_0000 - 0x5FFF_FFFF)
  *         @arg RGN4: region 4 (0x6000_0000 - 0x67FF_FFFF)
  *         @arg RGN5: region 5 (0x6800_0000 - 0x6FFF_FFFF)
  *         @arg RGN6: region 6 (0x7000_0000 - 0x7FFF_FFFF)
  *         @arg RGN7: region 7 (0x8000_0000 - 0xDFFF_FFFF)
  * @param  EBC_IT_FIX valeus:
  *					@arg EBC_IT_SEEC
  *					@arg EBC_IT_DEEC
  *					@arg EBC_FIX_SEEC
  *					@arg EBC_FIC_DEEC
  * @retval None
  */
void EBC_RGNx_ECCS_Config ( uint32_t RGNx, uint32_t EBC_IT_FIX, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_EBC_RGNx(RGNx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
  
	switch (RGNx)
	{
		case RGN0: 
			if (NewState != DISABLE)
				EXT_BUS_CNTR->RGN0_ECCS |= EBC_IT_FIX;
			else
				EXT_BUS_CNTR->RGN0_ECCS &= ~EBC_IT_FIX;
		break;
		
		case RGN1: 
			if (NewState != DISABLE)
				EXT_BUS_CNTR->RGN1_ECCS |= EBC_IT_FIX;
			else
				EXT_BUS_CNTR->RGN1_ECCS &= ~EBC_IT_FIX;
		break;
		
		case RGN2: 
			if (NewState != DISABLE)
				EXT_BUS_CNTR->RGN2_ECCS |= EBC_IT_FIX;
			else
				EXT_BUS_CNTR->RGN2_ECCS &= ~EBC_IT_FIX;
		break;
		
		case RGN3: 
			if (NewState != DISABLE)
				EXT_BUS_CNTR->RGN3_ECCS |= EBC_IT_FIX;
			else
				EXT_BUS_CNTR->RGN3_ECCS &= ~EBC_IT_FIX;
		break;
		
		case RGN4: 
			if (NewState != DISABLE)
				EXT_BUS_CNTR->RGN4_ECCS |= EBC_IT_FIX;
			else
				EXT_BUS_CNTR->RGN4_ECCS &= ~EBC_IT_FIX;;
		break;
		
		case RGN5: 
			if (NewState != DISABLE)
				EXT_BUS_CNTR->RGN5_ECCS |= EBC_IT_FIX;
			else
				EXT_BUS_CNTR->RGN5_ECCS &= ~EBC_IT_FIX;
		break;
		
		case RGN6: 
			if (NewState != DISABLE)
				EXT_BUS_CNTR->RGN6_ECCS |= EBC_IT_FIX;
			else
				EXT_BUS_CNTR->RGN6_ECCS &= ~EBC_IT_FIX;
		break;
		
		case RGN7: 
			if (NewState != DISABLE)
				EXT_BUS_CNTR->RGN7_ECCS |= EBC_IT_FIX;
			else
				EXT_BUS_CNTR->RGN7_ECCS &= ~EBC_IT_FIX;
		break;
	}
}

/**
  * @brief  
  * @param 	RGNx: specifies the region number.
  *         @arg RGN0: region 0 (0x1000_0000 - 0x17FF_FFFF)
  *         @arg RGN1: region 1 (0x1800_0000 - 0x1FFF_FFFF)
  *         @arg RGN2: region 2 (0x5000_0000 - 0x57FF_FFFF)
  *         @arg RGN3: region 3 (0x5800_0000 - 0x5FFF_FFFF)
  *         @arg RGN4: region 4 (0x6000_0000 - 0x67FF_FFFF)
  *         @arg RGN5: region 5 (0x6800_0000 - 0x6FFF_FFFF)
  *         @arg RGN6: region 6 (0x7000_0000 - 0x7FFF_FFFF)
  *         @arg RGN7: region 7 (0x8000_0000 - 0xDFFF_FFFF)
  * @param 	CNT:
			@arg DEEC_CNT
			@arg SEEC_CNT
  * @retval Counter value
  */
uint32_t EBC_RGNx_GetErrorCount (uint32_t RGNx, Cnt_type CNT)
{
	uint32_t cnt_value;
	
	assert_param(IS_EBC_RGNx(RGNx));
	
		switch (RGNx)
	{
		case RGN0: 
			if (CNT)
				cnt_value = (EXT_BUS_CNTR->RGN0_ECCS &EBC_CNT_SEEC_MSK)>>EBC_CNT_SEEC_Pos;
			else
				cnt_value = (EXT_BUS_CNTR->RGN0_ECCS &EBC_CNT_DEEC_MSK)>>EBC_CNT_DEEC_Pos;
		break;
		
		case RGN1: 
			if (CNT)
				cnt_value = (EXT_BUS_CNTR->RGN1_ECCS &EBC_CNT_SEEC_MSK)>>EBC_CNT_SEEC_Pos;
			else
				cnt_value = (EXT_BUS_CNTR->RGN1_ECCS &EBC_CNT_DEEC_MSK)>>EBC_CNT_DEEC_Pos;
		break;
		
		case RGN2: 
			if (CNT)
				cnt_value = (EXT_BUS_CNTR->RGN2_ECCS &EBC_CNT_SEEC_MSK)>>EBC_CNT_SEEC_Pos;
			else
				cnt_value = (EXT_BUS_CNTR->RGN2_ECCS &EBC_CNT_DEEC_MSK)>>EBC_CNT_DEEC_Pos;
		break;
		
		case RGN3: 
			if (CNT)
				cnt_value = (EXT_BUS_CNTR->RGN3_ECCS &EBC_CNT_SEEC_MSK)>>EBC_CNT_SEEC_Pos;
			else
				cnt_value = (EXT_BUS_CNTR->RGN3_ECCS &EBC_CNT_DEEC_MSK)>>EBC_CNT_DEEC_Pos;
		break;
		
		case RGN4: 
			if (CNT)
				cnt_value = (EXT_BUS_CNTR->RGN4_ECCS &EBC_CNT_SEEC_MSK)>>EBC_CNT_SEEC_Pos;
			else
				cnt_value = (EXT_BUS_CNTR->RGN4_ECCS &EBC_CNT_DEEC_MSK)>>EBC_CNT_DEEC_Pos;
		break;
		
		case RGN5: 
			if (CNT)
				cnt_value = (EXT_BUS_CNTR->RGN5_ECCS &EBC_CNT_SEEC_MSK)>>EBC_CNT_SEEC_Pos;
			else
				cnt_value = (EXT_BUS_CNTR->RGN5_ECCS &EBC_CNT_DEEC_MSK)>>EBC_CNT_DEEC_Pos;
		break;
		
		case RGN6: 
			if (CNT)
				cnt_value = (EXT_BUS_CNTR->RGN6_ECCS &EBC_CNT_SEEC_MSK)>>EBC_CNT_SEEC_Pos;
			else
				cnt_value = (EXT_BUS_CNTR->RGN6_ECCS &EBC_CNT_DEEC_MSK)>>EBC_CNT_DEEC_Pos;
		break;
		
		case RGN7: 
			if (CNT)
				cnt_value = (EXT_BUS_CNTR->RGN7_ECCS &EBC_CNT_SEEC_MSK)>>EBC_CNT_SEEC_Pos;
			else
				cnt_value = (EXT_BUS_CNTR->RGN7_ECCS &EBC_CNT_DEEC_MSK)>>EBC_CNT_DEEC_Pos;
		break;
	}
	return cnt_value;
}

/**
  * @brief  Configures the timing parameters for RGNx.
  * @param  RGNx: specifies the region number.
  *         @arg RGN0: region 0 (0x1000_0000 - 0x17FF_FFFF)
  *         @arg RGN1: region 1 (0x1800_0000 - 0x1FFF_FFFF)
  *         @arg RGN2: region 2 (0x5000_0000 - 0x57FF_FFFF)
  *         @arg RGN3: region 3 (0x5800_0000 - 0x5FFF_FFFF)
  *         @arg RGN4: region 4 (0x6000_0000 - 0x67FF_FFFF)
  *         @arg RGN5: region 5 (0x6800_0000 - 0x6FFF_FFFF)
  *         @arg RGN6: region 6 (0x7000_0000 - 0x7FFF_FFFF)
  *         @arg RGN7: region 7 (0x8000_0000 - 0xDFFF_FFFF)
  * @param  RGN_WS_SETUP: specifies the setup phase time.
  *         This parameter must be a number between 0 and 15.
  * @param  RGN_WS_ACTIVE: specifies the active phase time.
  *         This parameter must be a number between 0 and 255.
  * @param  RGN_WS_HOLD: specifies the hold phase time.
  *         This parameter must be a number between 0 and 15.
  * @retval None
  */
void EBC_RGNx_WSConfig(uint32_t RGNx, uint32_t RGN_WS_SETUP, uint32_t RGN_WS_ACTIVE, uint32_t RGN_WS_HOLD)
{
    uint32_t tmpreg_CNTRL;
    __IO uint32_t *EBC_RGNx_CNTRL_ptr;
    /* Check the parameters */
    assert_param(IS_EBC_RGNx(RGNx));
    assert_param(IS_EBC_RGNx_WS_SETUP(RGN_WS_SETUP));
    assert_param(IS_EBC_RGNx_WS_ACTIVE(RGN_WS_ACTIVE));
    assert_param(IS_EBC_RGNx_WS_HOLD(RGN_WS_HOLD));
    
    EBC_RGNx_CNTRL_ptr = (uint32_t *)(&EXT_BUS_CNTR->RGN0_CNTRL) + RGNx;
    
    /* Clear WS bits */
    tmpreg_CNTRL = *EBC_RGNx_CNTRL_ptr & (~(EBC_WS_SETUP_MSK | EBC_WS_ACTIVE_MSK | EBC_WS_HOLD_MSK));
    
    /* Form new value */
    tmpreg_CNTRL |= RGN_WS_SETUP<<EBC_WS_SETUP_pos | RGN_WS_ACTIVE<<EBC_WS_ACTIVE_pos | RGN_WS_HOLD<<EBC_WS_HOLD_pos;
    
    /* Configure CNTRL register with new value */
    *EBC_RGNx_CNTRL_ptr = tmpreg_CNTRL;
}
	
/** @} */ /* End of group EBC_Private_Functions */

/** @} */ /* End of group EBC */

/** @} */ /* End of group __MDR32F8_StdPeriph_Driver */

/******************* (C) COPYRIGHT 2021 Milandr *********************************
*
* END OF FILE mdr32f8_ebc.c */

