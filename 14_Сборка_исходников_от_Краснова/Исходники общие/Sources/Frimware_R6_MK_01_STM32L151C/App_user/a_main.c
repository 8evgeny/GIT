/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "global_vars.h"
#include "a_main.h"
#include "eeprom_i2c.h"
#include "uart.h"
#include "main_logic.h"

//===========================================================
uint16_t AdcData[ADCBUFMAX*ADC_CH];
int16_t TSin[ADCBUFMAX+ADCBUFMAX/4];
uint16_t Dac[DACBUFMAX];

#define FREQ_NUM_80Hz       0 // ����� �������� ������� ��� ������� 80 ��
#define FREQ_NUM_1kHz       1 // ����� �������� ������� ��� ������� 1 ���
#define FREQ_NUM_4kHz       2
#define FREQ_NUM_160Hz      3
#define FREQ_NUM_400Hz      4
#define FREQ_NUM_40Hz       5


const struct freq_cnst_s {
	uint32_t	AdcFSamples; // ������� ������������� ��� (350kHz MAX)
	uint32_t	AdcRat; // (AdcFSamples/f���) �������� ������� ������������� � ����� ������� ������� ���������
	uint16_t	AdcLen; // ����� ������ ��� (������ ������ �������� �� 4) (ADCBUFMAX ���� �� ������)
	uint8_t		AdcPer; // (AdcLen/AdcRat) ���������� �������� ������� ��������� � ������ ��� 
	uint8_t		Cycles; // (500��*f���/AdcPer) ������� ��� � ������ ����� ���������
	uint32_t	DacFSamples; // ������� ������������� ��� (SYSTEMCORECLOCK ������ ������ �������� �� DacFSamples) (1MHz MAX)
	uint16_t	DacLen; // (DacFSamples/f���) ����� ������ ��� (DACBUFMAX ���� �� ������)
	float		Value; // ������� � ��
} Freq[NUMBRSFREQ] = {
//AdcFSamples AdcRat AdcLen AdcPer Cycles DacFSamples DacLen Value
	{ 80000,  1000,   1000,    1,    40,    100000,    1250,   80}, // 0
	{200000,   200,   1000,    5,   100,    200000,     200, 1000}, // 1
	{200000,    50,   1000,   20,   100,    400000,     100, 4000}, // 2
	{ 80000,   500,   1000,    2,    40,    200000,    1250,  160}, // 3
	{200000,   500,   1000,    2,   100,    400000,    1000,  400}, // 4
	{ 40000,  1000,   1000,    1,    20,    100000,    2500,   40}  // 5
//	{ 40000,   500,   1000,    2,    20,    200000,    2500,   80}, // 0
//	{ 20000,   250,   1000,    4,    10,    200000,    2500,   80}, // 0
//	{100000,   250,   1000,    4,    50,    400000,    1000,  400}, // 0
//	{160000,   400,    800,    2,   100,    400000,    1000,  400}, // 0
};


struct Compute_s {
	cplx_t		Z;
	cplx_t		Ai;
	cplx_t		Au;
	cplx_t		K;
	cplx_t		R; // ������������� �����
	cplx_t		C[2]; // ��� ���������� ����������
	cplx_t		CalZo[NUMBRSFREQ];
	cplx_t		CalZs[NUMBRSFREQ];
} Cm = {0};

volatile struct Ip_s { // ���������� �2� ������
	float		Rs; // ���������������� �������������
	float		Cs; // �������
	float		Ls; // �������������
	float		Rp; // ������������ �������������
	float		Cp; // �������
	float		Lp; // �������������
	float		Z; // ������ �������������
	float		tg; // �������
	float		U; // ����������
	float		I; // ��� ����� ��������
	float		f; // �������
	uint8_t 	Flag; // 0xFF - ���������� � ������ ���������, 0xFE - ����� ���������, 0xFD - ��������� ����������, 0...0xFC - ������� ���������.
} Ip = {0};

struct Ee_s { // ���������� �2� ������
	uint8_t 	Mark;
	float		Cal_Ri; // ���������� ������������ �����
	float		Cal_Rr; // �������� ������������� �����
	float		Cal_Ku; // ������������� ����������� �� U
	float		Cal_Ki; // ������������� ����������� �� I
} Ee = {0};

uint8_t Isr_nF = 0; // ����� ������� ��� ���������� ���

float Tmp_f;
uint32_t Xx = 0;

//===========================================================
void DataDefault(void); // ��������� ���������
void EE_Save(void);
void cplxDiv(cplx_t *res, cplx_t *div);
void cplxMul(cplx_t *res, cplx_t *mul);
void cplxSub(cplx_t *subs1, cplx_t *subs2, cplx_t *res);
void cplxSum(cplx_t *summ1, cplx_t *summ2, cplx_t *res);


/////////////////////////////////////////////////////////////////////////

// ���������� ������� � ����������
void setInputReleToAmp(uint8_t line) {
  switch(line) {
    case 0:
      LL_GPIO_ResetOutputPin(RM1_GPIO_Port, RM1_Pin);
      break;
    case 1:
      LL_GPIO_ResetOutputPin(RM2_GPIO_Port, RM2_Pin);
      break;
    case 2:
      LL_GPIO_ResetOutputPin(RM3_GPIO_Port, RM3_Pin);
      break;
    case 3:
      LL_GPIO_ResetOutputPin(RM4_GPIO_Port, RM4_Pin);
      break;
    case 4:
      LL_GPIO_ResetOutputPin(RM5_GPIO_Port, RM5_Pin);
      break;
    case 5:
      LL_GPIO_ResetOutputPin(RM6_GPIO_Port, RM6_Pin);
      break;
  }
}
// ���������� ������� � ������������� �����
void setInputReleToMeasCircuit(uint8_t line) {
  switch (line) {
    case 0:
      LL_GPIO_SetOutputPin(RM1_GPIO_Port, RM1_Pin);
      break;
    case 1:
      LL_GPIO_SetOutputPin(RM2_GPIO_Port, RM2_Pin);
      break;
    case 2:
      LL_GPIO_SetOutputPin(RM3_GPIO_Port, RM3_Pin);
      break;
    case 3:
      LL_GPIO_SetOutputPin(RM4_GPIO_Port, RM4_Pin);
      break;
    case 4:
      LL_GPIO_SetOutputPin(RM5_GPIO_Port, RM5_Pin);
      break;
    case 5:
      LL_GPIO_SetOutputPin(RM6_GPIO_Port, RM6_Pin);
      break;
  }
}

// ���������� ������ ���� (�� �������) � ����� ������������ ������
void setSelReleToLeakTestCircuit(uint8_t line) {
  switch (line) {
    case 0:
      LL_GPIO_ResetOutputPin(RS1_GPIO_Port, RS1_Pin);
      break;
    case 1:
      LL_GPIO_ResetOutputPin(RS2_GPIO_Port, RS2_Pin);
      break;
    case 2:
      LL_GPIO_ResetOutputPin(RS3_GPIO_Port, RS3_Pin);
      break;
    case 3:
      LL_GPIO_ResetOutputPin(RS4_GPIO_Port, RS4_Pin);
      break;
    case 4:
      LL_GPIO_ResetOutputPin(RS5_GPIO_Port, RS5_Pin);
      break;
    case 5:
      LL_GPIO_ResetOutputPin(RS6_GPIO_Port, RS6_Pin);
      break;
  }
}

// ���������� ������ ���� (�� �������) � ����� ��������� ���������
void setSelReleToImpMeasCircuit(uint8_t line) {
  switch (line) {
    case 0:
      LL_GPIO_SetOutputPin(RS1_GPIO_Port, RS1_Pin);
      break;
    case 1:
      LL_GPIO_SetOutputPin(RS2_GPIO_Port, RS2_Pin);
      break;
    case 2:
      LL_GPIO_SetOutputPin(RS3_GPIO_Port, RS3_Pin);
      break;
    case 3:
      LL_GPIO_SetOutputPin(RS4_GPIO_Port, RS4_Pin);
      break;
    case 4:
      LL_GPIO_SetOutputPin(RS5_GPIO_Port, RS5_Pin);
      break;
    case 5:
      LL_GPIO_SetOutputPin(RS6_GPIO_Port, RS6_Pin);
      break;
  }
}

void User_init(void)
{
	__disable_irq();

	HAL_NVIC_DisableIRQ(DMA1_Channel2_IRQn); // ���������, �������� ��� ��� ������ �����

	// --------- �������� � ��� � ������� ������ ---------------------
  SCB_DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // ��������� ������������ �������
  DWT_CONTROL |= DWT_CTRL_CYCCNTENA_Msk;   // ��������� �������
	DWT_CYCCNT = 0;// �������� ������� ������

/*
	// EEPROM I2C ---------------------------
	eepInit();
	eepReadSeq(0, &(Ee.Mark), 1);
	if (Ee.Mark == EELABELOK) // ������ � ������ ����
	{
		eepReadSeq(0, (uint8_t*)&Ee, sizeof(Ee));
	}
	else // ������ ���
	{
		DataDefaul(); // ��������� ���������
		EE_Save(); // �����
	}
	LED0_ON();
*/
  DataDefault(); // ��������� ���������

	__enable_irq();

	Ip.Flag = 0xFF; // �����
}


/*
/////////////////////////////////////////////////////////////////////////
void User_main(void)
{
	if (Ip.Flag == 0xFD) // ��������� ����������
	{
		cplxSub(&Cm.Ai, &Cm.Au, &Cm.K); // K = Ui - Uu = U� -------
		Cm.Z = Cm.Au; // Z=Ux
		cplxDiv(&Cm.Z, &Cm.K); // Z=U�/U� 
		cplxMul(&Cm.Z, &Cm.R); // Z=(U�/U�)*R��

		Cm.C[0] = Cm.CalZo[Isr_nF]; // ���������� ----------------
		cplxSub(&Cm.Z, &Cm.CalZs[Isr_nF], &Cm.C[1]);
		cplxMul(&Cm.C[0],&Cm.C[1]);
		cplxSub(&Cm.CalZo[Isr_nF], &Cm.Z, &Cm.C[1]);
		cplxSum(&Cm.C[1], &Cm.CalZs[Isr_nF], &Cm.C[1]);
		cplxDiv(&Cm.C[0],&Cm.C[1]);
		Cm.Z = Cm.C[0];

		// ��������� ----------------------------------
		Ip.Rs = Cm.Z.Re; // �������������

		Ip.Ls = Cm.Z.Im / (2 * PI) / Ip.f; // �������������
		Ip.Cs = -1 / (2 * PI) / Ip.f / Cm.Z.Im; // �������

		Ip.tg = Cm.Z.Re / Cm.Z.Im;
		if (Ip.tg < 0) Ip.tg = -Ip.tg; // ������� ���� ������

		Ip.Z = sqrt((Cm.Z.Re * Cm.Z.Re)+(Cm.Z.Im * Cm.Z.Im)); // ������ �������������

		Ip.U = Ee.Cal_Ku * sqrt((Cm.Au.Re * Cm.Au.Re)+(Cm.Au.Im * Cm.Au.Im)); // Urms

		cplxDiv(&Cm.K, &Cm.R); // K = U�/R� 
		Ip.I = Ee.Cal_Ki * sqrt((Cm.K.Re * Cm.K.Re)+(Cm.K.Im * Cm.K.Im)); // Irms


		Ip.Flag = 0xFF; // ����� ������� ----------------


		Tmp_f = Ip.tg * Ip.tg;
		Ip.Rp = Ip.Rs *(1 + Tmp_f)/ Tmp_f; // ��� ������������ ����� ���������
		Ip.Lp = Ip.Ls *(1 + Tmp_f);
		Ip.Cp = Ip.Cs /(1 + Tmp_f);

//		sprintf(str, "Au.Re = %e\n\r", Cm.Au.Re); dbgUartPuts(str);
//		sprintf(str, "Au.Im = %e\n\r", Cm.Au.Im); dbgUartPuts(str);
//		sprintf(str, "Ai.Re = %e\n\r", Cm.Ai.Re); dbgUartPuts(str);
//		sprintf(str, "Ai.Im = %e\n\r", Cm.Ai.Im); dbgUartPuts(str);
//		dbgUartPuts("\n\r");
//		sprintf(str, "U = %e V\n\r", Ip.U); dbgUartPuts(str);
//		sprintf(str, "I = %e A\n\r", Ip.I); dbgUartPuts(str);
//		dbgUartPuts("\n\r");
//		sprintf(str, "tg = %e\n\r", Ip.tg); dbgUartPuts(str);
//		sprintf(str, "D = %e\n\r", 1/Ip.tg); dbgUartPuts(str);
//		dbgUartPuts("\n\r");


     // TODO: ���� ����������������
		uartPuts("\n\r");
		sprintf(str, "CalZ.Re = %e\n\r", Cm.Z.Re); uartPuts(str);
		sprintf(str, "CalZ.Im = %e\n\r", Cm.Z.Im); uartPuts(str);
		uartPuts("\n\r");
		sprintf(str, "f = %f Hz\n\r", Ip.f); uartPuts(str);
		sprintf(str, "Z = %f Ohm\n\r", Ip.Z); uartPuts(str);
		if (Ip.Z < 10000) // ������ ������������� ����� 10� - ����������������
		{
//			sprintf(str, "Rs = %f Ohm\n\r", Ip.Rs); dbgUartPuts(str); // TODO: ���� ����������������

			if (Ip.Cs > 0) sprintf(str, "Cs = %e F\n\r", Ip.Cs);
			else sprintf(str, "Ls = %e H\n\r", Ip.Ls);
		}
		else
		{
//			sprintf(str, "Rp = %f Ohm\n\r", Ip.Rp); dbgUartPuts(str); // TODO: ���� ����������������

			if (Ip.Cp > 0) sprintf(str, "Cp = %e F\n\r", Ip.Cp);
			else sprintf(str, "Lp = %e H\n\r", Ip.Lp);
		}
    
//		dbgUartPuts(str); // TODO: ���� ����������������

		// ------------------------------------------------------------- ����� 
		LeakageStart();  // ��������� ���� ������
     // TODO: ���� ����������������
//		sprintf(str, "Inp 1..6 = %05u %05u %05u %05u %05u %05u\n\r", AdcData[0], AdcData[1], AdcData[2], AdcData[3], AdcData[4], AdcData[5]); dbgUartPuts(str);
//		dbgUartPuts("\n\r");

		Ip.Flag = 0xFE; // ����� ---------------------
		ImpedansStart(FREQ_NUM_80Hz); // ������ ��������� ���������
	}
}
*/


///////////////////////////////////////////////////////////////////////////////
// �������� ������ �������� ����� line �� �����
// ���������� ������ � �������� ��� (12 ���)
uint16_t measLeakage(uint8_t line) {

	Ip.Flag = 0xFE; // ����� ---------------------

  // �������� ������ ����� � ������� ����
  setSelReleToLeakTestCircuit(line);
  HAL_Delay(1);
  setInputReleToMeasCircuit(line);
  HAL_Delay(300); // ���� ������ �������
    
	TIM6->CR1 &= ~TIM_CR1_CEN; // ���� TIM
	TIM7->CR1 &= ~TIM_CR1_CEN; // ���� TIM

	DAC1->CR &= ~DAC_CR_EN1; // ���� ���
	ADC1->CR2 &= ~ADC_CR2_ADON; // ���� ���

	DMA1_Channel1->CCR &= ~DMA_CCR_EN; // ���� ������
	DMA1_Channel1->CCR &= ~(DMA_CCR_TCIE | DMA_CCR_HTIE); // ������ ����������
	while (DMA1_Channel1->CCR & DMA_CCR_EN){}; // ���� ����������

	DMA1_Channel2->CCR &= ~DMA_CCR_EN; // ���� ������
	while (DMA1_Channel2->CCR & DMA_CCR_EN){}; // ���� ����������

	LL_ADC_REG_SetSequencerLength(ADC1, LL_ADC_REG_SEQ_SCAN_ENABLE_6RANKS);
	// ��������� ����� � ��� ������������
	LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_2);
	LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_2, LL_ADC_SAMPLINGTIME_24CYCLES);
	LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_2, LL_ADC_CHANNEL_3);
	LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_3, LL_ADC_SAMPLINGTIME_24CYCLES);

	ADC1->CR2 |= ADC_CR2_ADON;

	DMA1_Channel1->CPAR = (uint32_t)&ADC1->DR;
	DMA1_Channel1->CMAR = (uint32_t)AdcData;
	DMA1_Channel1->CNDTR = 6; // ���������� ���������
	DMA1->IFCR = DMA_IFCR_CGIF1; // ���� ������ ����� (����������� ����� ����������)
	DMA1_Channel1->CCR  |= DMA_CCR_EN; // ������ DMA
	ADC1->CR2 |= ADC_CR2_SWSTART;

  HAL_Delay(5); // ���� ��������� �������������� � ���������

	Ip.Flag = 0xFD; // ��������� ����������

  if (line_en[line]) setInputReleToAmp(line);
  
  return AdcData[line];
}



///////////////////////////////////////////////////////////////////////////////
void ImpedansStart(uint8_t NumF) // ������ ��������� ���������
{
	int32_t Ix;

	Ip.Flag = 0xFE; // ����� ---------------------

	TIM6->CR1 &= ~TIM_CR1_CEN; // ���� TIM
	TIM7->CR1 &= ~TIM_CR1_CEN; // ���� TIM

	DAC1->CR &= ~DAC_CR_EN1; // ���� ���
	ADC1->CR2 &= ~ADC_CR2_ADON; // ���� ���

	DMA1_Channel1->CCR &= ~DMA_CCR_EN; // ���� ������
	while (DMA1_Channel1->CCR & DMA_CCR_EN){}; // ���� ����������

	DMA1_Channel2->CCR &= ~DMA_CCR_EN; // ���� ������
	while (DMA1_Channel2->CCR & DMA_CCR_EN){}; // ���� ����������

	LL_ADC_REG_SetSequencerLength(ADC1, LL_ADC_REG_SEQ_SCAN_ENABLE_2RANKS); // ��� �����
	LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_0);
	LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_0, LL_ADC_SAMPLINGTIME_4CYCLES);
	LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_2, LL_ADC_CHANNEL_1);
	LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_1, LL_ADC_SAMPLINGTIME_4CYCLES);

	Isr_nF = NumF; // ��� �������� � ����������
	Ip.f = Freq[Isr_nF].Value; // �������

	for(Ix=0; Ix<Freq[Isr_nF].AdcLen; Ix++)
	{
		TSin[Ix] = 32760 * sin((PI * 2 * Freq[Isr_nF].AdcPer * (float)Ix)/ Freq[Isr_nF].AdcLen); // ����� ��� �������
	}
	for(Ix=0; Ix<Freq[Isr_nF].AdcLen/4; Ix++) TSin[Ix + Freq[Isr_nF].AdcLen] = TSin[Ix]; // �������� �������� �������

	// ------------- �A� -----------------
	for(Ix=0; Ix<Freq[Isr_nF].DacLen; Ix++) Dac[Ix] = 32767 +(30000 * sin((PI * 2 * (float)Ix)/Freq[Isr_nF].DacLen)); // ����� ��� ���

	TIM7->ARR = (SYSTEMCORECLOCK / Freq[Isr_nF].DacFSamples)-1;
	DAC1->CR |= DAC_CR_DMAEN1 | DAC_CR_TEN1;

	DMA1_Channel2->CPAR = (uint32_t)&DAC->DHR12L1;
	DMA1_Channel2->CMAR = (uint32_t)Dac;
	DMA1_Channel2->CNDTR = Freq[Isr_nF].DacLen; // ���������� ���������
	DMA1->IFCR = DMA_IFCR_CGIF2; // ���� ������ ����� (����������� ����� ����������)
	DMA1_Channel2->CCR  |= DMA_CCR_EN; // ������ DMA
	DAC1->CR |= DAC_CR_EN1; // ������� ���

	// ------------- ��� -----------------
	TIM6->ARR = (SYSTEMCORECLOCK / Freq[Isr_nF].AdcFSamples)-1;
	ADC1->CR2 |= ADC_CR2_DMA;
	ADC1->CR2 |= ADC_CR2_ADON;

	DMA1_Channel1->CPAR = (uint32_t)&ADC1->DR;
	DMA1_Channel1->CMAR = (uint32_t)AdcData;
	DMA1_Channel1->CNDTR = Freq[Isr_nF].AdcLen * ADC_CH; // ���������� ���������
	DMA1_Channel1->CCR  |= DMA_IT_TC | DMA_IT_HT; // �� ��������� � �� �������� ������
	DMA1->IFCR = DMA_IFCR_CGIF1; // ���� ������ ����� (����������� ����� ����������)
	DMA1_Channel1->CCR  |= DMA_CCR_EN; // ������ DMA
	ADC1->CR2 |= ADC_CR2_SWSTART;

	TIM7->CNT = 0; // ��������� ----------------
	TIM6->CNT = 0;
	TIM7->CR1 |= TIM_CR1_CEN; // ������ TIM
	TIM6->CR1 |= TIM_CR1_CEN; // ������ TIM

	Ip.Flag = CYCLDUMMY + Freq[Isr_nF].Cycles; // ������� �����
}


// �������� �������� �������� ����� line �� �������� ������� c ������� n_freq
// ������ ������ ��. � ������� Freq[NUMBRSFREQ]
// ���������� ��������, ��������� �������� ������� ~500 ��
float measImpedance(uint8_t line, uint8_t n_freq) {

  // �������� ������ ����� � ������� ����
  setInputReleToMeasCircuit(line);
  HAL_Delay(1);
  setSelReleToImpMeasCircuit(line);
  HAL_Delay(10);

  ImpedansStart(n_freq); // ������ ��������� ���������
  while (Ip.Flag != 0xFD) { }; // ���, ����� ��������� ���������

  if (line_en[line]) setInputReleToAmp(line);
  HAL_Delay(1);
  setSelReleToLeakTestCircuit(line);

  cplxSub(&Cm.Ai, &Cm.Au, &Cm.K); // K = Ui - Uu = U� -------
  Cm.Z = Cm.Au; // Z=Ux
  cplxDiv(&Cm.Z, &Cm.K); // Z=U�/U� 
  cplxMul(&Cm.Z, &Cm.R); // Z=(U�/U�)*R��

  Cm.C[0] = Cm.CalZo[Isr_nF]; // ���������� ----------------
  cplxSub(&Cm.Z, &Cm.CalZs[Isr_nF], &Cm.C[1]);
  cplxMul(&Cm.C[0], &Cm.C[1]);
  cplxSub(&Cm.CalZo[Isr_nF], &Cm.Z, &Cm.C[1]);
  cplxSum(&Cm.C[1], &Cm.CalZs[Isr_nF], &Cm.C[1]);
  cplxDiv(&Cm.C[0], &Cm.C[1]);
  Cm.Z = Cm.C[0];

  // ��������� ----------------------------------
  Ip.Rs = Cm.Z.Re; // �������������

  Ip.Ls = Cm.Z.Im / (2 * PI) / Ip.f; // �������������
  Ip.Cs = -1 / (2 * PI) / Ip.f / Cm.Z.Im; // �������

  Ip.tg = Cm.Z.Re / Cm.Z.Im;
  if (Ip.tg < 0) Ip.tg = -Ip.tg; // ������� ���� ������

  Ip.Z = sqrt((Cm.Z.Re * Cm.Z.Re)+(Cm.Z.Im * Cm.Z.Im)); // ������ �������������

  Ip.U = Ee.Cal_Ku * sqrt((Cm.Au.Re * Cm.Au.Re)+(Cm.Au.Im * Cm.Au.Im)); // Urms

  cplxDiv(&Cm.K, &Cm.R); // K = U�/R� 
  Ip.I = Ee.Cal_Ki * sqrt((Cm.K.Re * Cm.K.Re)+(Cm.K.Im * Cm.K.Im)); // Irms

  Ip.Flag = 0xFF; // ����� ������� ----------------

  return Ip.Z;
}



///////////////////////////////////////////////////////////////////////////////
void User_int_DMA_ADC(void)
{
	static int64_t Sum_U_Im, Sum_U_Re, Sum_I_Im, Sum_I_Re; // ���������
	uint16_t Ii, Jj;

//LED0_ON();

	if (DMA1->ISR & DMA_ISR_HTIF1)  // ������ �������� ������ ������
	{
		if (Ip.Flag == Freq[Isr_nF].Cycles) // ������ ������ �����
		{
			Sum_U_Im = 0; Sum_U_Re = 0; // ���������� ����������
			Sum_I_Im = 0; Sum_I_Re = 0;
		}
		Ii = 0;
		Jj = Freq[Isr_nF].AdcLen;
	}
	else
	{
		Ii = Freq[Isr_nF].AdcLen;
		Jj = Freq[Isr_nF].AdcLen * 2;
	}

	DMA1->IFCR = DMA_IFCR_CGIF1; // Fast Clear DMA pending bits

	if (Ip.Flag <= Freq[Isr_nF].Cycles) // �������� 
	{
		while (Ii < Jj) // �������� �� ������������ �������� ������� ��� ������
		{
			Sum_I_Im -= (int32_t)AdcData[Ii] * TSin[Ii/2]; // �������
			Sum_I_Re += (int32_t)AdcData[Ii] * TSin[(Ii/2)+(Freq[Isr_nF].AdcLen/4)];
			Ii++;
			Sum_U_Im -= (int32_t)AdcData[Ii] * TSin[Ii/2]; // SIN
			Sum_U_Re += (int32_t)AdcData[Ii] * TSin[(Ii/2)+(Freq[Isr_nF].AdcLen/4)]; // COS
			Ii++;
		}
	}

	if (Jj == (Freq[Isr_nF].AdcLen*2)) // ���������� ������ �������� ������
	{
		Ip.Flag--; // ������ �������
		if (Ip.Flag == 0) // ��������� ������ ���������� ������ ��������� (��������� �����)
		{
			Cm.Ai.Re = Sum_I_Re; // ���������� �� ����� U�
			Cm.Ai.Im = Sum_I_Im;
			Cm.Au.Re = Sum_U_Re; // ���������� �� ���������� �������� - U�
			Cm.Au.Im = Sum_U_Im;
			Ip.Flag = 0xFD; // ��������� ����������

			TIM6->CR1 &= ~TIM_CR1_CEN; // ���� TIM
			TIM7->CR1 &= ~TIM_CR1_CEN; // ���� TIM
		}
	}

//LED0_OFF();
}

///////////////////////////////////////////////////////////////////////////////
void DataDefault(void) // ��������� ���������
{
	Ee.Cal_Ki = 1E-12;
	Ee.Cal_Ku = 1E-12;

	Cm.R.Re = Ee.Cal_Rr = 300; // ������������� ����� R37
	Cm.R.Im = Ee.Cal_Ri = 0;

	Cm.CalZo[0].Re = 7E4; // ���������� ����-����
	Cm.CalZo[0].Im = 2.5E4;
	Cm.CalZs[0].Re = 5.1E-1;
	Cm.CalZs[0].Im = 0;

	Cm.CalZo[1].Re = 8.4E3;
	Cm.CalZo[1].Im = 2.7E4;
	Cm.CalZs[1].Re = 7.3E-1;
	Cm.CalZs[1].Im = -2.1E-3;

	Cm.CalZo[2].Re = 4.9E2;
	Cm.CalZo[2].Im = 7.5E3;
	Cm.CalZs[2].Re = 6.7E-1;
	Cm.CalZs[2].Im = -2.1E-2;

	Cm.CalZo[3].Re = 6.4E4;
	Cm.CalZo[3].Im = 3.2E4;
	Cm.CalZs[3].Re = 4.8E-1;
	Cm.CalZs[3].Im = -2.9E-2;

	Cm.CalZo[4].Re = 3.6E4;
	Cm.CalZo[4].Im = 4.4E4;
	Cm.CalZs[4].Re = 6.4E-1;
	Cm.CalZs[4].Im = -1.3E-2;

	Cm.CalZo[5].Re = 6.4E4;
	Cm.CalZo[5].Im = 2.5E4;
	Cm.CalZs[5].Re = 5E-1;
	Cm.CalZs[5].Im = 0;
}



///////////////////////////////////////////////////////////////////////////////
void EE_Save(void) // ����� ������ � ������
{
	uint8_t r = EELABELOK;

	Ee.Mark = 0; // ���� ��������� �� ���������
	eepWriteSeq(0, (uint8_t*)&Ee, sizeof(Ee));
	eepWriteSeq(0, &r, 1); // ����� - �� ������� � ����� ���������
}



//////////// �������� � �� //////////////////////////////////////////////
void Delay_ms(uint32_t ms)
{
	while (ms-- != 0) {Delay_us(1000);}
}



//////////// �������� � ��� //////////////////////////////////////////////
void Delay_us(uint32_t us)
{
    uint32_t us_count_tic =  us * (SystemCoreClock / 1000000);
    DWT->CYCCNT = 0U; // �������� �������
    while(DWT->CYCCNT < us_count_tic);
}

//----------------------------------------------------------------------------
// res = res / div
//----------------------------------------------------------------------------
void cplxDiv(cplx_t *res, cplx_t *div)
{
	cplx_t tmp;
	float mod2;
	
	mod2 = div->Re * div->Re + div->Im * div->Im;

	tmp.Re = (res->Re * div->Re + res->Im * div->Im)/ mod2;	// (a*c+b*d)/(c*c+d*d);
	tmp.Im = (res->Im * div->Re - res->Re * div->Im)/ mod2; // (b*c-a*d)/(c*c+d*d);
	res->Re = tmp.Re;
	res->Im = tmp.Im;
}


//----------------------------------------------------------------------------
// res = res * mul
//----------------------------------------------------------------------------
void cplxMul(cplx_t *res, cplx_t *mul)
{
	cplx_t tmp;

	tmp.Re = (res->Re * mul->Re - res->Im * mul->Im); // a*c - b*d
	tmp.Im = (res->Re * mul->Im + res->Im * mul->Re); // a*d + b*c
	res->Re = tmp.Re;
	res->Im = tmp.Im;
}


//----------------------------------------------------------------------------
// res = subs1 -subs2
//----------------------------------------------------------------------------
void cplxSub(cplx_t *subs1, cplx_t *subs2, cplx_t *res)
{
    res->Re = subs1->Re - subs2->Re;
    res->Im = subs1->Im - subs2->Im;
}


//----------------------------------------------------------------------------
// res = summ1 + summ2
//----------------------------------------------------------------------------
void cplxSum(cplx_t *summ1, cplx_t *summ2, cplx_t *res)
{
    res->Re = summ1->Re + summ2->Re;
    res->Im = summ1->Im + summ2->Im;
}






