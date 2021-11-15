/* ************************************
Titre : 			Driver pour Vitesse du plateau (F2)

Auteur: 			Clément Sevillano, Léa Scheer et Andrea Pérez 

Description: 	Code pour communication entre moteur et télécommande en utilisant le driver pournUSART
***************************************** */
#ifndef PLATEAU
#define PLATEAU

#include "MyGPIO.h"
#include "stdlib.h"
#include "stm32f10x.h"
#include "MyTimer.h"
#include "math.h"
#include "Antichavirement.h"
#include "direction.h"

#define TIM_PWM_100MS TIM2
#define ARR_PWM_100MS 9999
#define PSC_PWM_100MS 719

//Configure pin PB8 pour le signe
void Config_GPIOsigne(void);

//Gère la vitesse du plateau selon la commande envoyé par USART3
void Plateau(void);


#endif
