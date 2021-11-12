#include "direction.h"
#include "MyTimer.h"

void configure_usart1_9600bps(){
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;// validation horloge USART1
    USART1->CR1 |= USART_CR1_UE; // Activation de l'USART
    USART1->CR1 &= ~USART_CR1_M; // Choix d'une taille de 8 bits de données
    USART1->CR2 &= USART_CR2_STOP; // Choix d'un seul bit de stop
    USART1->BRR |= 36000000/9600; // Fixe le baud rate à 9600bps
    //USART1->BRR |= 75; // Fixe le baud rate à 9600bps
    USART1->CR1 |= USART_CR1_TE; // Envoi de la première trame d'attente
}

void configure_usart3_9600bps(){
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;// validation horloge USART1
    USART3->CR1 |= USART_CR1_UE; // Activation de l'USART
    USART3->CR1 &= ~USART_CR1_M; // Choix d'une taille de 8 bits de données
    USART3->CR2 &= USART_CR2_STOP; // Choix d'un seul bit de stop
    USART3->BRR |= 36000000/9600; // Fixe le baud rate à 9600bps à changer!!!!!!!!!!!!!!!!! 36.000.000/9600 DIRECT
    //USART1->BRR |= 75; // Fixe le baud rate à 9600bps 
    USART3->CR1 |= USART_CR1_TE; // Envoi de la première trame d'attente
}


void send(char donne){
    USART3->DR = donne; // Ecriture de la donnée dans le registre DR
    while(!(USART3->SR & USART_SR_TC)) {} // Attente de la fin de transmission
}

void alerte(int donne){
    USART3->DR = donne; // Ecriture de la donnée dans le registre DR
    while(!(USART3->SR & USART_SR_TC)) {} // Attente de la fin de transmission
}

int configure_gpio_alternate_push_pull(GPIO_TypeDef *gpio, int pin){
    if (gpio == GPIOA) {
        RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;	
    } else if (gpio == GPIOB){
        RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    } else if (gpio == GPIOC){
        RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    } else {
	    return -1;
    }
    if (pin < 8){
        gpio->CRL &= ~((0xF << 4*pin)| (0xF << 4*pin));
        gpio ->CRL |= (0xA << 4*pin);
    } else if (pin < 15){
        pin = pin - 8 ;
        gpio ->CRH &= ~((0xF << 4*pin) | (0xF << 4*pin));
        gpio ->CRH |= (0xA << 4*pin);
    } else {
        return -1;
    }
    return 0;
}


void configuration_usart3_9600_receive (){
	
	 USART3->CR1 |= USART_CR1_UE;
	 USART3->CR1 &= ~USART_CR1_M;
	 USART3->CR2 &= USART_CR2_STOP;
	 //USART1->CR3 |= USART_CR3_DMAR;
	 //USART3->BRR |= 468 << 4; // Fixe le baud rate à 9600bps
   //USART3->BRR |= 75; // Fixe le baud rate à 9600bps
	 USART3->CR1 |= USART_CR1_RE; //activation réception
}


	
int receive (void)
{
  while (!(USART3->SR & (USART_SR_RXNE)));
	// Wait for RXNE to SET.. This indicates that the data has been Received  // Read the data. 
	return USART3->DR;
}

//commande du moteur du plateau
void Plateau(){
	signed char cycle=receive();
	float ratio = 0;
	if (cycle >=0){
		MyGPIO_Reset(GPIOB,8);
	}
	else{
		MyGPIO_Set(GPIOB,8);
		cycle=-cycle;
	}
	//vitesse du plateau
	ratio = (float) cycle/100;
	change_ratio(TIM2, (ratio), 1);
	}