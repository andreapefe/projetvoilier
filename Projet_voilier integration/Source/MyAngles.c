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

void (* pFncext) (void) ;

void EXTI15_10_IRQHandler(void)
{
	if (pFncext != 0)
   	 (*pFncext) ();
}


void Active_IT_Zero(void(*IT_function_ext )(void))
{
	RCC->APB2ENR |= 1; //Enable clock of AFIO
	AFIO->EXTICR[3] |= 0x0 << 8; //Select GPIOA car PA10
	EXTI->IMR |= (1 << 10); //Activation of the mask register for interrupt (pin 10)
	EXTI->RTSR |= (1 << 10); //Rising edge selection 
	NVIC_SetPriority(EXTI15_10_IRQn, 1); //Set priority of the interruption 
	NVIC_EnableIRQ(EXTI15_10_IRQn); //Enable interruption on area 10-15
	
	pFncext = IT_function_ext; 
}

int Calibration(MyTimer_Struct_TypeDef*timer)
{
	int done = 0; 
	while(done == 0)
	{
		if ((GPIOA->IDR>>10) & 1)
		{
			timer->Timer->CNT = 0x0000;
			done = 1;
		}
	} 
	return done;
}

float ratio_moteur(int angle_vent) //valeur de l'angle entre 0 et 720 demi-degrées
{
	float angle_moteur;
	float angle_360 = angle_vent/4;
	
	//Angle à prendre par le moteur
	if (angle_360 < 180)
	{
		angle_moteur = 90 + angle_360/2; //aproximación temporal /2 (a calibrar haciendo tests??) al maximo 90 grados 
	} else if (angle_360 == 180)
	{
		angle_moteur = 179.99;
	}
	else
	{
		angle_moteur = (angle_360-180)/2; //aproximación temporal /2 (a calibrar haciendo tests??) al maximo -90 grados
	}
	
	//Correspond au ratio suivant de PWM (1ms + angle/90)/20 ms période demandé par le constructeur
	return 	((angle_moteur/90))/20; //verificar si es de 0 a 90 o de 0 a 180 (/180) y +1.5ms
}





