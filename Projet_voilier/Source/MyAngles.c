/* ************************************
Driver pour comptage et transformation des angles à partir des données de la girouette
Andrea Pérez
***************************************** */
#include "MyAngles.h"

void Configuration(MyTimer_Struct_TypeDef * timer_in)
{
	MyGPIO_Struct_TypeDef port_cc1; //channel 1 de TIM2 (PA0)
	MyGPIO_Struct_TypeDef port_cc2; //channnel 2 de TIM2 (PA1)
	
	//Définition des ports et timers
	port_cc1.GPIO = GPIOA;
	port_cc1.GPIO_Pin = 0;
	port_cc1.GPIO_Conf = In_Floating;
	
	port_cc2.GPIO = GPIOA;
	port_cc2.GPIO_Pin = 1;
	port_cc2.GPIO_Conf = In_Floating;
	
	
	//Config GPIOs en output
	MyGPIO_Init(&port_cc1);
	MyGPIO_Init(&port_cc2);
	
	//Configuration du timer 2 en input capture mode
	MyTimer_Base_Init(timer_in); //initilaisation timeer
	input_capture_mode(timer_in); 	//configuration
}

int Calibration(MyTimer_Struct_TypeDef*timer)
{
	int done = 0;
	MyGPIO_Struct_TypeDef port_zero;
	
	port_zero.GPIO = GPIOA;
	port_zero.GPIO_Pin = 2;
	port_zero.GPIO_Conf = In_Floating;
	
	MyGPIO_Init(&port_zero);
	while ((port_zero.GPIO ->ODR >> 2) != 1)
	{	}
	timer->Timer->CNT = 0x0000;	
	done = 1;
	return done;
}



