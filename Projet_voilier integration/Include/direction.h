#ifndef DIRECTION
#define DIRECTION

#include "MyGPIO.h"
#include "stdlib.h"
#include "stm32f10x.h"
#include "MyTimer.h"
#include "math.h"


void configure_usart1_9600bps();

void configure_usart3_9600bps();

void send(char data);

void alerte(int donne);

int configure_gpio_alternate_push_pull(GPIO_TypeDef *gpio, int pin);

void configuration_usart3_9600_receive ();

int receive (void);

int vitesse(void);
	
void signe(int vitesse) ;

void Plateau(void);

#endif