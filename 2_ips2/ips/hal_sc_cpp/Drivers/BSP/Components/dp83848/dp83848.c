/** 
  ******************************************************************************
  * @file    DP83848.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    08-March-2017
  * @brief   This file provides a set of functions needed to manage the LAN742
  *          PHY devices.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "dp83848.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Component
  * @{
  */ 
  
/** @defgroup DP83848 DP83848
  * @{
  */   
  
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @defgroup DP83848_Private_Defines DP83848 Private Defines
  * @{
  */
#define DP83848_SW_RESET_TO    ((uint32_t)500U)
#define DP83848_INIT_TO        ((uint32_t)2000U)
#define DP83848_MAX_DEV_ADDR   ((uint32_t)31U)
/**
  * @}
  */
 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/** @defgroup DP83848_Private_Functions DP83848 Private Functions
  * @{
  */
    
/**
  * @brief  Register IO functions to component object
  * @param  pObj: device object  of DP83848_Object_t.
  * @param  ioctx: holds device IO functions.  
  * @retval DP83848_STATUS_OK  if OK
  *         DP83848_STATUS_ERROR if missing mandatory function
  */
int32_t  DP83848_RegisterBusIO(DP83848_Object_t *pObj, DP83848_IOCtx_t *ioctx)
{
  if(!pObj || !ioctx->ReadReg || !ioctx->WriteReg || !ioctx->GetTick)
  {
    return DP83848_STATUS_ERROR;
  }
  
  pObj->IO.Init = ioctx->Init;
  pObj->IO.DeInit = ioctx->DeInit;
  pObj->IO.ReadReg = ioctx->ReadReg;
  pObj->IO.WriteReg = ioctx->WriteReg;
  pObj->IO.GetTick = ioctx->GetTick;
  
  return DP83848_STATUS_OK;
}

/**
  * @brief  Initialize the DP83848 and configure the needed hardware resources
  * @param  pObj: device object DP83848_Object_t.
  * @retval DP83848_STATUS_OK  if OK
  *         DP83848_STATUS_ADDRESS_ERROR if cannot find device address
  *         DP83848_STATUS_READ_ERROR if connot read register
  *         DP83848_STATUS_WRITE_ERROR if connot write to register
  *         DP83848_STATUS_RESET_TIMEOUT if cannot perform a software reset
  */

int32_t readPHYDevice(DP83848_Object_t *pObj)
{
	uint32_t BMCR=0,BMSR=0,PHYIDR=0,PHYIDR2=0,ANAR=0,ANLPAR=0,ANER=0,ANNPTR=0,PHYSTS=0,MICR=0,MISR=0,FCSCR=0,RECR=0,PCSR=0,RBR=0,PHYCR=0,BT10SERIAL=0,CDCTRL1=0,EDCR=0,addr=0;
	addr=pObj->DevAddr;
	pObj->IO.ReadReg(addr, ((uint16_t)0x0000U), &BMCR);
	pObj->IO.ReadReg(addr, ((uint16_t)0x0001U), &BMSR);
	pObj->IO.ReadReg(addr, ((uint16_t)0x0002U), &PHYIDR);
	pObj->IO.ReadReg(addr, ((uint16_t)0x0003U), &PHYIDR2);
	pObj->IO.ReadReg(addr, ((uint16_t)0x0004U), &ANAR);
	pObj->IO.ReadReg(addr, ((uint16_t)0x0005U), &ANLPAR);
	pObj->IO.ReadReg(addr, ((uint16_t)0x0006U), &ANER);
	pObj->IO.ReadReg(addr, ((uint16_t)0x0007U), &ANNPTR);
	pObj->IO.ReadReg(addr, ((uint16_t)0x0010U), &PHYSTS);
	pObj->IO.ReadReg(addr, ((uint16_t)0x0011U), &MICR);
	pObj->IO.ReadReg(addr, ((uint16_t)0x0012U), &MISR);
	pObj->IO.ReadReg(addr, ((uint16_t)0x0014U), &FCSCR);
	pObj->IO.ReadReg(addr, ((uint16_t)0x0015U), &RECR);
	pObj->IO.ReadReg(addr, ((uint16_t)0x0016U), &PCSR);
	pObj->IO.ReadReg(addr, ((uint16_t)0x0017U), &RBR);
	pObj->IO.ReadReg(addr, ((uint16_t)0x0019U), &PHYCR);
	pObj->IO.ReadReg(addr, ((uint16_t)0x001AU), &BT10SERIAL);
	pObj->IO.ReadReg(addr, ((uint16_t)0x001BU), &CDCTRL1);
	pObj->IO.ReadReg(addr, ((uint16_t)0x001DU), &EDCR);
	HAL_Delay(100);
}


 int32_t DP83848_Init(DP83848_Object_t *pObj)
 {
   uint32_t tickstart = 0, regvalue = 0, addr = 0;
   int32_t status = DP83848_STATUS_OK;
   
   if(pObj->Is_Initialized == 0)
   {
     if(pObj->IO.Init != 0)
     {
       /* GPIO and Clocks initialization */
       pObj->IO.Init();
     }
   
     /* for later check */
     pObj->DevAddr = DP83848_MAX_DEV_ADDR + 1;
   
     /* Get the device address from special mode register */  
     for(addr = 0; addr <= DP83848_MAX_DEV_ADDR; addr ++)
     {
       if(pObj->IO.ReadReg(addr, DP83848_PHYCR, &regvalue) < 0)
       { 
         status = DP83848_STATUS_READ_ERROR;
         /* Can't read from this device address GPIOInit
            continue with next address */
         continue;
       }
     
       if((regvalue & DP83848_PHYCR_PHY_ADDR) == addr)
       {
         pObj->DevAddr = addr;
         status = DP83848_STATUS_OK;
         break;
       }
     }
     //pObj->DevAddr=0;
     if(pObj->DevAddr > DP83848_MAX_DEV_ADDR)
     {
       status = DP83848_STATUS_ADDRESS_ERROR;
     }

     readPHYDevice(pObj);
     /* if device address is matched */
     if(status == DP83848_STATUS_OK)
     {
       /* set a software reset  */

	//if (0) {
		if(pObj->IO.WriteReg(pObj->DevAddr, DP83848_BCR, DP83848_BCR_SOFT_RESET) >= 0)
		{
		  /* get software reset status */
		  if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_BCR, &regvalue) >= 0)
		  {
			tickstart = pObj->IO.GetTick();

			/* wait until software reset is done or timeout occured  */
			while(regvalue & DP83848_BCR_SOFT_RESET)
			{
			  if((pObj->IO.GetTick() - tickstart) <= DP83848_SW_RESET_TO)
			  {
				if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_BCR, &regvalue) < 0)
				{
				  status = DP83848_STATUS_READ_ERROR;
				  break;
				}
			  }
			  else
			  {
				status = DP83848_STATUS_RESET_TIMEOUT;
			  }
			}
		  }
		  else
		  {
			status = DP83848_STATUS_READ_ERROR;
		  }
		}
		else
		{
		  status = DP83848_STATUS_WRITE_ERROR;
		}
	//}
     }
   }
      
   if(status == DP83848_STATUS_OK)
   {
     tickstart =  pObj->IO.GetTick();
     
     /* Wait for 2s to perform initialization */
     while((pObj->IO.GetTick() - tickstart) <= DP83848_INIT_TO)
     {
     }
     pObj->Is_Initialized = 1;
   }
   /*
   if(pObj->IO.ReadReg(pObj->DevAddr, ((uint16_t)0x0011U), &regvalue) >= 0)
     {
	   regvalue |= (((uint16_t)0x0001U)|((uint16_t)0x0002U));
       if(pObj->IO.WriteReg(pObj->DevAddr, ((uint16_t)0x0011U), regvalue) < 0)
       {
         status =  DP83848_STATUS_WRITE_ERROR;
       }
     }
     else
     {
       status = DP83848_STATUS_READ_ERROR;
     }
   */
   //DP83848_SetLinkState(pObj, DP83848_STATUS_10MBITS_HALFDUPLEX);
   readPHYDevice(pObj);
   ////////////////////////////////////////////////////////////////////////////////////////////////////

   if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_BCR, &regvalue) >= 0)
   {
     regvalue &= ~(DP83848_BCR_POWER_DOWN|DP83848_BCR_LOOPBACK|DP83848_BCR_AUTONEGO_EN);
     regvalue |= DP83848_BCR_ISOLATE;
     if(pObj->IO.WriteReg(pObj->DevAddr, DP83848_BCR, regvalue) < 0)
     {
       status =  DP83848_STATUS_WRITE_ERROR;
     }
   }
   else
   {
     status = DP83848_STATUS_READ_ERROR;
   }
   /////////////////////disable autonego, loopbak etc/ isolate phy
   if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_ANAR, &regvalue) >= 0)
   {
     regvalue |= DP83848_ANAR_10BASE_T|DP83848_ANAR_10BASE_T_FD|DP83848_ANAR_100BASE_TX|DP83848_ANAR_100BASE_TX_FD;
     if(pObj->IO.WriteReg(pObj->DevAddr, DP83848_ANAR, regvalue) < 0)
     {
       status =  DP83848_STATUS_WRITE_ERROR;
     }
   }
   else
   {
     status = DP83848_STATUS_READ_ERROR;
   }
   /////////////////////add autonego settings
   if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_BCR, &regvalue) >= 0)
      {
        regvalue |= DP83848_BCR_DUPLEX_MODE|DP83848_BCR_SPEED_SELECT|DP83848_BCR_AUTONEGO_EN;
        if(pObj->IO.WriteReg(pObj->DevAddr, DP83848_BCR, regvalue) < 0)
        {
          status =  DP83848_STATUS_WRITE_ERROR;
        }
        regvalue |= DP83848_BCR_RESTART_AUTONEGO;
        regvalue &= ~(DP83848_BCR_ISOLATE);
        if(pObj->IO.WriteReg(pObj->DevAddr, DP83848_BCR, regvalue) < 0)
                {
                  status =  DP83848_STATUS_WRITE_ERROR;
                }
      }
      else
      {
        status = DP83848_STATUS_READ_ERROR;
      }



   return status;
 }

/**
  * @brief  De-Initialize the DP83848 and it's hardware resources
  * @param  pObj: device object DP83848_Object_t.
  * @retval None
  */
int32_t DP83848_DeInit(DP83848_Object_t *pObj)
{
  if(pObj->Is_Initialized)
  {
    if(pObj->IO.DeInit != 0)
    {
      if(pObj->IO.DeInit() < 0)
      {
        return DP83848_STATUS_ERROR;
      }
    }
  
    pObj->Is_Initialized = 0;  
  }
  
  return DP83848_STATUS_OK;
}

/**
  * @brief  Disable the DP83848 power down mode.
  * @param  pObj: device object DP83848_Object_t.
  * @retval DP83848_STATUS_OK  if OK
  *         DP83848_STATUS_READ_ERROR if connot read register
  *         DP83848_STATUS_WRITE_ERROR if connot write to register
  */
int32_t DP83848_DisablePowerDownMode(DP83848_Object_t *pObj)
{
  uint32_t readval = 0;
  int32_t status = DP83848_STATUS_OK;
  
  if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_BCR, &readval) >= 0)
  {
    readval &= ~DP83848_BCR_POWER_DOWN;
  
    /* Apply configuration */
    if(pObj->IO.WriteReg(pObj->DevAddr, DP83848_BCR, readval) < 0)
    {
      status =  DP83848_STATUS_WRITE_ERROR;
    }
  }
  else
  {
    status = DP83848_STATUS_READ_ERROR;
  }
   
  return status;
}

/**
  * @brief  Enable the DP83848 power down mode.
  * @param  pObj: device object DP83848_Object_t.
  * @retval DP83848_STATUS_OK  if OK
  *         DP83848_STATUS_READ_ERROR if connot read register
  *         DP83848_STATUS_WRITE_ERROR if connot write to register
  */
int32_t DP83848_EnablePowerDownMode(DP83848_Object_t *pObj)
{
  uint32_t readval = 0;
  int32_t status = DP83848_STATUS_OK;
  
  if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_BCR, &readval) >= 0)
  {
    readval |= DP83848_BCR_POWER_DOWN;
  
    /* Apply configuration */
    if(pObj->IO.WriteReg(pObj->DevAddr, DP83848_BCR, readval) < 0)
    {
      status =  DP83848_STATUS_WRITE_ERROR;
    }
  }
  else
  {
    status = DP83848_STATUS_READ_ERROR;
  }
   
  return status;
}

/**
  * @brief  Start the auto negotiation process.
  * @param  pObj: device object DP83848_Object_t.
  * @retval DP83848_STATUS_OK  if OK
  *         DP83848_STATUS_READ_ERROR if connot read register
  *         DP83848_STATUS_WRITE_ERROR if connot write to register
  */
int32_t DP83848_DisableAutoNego(DP83848_Object_t *pObj)
{
  uint32_t readval = 0;
  int32_t status = DP83848_STATUS_OK;

  if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_BCR, &readval) >= 0)
  {
    readval &= (~DP83848_BCR_AUTONEGO_EN);


    /* Apply configuration */
    if(pObj->IO.WriteReg(pObj->DevAddr, DP83848_BCR, readval) < 0)
    {
      status =  DP83848_STATUS_WRITE_ERROR;
    }
  }
  else
  {
    status = DP83848_STATUS_READ_ERROR;
  }

  return status;
}





int32_t DP83848_StartAutoNego(DP83848_Object_t *pObj)
{
  uint32_t readval = 0;
  int32_t status = DP83848_STATUS_OK;
  
  if(DP83848_ResetFlag(pObj, DP83848_BCR, 12)<0){
	  return DP83848_STATUS_ERROR;
  }
  ////////
  readPHYDevice(pObj);
  HAL_Delay(1);
  ////////

  if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_BCR, &readval) >= 0)
  {
    readval |= DP83848_BCR_AUTONEGO_EN;
    readval |= DP83848_BCR_RESTART_AUTONEGO;
  
    /* Apply configuration */
    if(pObj->IO.WriteReg(pObj->DevAddr, DP83848_BCR, readval) < 0)
    {
      status =  DP83848_STATUS_WRITE_ERROR;
    }
  }
  else
  {
    status = DP83848_STATUS_READ_ERROR;
  }
  /*
  if(DP83848_Wait4Flag(pObj, DP83848_BSR, 5, DP83848_TRUE)<0){
	  return DP83848_STATUS_ERROR;
  }
  */

  return status;
}

int32_t DP83848_Isolate(DP83848_Object_t *pObj)
{
	return DP83848_SetFlag(pObj, DP83848_BCR, 10);
}

int32_t DP83848_DeIsolate(DP83848_Object_t *pObj)
{
	int32_t status=DP83848_ResetFlag(pObj, DP83848_BCR, 10);
	HAL_Delay(100);
	return status;
}


/*
 * ---------------------------------------------------------------
 * Bitwise MDIO operations
 *----------------------------------------------------------------
 */


int32_t DP83848_GetFlag(DP83848_Object_t *pObj,uint32_t reg,uint8_t pos)
{
  uint32_t readval = 0;
  if(pObj->IO.ReadReg(pObj->DevAddr, reg, &readval) >= 0)
  {
	  return DP83848_CAST2BOOL((readval&(1<<pos))); //((readval&(1<<pos))?DP83848_TRUE:DP83848_FALSE);
  }
  else
  {
	  return DP83848_STATUS_READ_ERROR;
  }

}

int32_t DP83848_SetFlag(DP83848_Object_t *pObj,uint32_t reg,uint8_t pos)
{
  uint32_t readval = 0;
  int32_t status = DP83848_STATUS_OK;

  if(pObj->IO.ReadReg(pObj->DevAddr, reg, &readval) >= 0)
  {
    readval |= (1<<pos);

    /* Apply configuration */
    if(pObj->IO.WriteReg(pObj->DevAddr, reg, readval) < 0)
    {
      status =  DP83848_STATUS_WRITE_ERROR;
    }
  }
  else
  {
    status = DP83848_STATUS_READ_ERROR;
  }

  return status;
}


int32_t DP83848_SetFlagByMask(DP83848_Object_t *pObj,uint32_t reg,uint16_t mask)
{
  uint32_t readval = 0;
  int32_t status = DP83848_STATUS_OK;

  if(pObj->IO.ReadReg(pObj->DevAddr, reg, &readval) >= 0)
  {
    readval |= mask;

    /* Apply configuration */
    if(pObj->IO.WriteReg(pObj->DevAddr, reg, readval) < 0)
    {
      status =  DP83848_STATUS_WRITE_ERROR;
    }
  }
  else
  {
    status = DP83848_STATUS_READ_ERROR;
  }

  return status;
}


int32_t DP83848_ResetFlag(DP83848_Object_t *pObj,uint32_t reg,uint8_t pos)
{
  uint32_t readval = 0;
  int32_t status = DP83848_STATUS_OK;

  if(pObj->IO.ReadReg(pObj->DevAddr, reg, &readval) >= 0)
  {
    readval &= ~(1<<pos);

    /* Apply configuration */
    if(pObj->IO.WriteReg(pObj->DevAddr, reg, readval) < 0)
    {
      status =  DP83848_STATUS_WRITE_ERROR;
    }
  }
  else
  {
    status = DP83848_STATUS_READ_ERROR;
  }

  return status;
}


int32_t DP83848_ResetFlagByMask(DP83848_Object_t *pObj,uint32_t reg,uint16_t mask)
{
  uint32_t readval = 0;
  int32_t status = DP83848_STATUS_OK;

  if(pObj->IO.ReadReg(pObj->DevAddr, reg, &readval) >= 0)
  {
    readval &= (~mask);

    /* Apply configuration */
    if(pObj->IO.WriteReg(pObj->DevAddr, reg, readval) < 0)
    {
      status =  DP83848_STATUS_WRITE_ERROR;
    }
  }
  else
  {
    status = DP83848_STATUS_READ_ERROR;
  }

  return status;
}


int32_t DP83848_Wait4Flag(DP83848_Object_t *pObj,uint32_t reg,uint8_t pos,int32_t state4wait)
{
  int32_t status = DP83848_STATUS_OK;
  int32_t flag=DP83848_GetFlag(pObj, reg, pos);
  if(flag < 0){
	  return flag;
  }
  while (flag!=DP83848_CAST2BOOL(state4wait)){
	  flag=DP83848_GetFlag(pObj, reg, pos);
	  if(flag<0){
		  return flag;
	  }
  }
  return DP83848_STATUS_OK;
}


/**
  * @brief  Get the link state of DP83848 device.
  * @param  pObj: Pointer to device object. 
  * @param  pLinkState: Pointer to link state
  * @retval DP83848_STATUS_LINK_DOWN  if link is down
  *         DP83848_STATUS_AUTONEGO_NOTDONE if Auto nego not completed
  *         DP83848_STATUS_100MBITS_FULLDUPLEX if 100Mb/s FD
  *         DP83848_STATUS_100MBITS_HALFDUPLEX if 100Mb/s HD
  *         DP83848_STATUS_10MBITS_FULLDUPLEX  if 10Mb/s FD
  *         DP83848_STATUS_10MBITS_HALFDUPLEX  if 10Mb/s HD
  *         DP83848_STATUS_READ_ERROR if connot read register
  *         DP83848_STATUS_WRITE_ERROR if connot write to register
  */
int32_t DP83848_GetLinkState(DP83848_Object_t *pObj)
{
  uint32_t readval = 0;

  /* Read Status register  */
  if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_BSR, &readval) < 0)
  {
    return DP83848_STATUS_READ_ERROR;
  }

  /* Read Status register again */
  if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_BSR, &readval) < 0)
  {
    return DP83848_STATUS_READ_ERROR;
  }

  if((readval & DP83848_BSR_LINK_STATUS) == 0)
  {
    /* Return Link Down status */
    return DP83848_STATUS_LINK_DOWN;
  }
  
  /* Check Auto negotiaition */
  if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_BCR, &readval) < 0)
  {
    return DP83848_STATUS_READ_ERROR;
  }
  
  if((readval & DP83848_BCR_AUTONEGO_EN) != DP83848_BCR_AUTONEGO_EN)
  {
    if(((readval & DP83848_BCR_SPEED_SELECT) == DP83848_BCR_SPEED_SELECT) && ((readval & DP83848_BCR_DUPLEX_MODE) == DP83848_BCR_DUPLEX_MODE))
    {
      return DP83848_STATUS_100MBITS_FULLDUPLEX;
    }
    else if ((readval & DP83848_BCR_SPEED_SELECT) == DP83848_BCR_SPEED_SELECT)
    {
      return DP83848_STATUS_100MBITS_HALFDUPLEX;
    }        
    else if ((readval & DP83848_BCR_DUPLEX_MODE) == DP83848_BCR_DUPLEX_MODE)
    {
      return DP83848_STATUS_10MBITS_FULLDUPLEX;
    }
    else
    {
      return DP83848_STATUS_10MBITS_HALFDUPLEX;
    }  		
  }
  else /* Auto Nego enabled */
  {
    if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_PHYSTS, &readval) < 0)
    {
      return DP83848_STATUS_READ_ERROR;
    }

    /* Check if auto nego not done */
    if((readval & DP83848_PHYSTS_AUTONEGO_DONE) == 0)
    {
      return DP83848_STATUS_AUTONEGO_NOTDONE;
    }

    if((readval & DP83848_PHYSTS_SPEEDMASK) == DP83848_PHYSTS_100BTX_FD)
    {
      return DP83848_STATUS_100MBITS_FULLDUPLEX;
    }
    else if ((readval & DP83848_PHYSTS_SPEEDMASK) == DP83848_PHYSTS_100BTX_HD)
    {
      return DP83848_STATUS_100MBITS_HALFDUPLEX;
    }
    else if ((readval & DP83848_PHYSTS_SPEEDMASK) == DP83848_PHYSTS_10BT_FD)
    {
      return DP83848_STATUS_10MBITS_FULLDUPLEX;
    }
    else
    {
      return DP83848_STATUS_10MBITS_HALFDUPLEX;
    }

  }
}

/**
  * @brief  Set the link state of DP83848 device.
  * @param  pObj: Pointer to device object. 
  * @param  pLinkState: link state can be one of the following
  *         DP83848_STATUS_100MBITS_FULLDUPLEX if 100Mb/s FD
  *         DP83848_STATUS_100MBITS_HALFDUPLEX if 100Mb/s HD
  *         DP83848_STATUS_10MBITS_FULLDUPLEX  if 10Mb/s FD
  *         DP83848_STATUS_10MBITS_HALFDUPLEX  if 10Mb/s HD
  * @retval DP83848_STATUS_OK  if OK
  *         DP83848_STATUS_ERROR  if parameter error
  *         DP83848_STATUS_READ_ERROR if connot read register
  *         DP83848_STATUS_WRITE_ERROR if connot write to register
  */
int32_t DP83848_SetLinkState(DP83848_Object_t *pObj, uint32_t LinkState)
{

  uint32_t bcrvalue = 0;
  int32_t status = DP83848_STATUS_OK;
  
  if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_BCR, &bcrvalue) >= 0)
  {
    /* Disable link config (Auto nego, speed and duplex) */
    bcrvalue &= ~(DP83848_BCR_AUTONEGO_EN | DP83848_BCR_SPEED_SELECT | DP83848_BCR_DUPLEX_MODE);
    
    if(LinkState == DP83848_STATUS_100MBITS_FULLDUPLEX)
    {
      bcrvalue |= (DP83848_BCR_SPEED_SELECT | DP83848_BCR_DUPLEX_MODE);
    }
    else if (LinkState == DP83848_STATUS_100MBITS_HALFDUPLEX)
    {
      bcrvalue |= DP83848_BCR_SPEED_SELECT;
    }
    else if (LinkState == DP83848_STATUS_10MBITS_FULLDUPLEX)
    {
      bcrvalue |= DP83848_BCR_DUPLEX_MODE;
    }
    else
    {
      /* Wrong link status parameter */
      status = DP83848_STATUS_ERROR;
    }	
  }
  else
  {
    status = DP83848_STATUS_READ_ERROR;
  }
  
  if(status == DP83848_STATUS_OK)
  {
    /* Apply configuration */
    if(pObj->IO.WriteReg(pObj->DevAddr, DP83848_BCR, bcrvalue) < 0)
    {
      status = DP83848_STATUS_WRITE_ERROR;
    }
  }
  
  return status;
}

/**
  * @brief  Enable loopback mode.
  * @param  pObj: Pointer to device object. 
  * @retval DP83848_STATUS_OK  if OK
  *         DP83848_STATUS_READ_ERROR if connot read register
  *         DP83848_STATUS_WRITE_ERROR if connot write to register
  */
int32_t DP83848_EnableLoopbackMode(DP83848_Object_t *pObj)
{
  uint32_t readval = 0;
  int32_t status = DP83848_STATUS_OK;
  
  if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_BCR, &readval) >= 0)
  {
    readval |= DP83848_BCR_LOOPBACK;
    
    /* Apply configuration */
    if(pObj->IO.WriteReg(pObj->DevAddr, DP83848_BCR, readval) < 0)
    {
      status = DP83848_STATUS_WRITE_ERROR;
    }
  }
  else
  {
    status = DP83848_STATUS_READ_ERROR;
  }
  
  return status;
}

/**
  * @brief  Disable loopback mode.
  * @param  pObj: Pointer to device object. 
  * @retval DP83848_STATUS_OK  if OK
  *         DP83848_STATUS_READ_ERROR if connot read register
  *         DP83848_STATUS_WRITE_ERROR if connot write to register
  */
int32_t DP83848_DisableLoopbackMode(DP83848_Object_t *pObj)
{
  uint32_t readval = 0;
  int32_t status = DP83848_STATUS_OK;
  
  if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_BCR, &readval) >= 0)
  {
    readval &= ~DP83848_BCR_LOOPBACK;
  
    /* Apply configuration */
    if(pObj->IO.WriteReg(pObj->DevAddr, DP83848_BCR, readval) < 0)
    {
      status =  DP83848_STATUS_WRITE_ERROR;
    }
  }
  else
  {
    status = DP83848_STATUS_READ_ERROR;
  }
   
  return status;
}

/**
  * @brief  Enable IT source.
  * @param  pObj: Pointer to device object. 
  * @param  Interrupt: IT source to be enabled
  *         should be a value or a combination of the following:
  *         DP83848_WOL_IT
  *         DP83848_ENERGYON_IT
  *         DP83848_AUTONEGO_COMPLETE_IT
  *         DP83848_REMOTE_FAULT_IT
  *         DP83848_LINK_DOWN_IT
  *         DP83848_AUTONEGO_LP_ACK_IT
  *         DP83848_PARALLEL_DETECTION_FAULT_IT
  *         DP83848_AUTONEGO_PAGE_RECEIVED_IT
  * @retval DP83848_STATUS_OK  if OK
  *         DP83848_STATUS_READ_ERROR if connot read register
  *         DP83848_STATUS_WRITE_ERROR if connot write to register
  */
int32_t DP83848_EnableIT(DP83848_Object_t *pObj, uint32_t Interrupt)
{
  uint32_t readval = 0;
  int32_t status = DP83848_STATUS_OK;
  
  if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_IMR, &readval) >= 0)
  {
    readval |= Interrupt;
  
    /* Apply configuration */
    if(pObj->IO.WriteReg(pObj->DevAddr, DP83848_IMR, readval) < 0)
    {
      status =  DP83848_STATUS_WRITE_ERROR;
    }
  }
  else
  {
    status = DP83848_STATUS_READ_ERROR;
  }
   
  return status;
}

/**
  * @brief  Disable IT source.
  * @param  pObj: Pointer to device object. 
  * @param  Interrupt: IT source to be disabled
  *         should be a value or a combination of the following:
  *         DP83848_WOL_IT
  *         DP83848_ENERGYON_IT
  *         DP83848_AUTONEGO_COMPLETE_IT
  *         DP83848_REMOTE_FAULT_IT
  *         DP83848_LINK_DOWN_IT
  *         DP83848_AUTONEGO_LP_ACK_IT
  *         DP83848_PARALLEL_DETECTION_FAULT_IT
  *         DP83848_AUTONEGO_PAGE_RECEIVED_IT
  * @retval DP83848_STATUS_OK  if OK
  *         DP83848_STATUS_READ_ERROR if connot read register
  *         DP83848_STATUS_WRITE_ERROR if connot write to register
  */
int32_t DP83848_DisableIT(DP83848_Object_t *pObj, uint32_t Interrupt)
{
  uint32_t readval = 0;
  int32_t status = DP83848_STATUS_OK;
  
  if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_IMR, &readval) >= 0)
  {
    readval &= ~Interrupt;
  
    /* Apply configuration */
    if(pObj->IO.WriteReg(pObj->DevAddr, DP83848_IMR, readval) < 0)
    {
      status = DP83848_STATUS_WRITE_ERROR;
    }
  }
  else
  {
    status = DP83848_STATUS_READ_ERROR;
  }
   
  return status;
}

/**
  * @brief  Clear IT flag.
  * @param  pObj: Pointer to device object. 
  * @param  Interrupt: IT flag to be cleared
  *         should be a value or a combination of the following:
  *         DP83848_WOL_IT
  *         DP83848_ENERGYON_IT
  *         DP83848_AUTONEGO_COMPLETE_IT
  *         DP83848_REMOTE_FAULT_IT
  *         DP83848_LINK_DOWN_IT
  *         DP83848_AUTONEGO_LP_ACK_IT
  *         DP83848_PARALLEL_DETECTION_FAULT_IT
  *         DP83848_AUTONEGO_PAGE_RECEIVED_IT
  * @retval DP83848_STATUS_OK  if OK
  *         DP83848_STATUS_READ_ERROR if connot read register
  */
int32_t  DP83848_ClearIT(DP83848_Object_t *pObj, uint32_t Interrupt)
{
  uint32_t readval = 0;
  int32_t status = DP83848_STATUS_OK;
  
  if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_ISFR, &readval) < 0)
  {
    status =  DP83848_STATUS_READ_ERROR;
  }
  
  return status;
}

/**
  * @brief  Get IT Flag status.
  * @param  pObj: Pointer to device object. 
  * @param  Interrupt: IT Flag to be checked, 
  *         should be a value or a combination of the following:
  *         DP83848_WOL_IT
  *         DP83848_ENERGYON_IT
  *         DP83848_AUTONEGO_COMPLETE_IT
  *         DP83848_REMOTE_FAULT_IT
  *         DP83848_LINK_DOWN_IT
  *         DP83848_AUTONEGO_LP_ACK_IT
  *         DP83848_PARALLEL_DETECTION_FAULT_IT
  *         DP83848_AUTONEGO_PAGE_RECEIVED_IT
  * @retval 1 IT flag is SET
  *         0 IT flag is RESET
  *         DP83848_STATUS_READ_ERROR if connot read register
  */
int32_t DP83848_GetITStatus(DP83848_Object_t *pObj, uint32_t Interrupt)
{
  uint32_t readval = 0;
  int32_t status = 0;

  if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_ISFR, &readval) >= 0)
  {
    status = ((readval & Interrupt) == Interrupt);
  }
  else
  {
    status = DP83848_STATUS_READ_ERROR;
  }
	
  return status;
}

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */      
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
