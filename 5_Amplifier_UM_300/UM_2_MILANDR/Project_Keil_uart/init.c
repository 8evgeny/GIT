/***********************************************************************************************************/
/*!
\file 
\brief 
\author 
\date

|
*/
/***********************************************************************************************************/

//----- Инклуды ---------------------------------------------------------------------------------------------
#include <stdint.h>
#include "init.h" /**<  */
#include "MDR32Fx.h"
#include "MDR32F9Qx_config.h"
#include "MDR32F9Qx_can.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_it.h"

//----- Дефайны ---------------------------------------------------------------------------------------------

//----- Прототипы функций -----------------------------------------------------------------------------------
void PORT_init ( void );
void CAN_init  ( void );
void RST_init  ( void );

//----- Функции ---------------------------------------------------------------------------------------------

/*****************************************************************************/
/*! Функция init() 
 * \brief Инициализация микроконтроллера
 */
void init ( void )
{
	RST_init();
	PORT_init();
	CAN_init();
}

/*****************************************************************************/
/*! Функция PORT_init() 
 * \brief  Инициализация портов ввода-вывода
 */
void PORT_init ( void )
{
    PORT_InitTypeDef PORT_InitStructure;
    // Запуск тактирования портов контроллера
	RST_CLK_PCLKcmd( RST_CLK_PCLK_PORTB, ENABLE);

    // Ноги мультиплексора каналов измерения
	// PF0 - PF3
	PORT_InitStructure.PORT_Pin   = PORT_Pin_0 | PORT_Pin_1 | PORT_Pin_2 | PORT_Pin_3;
	PORT_InitStructure.PORT_OE    = PORT_OE_OUT;
	PORT_InitStructure.PORT_FUNC  = PORT_FUNC_PORT;
	PORT_InitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
	PORT_InitStructure.PORT_SPEED = PORT_SPEED_FAST;
	PORT_Init(MDR_PORTB, &PORT_InitStructure);
	
}
/*****************************************************************************/
/*! Функция RST_init() 
 * \brief  Инициализация тактирования микроконтроллера
 */
void RST_init( void )
{
//	RST_CLK_DeInit();
////	RST_CLK_CPU_PLLconfig (RST_CLK_CPU_PLLsrcHSIdiv2, 0);
////	RST_CLK_PCLKcmd(RST_CLK_PCLK_RST_CLK, ENABLE);
////    
//    RST_CLK_HSEconfig( RST_CLK_HSE_ON );
//    while ( RST_CLK_HSEstatus() != SUCCESS );
//    
//    //* Select HSE/2 as CPU_CLK source
//	RST_CLK_CPU_PLLconfig ( RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul4 );
//    RST_CLK_CPUclkPrescaler( RST_CLK_CPUclkDIV1 );
//    RST_CLK_CPU_PLLcmd(ENABLE);
//    while ( RST_CLK_CPU_PLLstatus() == 0 )//wait ready PLL
//    
//	RST_CLK_CPU_PLLuse( ENABLE );
//    RST_CLK_CPUclkSelection( RST_CLK_CPUclkCPU_C3 );
}

/*****************************************************************************/
/*! Функция PORT_init() 
 * \brief Инициализация настроек CAN
 */
void CAN_init ( void )
{

}




//----- КОНЕЦ ФАЙЛА -----------------------------------------------------------------------------------------
