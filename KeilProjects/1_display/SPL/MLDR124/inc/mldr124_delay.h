
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MLDR124_DELAY_H
#define __MLDR124_DELAY_H

/* Includes ------------------------------------------------------------------*/
#include "mldr124_config.h"

void DELAY_Config( void );
__ramfunc void DELAY_us( uint32_t ulUs );
__ramfunc void DELAY_ms( uint32_t ulMs );

#endif /* __MLDR124_DELAY_H */


