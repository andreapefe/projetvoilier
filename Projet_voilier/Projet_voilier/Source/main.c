/* *************************************************************
Tire: Test compteur girouette pour Projet voilier 2021
Auteurs: Andrea P�rez
Description: Ce code permet de mettre en place le timer 2 pour lire l'angle de la 
girouette sur le compteur. Dans cet example les channels utilis� sont le 1 et 2 car 
c'est les seuls pour lesquels j'ai impl�ment� cette fonctionnalit�
***************************************************************** */

//librairies
#include "stm32f10x.h"
#include "MyGPIO.h"
#include "MyTimer.h"
#include "MyAngles.h"
#include "stdlib.h" //no need

u16 angle;
u16 compteur;

//Boucle principale
int main(void)
{	
	//Timer pour la girouette
	MyTimer_Struct_TypeDef timer_in; //TIM2	dans notre cas
	MyTimer_Struct_TypeDef timer_pwm; //TIM4 dans notre cas
	
	timer_in.Timer = TIM2;
	timer_in.shortARR = 719; //degr�s*2 pour un tour (mod 720 automatique) car deux channels de comptage 
	timer_in.shortPSC = 0;
	
	timer_pwm.Timer = TIM4;
	timer_pwm.shortARR = 39999; //pour avoir bonne r�solution de PWM
	timer_pwm.shortPSC = 35; 
	
	//Le clock pour les ports A et C (non n�cessaire en principe)
	RCC->APB2ENR |= 10100; 
	
	//Configuration du timer en input compare mode
	Configuration(&timer_in);
	
	//Configuration du timer 4 channel 1 en PWM PB6
	MyTimer_Base_Init(&timer_pwm);
	MyTimer_PWM(timer_pwm.Timer, 1);
	change_ratio(timer_pwm.Timer, 0.05, 1);
	
	//D�part des timers
	MyTimer_Base_Start(timer_in.Timer); 
	MyTimer_Base_Start(timer_pwm.Timer);

	
	
	//Calibration de la girouette (voir o� est le zero pour mettre le d�phasage) PA2
	Calibration(&timer_in); //to test
	
	
	do{
		
		compteur = timer_pwm.Timer->CNT; //r�cup�re valeur de l'angle entre 0 et 720
		//GPIOB->ODR |= (GPIOB->IDR & (0x1 << 6)) ; non n�cessaire
		angle = GPIOB->ODR;
		change_ratio(timer_pwm.Timer, ratio_moteur(angle), 1); //change l'angle du servo selon la valeur de l'angle 
		
	}while(1);
	
}
 