/**
 ******************************************************************************
 * @file    main.c
 * @author  Milandr Application Team
 * @version V1.0.2
 * @date    28/01/2021
 * @brief   Main program body.
 ******************************************************************************
 * <br><br>
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, MILANDR SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2021 Milandr</center></h2>
 */

/* Includes ------------------------------------------------------------------*/
#include "MDR32F2C10_port.h"
#include "MDR32F2C10_clk.h"
#include "demoboard.h"
#include "eth.h"


#include <string.h>
#include <stdlib.h>

/** @addtogroup  __MDR1986VK01_StdPeriph_Examples MDR1986VK01 StdPeriph Examples
 *  @{
 */

/** @addtogroup __MDR1986VK01_EVALBOARD MDR1986VK01 Demo Board
 *  @{
 */

/** @addtogroup eth_icmp eth_icmp
 *  @{
 */

#define CPU_CLK             50000000

#define ANABG_IREFEN                (1 << 0)
#define ANABG_BFEN(x)               ((x & 0x3F) << 1)
#define ANABG_BGEN                  (1 << 7)
#define ANABG_BFEXT(x)              ((x & 0xF) << 8)
#define ANABG_SWMODE(x)             ((x & 0x3F) << 12)
#define ANABG_EXTMODE               (1 << 18)
#define ANABG_IRECMODE              (1 << 19)
#define ANABG_VRECMODE              (1 << 20)
#define ANABG_ZMODE                 (1 << 21)


uint32_t ulOldIp = 1;

// PortA
#define YelLed 18
#define GrLed 17

#define PCLK_EN_PORTA 13

#define ETH_BUF 0x21008000
#define DILIMETER 0x1000
#define RxBuffer              ((unsigned int *)ETH_BUF)
#define TxBuffer              ((unsigned int *)(ETH_BUF+DILIMETER))

typedef struct
{
        unsigned short Data[288];
        unsigned short Counter;
        unsigned short Status;
} _Rec_Frame;

typedef struct
{
        unsigned char Data[1600];
        unsigned short Length;
        unsigned short Status;
        unsigned short Address;
        unsigned char Remote_IP[4];
        unsigned char Remote_MAC[6];

#if PRINTSTATUSON
        unsigned int CounterTxPacket;
        unsigned int CounterRxPacket;
#endif //PRINTSTATUSON

} _ethernet_packet;

_Rec_Frame Frame;

const uint16_t MyMAC[3]={0x3412,0x7856,0xBC9A};
const uint16_t MyIPAddress[2]={0xA8C0,0x5701};

int Read_Packet(_ethernet_packet*);
uint16_t CheckSum_IP(void);
uint16_t CheckSum_ICMP(uint16_t size);
void Answear_ARP(void);
void Answear_ICMP(void);
void ClkCfg(void);
//void ETH_DeInit(MDR_ETH_TypeDef * ETHERNETx );
void EthCfg(MDR_ETH_TypeDef * ETHERNETx);
void LedsSetup(void);

uint32_t ReadPacket(_Rec_Frame* Frame);
int SendPacket(void* buffer, int size);
void PacketAnaliser(void);
void MACReset(void);
void ClearMemory(void);

#if PRINTSTATUSON
void Request_Status(_ethernet_packet*);
#endif //PRINTSTATUSON

_ethernet_packet Packet;
unsigned short Temp, TypeOfProtocol;
unsigned char My_IP[4], My_MAC[6];
unsigned char Receive_IP[4];
unsigned char ICMP_Packet[1500];
unsigned char RF_OK=0;
unsigned char XF_OK=0;


void ClockInit()
{
    MDR_CLK->KEY = 0x8555AAA1;
    MDR_CLK->PER0_CLK =  0xFFFFFFFF;
    MDR_CLK->PER1_CLK =  0xFFFFFFFF;
    
    MDR_COMP0->ANABG_CTRL= ANABG_IREFEN | ANABG_BGEN | 
                     ANABG_BFEN( 0x3F ) | ANABG_SWMODE( 0x3F );

    MDR_BKP->KEY = 0x8555AAA1;

    CLK_XSEConfig( CLK_HSE0, CLK_XSE_ON );
    if( !CLK_XSEWaitReady( CLK_HSE0 ) )
        while(1){}

    CLK_SetPllClk( &MDR_CLK->PLL[0], PLL_CLK_HSE0, CPU_CLK );
    CLK_PllState( &MDR_CLK->PLL[0], ENABLE );
    if( !CLK_PLLWaitReady( &MDR_CLK->PLL[0] ) )
        while(1){}

    CLK_SetPllClk( &MDR_CLK->PLL[2], PLL_CLK_HSE0, 50000000 );
    CLK_PllState( &MDR_CLK->PLL[2], ENABLE );
    if( !CLK_PLLWaitReady( &MDR_CLK->PLL[2] ) )
        while(1){}

    //CLK_SetSystemClock( MAX_CLK_HSE0 );
    CLK_SetSystemClock( MAX_CLK_PLL0 );
}


static void vShifterInit( void )
{
    PORT_InitTypeDef ShifterPort;
    PORT_StructInit( &ShifterPort );

    MDR_PORTA->KEY = 0x8555AAA1;

    ShifterPort.PORT_Pin     = PORT_Pin_31;
    ShifterPort.PORT_SOE     = PORT_SOE_OUT;
    ShifterPort.PORT_SFUNC   = PORT_SFUNC_PORT;
    ShifterPort.PORT_SANALOG = PORT_SANALOG_DIGITAL;
    ShifterPort.PORT_SPWR    = PORT_SPWR_10;

    PORT_Init( MDR_PORTA, &ShifterPort );

    BUF_OE_OFF;
}


int main()
{
  ClockInit();
  
  vShifterInit();
  
  InitEth( 50000000 );
  EthCfg(MDR_ETH0);
  
  while(1)
  {
    EthLoop();
    while(MDR_ETH0->R_HEAD != MDR_ETH0->R_TAIL)
    {
      //MDR_PORTD->SETTX=1<<7;
      Frame.Counter=ReadPacket(&Frame);
      PacketAnaliser();
      //MDR_PORTD->CLRTX=1<<7;
    }
  
  }
}


uint16_t CheckSum_IP(void)
{
  unsigned long a, cs=0;
  for(a=0; a<10; a++)
  {
    if(a == 5) continue;
    else cs = cs + Frame.Data[7+a];
  }
  cs = (cs >> 16) + (cs & 0xFFFF);
  return (uint16_t)(~cs);
}


uint16_t CheckSum_ICMP(uint16_t size)
{
  unsigned long a, cs=0;
  for(a=0;a<size;a++)
  {
    if(a==1) continue;
    else cs+=Frame.Data[a+17];
  }
  cs=(cs>>16)+(cs&0xFFFF);
  return (uint16_t)(~cs);
}

int Read_Packet(_ethernet_packet* Dt)
{
  unsigned short Temp;
  unsigned int Val;
  unsigned int* MyPointer;
  MyPointer = (unsigned int*)((unsigned int)RxBuffer + (unsigned int)(Dt->Address) + 4);
  for(Temp=0;Temp<Dt->Length;Temp+=4)
  {
    Val = *MyPointer++;
    Dt->Data[Temp+3] = (unsigned char)Val;
    Dt->Data[Temp+2] = (unsigned char)(Val>>8);
    Dt->Data[Temp+1] = (unsigned char)(Val>>16);
    Dt->Data[Temp] = (unsigned char)(Val>>24);
    if( ((int)MyPointer & 0x00009000) == 0x00009000)
      MyPointer = RxBuffer;
  }
  if((Dt->Length & 0x0001) == 1)
  {
    Dt->Data[Dt->Length-4] = 0;
  }
  
  return 0;
}


void Answear_ARP(void)
{
 uint16_t Buf[22];
  Buf[0]=Frame.Data[3];         	
  Buf[1]=Frame.Data[4];         	
  Buf[2]=Frame.Data[5];       
  Buf[3]=MDR_ETH0->MAC_T;     
  Buf[4]=MDR_ETH0->MAC_M;     	
  Buf[5]=MDR_ETH0->MAC_H;     	
  Buf[6]=Frame.Data[6];         	//type - ARP
  Buf[7]=Frame.Data[7];         	//Hardware type - Ethernet
  Buf[8]=Frame.Data[8];         	//Protocol type - IP
  Buf[9]=Frame.Data[9];         	//Hardware size - 6; Protocol size - 4
  Buf[10]=0x0200;             
  Buf[11]=MDR_ETH0->MAC_T;    	//Sender MAC-address: 0A.1B.2C.3D.4E.5F
  Buf[12]=MDR_ETH0->MAC_M;    	//Sender MAC-address: 0A.1B.2C.3D.4E.5F
  Buf[13]=MDR_ETH0->MAC_H;    	//Sender MAC-address: 0A.1B.2C.3D.4E.5F
  Buf[14]=MyIPAddress[0]; 		//My_IP_Address[0];    //Sender IP-address: 192.168.1.87
  Buf[15]=MyIPAddress[1]; 		//My_IP_Address[1];    //Sender IP-address: 192.168.1.87
  Buf[16]=Frame.Data[3];        	//Target MAC-address
  Buf[17]=Frame.Data[4];        	//Target MAC-address
  Buf[18]=Frame.Data[5];        	//Target MAC-address
  Buf[19]=Frame.Data[14];       	//Target IP-address
  Buf[20]=Frame.Data[15];       	//Target IP-address
  Buf[21]=0;
  
  SendPacket(Buf,42);
}

void Answear_ICMP(void)
{
    unsigned long a;
	uint16_t buffer[288];
	uint16_t tmp;
	
  	tmp=Frame.Counter-34-4;	
	if((tmp&0x01)==1)	
	{
		tmp=(tmp+1)>>1;	
		Frame.Data[tmp+16]=Frame.Data[tmp+16]&0x00FF;
	}
	else tmp=tmp>>1;	
		
	//-------Ethernet 2 Protocol---------
    buffer[0]=Frame.Data[3];
    buffer[1]=Frame.Data[4];
    buffer[2]=Frame.Data[5];

	buffer[3]=MDR_ETH0->MAC_T;
	buffer[4]=MDR_ETH0->MAC_M;
	buffer[5]=MDR_ETH0->MAC_H;

	buffer[6]=Frame.Data[6];
    //-------IP Protocol---------
    for(a=7;a<12;a++)
	{
	    buffer[a]= Frame.Data[a];
    }
    //---------------------------
    buffer[12]=CheckSum_IP();
	buffer[13]=Frame.Data[15];//IP->DestinAddr[0];
	buffer[14]=Frame.Data[16];//IP->DestinAddr[1];
	buffer[15]=Frame.Data[13];//IP->SourceAddr[0];
	buffer[16]=Frame.Data[14];//IP->SourceAddr[1];
	//-------ICMP Protocol---------
	buffer[17]=0x0000; //ответ
    Frame.Data[17]=0x0000;
	//-----------------------------
	buffer[18]=CheckSum_ICMP(tmp);
    for(a=19;a<((tmp-2)+19);a++)
	{
    	buffer[a]=Frame.Data[a];
	}
	SendPacket(buffer,(tmp*2+34));		
}


uint32_t ReadPacket(_Rec_Frame* Frame)
{
	uint16_t space_start=0,space_end=0,tail,head;
	uint32_t *src, *dst;
	uint32_t size, i;
	uint16_t tmp[2];

	tail=MDR_ETH0->R_TAIL;
	head=MDR_ETH0->R_HEAD;

	if(tail>head)
	{
		space_end=tail-head;
		space_start=0;
	}
	else
	{
		space_end=0x1000-head;
		space_start=tail;
	}

	src=(uint32_t*)(0x21008000+head);
	dst=(uint32_t*)(Frame->Data);

	*((uint32_t*)tmp)=*src++;	
	space_end-=4;
	if((uint16_t)src>0x8FFF)	src=(uint32_t*)0x21008000;

	size=(tmp[0]+3)/4;
	if(tmp[0]<=space_end)
	{
		for(i=0;i<size;i++)
			*dst++ = *src++;
	}
	else
	{
		size=size-space_end/4;
		for(i=0; i<(space_end/4); i++)
			*dst++ = *src++;
		src=(uint32_t*)0x21008000;
		for(i=0; i<size; i++)
			*dst++ = *src++;
	}
	if((uint16_t)src>0x8FFF)	src=(uint32_t*)0x21008000;

	MDR_ETH0->R_HEAD=(uint16_t)src;
	MDR_ETH0->STAT-=0x20;
	return tmp[0];
}


int	SendPacket(void* buffer, int size)
{
	uint16_t i;
	uint32_t tmp, head, tail;
	uint32_t *src, *dst;
	uint16_t space[2];

	head = MDR_ETH0->X_HEAD;
	tail = MDR_ETH0->X_TAIL;

	if(head>tail)
	{
		space[0]=head-tail;
		space[1]=0;
	} else
	{
		space[0]=0x2000-tail;
		space[1]=head-0x1000;
	}

	if(size>(space[0]+space[1]-8))	return 0;

	tmp=size;
	src=buffer;
	dst=(uint32_t*)(0x21008000+tail);
	
	*dst++ =tmp;
	space[0]-=4;
	if((uint16_t)dst>0x9FFC)	dst=(uint32_t*)0x21009000;

	tmp=(size+3)/4;

	if(size<=space[0])
	{
		for(i=0; i<tmp; i++)
			*dst++ = *src++;
	}
	else
	{
		tmp-=space[0]/4;
		for(i=0;i<(space[0]/4);i++)
			*dst++ = *src++;
		dst=(uint32_t*)0x21009000;
		for(i=0;i<tmp;i++)
			*dst++ = *src++;
	}
	if((uint16_t)dst>0x9FFC)	dst=(uint32_t*)0x21009000;
	tmp=0;
	*dst++ =tmp;
	if((uint16_t)dst>0x9FFC)	dst=(uint32_t*)0x21009000;

	MDR_ETH0->X_TAIL=(uint16_t)dst;
	return	size;
}


void PacketAnaliser(void)
{
	switch(Frame.Data[6])  
    {
    	case 0x0008:       
        	if((Frame.Data[15]==MyIPAddress[0])&&(Frame.Data[16]==MyIPAddress[1]))  
            {                                                                                                              
            	if(CheckSum_IP()==Frame.Data[12])  
                {                                                
                //--------------------ICMP-------------------
                	if((Frame.Data[11]&0xFF00)==0x0100)     
                    {
                    	if(Frame.Data[17]==0x0008)       
						{
					//		MDR_PORTD->SETTX=1<<8;
                        	Answear_ICMP();    
					//		MDR_PORTD->CLRTX=1<<8;
						}
                    }
                }
                //-------------------------------------------
            }
            break;
		case 0x0608:    
        	if((Frame.Data[19]==MyIPAddress[0])&&(Frame.Data[20]==MyIPAddress[1]))  
            {                                                                          
            	Answear_ARP();  
            }
            break;
	}
}


void MACReset()
{
	uint32_t treg;
	treg = 0x00030000;
	
	MDR_ETH0->G_CFG|=(treg & 0xFFFF0000);

	ClearMemory();

	MDR_ETH0->DILIMETR=0x1000;	

	MDR_ETH0->HASH0=0;
	MDR_ETH0->HASH1=0;
	MDR_ETH0->HASH2=0;
	MDR_ETH0->HASH3=0x8000;

	MDR_ETH0->IPG=0x0060;
	MDR_ETH0->PSC=0x0050;
	MDR_ETH0->BAG=0x0200;
	MDR_ETH0->JITTERWND=0x0005;
	MDR_ETH0->R_CFG=0x8406;
	MDR_ETH0->X_CFG=0x81FA;

	treg = 0x30030080;
	
	MDR_ETH0->G_CFG = (treg & 0xFFFFFFFF);	
	//MDR_ETH0->G_CFGl = (treg & 0xFFFF);

	MDR_ETH0->IMR=0;
	MDR_ETH0->IFR=0xFFFF;

	MDR_ETH0->R_HEAD=0x0000;
	MDR_ETH0->X_TAIL=0x1000;

	treg = 0xFFFCBFFF;
	
	MDR_ETH0->G_CFG&=(treg & 0xFFFFFFFF);	
	//MDR_ETH0->G_CFGl&=(treg & 0xFFFF);
}

void ClearMemory()
{
	uint32_t Temp;
	uint32_t *ptr;
	ptr=(uint32_t*)0x21008000;
	for(Temp=0;Temp<2048;Temp++)	*ptr++=0;
}

void EthCfg(MDR_ETH_TypeDef * ETHERNETx)
{	
	ETHERNETx->MAC_T=MyMAC[0];
	ETHERNETx->MAC_M=MyMAC[1];
	ETHERNETx->MAC_H=MyMAC[2];
	
	// Start
	ETHERNETx->R_CFG |= (1<<15);
	ETHERNETx->X_CFG |= (1<<15);		
}

/** @} */ /* End of group eth_icmp */

/** @} */ /* End of group __MDR1986VK01_EVALBOARD */

/** @} */ /* End of group __MDR1986VK01_StdPeriph_Examples */

/******************* (C) COPYRIGHT 2021 Milandr *********/

/* END OF FILE main.c */
