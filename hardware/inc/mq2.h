#ifndef __MQ2_H
#define __MQ2_H

#include "stm32f10x.h"

void MQ2_Init(void);
uint16_t MQ2_Read_ADC(void);
uint16_t MQ2_Read_Average(uint8_t times);
float MQ2_Read_PPM(void);

#endif