#include "ADC.h"
#include "stm32f10x.h"

void MyADC_Init (ADC_TypeDef * ADC) 
{
    
	if (ADC == ADC1) {
		(RCC->APB2ENR)|= RCC_APB2ENR_ADC1EN;
		// freq(ADC) < 14MHz donc fclck(72MHz) / 6 = 12MHz 
	    RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6;

		// Active ADC1 
		ADC1->CR2 |= (ADC_CR2_ADON);
	}
	if (ADC == ADC2) {
		(RCC->APB2ENR)|= RCC_APB2ENR_ADC2EN;
		// freq(ADC) < 14MHz donc fclck(72MHz) / 6 = 12MHz 
	    RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6;
		
		// Active ADC2
		ADC2->CR2 |= (ADC_CR2_ADON);
	}
	
}

void Config_ADC(ADC_TypeDef * ADC, u8 Channel) 
{
    // Initialisation du registre EOC
		ADC->SR &= ~(ADC_SR_EOC);

    //Choix d'une seule conversion
    ADC->SQR1 &= ADC_SQR1_L; 

    //Choix de la voie
    ADC->SQR3 |= Channel;

}

int Val_ADC(ADC_TypeDef * ADC) 
{
	//On lance la conversion
    ADC->CR2 |= (ADC_CR2_ADON); // A/D CONVERTER ON
    
    //On attend que la conversion se termine
    while (!(ADC->SR & ADC_SR_EOC)){}
			
		//la conversion est effectué
		ADC->SR &= ~ADC_SR_EOC; 
			
	 //retour de la conversion
		return ADC->DR & ~((0x0F) << 12); //12 premiers bit du registre à lire
}