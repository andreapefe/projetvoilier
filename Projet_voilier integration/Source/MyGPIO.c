#include "stm32f10x.h"
#include "MyGPIO.h"

void MyGPIO_Init ( MyGPIO_Struct_TypeDef*GPIOStructPtr)
{
		//Activation des clocks selon la zone de GPIOs
		if (GPIOStructPtr->GPIO == GPIOA)
		{
			RCC->APB2ENR |= (1<<2);
		}
		else if (GPIOStructPtr->GPIO == GPIOB)
		{
			RCC->APB2ENR |= (1<<3);
		}
		else if (GPIOStructPtr->GPIO == GPIOC)
		{
			RCC->APB2ENR |= (1<<4);
		}
		
    //Selon le numéro de PIN on va sur les registres CRH ou CRL 
    if (GPIOStructPtr->GPIO_Pin > 7)
    {    
   	 GPIOStructPtr->GPIO->CRH &= ~(0xF << (GPIOStructPtr->GPIO_Pin-8)*4); //Masque
   	 GPIOStructPtr->GPIO->CRH |= (GPIOStructPtr->GPIO_Conf << (GPIOStructPtr->GPIO_Pin-8)*4); //On active le Pin avec la bonne configuration
    }else
    {
   	 GPIOStructPtr->GPIO->CRL &= ~(0xF << GPIOStructPtr->GPIO_Pin*4);
   	 GPIOStructPtr->GPIO->CRL |= (GPIOStructPtr->GPIO_Conf << GPIOStructPtr->GPIO_Pin*4);
    }
}

int MyGPIO_Read(GPIO_TypeDef*GPIO ,char GPIO_Pin) //  renvoie 0 ou autre  chose different  de 0
{
    return (GPIO->IDR) & (0x1 << GPIO_Pin);
}


void MyGPIO_Set(GPIO_TypeDef*GPIO ,char GPIO_Pin)
{
    GPIO -> ODR |= (0x1 << GPIO_Pin);
}


void MyGPIO_Reset(GPIO_TypeDef*GPIO ,char GPIO_Pin)
{
    GPIO -> BRR |= (0x1 << GPIO_Pin);
}


void MyGPIO_Toggle(GPIO_TypeDef*GPIO ,char GPIO_Pin)
{
    GPIO -> ODR ^= (0x1 << GPIO_Pin); //XOR Exclusif pour le changement
}
