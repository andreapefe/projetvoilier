/* ************************************
Driver pour comptage et transformation des angles à partir des données de la girouette
Andrea Pérez
***************************************** */

#ifndef ANGLES
#define ANGLES

#include "stm32f10x.h"
#include "MyGPIO.h"
#include "MyTimer.h"

//Configuration des 3 pins
void Configuration(MyTimer_Struct_TypeDef * timer_in);
	
//Calibration renvoi 1 quand elle a été réussi 0 si non
int Calibration(MyTimer_Struct_TypeDef*timer);  

//Angle à prendre par le servo moteur (% de signal)
int angle_moteur(void);

#endif