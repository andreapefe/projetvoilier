#include "MyTimer.h"

void MyTimer_Base_Init ( MyTimer_Struct_TypeDef*Timer )
{		
	  //Initialisation des clocks pour tous les timers
    if (Timer -> Timer == TIM1)
    {
   	 RCC -> APB2ENR |= (0x01<<11);
    }
    
   	 if (Timer -> Timer == TIM2)
    {
   	 RCC ->APB1ENR |= 0x01 ;
    }
    
   	 if (Timer -> Timer == TIM3)
    {
   	 RCC ->APB1ENR |= (0x01<<1);
    }
    
   	 if (Timer -> Timer == TIM4)
    {
   	 RCC ->APB1ENR |= (0x01<<2);
    }
   	//Configuration des registres timers pour avoir la bonne fréquence
    Timer -> Timer -> ARR  = Timer -> shortARR;
    Timer -> Timer -> PSC = Timer -> shortPSC;
}
//Déclaration d'un pointeur de fonction pFnc
void (* pFnc) (void) ;

//Association des fonctions (weak) à la valeur du pointeur pFnc
void TIM2_IRQHandler(void){
    if (pFnc != 0)
   	 (*pFnc) ();
}
 void TIM3_IRQHandler(void){
    if (pFnc != 0)
   	 (*pFnc) ();
}
void TIM4_IRQHandler(void){
    if (pFnc != 0)
   	 (*pFnc) ();
}
void TIM1_UP_IRQHandler(void){
    if (pFnc != 0)
   	 (*pFnc) ();
}

//Activation des interruptions
void MyTimer_ActiveIT(TIM_TypeDef*Timer ,char Prio ,void(*IT_function )(void))
{ 	 
   	 if (Timer == TIM1)
    {
   	 NVIC_EnableIRQ(TIM1_UP_IRQn); //Active interruptions
   	 NVIC_SetPriority(TIM1_UP_IRQn, Prio); //Associe la priorité de cette interruption
   	 pFnc = IT_function; //Associe au pointeur pFnc la fonction en interruption souhaitée
    }
    else if (Timer == TIM2)
    {
   	 NVIC_EnableIRQ(TIM2_IRQn);
   	 NVIC_SetPriority(TIM2_IRQn, Prio);
   	 pFnc = IT_function;
    }
    
    else if (Timer == TIM3)
    {
   	 NVIC_EnableIRQ(TIM3_IRQn);
   	 NVIC_SetPriority(TIM3_IRQn, Prio);
   	 pFnc = IT_function;
    }
    
    else if ( Timer == TIM4)
    {
   	 NVIC_EnableIRQ(TIM4_IRQn);
   	 NVIC_SetPriority(TIM4_IRQn, Prio);
   	 pFnc = IT_function;
    }
    
    Timer -> DIER |=  1;  //Autorise les interruptions?   
}

void MyTimer_PWM(TIM_TypeDef*Timer, char Channel )
{
		 MyGPIO_Struct_TypeDef channel; //variable de configuration de port

   	 if (Timer == TIM1)
    {
			Timer->BDTR |= (0x1 << 0xF);
   	 
   	 //for channel GPIO pin mapping see p178 of RM
			if (Channel == 1)
			{
				Timer->CCER |= 1; //enable channel 1
				Timer->CCMR1 &= ~(0x7 << 0x4);    //CCRM1 pour choisir le mode qu'on veut (ici PWM output)
				Timer->CCMR1 |= (0x6 <<0x4);      //channel 1 choisi
			 
				//set the associated output pin channel to alternate output push pull
				channel.GPIO = GPIOA;
				channel.GPIO_Pin = 8;
				channel.GPIO_Conf = AltOut_Ppull;
																		
			}
   	 else if (Channel == 2)
   	 {
			 Timer->CCER |= (1<<4); //enable channel 2
			 Timer->CCMR1 &= ~(0x7 << 0xC);    //CCRM1 pour choisir le mode qu'on veut (ici PWM output)
			 Timer->CCMR1 |= (0x6 <<0xC);
			 
   		 channel.GPIO = GPIOA;
			 channel.GPIO_Pin = 9;
			 channel.GPIO_Conf = AltOut_Ppull; 							 //set the associated output pin channel to alternate output push pull
   	 }
   	 else if (Channel == 3)
   	 {
			 Timer->CCER |= (1<<8); //enable channel 3
			 Timer->CCMR2 &= ~(0x7 << 0x4);    //CCRM2 pour choisir le mode qu'on veut (ici PWM output)
			 Timer->CCMR2 |= (0x6 <<0x4);      //channel 3 choisi
			 
   		 channel.GPIO = GPIOA;
			 channel.GPIO_Pin = 10;
			 channel.GPIO_Conf = AltOut_Ppull;						 //set the associated output pin channel to alternate output push pull
   	 }
   	 else if (Channel == 4)
   	 {
			 Timer->CCER |= (1<<12); //enable channel 4
			 Timer->CCMR2 &= ~(0x7 << 0xC);    //CCRM2 pour choisir le mode qu'on veut (ici PWM output)
			 Timer->CCMR2 |= (0x6 <<0xC);      //channel 4 choisi
			 
   		 channel.GPIO = GPIOA;
			 channel.GPIO_Pin = 11;
			 channel.GPIO_Conf = AltOut_Ppull; 							 //set the associated output pin channel to alternate output push pull
   	 }

    }
    
   	 else if (Timer == TIM2)
    {    	 
   	 //for channel GPIO pin mapping see p178 of RM
   	 if (Channel == 1)
   	 {
			 Timer->CCER |= 1;   //enable channel 1
			 Timer->CCMR1 &= ~(0x7 << 0x4);    //CCRM1 pour choisir le mode qu'on veut (ici PWM output)
			 Timer->CCMR1 |= (0x6 <<0x4);      //channel 1 choisi
			 
   		 channel.GPIO = GPIOA;
			 channel.GPIO_Pin = 0;
			 channel.GPIO_Conf = AltOut_Ppull;  //set the associated output pin channel to alternate output push pull
   	 }
   	 else if (Channel == 2)
   	 {
			 Timer->CCER |= (1<<4); //enable channel 2
			 Timer->CCMR1 &= ~(0x7 << 0xC);    //CCRM1 pour choisir le mode qu'on veut (ici PWM output)
			 Timer->CCMR1 |= (0x6 <<0xC); //au channel 2
			 
   		 channel.GPIO = GPIOA;
			 channel.GPIO_Pin = 1;
			 channel.GPIO_Conf = AltOut_Ppull; 							 //set the associated output pin channel to alternate output push pull
   	 }
   	 else if (Channel == 3)
   	 {
			 Timer->CCER |= (1<<8); //enable channel 3
			 Timer->CCMR2 &= ~(0x7 << 0x4);    //CCRM2 pour choisir le mode qu'on veut (ici PWM output)
			 Timer->CCMR2 |= (0x6 <<0x4);      //channel 3 choisi
			 
   		 channel.GPIO = GPIOA;
			 channel.GPIO_Pin = 2;
			 channel.GPIO_Conf = AltOut_Ppull; 							 //set the associated output pin channel to alternate output push pull
   	 }
   	 else if (Channel == 4)
   	 {
			 Timer->CCER |= (1<<12);
			 Timer->CCMR2 &= ~(0x7 << 0xC);    //CCRM2 pour choisir le mode qu'on veut (ici PWM output)
			 Timer->CCMR2 |= (0x6 <<0xC);      //channel 4 choisi
			 
   		 channel.GPIO = GPIOA;
			 channel.GPIO_Pin = 3;
			 channel.GPIO_Conf = AltOut_Ppull;							 //set the associated output pin channel to alternate output push pull
   	 }
    }
    
    else if (Timer == TIM3)
    {
   	 //for channel GPIO pin mapping see p178 of RM
   	 if (Channel == 1)
   	 {
			 Timer->CCER |= 1; //enbale channel 1
			 Timer->CCMR1 &= ~(0x7 << 0x4);    //CCRM1 pour choisir le mode qu'on veut (ici PWM output)
			 Timer->CCMR1 |= (0x6 <<0x4);      //channel 1 choisi
			 
   		 channel.GPIO = GPIOA;
			 channel.GPIO_Pin = 6;
			 channel.GPIO_Conf = AltOut_Ppull;        		 //set the associated output pin channel to alternate output push pull
   	 }
   	 else if (Channel == 2)
   	 {
			 Timer->CCER |= (1<<4); //enable channel 2
			 Timer->CCMR1 &= ~(0x7 << 0xC);    //CCRM1 pour choisir le mode qu'on veut (ici PWM output)
			 Timer->CCMR1 |= (0x6 <<0xC); //au channel 2
			 
   		 channel.GPIO = GPIOA;
			 channel.GPIO_Pin = 7;
			 channel.GPIO_Conf = AltOut_Ppull;		 //set the associated output pin channel to alternate output push pull
   	 }
   	 else if (Channel == 3)
   	 {
			 Timer->CCER |= (1<<8); //enable channel 3
			 Timer->CCMR2 &= ~(0x7 << 0x4);    //CCRM2 pour choisir le mode qu'on veut (ici PWM output)
			 Timer->CCMR2 |= (0x6 <<0x4);      //channel 3 choisi
			 
   		 channel.GPIO = GPIOB;
			 channel.GPIO_Pin = 0;
			 channel.GPIO_Conf = AltOut_Ppull;							 //set the associated output pin channel to alternate output push pull
   	 }
   	 else if (Channel == 4)
   	 {
			 Timer->CCER |= (1<<12);
			 Timer->CCMR2 &= ~(0x7 << 0xC);    //CCRM2 pour choisir le mode qu'on veut (ici PWM output)
			 Timer->CCMR2 |= (0x6 <<0xC);      //channel 4 choisi
			 
   		 channel.GPIO = GPIOB;
			 channel.GPIO_Pin = 1;
			 channel.GPIO_Conf = AltOut_Ppull;    			 //set the associated output pin channel to alternate output push pull
   	 }
   							 
    }
    else if (Timer == TIM4)
    {
			
   	 //for channel GPIO pin mapping see p178 of RM
   	 if (Channel == 1)
   	 {
			 Timer->CCER |= 1;
			 Timer->CCMR1 &= ~(0x7 << 0x4);    //CCRM1 pour choisir le mode qu'on veut (ici PWM output)
			 Timer->CCMR1 |= (0x6 <<0x4);      //channel 1 choisi
			 
   		 channel.GPIO = GPIOB;
			 channel.GPIO_Pin = 6;
			 channel.GPIO_Conf = AltOut_Ppull;       		 //set the associated output pin channel to alternate output push pull
   	 }
   	 else if (Channel == 2)
   	 {
			 Timer->CCER |= (1<<4);
			 Timer->CCMR1 &= ~(0x7 << 0xC);    //CCRM1 pour choisir le mode qu'on veut (ici PWM output)
			 Timer->CCMR1 |= (0x6 <<0xC); //au channel 2
			 
   		 channel.GPIO = GPIOB;
			 channel.GPIO_Pin = 7;
			 channel.GPIO_Conf = AltOut_Ppull; 			 //set the associated output pin channel to alternate output push pull
   	 }
   	 else if (Channel == 3)
   	 {
			 Timer->CCER |= (1<<8); //enable channel 3
			 Timer->CCMR2 &= ~(0x7 << 0x4);    //CCRM2 pour choisir le mode qu'on veut (ici PWM output)
			 Timer->CCMR2 |= (0x6 <<0x4);      //channel 3 choisi
			 
   		 channel.GPIO = GPIOB;
			 channel.GPIO_Pin = 8;
			 channel.GPIO_Conf = AltOut_Ppull; 							 //set the associated output pin channel to alternate output push pull
   	 }
   	 else if (Channel == 4)
   	 {
			 Timer->CCER |= (1<<12);
			 Timer->CCMR2 &= ~(0x7 << 0xC);    //CCRM2 pour choisir le mode qu'on veut (ici PWM output)
			 Timer->CCMR2 |= (0x6 <<0xC);      //channel 4 choisi
			 
   		 channel.GPIO = GPIOB;
			 channel.GPIO_Pin = 9;
			 channel.GPIO_Conf = AltOut_Ppull;    			 //set the associated output pin channel to alternate output push pull
   	 }
   							 //set the associated output pin channel to alternate output push pull
    }

		MyGPIO_Init(&channel); //initialize clock and configure port
}

void change_ratio(TIM_TypeDef*Timer, float ratio, char channel)
{
	unsigned compare = (int) (Timer->ARR * ratio);
	if (channel == 1)
	{
		Timer->CCR1 &= 0x0000; 
		Timer->CCR1 |= compare; 
	}
	else if (channel == 2)
	{
		Timer->CCR2 &= 0x0000;
		Timer->CCR2 |= compare; 
	} 
	else if (channel == 3)
	{
		Timer->CCR3 &= 0x0000;
		Timer->CCR3 |= compare; 
	} 
	else if (channel == 4)
	{
		Timer->CCR4 &= 0x0000;
		Timer->CCR4 |= compare; 
	}
	
}

void input_capture_mode(MyTimer_Struct_TypeDef*timer_in)
{
	//Declaration for config port in input floating selon le timer
	MyGPIO_Struct_TypeDef channel1;
	MyGPIO_Struct_TypeDef channel2; //set both associated intput pin channel to input floating
	
	timer_in->Timer->CCMR1 |= 01; //config channel e1 1 en input
	timer_in->Timer->CCMR1 |= (0x01 << 8); //config channel 2 en input
	timer_in->Timer->SMCR |= 0x3;
	
	//Congif according to selected timer (only channel 1 and 2)
	
	if (timer_in->Timer == TIM1)
    {
   	 //for channel GPIO pin mapping see p178 of RM
			channel1.GPIO = GPIOA;
			channel1.GPIO_Pin = 8;
			channel1.GPIO_Conf = In_Floating;
																					 
   		channel2.GPIO = GPIOA;
			channel2.GPIO_Pin = 9;
			channel2.GPIO_Conf = In_Floating; 							 
   	 }
    
  else if (timer_in->Timer == TIM2)
    {    	 
   		 channel1.GPIO = GPIOA;
			 channel1.GPIO_Pin = 0;
			 channel1.GPIO_Conf = In_Floating;     	 
   	 
   		 channel2.GPIO = GPIOA;
			 channel2.GPIO_Pin = 1;
			 channel2.GPIO_Conf = In_Floating; 							   	 
    }
    
    else if (timer_in->Timer == TIM3)
    {			 
   		 channel1.GPIO = GPIOA;
			 channel1.GPIO_Pin = 6;
			 channel1.GPIO_Conf = In_Floating;        		 
   	    	 
   		 channel2.GPIO = GPIOA;
			 channel2.GPIO_Pin = 7;
			 channel2.GPIO_Conf = In_Floating;		 					 
    }
		
    else if (timer_in->Timer == TIM4)
    {			 
   		 channel1.GPIO = GPIOB;
			 channel1.GPIO_Pin = 6;
			 channel1.GPIO_Conf = In_Floating;       		 
   	 		 
   		 channel2.GPIO = GPIOB;
			 channel2.GPIO_Pin = 7;
			 channel2.GPIO_Conf = In_Floating; 			 	
    }
		
		MyGPIO_Init(&channel1);
		MyGPIO_Init(&channel2);
}



