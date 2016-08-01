#define __STC15W4KxxS4_ADC_C__
// ==================================================================


// ����ͷ�ļ�
// ==================================================================
#include	"stc15w4kxxs4.h"
// ==================================================================


// ======================================================================================================================
// ======================================================================================================================



#if (TEST_ADC && USE_ADC)
/********************************************************************************
* ������:	TestAdc
* ��  ��:	ADC���Գ���
* ��  ��:	��
* ����ֵ:	��
* ��  ��:	V1.0
* ��  ��:	2015-09-11
* ��  ��:	zenghouyun
* ��  ע:	����ADC�����������е���
********************************************************************************/
void TestAdc(void)
{
	uchar *_pAdcTestBuf;
	uchar _Error, _AdcChannel;	
	sADC_InitTypeDef * _spAdcStruct;
	
	_AdcChannel	= ADC_2|ADC_3|ADC_4|ADC_5|ADC_6|ADC_7;	// ��Ҫ������ADCͨ����B0~B7��8λ����ͬʱѡ���λ��ADC_0/ADC_1/ADC_2/ADC_3/ADC_4/ADC_5/ADC_6/ADC_7��

	_spAdcStruct->ADC_ResultAdj	= HIGH_8B;		// ADCת��ֵ������ʽ��HIGH_8B(��8λ ��2λ)/HIGH_2B(��2λ ��8λ)��
	_spAdcStruct->ADC_Interrupt	= ENABLE;		// ADC�ж�ʹ�ܣ�	DISABL(�ر�)/ENABLE(ʹ��)��
	_spAdcStruct->ADC_Priority	= PRIORITY_L;	// ADC�ж����ȼ�, PRIORITY_L(��)/PRIORITY_H(��)��
	_spAdcStruct->ADC_SlewRate	= SPEED_FAST;	// ADCת�����ٶȣ�0~3���ĵ���SPEED_LOW(����)/SPEED_SLOW(��)/SPEED_FAST(��)/SPEED_SOON(���)��
	
	_Error = InitializeADC(_AdcChannel,_spAdcStruct);	// ��ʼ��ADCģ��
	if (_Error)
	{
		// ������������
		_Error = 0;
	}
	
	_pAdcTestBuf = &sAdcValue.ResultBuf;				// ���ػ�������ַָ��ֵ
	
	_Error = GetAdcValue(_pAdcTestBuf, 2);				// �˹���ѯADC	
	if (_Error)
	{
		// ������������
		_Error = 0;
	}
	
	AdcRunControl(0);		// ����ADCģ������״̬
	AdcPowerControl(0);		// ����ADCģ���Դ����	
}
#endif



#if (USE_ADC)
/********************************************************************************
* ������:	InitializeADC
* ��  ��:	��ʼ��ADC
* ��  ��:	ADC_Channel						��Ҫ������ADCͨ����B0~B7��8λ����ͬʱѡ���λ��	ADC_0/ADC_1/ADC_2/ADC_3/ADC_4/ADC_5/ADC_6/ADC_7��
*			__spAdcStruct->ADC_ResultAdj	ADCת��ֵ������ʽ��	HIGH_8B(��8λ ��2λ)/HIGH_2B(��2λ ��8λ)��
*			__spAdcStruct->ADC_Interrupt	ADC�ж�ʹ�ܣ�	DISABL(�ر�)/ENABLE(ʹ��)��
*			__spAdcStruct->ADC_Priority		ADC�ж����ȼ�, 	PRIORITY_L(��)/PRIORITY_H(��)��
*			__spAdcStruct->ADC_SlewRate		ADCת�����ٶȣ�	0~3���ĵ���SPEED_LOW(����)/SPEED_SLOW(��)/SPEED_FAST(��)/SPEED_SOON(���)��
* ����ֵ:	0:��ʼ���ɹ���1:ADCͨ���Ŵ���2:��������
* ��  ��:	V1.0
* ��  ��:	2015-09-10
* ��  ��:	zenghouyun
* ��  ע:	�����вο���STC�����ϼ�����
********************************************************************************/
uchar InitializeADC(uchar __AdcChannel, sADC_InitTypeDef *__spAdcStruct)
{
	uchar _AdcSpeed;
	
	// ������õ�ͨ������û�д���
	if (__AdcChannel != 0)
	{
		if (__AdcChannel & (~ADC_PASSAGE))		return 1;	// ʹ����δ��ͨ��ͨ�������ش������1
	}
	if (__spAdcStruct->ADC_SlewRate & ~0x60)	return 2;	// ADCת�����ٶ����ô���
	
	P1ASF = __AdcChannel;						// ����P1��(P1.0~P1.7)ΪADC����˿�
	sAdcValue.InitChannel = __AdcChannel;		// ADC������Ҫת����ͨ����1Ϊ��Ҫת����ͨ����0Ϊ��ת��ͨ��
	// ------------------------------------------

	if (__AdcChannel & BIT0)	{ P0M1 |=  0x01;	P0M0 &= ~BIT0; }	// ���ö˿�Ϊ(10)��������ģʽ
	if (__AdcChannel & BIT1)	{ P0M1 |=  0x02;	P0M0 &= ~BIT1; }
	if (__AdcChannel & BIT2)	{ P0M1 |=  0x04;	P0M0 &= ~BIT2; }
	if (__AdcChannel & BIT3)	{ P0M1 |=  0x08;	P0M0 &= ~BIT3; }
	if (__AdcChannel & BIT4)	{ P0M1 |=  0x10;	P0M0 &= ~BIT4; }
	if (__AdcChannel & BIT5)	{ P0M1 |=  0x20;	P0M0 &= ~BIT5; }
	if (__AdcChannel & BIT6)	{ P0M1 |=  0x40;	P0M0 &= ~BIT6; }
	if (__AdcChannel & BIT7)	{ P0M1 |=  0x80;	P0M0 &= ~BIT7; }
	
	_AdcSpeed = __spAdcStruct->ADC_SlewRate;	// ADCת���ٶ�
	sAdcValue.ConvertSpeed = _AdcSpeed;
	
	// ------------------------------------------
	if (__spAdcStruct->ADC_ResultAdj)	CLK_DIV |=  ADRJ;		// 1:ADC_RES[1:0]��Ÿ�2λֵ��ADC_RESL[7:0]��ŵ�8λֵ
	else								CLK_DIV &= ~ADRJ;		// 0:ADC_RES[7:0]��Ÿ�8λֵ��ADC_RESL[1:0]��ŵ�2λֵ
	
    ADC_RES = 0;	// ���ADC����Ĵ��������ֽ�
	ADC_RESL = 0;	// ���ADC����Ĵ��������ֽ�
	ADC_CONTR = 0;	// ���ADC���ƼĴ���������
	
	if (__AdcChannel == 0x00)				// ���ڲ�BandGap�ο���ѹ
	{
		EADC = 0;							// �ر�ADC�ж�
		ADC_CONTR = ADC_POWER | _AdcSpeed;	// ADC��Դ��ת��Ƶ��
	}
	else
	{
		if (__spAdcStruct->ADC_Interrupt)	// ADC�жϷ�ʽ
		{
			if (__spAdcStruct->ADC_Priority)	PADC = 1;	// ���ж����ȼ�
			else								PADC = 0;	// ���ж����ȼ�
			ADC_CONTR = ADC_POWER | _AdcSpeed | \
						ADC_START | __AdcChannel;			// ADC��Դ��ת��Ƶ�ʣ�A/DC��ʼת����ת���ĵ���(0~7)
		}
		else									// ADC��ѯ��ʽ
		{
			EADC = 0;							// �ر�ADC�ж�
			ADC_CONTR = ADC_POWER | _AdcSpeed;	// ADC��Դ��ת��Ƶ��
		}
	}	
    DelayAdc(2);                      			// ADC�ϵ粢��ʱ
	
	if (__spAdcStruct->ADC_Interrupt) EADC = 1;	// ʹ��ADC�ж�
		
	return 0;
}
#endif



#if (USE_ADC)
/********************************************************************************
* ������:	GetAdcValue
* ��  ��:	��ѯ��ʽ��ȡADC���
* ��  ��:	*__pResultBuf	����ADC����������ַ��ָ�룬�洢�ĵ�ַ�Զ����㣬ÿ��ʹ�õ�ͨ��ռ�����ֽڳ���(ͨ���ţ�������ֽڣ�������ֽ�)
* 			__Channel		ͨ����,0-7:��ADC�˿�(0-7)��8:���ڲ�BandGap�ο���ѹ
* ����ֵ:	������� 0:�޴���1:�����ͨ���Ŵ���2:��ͨ��û�п�ͨADC���ܡ�
* ��  ��:	V1.0
* ��  ��:	2015-09-16
* ��  ��:	zenghouyun
* ��  ע:	�����вο���STC�����ϼ�����
********************************************************************************/
uchar GetAdcValue(uchar *__pResultBuf, uchar __Channel)
{
    uchar _Eadc;
	uchar _ChPosition = 0x01;
	uchar _CurrentCh = 0;
	
	if (__Channel > 8)							return 1;	// �����ͨ���Ų�������
	if (_ChPosition & ~sAdcValue.InitChannel)	return 2;	// ��ͨ����ADC��ʼ��ʱû�б���ͨADC����
	
	_Eadc = EADC;				// �ݴ�ADC�ж�״̬
	EADC = 0;					// �ر�ADC�ж�

	if (__Channel < 8)			// ��Ҫ�ɼ���ADCͨ����0~7
	{
		// ÿһ��ͨ��ռ�����ֽڣ���ͨ��ͨ�����Զ�����洢��ַ�����Ը������ע�͵��˶δ��롣
		// -------------------------------------------------------------------------
		if (_CurrentCh < __Channel)		// �洢��Ԫ�ĵ�ַ�Ƶ����Ӧ��λ��
		{
			_CurrentCh++;
			_ChPosition <<= 1;
			if (_ChPosition & sAdcValue.InitChannel)
				__pResultBuf += 3;					// �����ͨ�����洢��Ԫ�ĵ�ַ�����3����Ԫ
		}
		// -------------------------------------------------------------------------
		ADC_CONTR = ADC_POWER | ADC_START | (sAdcValue.ConvertSpeed & 0x60) | __Channel;	// ADC��Դ��A/DC��ʼת����ת��Ƶ�ʣ�ת���ĵ���(0~7)
	}
	else							// ����9ͨ�����ڲ�BandGap�ο���ѹ
	{
		// ÿһ��ͨ��ռ�����ֽڣ���ͨ��ͨ�����Զ�����洢��ַ�����Ը������ע�͵��˶δ��롣
		// -------------------------------------------------------------------------
		if (_CurrentCh < 7)			// �洢��Ԫ�ĵ�ַ�Ƶ�����3����Ԫλ��
		{
			_CurrentCh++;
			_ChPosition <<= 1;
			if (_ChPosition & sAdcValue.InitChannel)
				__pResultBuf += 3;					// �����ͨ�����洢��Ԫ�ĵ�ַ�����3����Ԫ
		}
		// -------------------------------------------------------------------------
		P1ASF = 0;
		ADC_CONTR = ADC_POWER | ADC_START | (sAdcValue.ConvertSpeed & 0x60);	// ADC��Դ��A/DC��ʼת����ת��Ƶ��
	}
	
	NOP(8);								// �ȴ�4��NOP@11.0592MHz
	while (!(ADC_CONTR & ADC_FLAG));	// �ȴ�ADCת�����
	ADC_CONTR &= ~ADC_FLAG;				// ���ADC�жϱ�־
	
	*__pResultBuf++ = __Channel;		// ����ADCͨ����
	*__pResultBuf++ = ADC_RES;			// ��ȡADC����Ĵ��������ֽ�
	*__pResultBuf++ = ADC_RESL;			// ��ȡADC����Ĵ��������ֽ�
	
	if (__Channel == 8) P1ASF = sAdcValue.InitChannel;
	
	EADC = _Eadc;		// �ָ�ADC�ж�״̬
    return 0;			// �޴�
}
#endif



#if (USE_ADC)
/********************************************************************************
* ������:	AdcPowerControl
* ��  ��:	ADCģ���Դ���ؿ���
* ��  ��:	__Switch	0:�ر�ADCģ���Դ��1:��ADCģ���Դ
* ����ֵ:	��
* ��  ��:	V1.0
* ��  ��:	2015-09-16
* ��  ��:	zenghouyun
* ��  ע:	��
********************************************************************************/
void AdcPowerControl(uchar __Switch)
{
	if (__Switch)	ADC_CONTR |=  ADC_POWER;	// ��ADCģ���Դ
	else			ADC_CONTR &= ~ADC_POWER;	// �ر�ADCģ���Դ
}
#endif



#if (USE_ADC)
/********************************************************************************
* ������:	AdcRunCountrol
* ��  ��:	ADCģ������״̬����
* ��  ��:	__RunSate	0:ADCֹͣת����1:��ʼ���У�����ADCת��
* ����ֵ:	��
* ��  ��:	V1.0
* ��  ��:	2015-09-16
* ��  ��:	zenghouyun
* ��  ע:	��
********************************************************************************/
void AdcRunControl(uchar __Switch)
{
	if (__Switch)	ADC_CONTR |=  ADC_START;	// ADCģ�鿪ʼ����
	else			ADC_CONTR &= ~ADC_START;	// ADCģ��ֹͣ����
}
#endif



#if (USE_ADC)
/********************************************************************************
* ������:	ADC_Routine
* ��  ��:	ADC�жϷ������
* ��  ��:	��
* ����ֵ:	��
* ��  ��:	V1.0
* ��  ��:	2015-09-10
* ��  ��:	zenghouyun
* ��  ע:	�����вο���STC�����ϼ�����
********************************************************************************/
void ADC_Routine(void)			interrupt ADC_VECTOR		// ��ѯ����: 05	�ж�������ַ: 002BH	�ж�Դ: ADC(ģת��)
{
	static uchar _ChPosition=0x01;		// ǰǰADCͨ���ŵ�λ��(0x01~0x80)
	static uchar _ChNumber=0;			// ��ǰ��ADCͨ����(0~7)
	static uchar *_pResultBuf;

	ADC_CONTR &= ~ADC_FLAG;				// ���ADC�жϱ�־
    
	if (_ChNumber > 7)
	{
		_ChNumber = 0;							// �л�����һ��ͨ��
		_ChPosition = 0x01;						// ǰǰADCͨ���ŵ�λ��(0x01~0x80)
		_pResultBuf = &sAdcValue.ResultBuf;		// ���ػ�������ַָ��ֵ
	}
	
	for (_ChNumber=0; _ChNumber<7; _ChNumber++)
	{
		if (_ChPosition & sAdcValue.InitChannel)	// ͨ���п�ͨADC����
		{
			*_pResultBuf++ = _ChNumber;		// ��ȡ��ǰ��ADCͨ����
			*_pResultBuf++ = ADC_RES;		// ��ȡADC����Ĵ��������ֽ�
			*_pResultBuf++ = ADC_RESL;		// ��ʾADC����Ĵ��������ֽ�
			
			ADC_CONTR = ADC_POWER | ADC_START | sAdcValue.ConvertSpeed | _ChNumber;	// ADC��Դ��A/DC��ʼת����ת��Ƶ�ʣ�ת���ĵ���(0~7)
		}
		_ChPosition<<=1;					// û��ͨADC����ת����һ��ͨ��
	}
}
#endif



#if (USE_ADC)
/********************************************************************************
* ������:	DelayAdc
* ��  ��:	������ADC��������ʱ����
* ��  ��:	__Count		��ʱʱ��ֵ
* ����ֵ:	��
* ��  ��:	V1.0
* ��  ��:	2015-09-10
* ��  ��:	zenghouyun
* ��  ע:	�����вο���STC�����ϼ�����
********************************************************************************/
void DelayAdc(uint __Count)
{
    uint i;

    while (__Count--)
    {
        i = 5000;			// i=5000@11.0592MHz
        while (i--);
    }
}
#endif