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

u16 angle1;
u16 angle2;
u16 angle3;
u16 angle4; 

u16 out1;
u16 out2;
u16 out3;
u16 out4;


//Boucle principale
int main(void)
{

	MyTimer_Struct_TypeDef Timer3 = {TIM3, 0x4E1F, 0x6B};
	MyTimer_Struct_TypeDef Timer2 = {TIM2, 0x4E1F, 0x6B};
	MyTimer_Struct_TypeDef Timer1 = {TIM1, 0x4E1F, 0x6B};
	MyTimer_Struct_TypeDef Timer4 = {TIM4, 0x4E1F, 0x6B};
	
	MyTimer_Base_Init(&Timer3);
	MyTimer_Base_Init(&Timer1);
	MyTimer_Base_Init(&Timer2);
	MyTimer_Base_Init(&Timer4);
	
	MyTimer_PWM(TIM3, 1);
	MyTimer_PWM(TIM1, 4);
	MyTimer_PWM(TIM2, 2);
	MyTimer_PWM(TIM4, 3);
	TIM3->CCR1 |= 0x03E8; //set ratio 1
	change_ratio(TIM3, 0.3, 1);
	change_ratio(TIM1, 0.4, 4);
	change_ratio(TIM4, 0.7, 3);
	change_ratio(TIM2, 0.8, 2);
	
	MyTimer_Base_Start(TIM3);
	MyTimer_Base_Start(TIM1);
	MyTimer_Base_Start(TIM2);
	MyTimer_Base_Start(TIM4);
	
	do{
		
		angle3 = TIM3->CNT; //ne fais rien
		angle4 = TIM4->CNT; //ne fais rien
		angle1 = TIM1->CNT; //ne fais rien
		angle2 = TIM2->CNT; //ne fais rien
		out1 = GPIOA -> IDR & (0x0800) ; //11
		out2 = GPIOA -> IDR & (0x0002); //1
		out3 = GPIOA -> IDR & (0x0040); //6
		out4 = GPIOB -> IDR & (0x0100); //8
		
	}while(1);
	
}
 