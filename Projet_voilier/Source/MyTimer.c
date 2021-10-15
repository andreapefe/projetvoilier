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
    
   	 Timer->CCMR1 &= ~(0x7 << 0x4);    //CCRM1 pour choisir le mode qu'on veut (ici PWM output)
   	 Timer->CCMR1 |= (0x6 <<0x4);

   	 if (Timer == TIM1)
    {
   	 Timer->BDTR |= (0x1 << 0xF);
   	 
   	 //for channel GPIO pin mapping see p178 of RM
   	 if (Channel == 1)
   	 {
   		 GPIOA->CRH &= 0000;
   		 GPIOA->CRH |= 1001;  //set the associated output pin channel to alternate output push pull
   	 }
   	 else if (Channel == 2)
   	 {
   		 GPIOA->CRH &= ~(0xF << 0x4);
   		 GPIOA->CRH |= (1001 << 0x4); 							 //set the associated output pin channel to alternate output push pull
   	 }
   	 else if (Channel == 3)
   	 {
   		 GPIOA->CRH &= ~(0xF << 0x8);
   		 GPIOA->CRH |= (1001 << 0x8); 							 //set the associated output pin channel to alternate output push pull
   	 }
   	 else if (Channel == 4)
   	 {
   		 GPIOA->CRH &= ~(0xF << 0xC);
   		 GPIOA->CRH |= (1001 << 0xC); 							 //set the associated output pin channel to alternate output push pull
   	 }

    }
    
   	 else if (Timer == TIM2)
    {    	 
   	 //for channel GPIO pin mapping see p178 of RM
   	 if (Channel == 1)
   	 {
   		 GPIOA->CRL &= 0000;
   		 GPIOA->CRL |= 1001;  //set the associated output pin channel to alternate output push pull
   	 }
   	 else if (Channel == 2)
   	 {
   		 GPIOA->CRL &= ~(0xF << 0x4);
   		 GPIOA->CRL |= (1001 << 0x4); 							 //set the associated output pin channel to alternate output push pull
   	 }
   	 else if (Channel == 3)
   	 {
   		 GPIOA->CRL &= ~(0xF << 0x8);
   		 GPIOA->CRL |= (1001 << 0x8); 							 //set the associated output pin channel to alternate output push pull
   	 }
   	 else if (Channel == 4)
   	 {
   		 GPIOA->CRL &= ~(0xF << 0xC);
   		 GPIOA->CRL |= (1001 << 0xC); 							 //set the associated output pin channel to alternate output push pull
   	 }
    }
    
    else if (Timer == TIM3)
    {
   	 //for channel GPIO pin mapping see p178 of RM
   	 if (Channel == 1)
   	 {
   		 GPIOA->CRL &= ~(0xF << 0x18);
   		 GPIOA->CRL |= (0x9 << 0x18);         		 //set the associated output pin channel to alternate output push pull
   	 }
   	 else if (Channel == 2)
   	 {
   		 GPIOA->CRL &= ~(0xF << 0x1C);
   		 GPIOA->CRL |= (0X9 << 0x1C); 			 //set the associated output pin channel to alternate output push pull
   	 }
   	 else if (Channel == 3)
   	 {
   		 GPIOB->CRL &= 0000;
   		 GPIOB->CRL |= 1001 ; 							 //set the associated output pin channel to alternate output push pull
   	 }
   	 else if (Channel == 4)
   	 {
   		 GPIOB->CRL &= ~(0xF << 0x4);
   		 GPIOB->CRL |= (1001 << 0x4);    			 //set the associated output pin channel to alternate output push pull
   	 }
   							 
    }
    else if (Timer == TIM4)
    {
   	 //for channel GPIO pin mapping see p178 of RM
   	 if (Channel == 1)
   	 {
   		 GPIOB->CRL &= ~(0xF << 0x18);
   		 GPIOB->CRL |= (0x9 << 0x18);         		 //set the associated output pin channel to alternate output push pull
   	 }
   	 else if (Channel == 2)
   	 {
   		 GPIOB->CRL &= ~(0xF << 0x1C);
   		 GPIOB->CRL |= (0x9 << 0x1C); 			 //set the associated output pin channel to alternate output push pull
   	 }
   	 else if (Channel == 3)
   	 {
   		 GPIOB->CRH &= 0000;
   		 GPIOB->CRH |= 1001 ; 							 //set the associated output pin channel to alternate output push pull
   	 }
   	 else if (Channel == 4)
   	 {
   		 GPIOB->CRH &= ~(0xF << 0x4);
   		 GPIOB->CRH |= (1001 << 0x4);    			 //set the associated output pin channel to alternate output push pull
   	 }
   							 //set the associated output pin channel to alternate output push pull
    }
    

}


