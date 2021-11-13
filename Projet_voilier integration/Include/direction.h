/* ************************************
Titre : 			Driver pour transimssion d'informations par USART et vitesse du plateau (F3)

Auteur: 			Clément Sevillano et Léa Scheer

Description: 	Code pour mise en place de l'USART et transmission/reception
							Gestion de la vitesse du plateau
***************************************** */
#ifndef DIRECTION
#define DIRECTION

#include "MyGPIO.h"
#include "stdlib.h"
#include "stm32f10x.h"
#include "MyTimer.h"
#include "math.h"
#include "Antichavirement.h"

#define SIZE_MESSAGE 24 //Taille max des messages qu'on envoi par USART 3 

//Configure GPIO PB10 pour transmission (AltOut_Ppull)
void GPIO_TX_Config(void);

//Configure GPIO PB11 pour reception (In_Floating)
void GPIO_RX_Config(void);

//Configure le USART 1 en transmission
void configure_usart1_9600bps(void);

//Configure USART 3 en transmission
void configure_usart3_9600bps(void);

//envoi d'un seul carctère avec USART 3
void alerte(int donne);

//envoi un groupement de bit avec USART 3
void send(int * data);

//Configure USART 3 en reception
void configuration_usart3_9600_receive (void);

// Reçois les infos envoyées par USART 
int receive (void);



#endif
