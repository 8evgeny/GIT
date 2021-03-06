//?????? ????????? ??? ?????? ???????? ?? ????????? MT-12864

void main(void) {
byte	p;//????? ??????? ???????? ??????????
byte	c;//??????? ?? ??????????? ?????????? ?????

	LCDinit();
	for(p=0; p<8; p++) {//???? ?? ???? 8-?? ????????? ??????????
		WriteCodeL(p|0xB8);//????????? ??????? ???????? ??? ?????? ????????? ??????????
		WriteCodeL(0x40);//????????? ???????? ?????? ??? ?????? ?????? ? 0 ??? ?????? ????????? ??????????
		for(c=0; c<64; c++) {//???? ?????? ?????? ? ????? ???????? ??????????
			WriteDataL(Logo128[p][c]);//????? ?????????? ????? ? ?????????
		}
		WriteCodeR(p|0xB8);//????????? ??????? ???????? ??? ??????? ????????? ??????????
		WriteCodeR(0x40);//????????? ???????? ?????? ??? ?????? ?????? ? 0 ??? ??????? ????????? ??????????
		for(c=64; c<128; c++) {//???? ?????? ?????? ? ?????? ???????? ??????????
			WriteDataR(Logo128[p][c]);//????? ?????????? ????? ? ?????????
		}
	}
}

//????????? ??????????? ????????????? ??????????
void LCDinit(void) {
	LCD.E=0;//????????? ???????? ??????? ??????????
	LCD.RES=0;//?????? ?????? RES=0 ??????????
	Delay(>1us);//???????? ?? ????? ?????? 1 ???
	LCD.RES=1;//????? ?????? RES
	Delay(>10us);//???????? ?? ????? ?????? 10 ???
	WriteCodeL(0xC0);//??????? ?????? ?? 0
	WriteCodeR(0xC0);//??????? ?????? ?? 0
	WriteCodeL(0x3F);//Display on
	WriteCodeR(0x3F);//Display on
}

void Pset(byte x, byte y, bit c) {//???????? ???? ????? ? ????????? (?????????? 0,0 ? ????? ??????? ???? ??????????)
byte c8;//????????? ???????? ????????? ?? ?????????? ?????
byte m8;//????? ??????? ???? ? ?????
	if ((x>127)||(y>63)) return;//????? ?? ??????? ??????????
	if (x<64) {//???????? ? ????? ???????? ??????????
		WriteCodeL(0xB8|(y>>3));//?????????? ?????? ???????? ??????????
		WriteCodeL(0x40|x);//?????????? ????? ??????? ?????
		c8=ReadDataL();//????????? ??????
		c8=ReadDataL();//?????? ??????? ????? ?? ??????????
		m8=1<<(y&0x07);//????????? ????? ??????? ???? ? ?????
		if (c==1)	//???????? ??????
			c8|=m8//?????????? ?????? ??? ? ?????
		else		//??? ?????? ??????
			c8&=~m8;//???????? ?????? ??? ? ?????
		WriteCodeL(0x40|x);//????? ?????????? ????? ??????? ?????
		WriteDataL(c8);//???????? ?????????? ???? ??????? ? ?????????
	} else {//???????? ? ?????? ???????? ??????????
		WriteCodeR(0xB8|(y>>3));//?????????? ?????? ???????? ??????????
		WriteCodeR(0x40|(x-64));//?????????? ????? ??????? ?????
		c8=ReadDataR();//????????? ??????
		c8=ReadDataR();//?????? ??????? ????? ?? ??????????
		m8=1<<(y&0x07);//????????? ????? ??????? ???? ? ?????
		if (c==1)	//???????? ??????
			c8|=m8//?????????? ?????? ??? ? ?????
		else		//??? ?????? ??????
			c8&=~m8;//???????? ?????? ??? ? ?????
		WriteCodeR(0x40|(x-64));//????? ?????????? ????? ??????? ?????
		WriteDataR(c8);//???????? ?????????? ???? ??????? ? ?????????
	}
}

void WriteCodeL(byte b) { WriteByte(b,0,1,0); }//??????? ? ????? ???????? ??????????
void WriteCodeR(byte b) { WriteByte(b,0,0,1); }//??????? ? ?????? ???????? ??????????

void WriteDataL(byte b) { WriteByte(b,1,1,0); }//?????? ? ????? ???????? ??????????
void WriteDataR(byte b) { WriteByte(b,1,0,1); }//?????? ? ?????? ???????? ??????????

byte ReadDataL(void) { return ReadByte(1,1,0); }//????????? ???? ?????? ?? ????? ???????? ??????????
byte ReadDataR(void) { return ReadByte(1,0,1); }//????????? ???? ?????? ?? ?????? ???????? ??????????

//????????? ?????? ????? ? ?????????
void WriteByte(byte b, bit cd, bit l, bit r) {
	if ((l==1)&&(r==1)) return;	//?????? (??? ?????? ??????????) ???????????? ?? ????? ?????????? ???????????!
	WaitReady(l,r);		//????? ?????????? ??????????
//??? ????????????? ????????? ????? ???? ?????? ?? ?????
	LCD.RW=0; LCD.A0=cd;	//?????? ????? ? ????????? ??? ?????? ??? ???????
	LCD.E1=l; LCD.E2=r;	//??????? ?????? ?????? ????????? ? ??????????
	LCD.D=b;		//?????? ???? ?? ???? ?????? ??????????
	Delay(>140ns);		//??? ????? ????????????? ?????? (tAW)
	LCD.E=1;		//???????????? ????? ?????? ? ????????? E=1
	Delay(>450ns);		//???????????? ??????? E=1 (????? ????????????? ?????? ?????? ???? (tDS))
	LCD.E=0;		//???????? ?????? E
	Delay(>(1000ns-140ns-450ns));	//?????????? ?????????? ???????? ????? ????????? E=1
}

byte ReadByte(bit cd, bit l, bit r) {
byte b;
	if ((l==1)&&(r==1)) return 0;	//?????? ???????????? ?? ????? ?????????? ???????????!
	WaitReady(l,r);		//????? ?????????? ??????????
	LCD.RW=1; LCD.A0=cd;	//????? ?????? ???? ??? ??????? ??? ??????
	LCD.E1=l; LCD.E2=r;	//??????? ?????? ?????? ????????? ? ??????????
	Delay(>140ns);		//??? ????? ????????????? ?????? (tAW)
	LCD.E=1; 		//?????? ????? ? ?????????
	Delay(>450ns);		//?????????? ?????????? ???????????? ??????? E=1 (????? ??????? (tDDR) ?????? ????)
	b=LCD.D;		//????????? ?????? ? ???? ?????????? (??? ?? ??? ??? ??????? 130??)
	LCD.E=0;		//???????? ?????? E
	Delay(>(1000ns-140ns-450ns));	//?????????? ?????????? ???????? ????? ????????? E=1
	return b;		//??????? ??????????? ????
}

void WaitReady(bit l, bit r) {//????? ?????????? ??????????, ????????? ???? ???????
//??? ????????????? ????????? ????? ???? ?????? ?? ????
	LCD.RW=1; LCD.A0=0;	//?????? ????? ?????????
	LCD.E1=l; LCD.E2=r;	//??????? ?????? ?????? ????????? ? ??????????
	Delay(>140ns);		//??? ????? ????????????? ?????? (tAW)
	LCD.E=1; Delay(>450ns);	//??????????? ???????????? ??????? E=1 (?????????? ?? ???? ?????? ??????????? ????? ?????? ??????, ?? ????? ??? ????? 320??)
	while(LCD.D.7==1);	//????? ?????? ????? ?????????
	LCD.E=0;		//???????? ?????? E
	Delay(>(1000ns-140ns-450ns));	//?????????? ?????????? ???????? ????? ????????? E=1
}

//?????? ???????????, ??????? ?? ??????? ? ????? ????? ?? ?????????? ???????????.
//??? ?????? ?????????????????? ???? ??? ?????? ? ????????? ??????? ? ??????? ????????.
//????????? ????????????? ???????? ????????????? ??? ? ???????????? ?? ??????.
const byte Logo128[8][128]=//128x64 pixel, ?????? 8 ???????????? ????? ??????? ? ????
{
	{	0xFF,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
		0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
		0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
		0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
		0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
		0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
		0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
		0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0xFF
	},
	{	0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,0xC0,0xE0,0xF0,0xF0,0xF0,
		0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xE0,0xE0,0xE0,0xC0,0x80,0x00,
		0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,
		0x00,0x80,0x80,0x00,0x00,0x80,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x80,0x00,0x00,
		0x80,0x00,0x00,0x80,0x00,0x80,0x00,0x80,0x80,0x80,0x00,0x80,0x80,0x00,0x00,0x80,
		0x00,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,
		0x00,0x00,0x80,0x00,0x80,0x00,0x00,0x80,0x80,0x80,0x00,0x80,0x00,0x00,0x80,0x00,
		0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x3F
	},
	{	0x1F,0xC0,0xE0,0xE0,0xFC,0xFE,0xF3,0xE3,0xE3,0xE3,0xE3,0xAF,0x6F,0xFF,0xFF,0xFF,
		0xBF,0xBF,0xFF,0xFF,0x7F,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
		0xFF,0xF6,0xF4,0x1F,0x21,0xE2,0x61,0x6F,0x60,0x6F,0x64,0x6F,0x60,0x6F,0x65,0x68,
		0x60,0x6F,0x62,0x61,0x67,0x68,0x67,0x60,0x6A,0x67,0x60,0x6E,0x61,0x6F,0x60,0x67,
		0x6A,0x6B,0x60,0x6F,0x65,0x68,0x60,0x60,0x6F,0x60,0x60,0x6F,0x62,0x61,0x67,0x68,
		0x67,0x60,0x6F,0x62,0x6F,0x60,0x6F,0x64,0x6F,0x60,0x6F,0x65,0x68,0x64,0x6A,0x6F,
		0x60,0x60,0x6F,0x64,0x6F,0x60,0x60,0x60,0x6F,0x60,0x67,0x6A,0x6B,0x60,0x6D,0x62,
		0x6D,0x60,0x6F,0x62,0x6F,0x60,0x6F,0x64,0x6F,0x60,0x6F,0x65,0x68,0x64,0x6A,0x6F
	},
	{	0xFE,0x3F,0x3F,0x3F,0xBF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xF9,0x77,0x37,
		0x03,0x03,0x01,0x00,0x00,0x00,0x00,0x01,0x07,0x0F,0x13,0x01,0x00,0x00,0x01,0x07,
		0xD7,0x0E,0x08,0xC0,0x00,0x10,0x0F,0xF8,0x00,0x00,0x80,0xE0,0xF8,0xFE,0xFF,0xFF,
		0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0xC0,0xF0,0xFE,0xFF,0xFF,0xFF,0xFE,0x00,0x00,
		0x00,0xE0,0xF0,0xF8,0xFC,0x7C,0x3E,0x1E,0x1F,0x0F,0x0F,0x0F,0x0F,0x1F,0x1E,0x1E,
		0x3E,0x7C,0xFC,0xF8,0xF0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x80,0xC0,0xF0,0xFC,0xFE,0xFF,0xFF,0xFE,0x00,0x00,0x06,0x0F,0x0F,0x0F,0x0F,0x0F,
		0x0F,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x06,0xF0
	},
	{	0x3F,0xE0,0x00,0x00,0x01,0x3F,0xFF,0xFF,0xFF,0xFF,0xFF,0x3F,0x3F,0x00,0x00,0xE0,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x01,0x01,0xC1,0xE0,0xE0,0xF9,0xFF,0xFC,0xFF,0xFF,0x7F,0x1F,0xFF,0xFF,0xFF,
		0xFF,0xFF,0x80,0xE0,0xF8,0xFE,0xFF,0xFF,0x3F,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,
		0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x30,0x78,0x78,0xFC,0xFC,0xFC,0xFC,0x78,0x78,
		0x30,0x00,0x01,0x07,0xFF,0xFF,0xFF,0xFC,0x00,0x00,0x00,0x00,0x80,0xE0,0xF8,0xFE,
		0xFF,0xFF,0x3F,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF
	},
	{	0xFC,0x01,0x03,0x04,0x18,0x20,0x40,0x81,0x87,0x07,0x07,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,
		0x80,0xE4,0xE7,0xFB,0xFF,0xFF,0x7F,0x3F,0x0F,0x03,0x01,0x00,0x00,0xFF,0xFF,0xFF,
		0xFF,0xFF,0xFF,0x7F,0x1F,0x07,0x03,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,
		0x00,0x70,0xF8,0xF8,0xF0,0xE0,0xE0,0xC0,0xC0,0x80,0x80,0x80,0x80,0xC0,0xC0,0xC0,
		0xE0,0xF0,0xFC,0xFF,0x7F,0x3F,0x0F,0x81,0xE0,0xF8,0xFC,0xFF,0xFF,0x3F,0x1F,0x07,
		0x01,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF
	},
	{	0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x02,0x04,0x04,0x04,0x0C,
		0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x04,0x04,0x04,0x02,0x03,0x01,0x00,
		0x00,0x03,0x07,0x07,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x07,0x07,
		0x07,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x07,0x07,0x07,0x03,0x00,0x00,
		0x00,0x00,0x00,0x01,0x01,0x03,0x03,0x03,0x07,0x07,0x07,0x07,0x07,0x07,0x03,0x03,
		0x03,0x01,0x01,0x00,0x00,0x00,0x00,0x03,0x07,0x07,0x07,0x03,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x03,0x07,0x07,0x07,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x03,0x07,0x07,0x07,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF
	},
	{	0xFF,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
		0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
		0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
		0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
		0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
		0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
		0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
		0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0xFF
	}
};
