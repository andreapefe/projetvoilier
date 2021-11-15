#include "plateau.h"

void Config_GPIOsigne(void)
{
	//GPIOs pour signe plateau
	MyGPIO_Struct_TypeDef gpio_signe;
	
	//Config GPIO  PB8 pour le signe du plateau
	gpio_signe.GPIO_Conf=Out_Ppull;
	gpio_signe.GPIO=GPIOB;
	gpio_signe.GPIO_Pin=8;
	MyGPIO_Init (&gpio_signe); 
}

//commande du moteur du plateau
void Plateau(){
	//Reception d'une valeur entre -100 et 100
	signed char cycle=receive();
	
	float ratio = 0;
	if (cycle >=0){
		MyGPIO_Reset(GPIOB,8); //on met à zero sens normal
	}
	else{
		MyGPIO_Set(GPIOB,8); //on met à 1 car sens inverse
		cycle=-cycle; //pour avoir une valeur > 0
	}
	//vitesse du plateau
	ratio = (float) cycle/100; //calcule du ratio
	change_ratio(TIM2, (ratio), 1); //affecte ratio PWM
	}
