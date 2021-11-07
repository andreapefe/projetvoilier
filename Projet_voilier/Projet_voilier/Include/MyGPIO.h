/* ************************************
Driver pour les GPIOs
Léa Sheer, Andrea Pérez et Camille Noalletas
***************************************** */

#ifndef MYGPIO_H
#define MYGPIO_H
#include "stm32f10x.h"

//Structure pour definir un GPIO
typedef struct
{
    GPIO_TypeDef*GPIO ;
    char GPIO_Pin ;//numero de 0 a 15
    char GPIO_Conf ;//  voir  c i  dessous
} MyGPIO_Struct_TypeDef ;

//Défintions des modes de configuration des ports
#define In_Floating 0x4
#define In_PullDown 0x8
#define In_PullUp 0x8
#define In_analog 0x0
#define Out_Ppull 0x1
#define Out_OD 0x6
#define AltOut_Ppull 0xB
#define AltOut_OD 0xD

//Initialiser les ports GPIO
void MyGPIO_Init ( MyGPIO_Struct_TypeDef*GPIOStructPtr );

//Lire le registre associé à un port
//Renvoie 0 ou autre  chose  different de 0 si =1
int MyGPIO_Read(GPIO_TypeDef*GPIO ,char GPIO_Pin );

//Met le port à 1 (HIGH)
void MyGPIO_Set(GPIO_TypeDef*GPIO ,char GPIO_Pin );

//Met le port à 0 (LOW)
void MyGPIO_Reset(GPIO_TypeDef*GPIO ,char GPIO_Pin );

//Change la valeur du port (1->0 ou 0->1)
void MyGPIO_Toggle(GPIO_TypeDef*GPIO ,char GPIO_Pin );

#endif
