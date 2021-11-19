#ifndef BATTERIE_H
#define BATTERIE_H

#include "MyGPIO.h"
#include "ADC.h"

// Configure PB0 en analog input et initialise l'ADC
void MyBatterie_Init(void);

// Valeur de la conversion
int ADC_Batterie_Charge(void);

// Tension batterie
uint16_t Tension_Batt(void);

// Pourcentage batterie restant
uint16_t Pourcentage_Batt(void);


int *ascii_to_numbers(int pourcentage);

int intToAscii(int number);

#endif

