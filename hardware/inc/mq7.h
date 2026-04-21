#ifndef __MQ7_H
#define __MQ7_H

#include "stm32f10x.h"
#include "Delay.h"

void MQ7_ADC_Init(void);        // MQ7专用ADC初始化
u16 MQ7_Get_ADC(u8 ch);         // MQ7获取单次ADC值
u16 MQ7_Get_ADC_Average(u8 ch,u8 times); // MQ7多次采样平均
u16 MQ7_Check(void);            // MQ7传感器采集

#endif