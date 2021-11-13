/* ************************************
Driver pour comptage et transformation des angles à partir des données de la girouette
Andrea Pérez
***************************************** */
#include "MyAngles.h"


void Configuration(MyTimer_Struct_TypeDef * timer_in)
{
	MyGPIO_Struct_TypeDef port_cc1; //channel 1 de TIM2 (PA0)
	MyGPIO_Struct_TypeDef port_cc2; //channnel 2 de TIM2 (PA1)
	
	//Définition des ports et timers
	port_cc1.GPIO = GPIOA;
	port_cc1.GPIO_Pin = 0;
	port_cc1.GPIO_Conf = In_Floating;
	
	port_cc2.GPIO = GPIOA;
	port_cc2.GPIO_Pin = 1;
	port_cc2.GPIO_Conf = In_Floating;
	
	
	//Config GPIOs en output
	MyGPIO_Init(&port_cc1);
	MyGPIO_Init(&port_cc2);
	
	//Configuration du timer 2 en input capture mode
	MyTimer_Base_Init(timer_in); //initilaisation timer
	input_capture_mode(timer_in); 	//configuration
}

//Déclaration du pointeur à utiliser pour interruption
void (* pFncext) (void) ;

void EXTI15_10_IRQHandler(void)
{
	//Association du handler
	if (pFncext != 0)
   	 (*pFncext) ();
}


void Active_IT_Zero(void(*IT_function_ext )(void))
{
	MyGPIO_Struct_TypeDef port_zero; // port mise à zero	
	
	//Config du GPIO PA10 pour la mise à zero
	port_zero.GPIO = GPIOA;
	port_zero.GPIO_Pin = 10;
	port_zero.GPIO_Conf = In_Floating;
	
	//Initialisation
	MyGPIO_Init(&port_zero);
	
	//Mise en place de l'interruption sur ce pin
	RCC->APB2ENR |= 1; //Activation du clock sur AFIO
	AFIO->EXTICR[3] |= 0x0 << 8; //Selecttion GPIOA car PA10
	EXTI->IMR |= (1 << 10); //Activation du mask register pour interrupt (pin 10)
	EXTI->RTSR |= (1 << 10); //Rising edge selection 
	NVIC_SetPriority(EXTI15_10_IRQn, 1); //Selection priorité de l'interruption 
	NVIC_EnableIRQ(EXTI15_10_IRQn); //Acitve interruption sur zone 10-15
	
	pFncext = IT_function_ext; //Asocie fonction handler à celle donnée en paramètre
}

int Calibration(MyTimer_Struct_TypeDef*timer)
{
	int done = 0; 
	while(done == 0)
	{
		if ((GPIOA->IDR>>10) & 1)
		{
			timer->Timer->CNT = 0x0000;
			done = 1;
		}
	} 
	return done;
}

float ratio_moteur(int angle_vent) //valeur de l'angle entre 0 et 720 demi-degrées
{
	float angle_moteur;
	float angle_360 = angle_vent/4;
	
	//Angle à prendre par le moteur
	if (angle_360 < 180)
	{
		angle_moteur = 90 + angle_360/2; //aproximación temporal /2 (a calibrar haciendo tests??) al maximo 90 grados 
	} else if (angle_360 == 180)
	{
		angle_moteur = 179.99; //n'accepte pas les limites 0 ou 2 ms 
	}
	else
	{
		angle_moteur = (angle_360-180)/2; //aproximación temporal /2 (a calibrar haciendo tests??) al maximo -90 grados
	}
	//Correspond au ratio suivant de PWM (1ms + angle/90)/20 ms période demandé par le constructeur
	return 	((angle_moteur/90))/20; //verificar si es de 0 a 90 o de 0 a 180 (/180) y +1.5ms
}


void infos_angle(int angle)
{
	int data_allure_vent_debout[SIZE_MESSAGE]={32,32,65,76,76,85,82,69,32,58,32,86,69,78,84,32,68,69,66,79,85,84};
	int data_allure_au_pres[SIZE_MESSAGE]={32,32,65,76,76,85,82,69,32,58,32,65,85,32,80,82,69,83};
	int data_allure_bon_plein[SIZE_MESSAGE]={32,32,65,76,76,85,82,69,32,58,32,66,79,78,32,80,76,69,73,78};
	int data_allure_petit_largue[SIZE_MESSAGE]={32,32,65,76,76,85,82,69,32,58,32,80,69,84,73,84,32,76,65,82,71,85,69};
	int data_allure_travers[SIZE_MESSAGE]={32,32,65,76,76,85,82,69,32,58,32,84,82,65,86,69,82,83};
	int data_allure_largue[SIZE_MESSAGE]={32,32,65,76,76,85,82,69,32,58,32,76,65,82,71,85,69};
	int data_allure_grand_largue[SIZE_MESSAGE]={32,32,65,76,76,85,82,69,32,58,32,71,82,65,78,68,32,76,65,82,71,85,69};
	int data_allure_vent_arriere[SIZE_MESSAGE]={32,32,65,76,76,85,82,69,32,58,32,65,82,82,73,69,82,69};
	
	//Défintion des zones selon le schéma donné par le cahier de charges
	if ((0<=(float)angle/4 & angle<40) | ((float)angle/4>=320))
	{
		send(data_allure_vent_debout); 
	} 
	else if ((40<=(float)angle/4 & angle<50) | ((float)angle/4>=310 & (float)angle/4 < 320))
	{
		send(data_allure_au_pres); 
	} 
	else if ((50<=(float)angle/4 & angle<65) | ((float)angle/4>=295 & (float)angle/4 < 310))
	{
		send(data_allure_bon_plein); 
	} 
	else if ((65<=(float)angle/4 & angle<90) | ((float)angle/4>270 & (float)angle/4 <= 295))
	{
		send(data_allure_petit_largue); 
	} 
	else if ((90==(float)angle/4) | ((float)angle/4==270))
	{
		send(data_allure_travers); 
	} 
	else if ((90<(float)angle/4 & angle<115) | ((float)angle/4>245 & (float)angle/4 < 270))
	{
		send(data_allure_largue); 
	} 
	else if ((115<(float)angle/4 & angle<155) | ((float)angle/4>205 & (float)angle/4 < 245))
	{
		send(data_allure_grand_largue); 
	} 
	else if ((155<(float)angle/4 & angle<205))
	{
		send(data_allure_vent_arriere); 
	} 
}



