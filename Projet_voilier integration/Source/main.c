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
#include "AntiChavirement.h" 
#include "direction.h" 

u16 pwm_out;
u16 compteur;

//Varibles globales pour le SPI
uint8_t RxData[6]; //Reception data
uint16_t x,y,z; //valeurs axes
float	x_acc,y_acc,z_acc; //valeurs axes
float ratio;
float result;
uint8_t dataoff[1];
//Messages
int data_chavirement[]={32,32,67,72,65,86,73,82,69,77,69,78,84,32,68,69,84,69,67,84,69};

//Gestion de l'interruption pour remettre à zero la girouette
void Remise_zero(void)
{
	if ((EXTI->PR) & EXTI_PR_PR10) 
	{		
		TIM1->CNT = 0x0000;
		EXTI->PR |= (EXTI_PR_PR10); //mise en pending (en cours de traitement)
	}
}


//Boucle principale
int main(void)
 {	
	int angle, done;
	int i;
	
	//Timers pour la girouette
	MyTimer_Struct_TypeDef timer_in; //TIM2	dans notre cas
	MyTimer_Struct_TypeDef timer_pwm; //TIM4 dans notre cas
	MyGPIO_Struct_TypeDef port_zero; // port PA10	
	 
	//Timer PWM plateau
	MyTimer_Struct_TypeDef plateau;
	 
	//Timer pour delay
	MyTimer_Struct_TypeDef delay;
	 
	//GPIOs pour tx, rx usart et signe plateau
	MyGPIO_Struct_TypeDef gpio_reception;
	MyGPIO_Struct_TypeDef gpio_transmission;
	MyGPIO_Struct_TypeDef gpio_signe;
	
	//Configuration des timers 
	timer_in.Timer = TIM1;
	timer_in.shortARR = 1439; //degrés*4 pour un tour (mod 720 automatique) car deux channels de comptage 
	timer_in.shortPSC = 0;
	
	timer_pwm.Timer = TIM4;
	timer_pwm.shortARR = 39999; //pour avoir bonne résolution de PWM à 20 ms
	timer_pwm.shortPSC = 35; 
	 
	delay.Timer = TIM3; //pour avoir une période de 1 us
	delay.shortARR = 71;
	delay.shortPSC = 0;
	
	plateau.Timer = TIM2;
	plateau.shortARR = 9999; //Bonne résolution PWM à 100 ms
	plateau.shortPSC = 719;
	
	//Le clock pour les ports A et C (non nécessaire en principe)
	//RCC->APB2ENR |= 10100; 
	
	//Config GPIOs pour la communication USART
	gpio_reception.GPIO_Conf=In_Floating;
	gpio_reception.GPIO=GPIOB;
	gpio_reception.GPIO_Pin=11;
	MyGPIO_Init (&gpio_reception);
	
	gpio_transmission.GPIO_Conf=AltOut_Ppull;
	gpio_transmission.GPIO=GPIOB;
	gpio_transmission.GPIO_Pin=10;
	MyGPIO_Init (&gpio_transmission);
	
	//Config GPIO pour le signe du plateau
	gpio_signe.GPIO_Conf=Out_Ppull;
	gpio_signe.GPIO=GPIOB;
	gpio_signe.GPIO_Pin=8;
	MyGPIO_Init (&gpio_signe); 
	
	//Configuration du timer 1 girouette en input compare mode
	Configuration(&timer_in);
	
	//Configuration du SPI
	GPIO_Config(); //Initialise GPIO
	SPI_Config(); 
	SPI_Enable();
	adxl345_init(); //Config adx pour communication
	//Mise à zéro??
	adxl345_write(OFSX,0); //mise à zéro
	adxl345_write(OFSY,0); //mise à zéro
	adxl345_write(OFSZ,0); //mise à zéro
	adxl345_read(OFSZ, dataoff);
	
	
	//Configuration du timer 4 channel 1 en PWM (PB6 servo voiles)
	MyTimer_Base_Init(&timer_pwm);
	MyTimer_PWM(timer_pwm.Timer, 1);
	change_ratio(timer_pwm.Timer, 0.05, 1);
	
	//Configuration du timer 3 pour le delay
	MyTimer_Base_Init(&delay);
	
	//Configuration timer 2 vitesse plateau en PWM (PA0 moteur)
	MyTimer_Base_Init(&plateau);
	MyTimer_PWM(plateau.Timer, 1);
	
	//Configuration de l'USART 3 en émetteur et récepteur
	configure_usart3_9600bps();
	configuration_usart3_9600_receive ();	
	
	//Départ des timers 
	MyTimer_Base_Start(timer_pwm.Timer);
	MyTimer_Base_Start(timer_in.Timer);
	MyTimer_Base_Start(delay.Timer);
	MyTimer_Base_Start(plateau.Timer);
	
	//Mise à zero de la girouette 
	//Config du GPIO pour la mise à zero
	port_zero.GPIO = GPIOA;
	port_zero.GPIO_Pin = 10;
	port_zero.GPIO_Conf = In_Floating;
	MyGPIO_Init(&port_zero);
	//Activation de l'interruption sur le pin PA10 (voir où est le zero pour mettre le déphasage) PA2
	Active_IT_Zero(&Remise_zero);
	
	//Calibration(&timer_in); autre forme de faire moins propre
	
	Delay_ms(100);
	//MyGPIO_Set(gpio_signe.GPIO, gpio_signe.GPIO_Pin);
	
	
	do{
		
		//Check anti-chavirement (F4)
		adxl345_read(DATAX0,RxData);
		x = ((RxData[1]<<8)|RxData[0]); // DATA X0, X1
		y = ((RxData[3]<<8)|RxData[2]); // DATA Y0, Y1
		z = ((RxData[5]<<8)|RxData[4]); // DATA Z0, Z1

		
		// Scale Factor for Xout, Yout and Zout is 7.8 mg/LSB for a +-4g, 10-bit resolution
		// ==> multiply by 0.0078 to get real acceleration values in g
		
		x_acc= x * 0.0078; 
		y_acc= y * 0.0078;
		z_acc= z * 0.0078 -1; //-1 à mettre??
		result = acos(z_acc)*180/(atan(1)*4);
		
		//Communciation USART pour vitesse (F2)
		Plateau();
		Delay_ms(300);
		
		if (acos(z_acc)*180/(atan(1)*4)>= 40){
			change_ratio(timer_pwm.Timer, 0.05, 1); //repos des voiles (on les mets à 0º)
			//Alerte de chavirement
			alerte(10);
			for(i=0; i<sizeof(data_chavirement);i++){
				Delay_ms(50);
				alerte(data_chavirement[i]);
			}
		} else {	
			
			//Bordage des voiles automatique (F1)
			for (i=0; i<5; i++){
				//Bordage des voiles (F1)
				angle = timer_in.Timer->CNT; //récupère valeur de l'angle entre 0 et 720
				ratio = ratio_moteur(angle); //variable utilisé pour supervision en debug
				change_ratio(timer_pwm.Timer, ratio_moteur(angle), 1); //change l'angle du servo selon la valeur de l'angle
				Delay_ms(20);
			}
		}
				
		
	}while(1);
	
}
 
