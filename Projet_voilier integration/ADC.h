#ifndef ADC_H
#define ADC_H

#include "stm32f10x.h"

// Initialise l'ADC
void MyADC_Init (ADC_TypeDef * ADC);

// Configure l'ADC
void Config_ADC(ADC_TypeDef * ADC, u8 Channel);

// Valeur de la conversion
int Val_ADC(ADC_TypeDef * ADC);







#endif