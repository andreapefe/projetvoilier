/* ************************************
Titre : 			Driver pour brodage automatique des voiles (F1)

Auteur: 			Andrea Pérez

Description: 	Code pour compter l'angle indiquer par la girouette (encoder mode sur un timer)
							Mise en place d'une interruption externe pour la mise à zéro du compteur sur PA10
							Calcul de l'angle à prendre par le servo selon angle de la girouette
***************************************** */

#ifndef ANGLES
#define ANGLES

#include "stm32f10x.h"
#include "MyGPIO.h"
#include "MyTimer.h"
#include "direction.h"

#define ANGLE_MAX 1439
#define ARR_PWM_20MS 39999 
#define PSC_PWM_20MS 35
#define TIM_GIROUETTE TIM1
#define TIM_PWM_VOILES TIM4
#define REPOS_VOILES 0.05 //valeur PWM pour mettre les voiles en repos

//Configuration des 3 pins
void Configuration(MyTimer_Struct_TypeDef * timer_in);

//Association des pointeurs des fonctions aux routines correspondantes
void EXTI15_10_IRQHandler(void);

//Activation de l'interruption pour mettre à zero la girouette sur le pin A10
void Active_IT_Zero(void(*IT_function )(void));
	
//Calibration renvoi 1 quand elle a été réussi 0 si non (ancienne fonction)
int Calibration(MyTimer_Struct_TypeDef*timer);  

//A partir de l'angle du vend rend l'angle à prendre par le servo moteur (en ratio de 1)
float ratio_moteur(int angle_vent);

//Prend l'angle de la girouette et indique ou nous sommes
void infos_angle(int * anglep);

#endif
