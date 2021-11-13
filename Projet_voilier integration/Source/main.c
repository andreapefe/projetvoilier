/* *************************************************************
Tire: Projet voilier 2021
Auteurs: Zaouali Mazin, L�a Scheer, Cl�ment Sevillano et Andrea P�rez, 
Description: Ce code permet de mettre en place le timer 2 pour lire l'angle de la 
girouette sur le compteur. Dans cet example les channels utilis� sont le 1 et 2 car 
c'est les seuls pour lesquels j'ai impl�ment� cette fonctionnalit�
***************************************************************** */

//librairies
#include "stm32f10x.h"
#include "MyGPIO.h"
#include "MyTimer.h"
#include "MyAngles.h"
#include "AntiChavirement.h" 
#include "direction.h" 
#include "plateau.h"

//Varibles globales pour le SPI
uint8_t RxData[6]; //Reception data
uint16_t x,y,z; //valeurs axes
float	x_acc,y_acc,z_acc; //valeurs axes
uint8_t dataoff[1];

//Variables pour surveiller en mode simul�
float ratio;
float result;

//Messages en format ASCII
int data_chavirement[SIZE_MESSAGE]={32,32,67,72,65,86,73,82,69,77,69,78,84,32,68,69,84,69,67,84,69};

//Gestion de l'interruption pour remettre � zero la girouette
void Remise_zero(void)
{
	if ((EXTI->PR) & EXTI_PR_PR10) 
	{		
		TIM1->CNT = 0x0000;
		EXTI->PR |= (EXTI_PR_PR10); //mise en pending (en cours de traitement)
	}
}


//Programme principale
int main(void)
 {
	//Variables auxiliaires
	int angle;
	int i;
	
	/**** Configuration ******/
	 
	//Timers et GPIO pour la girouette
	MyTimer_Struct_TypeDef timer_in; //TIM2	dans notre cas
	MyTimer_Struct_TypeDef timer_pwm; //TIM4 dans notre cas
	 
	//Timer PWM plateau
	MyTimer_Struct_TypeDef plateau;
	 
	//Timer pour delay
	MyTimer_Struct_TypeDef delay;
	
	//Configuration des timers 
	timer_in.Timer = TIM_GIROUETTE; //correspond au TIM1
	timer_in.shortARR = ANGLE_MAX; //degr�s*4 pour un tour (mod 720 automatique) car deux channels de comptage 
	timer_in.shortPSC = 0;
	
	timer_pwm.Timer = TIM_PWM_VOILES; //correspond au TIM4
	timer_pwm.shortARR = ARR_PWM_20MS; //pour avoir bonne r�solution de PWM � 20 ms
	timer_pwm.shortPSC = PSC_PWM_20MS; 
	 
	delay.Timer = TIM_DELAY; 
	delay.shortARR = ARR_1MS; //pour avoir une p�riode de 1 us
	delay.shortPSC = PSC_1MS;
	
	plateau.Timer = TIM_PWM_100MS;
	plateau.shortARR = ARR_PWM_100MS; //Bonne r�solution PWM � 100 ms
	plateau.shortPSC = PSC_PWM_100MS;
	
	//Config GPIOs pour la communication USART
	GPIO_TX_Config();
	GPIO_RX_Config();
	
	//Configuration du timer 1 pour girouette en encoder input mode
	Configuration(&timer_in);
	
	//Configuration du SPI
	GPIO_Config(); //Initialise GPIO
	SPI_Config(); 
	SPI_Enable();
	adxl345_init(); //Config adx pour communication
	//Mise � z�ro??
	adxl345_write(OFSX,0); //mise � z�ro
	adxl345_write(OFSY,0); //mise � z�ro
	adxl345_write(OFSZ,0); //mise � z�ro
	adxl345_read(OFSZ, dataoff);
	
	//Configuration du timer 4 channel 1 en PWM (PB6 servo voiles)
	MyTimer_Base_Init(&timer_pwm);
	MyTimer_PWM(timer_pwm.Timer, 1);
	change_ratio(timer_pwm.Timer, REPOS_VOILES, 1);
	
	//Configuration du timer 3 pour le delay
	MyTimer_Base_Init(&delay);
	
	//Configuration timer 2 vitesse plateau en PWM (CH1 - PA0 moteur)
	MyTimer_Base_Init(&plateau);
	MyTimer_PWM(plateau.Timer, 1);
	
	//Configuration du GPIO PB8 pour le signe
	Config_GPIOsigne();
	
	//Configuration de l'USART 3 en �metteur et r�cepteur
	configure_usart3_9600bps();
	configuration_usart3_9600_receive ();	
	
	//D�part des timers 
	MyTimer_Base_Start(timer_pwm.Timer);
	MyTimer_Base_Start(timer_in.Timer);
	MyTimer_Base_Start(delay.Timer);
	MyTimer_Base_Start(plateau.Timer);
	
	//Activation de l'interruption sur le pin PA10 (voir o� est le zero pour mettre le d�phasage) PA2
	Active_IT_Zero(&Remise_zero);
	
	//Retard pour garantir mise en place
	Delay_ms(100);
	
	
	/***** Entr�e dans la boucle infinie ********/
	
	do{
		//Boucle 10 fois pour faire environ 3 secondes
		for (i=0; i<10; i++)
		{
			//Check anti-chavirement (F4) -> faire une seule fonction?
			adxl345_read(DATAX0,RxData);
			x = ((RxData[1]<<8)|RxData[0]); // DATA X0, X1
			y = ((RxData[3]<<8)|RxData[2]); // DATA Y0, Y1
			z = ((RxData[5]<<8)|RxData[4]); // DATA Z0, Z1

			// Scale Factor for Xout, Yout and Zout is 7.8 mg/LSB for a +-4g, 10-bit resolution
			// ==> multiply by 0.0078 to get real acceleration values in g
			
			x_acc= x * 0.0078; 
			y_acc= y * 0.0078;
			z_acc= z * 0.0078 -1; //-1 � mettre??
			result = acos(z_acc)*180/(atan(1)*4);
			
			//Communication USART pour vitesse (F2)
			Plateau();
			
			Delay_ms(200);
			
			if (acos(z_acc)*180/(atan(1)*4)>= 40)
			{
				//Anti-chavirement
				change_ratio(timer_pwm.Timer, 0.05, 1); //repos des voiles (on les mets � 0�)
				//Alerte de chavirement
				send(data_chavirement);
			} 
			else 
			{	
				//Bordage des voiles automatique (F1) en boucle pour environ 100 ms
				for (i=0; i<5; i++)
				{
					angle = timer_in.Timer->CNT; //r�cup�re valeur de l'angle entre 0 et 720
					ratio = ratio_moteur(angle); //variable utilis� pour supervision en debug
					change_ratio(timer_pwm.Timer, ratio_moteur(angle), 1); //change l'angle du servo selon la valeur de l'angle
					Delay_ms(20);
				}
			}
		}
		
		//Communication infos angle de vent(F3) tous les 3 secondes
		angle = (timer_in.Timer->CNT); 
		infos_angle(angle);
						
	}while(1);
	
}
 
