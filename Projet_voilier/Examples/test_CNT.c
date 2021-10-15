/* *************************************************************
Tire: Test compteur girouette pour Projet voilier 2021
Auteurs: Andrea Pérez
Description: Ce code permet de mettre en place le timer 2 pour lire l'angle de la 
girouette sur le compteur. Dans cet example les channels utilisé sont le 1 et 2 car 
c'est les seuls pour lesquels j'ai implémenté cette fonctionnalité
***************************************************************** */

//librairies
#include "stm32f10x.h"
#include "MyGPIO.h"
#include "MyTimer.h"
#include "stdlib.h" //no need

u16 angle;

//Boucle principale
int main(void)
{
	//Déclaration structures des ports  timer
	MyGPIO_Struct_TypeDef port_cc1; //channel 1 de TIM2 (PA0)
	MyGPIO_Struct_TypeDef port_cc2; //channnel 2 de TIM2 (PA1)
	MyTimer_Struct_TypeDef timer_in; //TIM2
	
	//Définition des ports et timers
	port_cc1.GPIO = GPIOA;
	port_cc1.GPIO_Pin = 0;
	port_cc1.GPIO_Conf = In_Floating;
	
	port_cc2.GPIO = GPIOA;
	port_cc2.GPIO_Pin = 1;
	port_cc2.GPIO_Conf = In_Floating;
	
	timer_in.Timer = TIM2;
	timer_in.shortARR = 359; //degrés un tour (mod 360 automatique)
	timer_in.shortPSC = 0;
	
	//Le clock pour les ports A et C
	RCC->APB2ENR |= 10100; 
	
	//Configuration du timer 2 en input capture mode
	MyTimer_Base_Init(&timer_in); //initilaisation timeer
	input_capture_mode(&timer_in); 	//configuration
	
	//Départ du timer
	MyTimer_Base_Start(timer_in.Timer); //
	//MyGPIO_Init(&port_led);
	//MyGPIO_Init(&port_but);
	//timer_in.Timer->DIER |= (1 << 10); //active DMA 
	
	
	do{
		
		angle = timer_in.Timer->CNT; //ne fais rien
		
	}while(1);
	
}
 