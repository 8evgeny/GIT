/**
  ******************************************************************************
  * @file    dp83848.c
  * @author  dmib@@ETS
  * @brief   This file contains DP83848 PHY controller driver implementation
  ******************************************************************************
  */
#include "stm32h7xx_hal.h"
#include "dp83848.h"
#include "rs232_printf.h"

/// DP83848 software reset timeout
#define DP83848_SW_RESET_TO    ((uint32_t)500U)
 
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

int32_t DP83848_Init(DP83848_Object_t *pObj)
{
   uint32_t tickstart = 0, regvalue = 0;
   int32_t status = DP83848_STATUS_OK;
RS232Puts("DP83848_Init  1 \n") ;
   if(pObj->Is_Initialized == 0)
   {

	 if(pObj->IO.Init != 0)
     {
       // GPIO and Clocks initialization
       pObj->IO.Init();
     }
   
     pObj->DevAddr = DP83848_DEV_ADDR;
	    
	 if (pObj->IO.ReadReg(pObj->DevAddr, DP83848_PHYCR, &regvalue) < 0)
	 {
	    return DP83848_STATUS_READ_ERROR;
	 }
   
     if ((regvalue & DP83848_PHYCR_PHYADR) != pObj->DevAddr)
     {
        return DP83848_STATUS_ADDRESS_ERROR;
     }
	 
     // set a software reset
	 if(pObj->IO.WriteReg(pObj->DevAddr, DP83848_BMCR, DP83848_BMCR_SOFT_RESET | DP83848_BMCR_AUTONEGO_EN) >= 0)
	 {
		  // get software reset status
		  if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_BMCR, &regvalue) >= 0)
		  {
			tickstart = pObj->IO.GetTick();

			// wait until software reset is done or timeout occured
			while(regvalue & DP83848_BMCR_SOFT_RESET)
			{
			  if((pObj->IO.GetTick() - tickstart) <= DP83848_SW_RESET_TO)
			  {
				if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_BMCR, &regvalue) < 0)
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
       
     //set straped autonegotiation caps
     if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_ANAR, &regvalue) >= 0)
     {
       regvalue |= DP83848_ANAR_10BASE_T|DP83848_ANAR_10BASE_T_FD|DP83848_ANAR_100BASE_TX|DP83848_ANAR_100BASE_TX_FD;
       if(pObj->IO.WriteReg(pObj->DevAddr, DP83848_ANAR, regvalue) < 0)
       {
         return DP83848_STATUS_WRITE_ERROR;
       }
     }
     else
     {
       return DP83848_STATUS_READ_ERROR;
     }

     //set Auto MDIX
     if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_PHYCR, &regvalue) >= 0)
     {
       regvalue |= DP83848_PHYCR_MDIX_EN;
       if(pObj->IO.WriteReg(pObj->DevAddr, DP83848_PHYCR, regvalue) < 0)
       {
         return DP83848_STATUS_WRITE_ERROR;
       }
     }
     else
     {
       return DP83848_STATUS_READ_ERROR;
     }

     HAL_Delay(1000);

     // restart autonegotiation
     if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_BMCR, &regvalue) >= 0)
      {
        regvalue |= DP83848_BMCR_AUTONEGO_EN;
        if(pObj->IO.WriteReg(pObj->DevAddr, DP83848_BMCR, regvalue) < 0)
        {
          status =  DP83848_STATUS_WRITE_ERROR;
        }
        regvalue |= DP83848_BMCR_RESTART_AUTONEGO;
        if(pObj->IO.WriteReg(pObj->DevAddr, DP83848_BMCR, regvalue) < 0)
        {
          status =  DP83848_STATUS_WRITE_ERROR;
        }
      }
      else
      {
        status = DP83848_STATUS_READ_ERROR;
      }

     pObj->Is_Initialized = 1;
   }

   return status;
 }

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

int32_t DP83848_DisablePowerDownMode(DP83848_Object_t *pObj)
{
  uint32_t readval = 0;
  int32_t status = DP83848_STATUS_OK;
  
  if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_BMCR, &readval) >= 0)
  {
    readval &= ~DP83848_BMCR_POWER_DOWN;
  
    // Apply configuration
    if(pObj->IO.WriteReg(pObj->DevAddr, DP83848_BMCR, readval) < 0)
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

int32_t DP83848_EnablePowerDownMode(DP83848_Object_t *pObj)
{
  uint32_t readval = 0;
  int32_t status = DP83848_STATUS_OK;
  
  if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_BMCR, &readval) >= 0)
  {
    readval |= DP83848_BMCR_POWER_DOWN;
  
    // Apply configuration
    if(pObj->IO.WriteReg(pObj->DevAddr, DP83848_BMCR, readval) < 0)
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
  
  if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_BMCR, &readval) >= 0)
  {
    readval |= DP83848_BMCR_AUTONEGO_EN;
    readval |= DP83848_BMCR_RESTART_AUTONEGO;
  
    // Apply configuration
    if(pObj->IO.WriteReg(pObj->DevAddr, DP83848_BMCR, readval) < 0)
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

int32_t DP83848_GetLinkState(DP83848_Object_t *pObj)
{
  uint32_t readval = 0;

  // Read Status register
  if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_BMSR, &readval) < 0)
  {
    return DP83848_STATUS_READ_ERROR;
  }

  if((readval & DP83848_BMSR_LINK_STATUS) == 0)
  {
    // Return Link Down status
    return DP83848_STATUS_LINK_DOWN;
  }
  
  // Check Auto negotiaition
  if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_BMCR, &readval) < 0)
  {
    return DP83848_STATUS_READ_ERROR;
  }
  
  if((readval & DP83848_BMCR_AUTONEGO_EN) != DP83848_BMCR_AUTONEGO_EN)
  {
    if(((readval & DP83848_BMCR_SPEED_SELECT) == DP83848_BMCR_SPEED_SELECT) && ((readval & DP83848_BMCR_DUPLEX_MODE) == DP83848_BMCR_DUPLEX_MODE))
    {
      return DP83848_STATUS_100MBITS_FULLDUPLEX;
    }
    else if ((readval & DP83848_BMCR_SPEED_SELECT) == DP83848_BMCR_SPEED_SELECT)
    {
      return DP83848_STATUS_100MBITS_HALFDUPLEX;
    }        
    else if ((readval & DP83848_BMCR_DUPLEX_MODE) == DP83848_BMCR_DUPLEX_MODE)
    {
      return DP83848_STATUS_10MBITS_FULLDUPLEX;
    }
    else
    {
      return DP83848_STATUS_10MBITS_HALFDUPLEX;
    }  		
  }
  else // Auto Nego enabled
  {
    if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_PHYSTS, &readval) < 0)
    {
      return DP83848_STATUS_READ_ERROR;
    }

    // Check if auto nego not done
    if((readval & DP83848_PHYSTS_AUTONEG_CPLT) == 0)
    {
      return DP83848_STATUS_AUTONEGO_NOTDONE;
    }

    if(((readval & DP83848_PHYSTS_SPEED10) == DP83848_PHYSTS_SPEED10) && ((readval & DP83848_PHYSTS_DUPLEX) == DP83848_PHYSTS_DUPLEX))
    {
      return DP83848_STATUS_10MBITS_FULLDUPLEX;
    }
    else if ((readval & DP83848_PHYSTS_SPEED10) == DP83848_PHYSTS_SPEED10)
    {
      return DP83848_STATUS_10MBITS_HALFDUPLEX;
    }        
    else if ((readval & DP83848_PHYSTS_DUPLEX) == DP83848_PHYSTS_DUPLEX)
    {
      return DP83848_STATUS_100MBITS_FULLDUPLEX;
    }
    else
    {
      return DP83848_STATUS_100MBITS_HALFDUPLEX;
    }  		

  }
}

int32_t DP83848_SetLinkState(DP83848_Object_t *pObj, uint32_t LinkState)
{

  uint32_t regvalue = 0;
  int32_t status = DP83848_STATUS_OK;
  
  if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_BMCR, &regvalue) >= 0)
  {
    // Disable link config (Auto nego, speed and duplex)
    regvalue &= ~(DP83848_BMCR_AUTONEGO_EN | DP83848_BMCR_SPEED_SELECT | DP83848_BMCR_DUPLEX_MODE);
    
    if(LinkState == DP83848_STATUS_100MBITS_FULLDUPLEX)
    {
      regvalue |= (DP83848_BMCR_SPEED_SELECT | DP83848_BMCR_DUPLEX_MODE);
    }
    else if (LinkState == DP83848_STATUS_100MBITS_HALFDUPLEX)
    {
      regvalue |= DP83848_BMCR_SPEED_SELECT;
    }
    else if (LinkState == DP83848_STATUS_10MBITS_FULLDUPLEX)
    {
      regvalue |= DP83848_BMCR_DUPLEX_MODE;
    }
    else
    {
      // Wrong link status parameter
      status = DP83848_STATUS_ERROR;
    }	
  }
  else
  {
    status = DP83848_STATUS_READ_ERROR;
  }
  
  if(status == DP83848_STATUS_OK)
  {
    // Apply configuration
    if(pObj->IO.WriteReg(pObj->DevAddr, DP83848_BMCR, regvalue) < 0)
    {
      status = DP83848_STATUS_WRITE_ERROR;
    }
  }
  
  return status;
}

int32_t DP83848_EnableLoopbackMode(DP83848_Object_t *pObj)
{
  uint32_t readval = 0;
  int32_t status = DP83848_STATUS_OK;
  
  if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_BMCR, &readval) >= 0)
  {
    readval |= DP83848_BMCR_LOOPBACK;
    
    // Apply configuration
    if(pObj->IO.WriteReg(pObj->DevAddr, DP83848_BMCR, readval) < 0)
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

int32_t DP83848_DisableLoopbackMode(DP83848_Object_t *pObj)
{
  uint32_t readval = 0;
  int32_t status = DP83848_STATUS_OK;
  
  if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_BMCR, &readval) >= 0)
  {
    readval &= ~DP83848_BMCR_LOOPBACK;
  
    // Apply configuration
    if(pObj->IO.WriteReg(pObj->DevAddr, DP83848_BMCR, readval) < 0)
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

int32_t DP83848_EnableIT(DP83848_Object_t *pObj, uint32_t Interrupt)
{
  uint32_t readval = 0;
  int32_t status = DP83848_STATUS_OK;
  
  if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_MISR, &readval) >= 0)
  {
    readval |= Interrupt;
  
    // Apply configuration
    if(pObj->IO.WriteReg(pObj->DevAddr, DP83848_MISR, readval) < 0)
    {
      status =  DP83848_STATUS_WRITE_ERROR;
    }
  }
  else
  {
    status = DP83848_STATUS_READ_ERROR;
  }

  if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_MICR, &readval) >= 0)
  {
    readval |= DP83848_MICR_INT_EN;
  
    // Apply configuration
    if(pObj->IO.WriteReg(pObj->DevAddr, DP83848_MICR, readval) < 0)
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

int32_t DP83848_DisableIT(DP83848_Object_t *pObj, uint32_t Interrupt)
{
  uint32_t readval = 0;
  int32_t status = DP83848_STATUS_OK;
  
  if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_MISR, &readval) >= 0)
  {
    readval &= ~Interrupt;
  
    // Apply configuration
    if(pObj->IO.WriteReg(pObj->DevAddr, DP83848_MISR, readval) < 0)
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

int32_t  DP83848_ClearIT(DP83848_Object_t *pObj, uint32_t Interrupt)
{
  uint32_t readval = 0;
  int32_t status = DP83848_STATUS_OK;
  
  if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_MISR, &readval) < 0)
  {
    status =  DP83848_STATUS_READ_ERROR;
  }
  
  return status;
}

int32_t DP83848_GetITStatus(DP83848_Object_t *pObj, uint32_t Interrupt)
{
  uint32_t readval = 0;
  int32_t status = 0;

  if(pObj->IO.ReadReg(pObj->DevAddr, DP83848_MISR, &readval) >= 0)
  {
    status = (((readval >> 8) & Interrupt) == Interrupt);
  }
  else
  {
    status = DP83848_STATUS_READ_ERROR;
  }
	
  return status;
}

int32_t DP83848_GetFlagByMask(DP83848_Object_t *pObj,uint32_t reg, uint16_t mask)
{
  uint32_t readval;
  if(pObj->IO.ReadReg(pObj->DevAddr, reg, &readval) >= 0)
  {
      if (readval & mask) return 1; else return 0;
  }
  else
  {
	  return DP83848_STATUS_READ_ERROR;
  }

}

int32_t DP83848_SetFlagByMask(DP83848_Object_t *pObj, uint32_t reg, uint16_t mask)
{
  uint32_t readval;
  int32_t status = DP83848_STATUS_OK;

  if(pObj->IO.ReadReg(pObj->DevAddr, reg, &readval) >= 0)
  {
    readval |= mask;

    // Apply configuration
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

int32_t DP83848_ResetFlagByMask(DP83848_Object_t *pObj, uint32_t reg, uint16_t mask)
{
  uint32_t readval;
  int32_t status = DP83848_STATUS_OK;

  if(pObj->IO.ReadReg(pObj->DevAddr, reg, &readval) >= 0)
  {
    readval &= (~mask);

    // Apply configuration
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
