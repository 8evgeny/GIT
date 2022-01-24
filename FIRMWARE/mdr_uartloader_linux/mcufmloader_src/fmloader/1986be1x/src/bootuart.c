#include "inc\opora.h"

#define NVSTR	0x2000
#define PROG	0x1000
#define MAS1    0x800	
#define ERASE	0x400
#define IFREN	0x200
#define SE		0x100
#define YE		0x80
#define XE   	0x40
#define CON	 	0x1
 
void bootuart(void);
void sleep(int id);
unsigned char UARTReadByte(void);
void UARTWriteByte(unsigned char chout);
void UARTWriteU32(unsigned int iout);
void Erase(void);
void Program(void);
void Verify(void);
unsigned int UARTReadU32(void);
void Getbufdata(void);

unsigned int bufdata[64];
const unsigned char id_str[12] = "1986BOOTUART";
unsigned int adr,ibuf;
unsigned char ks;


void bootuart(void)
{
int i1;
//	RST_CLK->PER_CLOCK |= 1<<3; 
	while(1)
	{									         
	 i1 = UARTReadByte();
	 switch(i1)
	 {
	 case	'I':
	 	for(i1=0;i1<12;i1++)
	 		UARTWriteByte(id_str[i1]);
	 	break;
	 case	'E':
			Erase();
	 	break;
	 case	'P':
			Program();
	 	break;
	 case	'V':
			Verify();
	 	break;
	 case	'A':
 			ks = 0;
			adr = UARTReadU32();
			ibuf = 0;
			UARTWriteByte(ks);
	 	break;
	   case	'R':
 			UARTWriteByte('R');
			sleep(2400);
			RST_CLK->PER_CLOCK |= 1 << 27;
			i1 = BKP->REG_0E ;
			BKP->REG_0E = i1 | BKP_REG_0E_FPOR;
	 	break;
	 }
  	}
}
__asm void sleep(int id)
{
sleepn
				 subs			r0,#1
				 bne			sleepn
			   bx	lr
}		
// void sleep(int id)
// {
// 	do
// 	{
// 	 id++;
// 	 id -= 2;
// 	} 
//  	while (id!=0);
// }

unsigned char UARTReadByte(void)
{
 	while (UART1->FR & UART_FR_RXFE);
	return UART1->DR ;
}
void UARTWriteByte(unsigned char chout)
{
 	while (UART1->FR & UART_FR_TXFF);
	UART1->DR = chout;
	return;
}

void UARTWriteU32(unsigned int iout)
{
int i1;
 	for(i1=0;i1<4;i1++)
	{
		UARTWriteByte(iout&0xff);
		iout >>= 8;
	}	
	return;
}

void ErasePage(unsigned int adrp)
{
unsigned int i1;	
		for (i1 = 0; i1 < 16; i1 += 4)
		{
			EEPROM->ADR = adrp + i1;
			EEPROM->CMD &= ~(XE|YE|SE|MAS1|ERASE|NVSTR);
			EEPROM->CMD |= XE|ERASE;
			sleep(6);						//Pause ~ 6 us
			EEPROM->CMD |= NVSTR;
			sleep(50000);					//Pause ~ 50 ms
			EEPROM->CMD &= ~ERASE;
			sleep(6);						//Pause ~ 6 us
			EEPROM->CMD &= ~(XE|NVSTR);
			sleep(2);						//Pause ~ 2 us
		}
}	

void EraseFull(void)	
{
unsigned int i1;	
	for (i1=0;i1<16;i1+=4)
	{
		EEPROM->ADR = 0x00000000+i1;
		EEPROM->CMD &= ~(XE|YE|SE|MAS1|ERASE|NVSTR);
		EEPROM->CMD |= XE|MAS1|ERASE;
		sleep(6);// 6us 
		EEPROM->CMD |= NVSTR;
		sleep(50000);// 50ms 
		EEPROM->CMD &= ~ERASE;
		sleep(110);// 110us 
		EEPROM->CMD &= ~(XE|MAS1|NVSTR);
		sleep(2);// 2us
	}
}	

void Erase(void)
{
unsigned int i1,i2,i3;
	ks =0;
	i1 = UARTReadU32();
	UARTWriteByte(ks);
	EEPROM->KEY = 0x8AAA5551;
	EEPROM->CMD |= CON;
	if (i1==0)
	{
		EraseFull();
		i3 = 0;
		adr = 0x00020000;
	}
	else
	{
		i3 = i1;
		for(i2=0;i2<32;i2++)
		{
			if(i3 & 0x1)
			{
				adr = i2*0x1000;
				ErasePage(adr);
			}
			i3 >>= 1;
		}
		adr += 0x1000;
		i3 = 0;
		while((i1 & 0x1)==0)
		{
			i3 += 0x1000;
			i1 >>= 1;
		}
	}
	for (i1=i3;i1<adr;i1+=4)
	{
 		EEPROM->ADR = i1;
		EEPROM->CMD |= XE|YE|SE;
		sleep(1);
		i2 = EEPROM->DO;
		EEPROM->CMD &= ~(XE|YE|SE);
		sleep(1);
		if(i2 != 0xffffffff)
			break;
	}
	EEPROM->CMD &= ~CON;
	UARTWriteByte('E');
	UARTWriteU32(i1);
	UARTWriteU32(i2);
	return;
}

void Program(void)
{
unsigned int i1,i2;
	ks =0;
	for(i1=0;i1<64;i1++)
	  bufdata[i1] = UARTReadU32();
	EEPROM->KEY = 0x8AAA5551;
	EEPROM->CMD |= CON;
	EEPROM->CMD &= ~(XE|YE|SE|MAS1|ERASE|NVSTR|PROG);
	for (i1=0;i1<16;i1+=4)
	{
		EEPROM->ADR = adr+i1;
		EEPROM->CMD |= XE|PROG;
		sleep(6);// 6us 
		EEPROM->CMD |= NVSTR;
		sleep(11);// 11us 
		for(i2=0;i2<256;i2+=16)
		{
			EEPROM->ADR = adr+i2+i1;
			EEPROM->DI = bufdata[((i1+i2)>>2)];
			EEPROM->CMD |= YE;
			sleep(50);// 50us
			EEPROM->CMD &= ~YE;
			sleep(1);// 1us 
		}
		EEPROM->CMD &= ~PROG;
	   	sleep(6);// 6us 
		EEPROM->CMD &= ~(XE|NVSTR);
	   	sleep(1);// 1us 
	}
	EEPROM->CMD &= ~CON;
	adr += 0x100;
	UARTWriteByte(ks);
}

void Verify(void)
{
int i1;
	i1 = ibuf; 
	if(i1 == 0)
		Getbufdata();
	UARTWriteU32(bufdata[i1]);
	i1++;
	UARTWriteU32(bufdata[i1]);
	i1++;
	ibuf = i1 & 0x3f;
}
unsigned int UARTReadU32(void)
{
unsigned int i1,i2,i3;
	i3 = 0;
	for(i1=0;i1<4;i1++)
	{
		i2 = UARTReadByte();
		ks += i2;
		i3 >>= 8;
		i2 <<= 24;
		i3 += i2;
	}	
	return i3; 
}

void Getbufdata(void)
{
int i1;
	EEPROM->KEY = 0x8AAA5551;
	EEPROM->CMD |= CON;
	EEPROM->CMD &= ~(XE|YE|SE);
	for(i1=0;i1<64;i1++)
	{
		EEPROM->ADR = adr;
		adr += 4;
		EEPROM->CMD |= XE|YE|SE;
		sleep(1);
		bufdata[i1] = EEPROM->DO;
		EEPROM->CMD &= ~(XE|YE|SE);
		sleep(1);
	}
	EEPROM->CMD &= ~CON;
}
