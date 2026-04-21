#include "mq7.h"

u8 time;  // 保持与原工程风格一致

/************************************************
函数名称：MQ7_ADC_Init
函数功能：MQ7 专用ADC初始化 引脚 PB1 对应通道9
************************************************/
void MQ7_ADC_Init(void)
{
	GPIO_InitTypeDef GPIO_Initstructre;
	ADC_InitTypeDef ADC_Initstructre;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	// PB1 配置为模拟输入
	GPIO_Initstructre.GPIO_Pin = GPIO_Pin_1;
	GPIO_Initstructre.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB,&GPIO_Initstructre);
	
	ADC_Initstructre.ADC_Mode = ADC_Mode_Independent;
	ADC_Initstructre.ADC_ScanConvMode = DISABLE;
	ADC_Initstructre.ADC_ContinuousConvMode = DISABLE;
	ADC_Initstructre.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_Initstructre.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_Initstructre.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1,&ADC_Initstructre);
	
	ADC_Cmd(ADC1,ENABLE);
	
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
}

/************************************************
函数名称：MQ7_Get_ADC
函数功能：获取MQ7对应通道单次采样值
************************************************/
u16 MQ7_Get_ADC(u8 ch)
{
	ADC_RegularChannelConfig(ADC1,ch,1,ADC_SampleTime_239Cycles5);
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
	
	return ADC_GetConversionValue(ADC1);
}

/************************************************
函数名称：MQ7_Get_ADC_Average
函数功能：多次采样取平均
************************************************/
u16 MQ7_Get_ADC_Average(u8 ch,u8 time)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<time;t++)
	{
		temp_val += MQ7_Get_ADC(ch);
		DelayMs(5);
	}
	return temp_val/time;
}

/************************************************
函数名称：MQ7_Check
函数功能：读取MQ7传感器浓度值
************************************************/
u16 MQ7_Check(void)
{
	u16 val = 0;
	val = MQ7_Get_ADC_Average(ADC_Channel_9,10);
	return val;
}
