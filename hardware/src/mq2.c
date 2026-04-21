/***************STM32F103C8T6**********************
 * 文件名  ：MQ2.c
 * 描述    ：MQ2烟雾气体传感器(A0模拟量采集)
 * 备注    : 修复乱码/ppm计算，ADC采样正常
 * 接口    ：PA4-A0(模拟输出)  ADC1_IN4
 * 供电    ：5V  预热时间：≥2分钟
 ********************LIGEN*************************/

#include "mq2.h"
#include "delay.h"

#define MQ2_ADC_PIN     GPIO_Pin_4
#define MQ2_ADC_PORT    GPIOA
#define MQ2_ADC_CHANNEL ADC_Channel_4
#define VCC             3.3f
#define ADC_MAX         4095.0f
#define PPM_MAX         1000.0f

void MQ2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    GPIO_InitStructure.GPIO_Pin = MQ2_ADC_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(MQ2_ADC_PORT, &GPIO_InitStructure);

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_Cmd(ADC1, ENABLE);

    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
}

uint16_t MQ2_Read_ADC(void)
{
    ADC_RegularChannelConfig(ADC1, MQ2_ADC_CHANNEL, 1, ADC_SampleTime_239Cycles5);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
    return (uint16_t)(ADC_GetConversionValue(ADC1) & 0x0FFF);
}

uint16_t MQ2_Read_Average(uint8_t times)
{
    uint32_t temp = 0;
	uint8_t i;
    for(i=0;i<times;i++)
    {
        temp += MQ2_Read_ADC();
        DelayUs(100);
    }
    return (uint16_t)(temp / times);
}

float MQ2_Read_PPM(void)
{
    uint16_t adc_val = MQ2_Read_Average(10);
    return (float)adc_val * PPM_MAX / ADC_MAX;
}