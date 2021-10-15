/* ************************************
Driver pour les Timer avec PWM et mangement des Interruptions
L�a Sheer, Andrea P�rez et Camille Noalletas
***************************************** */

#ifndef TIMER_H
#define TIMER_H

#include "stm32f10x.h"
//Structure du Timer
typedef  struct{
    
    TIM_TypeDef*Timer ;   // TIM1 � TIM4
    unsigned shortARR;
    unsigned shortPSC;
    
}MyTimer_Struct_TypeDef ;

//Initialise le Timer avec la bonne fr�quence
void MyTimer_Base_Init ( MyTimer_Struct_TypeDef*Timer );

#define MyTimer_Base_Start(Timer)  Timer -> CR1 |= 0x01; //Allumage du Timer
#define MyTimer_Base_Stop(Timer)  Timer -> CR1 &= 0; //Arret du timer


void TIM2_IRQHandler(void); //fonction pour le pointeur 
void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);
void TIM1_UP_IRQHandler(void);

//Activation des Interruption pour le Timer consid�rer
void MyTimer_ActiveIT(TIM_TypeDef*Timer ,char Prio,void(*IT_function )(void) );

//Fonction pour metttre un channel d'un timer en mode PWM. 
void MyTimer_PWM(TIM_TypeDef*Timer, char Channel );

//Fonction pour changer le ratio


#endif
