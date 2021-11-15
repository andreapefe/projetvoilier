#include "direction.h"
#include "MyTimer.h"

void GPIO_TX_Config(void) {
		MyGPIO_Struct_TypeDef gpio_transmission;
		
		//Définiton du port PB10 (sans remap)
		gpio_transmission.GPIO_Conf=AltOut_Ppull;
		gpio_transmission.GPIO=GPIOB;
		gpio_transmission.GPIO_Pin=10;
		
		//Initialisation clock et config 
		MyGPIO_Init (&gpio_transmission);
}

void GPIO_RX_Config(void) {
		MyGPIO_Struct_TypeDef gpio_reception;
		
		//Définiton du port PB11 (sans remap)
		gpio_reception.GPIO_Conf=In_Floating;
		gpio_reception.GPIO=GPIOB;
		gpio_reception.GPIO_Pin=11;
	
		//Initialisation clock et config 
		MyGPIO_Init (&gpio_reception);
}



void configure_usart1_9600bps(void){
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;// validation horloge USART1
    USART1->CR1 |= USART_CR1_UE; // Activation de l'USART
    USART1->CR1 &= ~USART_CR1_M; // Choix d'une taille de 8 bits de données
    USART1->CR2 &= USART_CR2_STOP; // Choix d'un seul bit de stop
    USART1->BRR |= 36000000/9600; // Fixe le baud rate à 9600bps
    //USART1->BRR |= 75; // Fixe le baud rate à 9600bps
    USART1->CR1 |= USART_CR1_TE; // Envoi de la première trame d'attente
}

void configure_usart3_9600bps(void){
		
		//Configuration de l'usart
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;// validation horloge USART1
    USART3->CR1 |= USART_CR1_UE; // Activation de l'USART
    USART3->CR1 &= ~USART_CR1_M; // Choix d'une taille de 8 bits de données
    USART3->CR2 &= USART_CR2_STOP; // Choix d'un seul bit de stop
    USART3->BRR |= 36000000/9600; // Fixe le baud rate à 9600bps à changer!!!!!!!!!!!!!!!!! 36.000.000/9600 DIRECT
    //USART1->BRR |= 75; // Fixe le baud rate à 9600bps 
    USART3->CR1 |= USART_CR1_TE; // Envoi de la première trame d'attente
}



void alerte(int donne){
    USART3->DR = donne; // Ecriture de la donnée dans le registre DR
    while(!(USART3->SR & USART_SR_TC)) {} // Attente de la fin de transmission
}

void send(int * data){
		int i = 0;
		alerte(10); //saut de ligne pour écrire
		for(i=0; i<SIZE_MESSAGE; i++){
			Delay_ms(20);
			alerte(data[i]);
		}
}


void configuration_usart3_9600_receive (void){
	
	 USART3->CR1 |= USART_CR1_UE;
	 USART3->CR1 &= ~USART_CR1_M;
	 USART3->CR2 &= USART_CR2_STOP;
	 USART3->CR1 |= USART_CR1_RE; //activation réception
}


	
int receive (void)
{
  while (!(USART3->SR & (USART_SR_RXNE)));
	// Wait for RXNE to SET.. This indicates that the data has been Received  // Read the data. 
	return USART3->DR;
}

