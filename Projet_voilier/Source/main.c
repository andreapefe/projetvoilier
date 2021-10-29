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
#include "MyAngles.h"
#include "stdlib.h" //no need

u16 angle;

//Boucle principale
int main(void)
{	
	//Timer pour la girouette
	MyTimer_Struct_TypeDef timer_in; //TIM2	
	
	timer_in.Timer = TIM2;
	timer_in.shortARR = 359; //degrés un tour (mod 360 automatique)
	timer_in.shortPSC = 0;
	
	//Le clock pour les ports A et C
	RCC->APB2ENR |= 10100; 
	
	//Configuration du timer
	Configuration(&timer_in);
	
	//Départ du timer
	MyTimer_Base_Start(timer_in.Timer); //

	//Calibration
	Calibration(&timer_in);
	
	do{
		
		angle = timer_in.Timer->CNT; //ne fais rien
		
	}while(1);
	
}
 