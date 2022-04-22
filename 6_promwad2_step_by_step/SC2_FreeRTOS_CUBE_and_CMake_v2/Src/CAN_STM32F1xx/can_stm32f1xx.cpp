/*!
 \file can_stm32f1xx.cpp

*/
#include "can_stm32f1xx.h"
#include "gpio_stm32f1xx.h"
//#include "command.h"

#ifdef __cplusplus
extern "C" {
#endif

osMutexDef (mutexRingBufferRx);
osMutexDef (mutexRingBufferTx);
osMailQDef (mail, 10, uint8_t);                     // mail queue definition
osThreadDef(sendCanThread, sendCanThread, osPriorityNormal, 1, configMINIMAL_STACK_SIZE*1); /*!< Define the attributes of the sendToCanThread functions */
osThreadDef(readCanThread, readCanThread, osPriorityNormal, 1, configMINIMAL_STACK_SIZE*1); /*!< Define the attributes of the sendToCanThread functions */// mail queue definition
static CAN_HandleTypeDef hcan1;
static CAN_FilterTypeDef fcan1;
static CAN_TxHeaderTypeDef TxMessage;
static CAN_RxHeaderTypeDef RxMessage;

static uint8_t aDataRx[8],
       aDataTx[2];

static uint32_t TxMailbox;

void CANInit()
{
    // CAN Init
    hcan1.Instance = CAN1;
    hcan1.Init.Prescaler = 18;
    hcan1.Init.Mode = CAN_MODE_NORMAL;
    hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
    hcan1.Init.TimeSeg1 = CAN_BS1_11TQ;
    hcan1.Init.TimeSeg2 = CAN_BS2_4TQ;
    hcan1.Init.TimeTriggeredMode = DISABLE;
    hcan1.Init.AutoBusOff = DISABLE;
    hcan1.Init.AutoWakeUp = DISABLE;
    hcan1.Init.AutoRetransmission = DISABLE;
    hcan1.Init.ReceiveFifoLocked = DISABLE;
    hcan1.Init.TransmitFifoPriority = ENABLE;

    if (HAL_CAN_Init(&hcan1) != HAL_OK) {
        while (1) {
            Debug::getInstance().dbg << "CAN Init Error!" << "\n";
        }
    }

    // CAN Init filter
    fcan1.FilterBank = 3;
    fcan1.FilterMode = CAN_FILTERMODE_IDLIST;
    fcan1.FilterScale = CAN_FILTERSCALE_16BIT;
    fcan1.FilterIdHigh = 0x10<<5;
    fcan1.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    fcan1.FilterActivation = ENABLE;
    fcan1.SlaveStartFilterBank = 14;

    if (HAL_CAN_ConfigFilter(&hcan1, &fcan1)) {
        while (1) {
            Debug::getInstance().dbg << "CAN Init Filter Error!" << "\n";
        }
    }

    if (HAL_CAN_Start(&hcan1) != HAL_OK) {
        while (1) {
            Debug::getInstance().dbg << "Start CAN Error!" << "\n";
        }
    }

    /* get configuration data to set ID for Rx Mailboxes  */
    switch (GPIO::getInstance()->getCFG()) {
    case 0: {
        GPIO::getInstance()->bedCfgFlag = true;
        TxMessage.StdId = 0x11;
    }
    break;
    case 1:
        TxMessage.StdId = 0x11; // idSLAVE1 = 0x11
        break;
    case 2:
        GPIO::getInstance()->detectMode();
        switch(GPIO::getInstance()->mode) {
        case 1:
            TxMessage.StdId = 0x12; // idSLAVE2 = 0x12
            break;
        case 2:
            TxMessage.StdId = 0x13; // idSLAVE3 = 0x13
            break;
        case 3:
            TxMessage.StdId = 0x14; // idSLAVE4 = 0x14
            break;
        }
        break;
    default: {
        TxMessage.StdId = 0x11;
    }
    break;
    }

    TxMessage.ExtId = 0x00;
    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.IDE = CAN_ID_STD;
    TxMessage.DLC = 2;
    TxMessage.TransmitGlobalTime = DISABLE;
}


[[ noreturn ]]
void sendCanThread(void const *arg)
{
    (void)arg;
    while(true) {
        osMutexWait(CAN::getInstance()->mutexRingBufferRx_id, osWaitForever);
        if (!CAN::getInstance()->ringBufferTx.isEmpty()) {
            osMutexRelease(CAN::getInstance()->mutexRingBufferRx_id);
            CAN::getInstance()->sendMessage();
        } else osMutexRelease(CAN::getInstance()->mutexRingBufferRx_id);

        osDelay(1);
    }

}


[[ noreturn ]]
void readCanThread(void const *arg)
{
    (void)arg;
    while(true) {
        CAN::getInstance()->read();

        osDelay(1);
    }

}

#ifdef __cplusplus
}
#endif


CAN::CAN()
{
    canHandle = &hcan1;

    mailID = osMailCreate(osMailQ(mail), NULL);         // mail queue creating

    mutexRingBufferRx_id = osMutexCreate(osMutex(mutexRingBufferRx));
    if (mutexRingBufferRx_id == nullptr) {
        while(1)
            Debug::getInstance().dbg << "Failed to create [mutexRingBufferRx]" << "\n";
    }

    mutexRingBufferTx_id = osMutexCreate(osMutex(mutexRingBufferTx));
    if (mutexRingBufferTx_id == nullptr) {
        while(1)
            Debug::getInstance().dbg << "Failed to create [mutexRingBufferRx]" << "\n";
    }

    if ((sendToCanThreadId = osThreadCreate( osThread(sendCanThread), nullptr)) == nullptr) {
        Debug::getInstance().dbg << "Failed to create [sendToCanThread]" << "\n";
        while(1);
    }

    if ((osThreadCreate( osThread(readCanThread), nullptr)) == nullptr) {
        Debug::getInstance().dbg << "Failed to create [readToCanThread]" << "\n";
        while(1);
    }

}

CAN *CAN::getInstance()
{
    if(!pInstance) pInstance = new CAN();
    return pInstance;
}

CAN *CAN::pInstance = nullptr;

HAL_StatusTypeDef CAN::read()
{

    HAL_StatusTypeDef status;
    std::fill(aDataRx, aDataRx + 2, 0);
    uint8_t tempType = 0,
            tempSequence = 0;

    if (HAL_CAN_GetRxFifoFillLevel(canHandle, CAN_RX_FIFO0) != 0) {
        status = HAL_CAN_GetRxMessage(canHandle, CAN_RX_FIFO0, &RxMessage, aDataRx);

        tempType = aDataRx[0];
        tempSequence = aDataRx[1];

        if (tempType == data || tempType == mikeLed) {

            tempRxPackage_.s.packetType = tempType;
            tempRxPackage_.s.sequence = tempSequence;

            for (uint8_t i = 2; i < 7; ++i) {

                if (tempSequence == 1)
                    tempRxPackage_.b[i] = aDataRx[i];
                else if (tempSequence == 2)
                    tempRxPackage_.b[i+5] = aDataRx[i];
            }

            if (tempSequence == 2) {
                osMutexWait(mutexRingBufferRx_id, osWaitForever);
                ringBufferRx.push(tempRxPackage_);  // ring buffer filling
                osMutexRelease(mutexRingBufferRx_id);
            }
        } else if (GPIO::getInstance()->testFlag && tempType == button) {

            tempRxPackage_.s.packetType = aDataRx[0];
            tempRxPackage_.s.payloadData[0] = aDataRx[1];

            osMutexWait(mutexRingBufferRx_id, osWaitForever);
            ringBufferRx.push(tempRxPackage_);  // ring buffer filling
            osMutexRelease(mutexRingBufferRx_id);
        }
    }

    return status;
}

void CAN::readToRingBuffer()
{
    evt = osMailGet(mailID, osWaitForever);   // mail waiting
    if(evt.status == osEventMail) {

        uint8_t *aDataRxPtr = (uint8_t*)evt.value.p;

        uint8_t tempType = *aDataRxPtr,
                tempSequence = *(aDataRxPtr+1);

        if (tempType == data) {

            if (tempSequence == 1) {

                tempRxPackage_.s.packetType = data;
                tempRxPackage_.s.sequence = *(aDataRxPtr+1);

            }
            for (uint8_t i = 2; i < 7; ++i) {
                tempRxPackage_.b[i] = *(aDataRxPtr+i);
            }

            if (tempSequence == 2) {
                osMutexWait(mutexRingBufferRx_id, osWaitForever);
                ringBufferRx.push(tempRxPackage_);  // ring buffer filling
                osMutexRelease(mutexRingBufferRx_id);
            }
        }

        osMailFree(mailID, aDataRxPtr);   // free memory allocated for mail

    }
}

HAL_StatusTypeDef CAN::sendData()
{
    HAL_StatusTypeDef status = HAL_CAN_AddTxMessage(canHandle, &TxMessage, aDataTx, &TxMailbox);
    while(HAL_CAN_GetTxMailboxesFreeLevel(canHandle) != 3);  // relise of Mailboxes waiting
    return status;
}

/*!
  \brief CAN1 Test Function
  \param None
  \retval None
  */
void CAN::test(void)
{
    canHandle->Init.Mode = CAN_MODE_LOOPBACK;
    canHandle->Init.Prescaler = 27;

    if (HAL_CAN_DeInit(canHandle) != HAL_OK) {
        while (1) {
            Debug::getInstance().dbg << "[TEST] CAN DeInit Error!" << "\n";
        }
    }
    if (HAL_CAN_Init(canHandle) != HAL_OK) {
        while (1) {
            Debug::getInstance().dbg << "[TEST] CAN Init Error!" << "\n";
        }
    }

    if (HAL_CAN_Start(canHandle) != HAL_OK) {
        while (1) {
            Debug::getInstance().dbg << "[TEST] Start CAN Error!" << "\n";
        }
    }

    TxMessage.StdId = 0x10;
}

void CAN::createPackage(uint8_t payload)
{
    packageTx.s.packetType = button;
    packageTx.s.payloadData = payload;

    osMutexWait(mutexRingBufferTx_id, osWaitForever);
    ringBufferTx.push(packageTx);
    osMutexRelease(mutexRingBufferTx_id);
}

void CAN::sendMessage()
{
    osMutexWait(mutexRingBufferTx_id, osWaitForever);
    uPackageTx temp = ringBufferTx.shift();
    osMutexRelease(mutexRingBufferTx_id);

    std::fill(aDataTx, aDataTx + sizeof(aDataTx), 0);

    aDataTx[0] = temp.s.packetType;
    aDataTx[1] = temp.s.payloadData;

    sendData();
}

#ifdef __cplusplus
extern "C" {
#endif

/*!
  \brief CAN MSP Initialization


          This function configures the used hardware resources:
             - Peripheral's clock enable
             - Peripheral's GPIO Configuration
             - NVIC configuration for interrupt request enable


  \param huart: CAN handle pointer
  \retval None
  */
void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan1)
{
    if (hcan1->Instance == CAN1) {

        GPIO_InitTypeDef  GPIO_InitStruct;

        /*##-1- Enable peripherals and GPIO Clocks #################################*/
        /* Enable GPIO TX/RX clock */
        __HAL_RCC_GPIOA_CLK_ENABLE();

        /* Enable CAN1 clock */
        __HAL_RCC_CAN1_CLK_ENABLE();

        /*##-2- Configure peripheral GPIO ##########################################*/
        /* CAN1 TX GPIO pin configuration  */
        GPIO_InitStruct.Pin = GPIO_PIN_11;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_12;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /*##-3- Enable interrupt request for CAN1 #########################################*/
        // __HAL_CAN_ENABLE_IT(hcan1, CAN_IT_TX_MAILBOX_EMPTY); //Transmit mailbox empty
        __HAL_CAN_ENABLE_IT(hcan1, CAN_IT_RX_FIFO0_MSG_PENDING); //FIFO 0 message pending
        __HAL_CAN_ENABLE_IT(hcan1, CAN_IT_RX_FIFO0_FULL); //FIFO 0 full


        /*##-4- Configure the NVIC for CAN1 #########################################*/
        /* NVIC configuration for CAN transfer complete interrupt (CAN1_TX) */
        HAL_NVIC_SetPriority(CAN1_TX_IRQn, 5, 1);
        HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);

        /* NVIC configuration for CAN transfer complete interrupt (CAN1_RX) */
//        HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 5, 0);
//        HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
    }
}

/*!
  \brief CAN MSP De-Initialization
          This function frees the used hardware resources:
            - Disable the Peripheral's clock
            - Revert GPIO and NVIC configuration to their default state
  \param huart: CAN handle pointer
  \retval None
  */
void HAL_CAN_MspDeInit(CAN_HandleTypeDef *hcan1)
{
    if (hcan1->Instance == CAN1) {

        /*##-1- Peripheral clock disable ##################################################*/
        __HAL_RCC_CAN1_CLK_DISABLE();

        /*##-2- Disable peripherals and GPIO Clocks #################################*/
        __HAL_RCC_GPIOA_CLK_DISABLE();
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11 | GPIO_PIN_12);

        /*##-3- Enable interrupt request for CAN1 #########################################*/
        __HAL_CAN_DISABLE_IT(hcan1, CAN_IT_TX_MAILBOX_EMPTY);

        __HAL_CAN_DISABLE_IT(hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
        __HAL_CAN_DISABLE_IT(hcan1, CAN_IT_RX_FIFO0_FULL);

        /*##-4- Disable the NVIC for CAN ###########################################*/
        HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
        HAL_NVIC_DisableIRQ(CAN1_TX_IRQn);
    }
}

/*!
  \brief  Rx Transfer completed callback
  \param  CanHandle: CAN handle
  \retval None
*/
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *CanHandle)
{

}

/*!
  \brief  Tx Transfer completed callback
  \param  CanHandle: CAN handle
  \retval None
*/
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *CanHandle)
{
    if (CanHandle->Instance == CAN1) {
        /* Transmission flag setting: recieve complete*/
        CAN::getInstance()->canWriteReady = SET;
    }
}

/*!
  \brief  CAN error callbacks
  \param  CanHandle: CAN handle
  \retval None
*/
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *CanHandle)
{
    if (CanHandle->Instance == CAN1) {
        CAN::getInstance()->canErrorCallback = true;
    }
}

/*!
 \brief  This function handles CAN1 TX interrupts.
 \param  None
 \retval None
*/
void CAN1_TX_IRQHandler(void)
{
    HAL_CAN_IRQHandler(CAN::getInstance()->canHandle);
}

/*!
 \brief This function handles CAN1 RX interrupts.
 \param  None
 \retval None
*/
void CAN1_RX0_IRQHandler(void)
{
    HAL_CAN_IRQHandler(CAN::getInstance()->canHandle);
}

#ifdef __cplusplus
}
#endif
